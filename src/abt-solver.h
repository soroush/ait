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

#include "common_csp.hpp"

namespace AIT {

struct assignment;
struct nogood;

template<typename V, typename T>
class ABT_Solver {

	enum messageType {
		OK, NOGOOD, ADDLINK, STOP,
	};

	struct message {
		messageType type;
	};
public:

	ABT_Solver(const std::string&, const long&);
	virtual ~ABT_Solver();

	void connect();
	void solve();
	void checkAgentView();
	void chooseValue(V&);
	void backtrack();
	void processInfo(const message&);
	void updateAgentView(const assignment&);
	bool coherent(const nogood&, const assignment&);
	void resolveConflict(const message&);
	void checkAddLink(const message&);
	void setLink(const message&);
	bool consistent(const V&, const AgentView&);
	void sendMessage(const message&);

private:
	T* myValue;
	message getMessage();
	void getAgentList();
	unsigned long id;
	std::list<unsigned long> preceding;   	// Γ+
	std::list<unsigned long> succeeding;	// Γ-
	CompoundAssignment<V,T> myAgentView;

	std::string address;
	long portNumber;

	boost::asio::io_service io;
	boost::asio::ip::tcp::resolver resolver;
	boost::asio::ip::tcp::socket socket;
};

}

template<typename V, typename T>
inline AIT::ABT_Solver<V>::ABT_Solver(const std::string& host,
		const long& port) :
		address(host), portNumber(port) {
	using boost::asio::ip::tcp;
	this->resolver = tcp::resolver(this->io)  ;
	this->socket = tcp::socket(this->io);
}

template<typename V, typename T>
inline AIT::ABT_Solver<V,T>::~ABT_Solver() {
	delete this->myValue;
}

template<typename V, typename T>
inline void AIT::ABT_Solver<V,T>::solve() {
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
inline void AIT::ABT_Solver<V,T>::connect() {
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
inline void AIT::ABT_Solver<V,T>::checkAgentView() {
	if (!consistent(*myValue, myAgentView)) {
		chooseValue(*myValue);
		if (myValue != nullptr) {
			for (const auto& agent : succeeding) {

			}
		}
	}
}

/* namespace AIT */
#endif /* ABT_SOLVER_H_ */
