/*
 * abt-socket.cpp
 *
 *  Created on: May 24, 2013
 *      Author: soroush
 */

#include "abt-socket.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>

using namespace AIT;
using namespace zmq;

Socket::Socket(context_t &context_, int type_) :
		socket_t(context_, type_) {
	// TODO Auto-generated constructor stub
}

Socket::~Socket() {
	// TODO Auto-generated destructor stub
}

size_t Socket::sendMessage(const protocols::ABT::P_CommunicationProtocol& packet) {
	size_t length = packet.ByteSize();
	message_t message(length);
	packet.SerializeToArray(message.data(), length);
	return socket_t::send(message);
}

size_t Socket::sendMessage(const protocols::ABT::P_Message& packet) {
	size_t length = packet.ByteSize();
	message_t message(length);
	packet.SerializeToArray(message.data(), length);
	return socket_t::send(message);
}

size_t AIT::Socket::recvMessage(protocols::ABT::P_CommunicationProtocol& packet) {
	message_t message;
	bool returnValue = recv(&message);
	packet.ParseFromArray(message.data(), message.size());
	return returnValue;
}

size_t AIT::Socket::recvMessage(protocols::ABT::P_Message& packet) {
}

std::string AIT::Socket::getIP() {
	std::string address = "";
	FILE * fp = popen("ifconfig", "r");
	if (fp) {
		char *p = NULL, *e;
		size_t n;
		while ((getline(&p, &n, fp) > 0) && p) {
			if (p = strstr(p, "inet addr:")) {
				p += 10;
				if (e = strchr(p, ' ')) {
					*e = '\0';
					return std::string(p);
					address = std::string(p);

				}
			}
		}
	}
	pclose(fp);
	return address;
}

