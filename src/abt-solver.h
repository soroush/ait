/*
 * abt-solver.h
 *
 *  Created on: Apr 30, 2013
 *      Author: soroush
 */

#ifndef ABT_SOLVER_H_
#define ABT_SOLVER_H_

#include <list>
#include <map>
#include <string>
#include <boost/asio.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include "common_csp.hpp"
#include "common_async.hpp"

namespace AIT {

template<typename V, typename T>
class ABT_Solver {

	enum messageType {
		OK, NOGOOD, ADDLINK, STOP,
	};

	class CommunicationRequest {
		friend class boost::serialization::access;
	public:
		enum Type {
			INTRODUCE, BYE,
		};

		CommunicationRequest(const Type&, const AgentID&);
		~CommunicationRequest();
	public:
		AgentID id();
		Type type();

	private:
		Type type_;
		AgentID senderID_;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version) {
			ar & type_;
			ar & senderID_;
		}
	};

	class message {
		friend class boost::serialization::access;
	public:
		message(const messageType&, const AgentID&,
				const Assignment<AgentID, T>&,
				const CompoundAssignment<AgentID, T>);
		messageType type;
		AgentID sender;
		Assignment<AgentID, T> assignment;
		CompoundAssignment<AgentID, T> nogood;

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version) {
			ar & type;
			ar & sender;
			ar & assignment;
			ar & nogood;
		}
	};
public:

	ABT_Solver(const std::string&, const long&);
	virtual ~ABT_Solver();

	void connect();
	void solve();
	void checkAgentView();
	void chooseValue(V*);
	void backtrack();
	void processInfo(const message&); // OK
	void updateAgentView(const Assignment<V, T>&);
	bool coherent(const CompoundAssignment<V, T>& nogood,
			const Assignment<V, T>& assign);
	void resolveConflict(const message&);
	void checkAddLink(const message&);
	void setLink(const message&);
	bool consistent(const V&);
	void sendMessage(const AgentID&, const message&);

private:
	T* myValue;
	message getMessage();
	void getAgentList();
	AgentID id;
	std::list<AgentID> preceding; // Γ+
	std::list<AgentID> succeeding; // Γ-
	CompoundAssignment<V, T> myAgentView;

	std::string address;
	long portNumber;

	boost::asio::io_service io;
	boost::asio::ip::tcp::resolver resolver;
	boost::asio::ip::tcp::socket socket;
};

}

template<typename V, typename T>
inline AIT::ABT_Solver<V, T>::ABT_Solver(const std::string& host,
		const long& port) :
		address(host), portNumber(port) {
	using boost::asio::ip::tcp;
	this->resolver = tcp::resolver(this->io);
	this->socket = tcp::socket(this->io);
}

template<typename V, typename T>
inline AIT::ABT_Solver<V, T>::~ABT_Solver() {
	delete this->myValue;
}

template<typename V, typename T>
inline void AIT::ABT_Solver<V, T>::solve() {
	connect();
	myValue = nullptr;
	bool end = false;
	getAgentList();
	checkAgentView();
	while (!end) {
		message m = getMessage();
		switch (m.type) {
		case OK:
			processInfo(m);
			break;
		case NOGOOD:
			resolveConflict(m);
			break;
		case ADDLINK:
			setLink(m);
			break;
		case STOP:
			end = true;
			break;
		default:
			end = true;
			break;
		}
	}
}

template<typename V, typename T>
inline void AIT::ABT_Solver<V, T>::connect() {
	using boost::asio::ip::tcp;
	using namespace boost::asio;
	using namespace std;

	tcp::resolver::query query(this->address, this->portNumber);
	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	tcp::resolver::iterator end;
	boost::system::error_code error = error::host_not_found;
	while (error && endpoint_iterator != end) {
		socket.close();
		socket.connect(*endpoint_iterator++, error);
	}
	if (error)
		throw boost::system::system_error(error);

	// Let's introduce ourselves to synchronizer agent
	boost::asio::streambuf bufx;
	ostream os(&bufx);
	boost::archive::binary_oarchive ar(os);
	CommunicationRequest request(CommunicationRequest::INTRODUCE, this->id);
	ar << request;
	const size_t sc = boost::asio::write(this->socket, bufx);
}

template<typename V, typename T>
inline void AIT::ABT_Solver<V, T>::checkAgentView() {
	if (!consistent(*myValue, myAgentView)) {
		chooseValue(myValue);
		if (myValue != nullptr) {
			for (const auto& agent : succeeding) {
				sendMessage(agent,
						message(messageType::OK, this->id,
								Assignment<AgentID, V>(this->id,
										*(this->myValue))));
			}
		} else {
			backtrack();
		}
	} // end if !consistent
}

template<typename V, typename T>
inline void AIT::ABT_Solver<V, T>::processInfo(const message& m) {
	updateAgentView(m.assignment);
	checkAgentView();
}

template<typename V, typename T>
typename AIT::ABT_Solver<V, T>::message AIT::ABT_Solver<V, T>::getMessage() {
	using namespace boost;
	using namespace boost::asio;
	using boost::asio::ip::tcp;
	using namespace std;

	while (true) {
		// TODO: implement messaging mechanism using ASIO/Serialization.
	}
}

template<typename V, typename T>
inline AIT::ABT_Solver<V, T>::message::message(
		const AIT::ABT_Solver<V, T>::messageType& type, const AgentID& id,
		const Assignment<AgentID, T>& assignment,
		const CompoundAssignment<AgentID, T> compoundAssignment) {
}

template<typename V, typename T>
AIT::AgentID AIT::ABT_Solver<V, T>::CommunicationRequest::id() {
	return this->senderID_;
}

template<typename V, typename T>
inline AIT::ABT_Solver<V, T>::CommunicationRequest::CommunicationRequest(
		const Type& messageType, const AgentID& agent) :
		type_(messageType), senderID_(agent) {
}

template<typename V, typename T>
inline AIT::ABT_Solver<V, T>::CommunicationRequest::~CommunicationRequest() {
	delete senderID_;
	delete type_;
}

template<typename V, typename T>
typename AIT::ABT_Solver<V, T>::CommunicationRequest::Type AIT::ABT_Solver<V, T>::CommunicationRequest::type() {
	return this->type_;
}

/* namespace AIT */
#endif /* ABT_SOLVER_H_ */
