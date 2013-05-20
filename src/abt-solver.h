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
#include <queue>
#include <pthread.h>
#include <stdlib.h>
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

	class CommunicationPacket {
		friend class boost::serialization::access;
	public:
		enum Type {
			INTRODUCE, GET_LIST, BYE,
		};

		CommunicationPacket(const Type&, const AgentID&);
		~CommunicationPacket();
	public:
		AgentID id();
		Type type();
//		struct AgentAdresses {
//			AgentID id;
//			boost::asio::ip::address address;
//			unsigned short port;
//		};
//		std::list<AgentAdresses> agents();

	private:
		Type type_;
		AgentID senderID_;
//		std::list<AgentAdresses> agents_;

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version) {
			ar & type_;
			ar & senderID_;
//			ar & agents_;
		}
	};

	class message {
		friend class boost::serialization::access;
	public:
		message();
		message(const messageType&, const AgentID&,
				const Assignment<AgentID, T>&,
				const CompoundAssignment<AgentID, T>);
		message(const messageType&, const AgentID&,
				const Assignment<AgentID, T>&);
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

	ABT_Solver(const std::string&, const std::string&);
	virtual ~ABT_Solver();

	void connect();
	void solve();
	void checkAgentView();
	void chooseValue(V*);
	void backtrack();
	void processInfo(const message&); // OK
	void updateAgentView(const Assignment<AgentID, T>&);
	bool coherent(const CompoundAssignment<V, T>& nogood,
			const Assignment<V, T>& assign);
	void resolveConflict(const message&);
	void checkAddLink(const message&);
	void setLink(const message&);
	bool consistent(const V&);
	void sendMessage(const AgentID&, const message&);
	message getMessage();
	static void* messageReaderWorkhorse(void*);

private:
	T* myValue;
	void getAgentList();
	AgentID id;
	std::list<AgentID> preceding; // Γ+
	std::list<AgentID> succeeding; // Γ-
	CompoundAssignment<V, T> myAgentView;

	std::string address;
	std::string portNumber;

	// Networking
	boost::asio::io_service io;
	boost::asio::ip::tcp::socket serverSocket;
	boost::asio::ip::tcp::socket p2pSocket;

	// Messaging
	std::queue<message> messages;

	// P2P connection information

	// Synchronization mechanism
	pthread_mutex_t queueLock;
	pthread_mutex_t readLock;
	pthread_t messageReader;
};

}

template<typename V, typename T>
inline AIT::ABT_Solver<V, T>::ABT_Solver(const std::string& host,
		const std::string& port):
		serverSocket(this->io), p2pSocket(this->io){
//	using boost::asio::ip::tcp;
//	resolver = tcp::resolver(this->io);
//	this->serverSocket = tcp::socket(this->io);
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

	tcp::resolver resolver(this->io);

	tcp::resolver::query query(this->address, this->portNumber);
	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	tcp::resolver::iterator end;
	boost::system::error_code error = error::host_not_found;
	while (error && endpoint_iterator != end) {
		serverSocket.close();
		serverSocket.connect(*endpoint_iterator++, error);
	}
	if (error)
		throw boost::system::system_error(error);

	// Start message reader thread

	pthread_create(&(this->messageReader), NULL,ABT_Solver<V, T>::messageReaderWorkhorse,NULL);

	// Let's introduce ourselves to synchronizer agent
	boost::asio::streambuf bufx;
	ostream os(&bufx);
	boost::archive::binary_oarchive ar(os);
	CommunicationPacket request(CommunicationPacket::INTRODUCE, this->id);
	ar << request;
	const size_t sc = boost::asio::write(this->serverSocket, bufx);
	// TODO: Wait for list of agents and their addresses
}

template<typename V, typename T>
inline void AIT::ABT_Solver<V, T>::checkAgentView() {
	if (!consistent(*myValue)) {
		chooseValue(myValue);
		if (myValue != nullptr) {
			for (const auto& agentID : succeeding) {
				sendMessage(agentID, message());
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
AIT::AgentID AIT::ABT_Solver<V, T>::CommunicationPacket::id() {
	return this->senderID_;
}

template<typename V, typename T>
inline AIT::ABT_Solver<V, T>::CommunicationPacket::CommunicationPacket(
		const Type& messageType, const AgentID& agent) :
		type_(messageType), senderID_(agent) {
}

template<typename V, typename T>
inline AIT::ABT_Solver<V, T>::CommunicationPacket::~CommunicationPacket() {
}

template<typename V, typename T>
typename AIT::ABT_Solver<V, T>::CommunicationPacket::Type AIT::ABT_Solver<V, T>::CommunicationPacket::type() {
	return this->type_;
}

template<typename V, typename T>
inline void* AIT::ABT_Solver<V, T>::messageReaderWorkhorse(void*) {
}

/* namespace AIT */
#endif /* ABT_SOLVER_H_ */
