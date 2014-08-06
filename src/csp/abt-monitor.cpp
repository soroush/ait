/*
 AIT Library (Artificial Intelligence Toolkit), A C++ library of AI tools.

 Copyright (c) 2012,2013 Soroush Rabiei <soroush-r@users.sf.net>,
 Roya Ghasemzadeh <ghasemzadeh.roya1@gmail.com>

 AIT is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 See the file COPYING included with this distribution for more
 information.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "abt-monitor.hpp"
#include "csp-problem.hpp"
#include "csp-solver.hpp"
#include "parser/xinstance-parser.hpp"
#include "parser/xpresentation-parser.hpp"
#include "parser/xproblem-type-parser.hpp"
#include "parser/xproblem-format-parser.hpp"
#include "parser/xdomains-parser.hpp"
#include "parser/xdomain-parser.hpp"
#include "parser/xvariables-parser.hpp"
#include "parser/xvariable-parser.hpp"
#include "parser/xpredicates-parser.hpp"
#include "parser/xpredicate-parser.hpp"
#include "parser/xexpression-parser.hpp"
#include "parser/xconstraints-parser.hpp"
#include "parser/xconstraint-parser.hpp"
#include "parser/xrelations-parser.hpp"
#include "parser/xrelation-parser.hpp"
#define BOOST_ALL_DYN_LINK
#define DSO
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/format.hpp>
#include "abt-monitor.h"
#include <chrono>
#include <sstream>
#include <algorithm>
#include <stdexcept>

using namespace AIT::CSP;
using namespace AIT::protocols::csp::abt;
using namespace zmq;
using namespace std;
using namespace std::chrono;
namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
using namespace logging::trivial;

ABT_Monitor::ABT_Monitor(const std::string& host,
        const unsigned short & responserPort,
        const unsigned short & publisherPort, const std::string& xcsp,
        const std::string& logfile) :
        m_host(host), m_responserPort(responserPort), m_publisherPort(
                publisherPort), m_context(2), m_responser(m_context, ZMQ_REP), m_publisher(
                m_context, ZMQ_PUB), m_finished(false), m_instance(
                unique_ptr<CSP_Problem> { new CSP_Problem() }) {
    parse(xcsp);
    this->m_agentCount = m_instance->variables().size();
    m_time = new Awareness*[m_agentCount];
    for (int i = 0; i < m_agentCount; ++i) {
        m_time[i] = new Awareness[m_agentCount];
        for (int j = 0; j < m_agentCount; ++j) {
            m_time[i][j].approved = false;
            m_time[i][j].sequence = 0;
        }
    }
    logging::add_common_attributes();
    logging::register_simple_formatter_factory<logging::trivial::severity_level,
            char>("Severity");
    logging::add_file_log(keywords::file_name = logfile, keywords::auto_flush =
            true, keywords::format = "[%TimeStamp%][%Severity%]: %Message%");
}

ABT_Monitor::~ABT_Monitor() {
    this->m_responser.close();
    this->m_publisher.close();
    for (int i = 0; i < m_agentCount; i++) {
        delete[] m_time[i];
        m_time[i] = nullptr;
    }
    delete[] m_time;
}

void ABT_Monitor::start() {
    // Listen for incoming requests
    stringstream address;
    address << "tcp://*:" << this->m_responserPort;
    try {
        this->m_responser.bind(address.str().data());
    } catch (zmq::error_t* e) {
        cerr << e->what() << endl;
    }
    // Open broadcast channel
    address.str("");
    address << "tcp://*:" << this->m_publisherPort;
    try {
        BOOST_LOG_SEV(lg, trace)<<
        boost::format("Binding to *:%1% broadcast end point...")
        % this->m_publisherPort;
        this->m_publisher.bind(address.str().data());
    } catch (zmq::error_t &e) {
        BOOST_LOG_SEV(lg, fatal) <<
        boost::format("Unable to bind to broadcast channel at *:%1%. "
                "Following information may describe possible failure "
                "causes: %2%")
        % this->m_publisherPort % e.what();
    }
    BOOST_LOG_SEV(lg, trace)<< "Successfully bound to broadcast channel.";
    size_t counter = 0;
    size_t priority = 1;
    while (counter != 2 * this->m_agentCount) {
        P_CommunicationProtocol requestPacket;
        this->m_responser.recvMessage(requestPacket);
        if (requestPacket.type() == CP_MessageType::T_INTRODUCE) {
            BOOST_LOG_SEV(lg, trace)<<
            boost::format(
                    "New agent subscribed. This information is introduced "
                    "by new agent"
                    "\n\tHost          : %1%"
                    "\n\tVariable Name : %2%"
                    "\n\tListener Port : %3%")
            % requestPacket.identity().host()
            % requestPacket.identity().name()
            % requestPacket.identity().port();
            bool repeatedName =
            std::any_of(m_agents.begin(), m_agents.end(),
                    [&](P_EndPoint i) {return (requestPacket.identity().name()==i.name());});
            if (repeatedName) {
                BOOST_LOG_SEV(lg, trace) <<
                boost::format("An agent with name `%1%'is already registered."
                        "Ignoring this request")
                %requestPacket.identity().name();
                P_CommunicationProtocol nackPacket;
                nackPacket.set_type(CP_MessageType::ERR_REPEATED_ID);
                this->m_responser.sendMessage(nackPacket);
            } else {
                requestPacket.mutable_identity()->set_priority(priority);
                m_agents.push_back(requestPacket.identity());
                this->m_p2v.insert(
                        std::make_pair<size_t, Variable*>(
                                requestPacket.identity().priority(),
                                this->m_instance->variable(
                                        requestPacket.identity().name())));
                P_CommunicationProtocol ackPacket;
                ackPacket.set_type(CP_MessageType::T_INTRODUCE_ACK);
                ackPacket.set_priority(priority);
                this->m_responser.sendMessage(ackPacket);
                ++counter;
                ++priority;
            }
        } else if (requestPacket.type() == CP_MessageType::T_REQUEST_LIST) {
            P_CommunicationProtocol requestListAck;
            requestListAck.set_type(CP_MessageType::T_REQUEST_ACK);
            this->m_responser.sendMessage(requestListAck);
            ++counter;
        }
    } // end of while

    // Send list on broadcast channel
    P_CommunicationProtocol listPacket;
    listPacket.set_type(CP_MessageType::T_LIST);
    for (auto endPoint : this->m_agents) {
        auto item = listPacket.add_others();
        item->CopyFrom(endPoint);
    }
    this->m_publisher.sendMessage(listPacket);

//    _INFO("Waiting for value assignment notifications...");
    auto start = std::chrono::steady_clock::now();

    while (!this->m_finished) {
        P_Message notif;
//        _INFO("Waiting for new message...");
        this->m_responser.recvMessage(notif);
        if (notif.type() == P_MessageType::T_OK_MONITOR) {
//            _INFO("A notification is received: (%d, %d, %d).", notif.sender(),
//                    notif.sequence(), notif.assignment().priority());
            Awareness x;
            x.sequence = notif.sequence();
            x.approved = notif.approved();
            int i = notif.sender() - 1;
            int j = notif.assignment().priority() - 1;
            if (i == j) {
                this->m_p2v[notif.sender()]->setValue(
                        notif.assignment().value());
            }
            if (m_time[i][j].sequence < x.sequence)
                m_time[i][j] = x;
            else if (m_time[i][j].sequence == x.sequence
                    and !m_time[i][j].approved)
                m_time[i][j] = x;
            // Send acknowledge
            P_Message okAcknowledge;
            okAcknowledge.set_type(P_MessageType::T_OK_MONITOR);
            okAcknowledge.set_sender(0);
//            _INFO("Sending Ack for OK_MONITOR...");
            this->m_responser.sendMessage(okAcknowledge);

        } else {
            throw logic_error { "A `T_OK_MONITOR' message is expected though "
                    "received message is not a valid `T_OK_MONITOR' message." };
        }
        this->m_finished = checkMatrix();
    }
    auto end = steady_clock::now();
    BOOST_LOG_SEV(lg, info)<<
    boost::format("Time to execute algorithm: %d ms.")%duration_cast<milliseconds>(end-start).count();
    BOOST_LOG_SEV(lg, info)<< "Sending all agents finish signal...";

    P_Message finished;
    finished.set_type(P_MessageType::T_FINISH);
    finished.set_sender(0);
    this->m_publisher.sendMessage(finished);

    BOOST_LOG_SEV(lg, info)<<"Algorithm is finished. Found values are";
    for (const auto& x : this->m_instance->variables()) {
        cout << x->getName() << "=" << x->getValue() << endl;
    }
}

void ABT_Monitor::parse(const string& path) {
    // Initialize parsers
    // Common parsers:
    xml_schema::string_pimpl stringParser;
    xml_schema::positive_integer_pimpl positiveIntegerParser;

    XInstanceParser instanceParser { this->m_instance.get() };
    XPresentationParser presentation;
    XProblemTypeParser problemType;
    XProblemFormatParser problemFormat;
    presentation.parsers(stringParser, positiveIntegerParser, stringParser,
            stringParser, stringParser, problemType, problemFormat);
    XDomainsParser domains;
    XDomainParser domain;
    domain.parsers(stringParser, positiveIntegerParser);
    domains.parsers(domain, positiveIntegerParser);
    XVariablesParser variables;
    XVariableParser variable { this->m_instance.get() };
    variable.parsers(stringParser, stringParser);
    variables.parsers(variable, positiveIntegerParser);
    XPredicatesParser predicates;
    XPredicateParser predicate;
    XExpressionParser expression;
    expression.parsers(stringParser, stringParser, stringParser, stringParser);
    predicate.parsers(stringParser, expression, stringParser);
    predicates.parsers(predicate, positiveIntegerParser);
    XConstraintsParser constraints;
    XConstraintParser constraint { this->m_instance.get() };
    constraint.parsers(stringParser, stringParser, stringParser, stringParser,
            positiveIntegerParser);
    constraints.parsers(constraint, positiveIntegerParser);
    XRelationsParser relations;
    XRelationParser relation;
    relations.relation_parser(relation);
    instanceParser.parsers(presentation, domains, variables, relations,
            predicates, constraints);
//  // Prepare document and parse
    xml_schema::document doc(instanceParser, "instance");
    doc.parse(path);
}

bool ABT_Monitor::checkMatrix() {
//    cout << "Current matrix: " << endl;
//    for (int i = 0; i < this->agentCount_; ++i) {
//        for (int j = 0; j < this->agentCount_; ++j) {
//            cout << acceptanceMatrix[i][j].sequence << ","
//                    << acceptanceMatrix[i][j].approved << " | ";
//        }
//        cout << endl;
//    }

    for (int j = 0; j < this->m_agentCount; ++j) {
        int maxSequence = m_time[j][j].sequence;
        if (maxSequence == 0) {
            return false;
        } else {
            for (int i = j; i < this->m_agentCount; ++i) {
                if (!m_time[i][j].approved) {
                    return false;
                }
                int &currentSequence = m_time[i][j].sequence;
                if (currentSequence == 0 or currentSequence == maxSequence) {
                    // noop
                } else if (currentSequence < maxSequence) {
                    return false;

                } else if (currentSequence > maxSequence) {
                    return false;

                }
            }
        }
    }
    return true;
}

/* ********************************** C API **********************************
 * This is C wrapper around C++ implementation API.
 *
 * **************************************************************************/

C_ABT_Monitor* abt_monitor_create(char* host,
        const unsigned short responserPort, const unsigned short publisherPort,
        char* xcsp) {
    return reinterpret_cast<C_ABT_Monitor*>(new ABT_Monitor(host, responserPort,
            publisherPort, xcsp));
}

void abt_monitor_destroy(C_ABT_Monitor* m) {
    delete reinterpret_cast<ABT_Monitor*>(m);
}

void abt_monitor_start(C_ABT_Monitor* m) {
    auto monitor = reinterpret_cast<ABT_Monitor*>(m);
    monitor->start();
}
