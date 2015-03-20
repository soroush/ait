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

#ifndef ABT_MONITOR_H_
#define ABT_MONITOR_H_

#ifndef __cplusplus
#error "This header is a C++ header and it cannot be compiled using a C compiler."
#endif

#include <string>
#include <vector>
#include <memory>
#include <zmq.hpp>
#define BOOST_ALL_DYN_LINK
#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include "common_async.hpp"
#include "abt-solver.hpp"
#include "abt-socket.hpp"

namespace AIT {
namespace CSP {

class CSP_Problem;
class Variable;

class ABT_Monitor {
public:
	ABT_Monitor(const std::string& host, const unsigned short& responserPort,
			const unsigned short& publisherPort, const std::string& xcsp,
			const std::string& logfile="std::cout");
	virtual ~ABT_Monitor();
	void start();
private:
	void parse(const std::string& path);
	struct Awareness{
	    int sequence;
	    bool approved;
	};
	unsigned short m_responserPort, m_publisherPort;
	std::string m_host;
	zmq::context_t m_context;
	Socket m_publisher;
	Socket m_responser;
	size_t m_agentCount;
	std::vector<protocols::csp::abt::P_EndPoint> m_agents;
	Awareness** m_time;
	bool m_finished;
	std::unique_ptr<CSP_Problem> m_instance;
	std::map<size_t,Variable*> m_p2v;

	bool checkMatrix();
    boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;
};

} /* namespace CSP */
} /* namespace AIT */
#endif /* ABT_MONITOR_H_ */
