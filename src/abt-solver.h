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

	class message {
		friend class boost::serialization::access;
	public:
		message(const messageType&, const AgentID&, const Assignment<V, T>&,
				const CompoundAssignment<V, T>);
		messageType type;
		AgentID sender;
		Assignment<V, T> assignment;
		CompoundAssignment<V, T> nogood;

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
	tcp::resolver::query query(this->address, this->portNumber);
	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	tcp::resolver::iterator end;
	boost::system::error_code error = boost::asio::error::host_not_found;
	while (error && endpoint_iterator != end) {
		socket.close();
		socket.connect(*endpoint_iterator++, error);
	}
	if (error)
		throw boost::system::system_error(error);
}

template<typename V, typename T>
inline void AIT::ABT_Solver<V, T>::checkAgentView() {
	if (!consistent(*myValue, myAgentView)) {
		chooseValue(myValue);
		if (myValue != nullptr) {
			for (const auto& agent : succeeding) {
				sendMessage(agent,
						message(messageType::OK, this->id,
								Assignment<T, V>(this->id, *(this->myValue))));
				// FIXME: replace assignment with a new structure designed for
				//        Asynchronous algorithms
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
		const Assignment<V, T>& assignment,
		const CompoundAssignment<V, T> compoundAssignment) {
}

/* namespace AIT */
#endif /* ABT_SOLVER_H_ */
