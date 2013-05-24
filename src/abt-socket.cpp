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

ABT_Socket::ABT_Socket(context_t &context_, int type_) :
		socket_t(context_, type_) {
	// TODO Auto-generated constructor stub
}

ABT_Socket::~ABT_Socket() {
	// TODO Auto-generated destructor stub
}

size_t ABT_Socket::sendMessage(const ABT_CommunicationProtocol& packet) {
	size_t length = packet.ByteSize();
	message_t message(length);
	packet.SerializeToArray(message.data(), length);
	return socket_t::send(message);
}

size_t ABT_Socket::sendMessage(const ABT_Message& packet) {
	size_t length = packet.ByteSize();
	message_t message(length);
	packet.SerializeToArray(message.data(), length);
	return socket_t::send(message);
}

size_t AIT::ABT_Socket::recvMessage(ABT_CommunicationProtocol& packet) {
	message_t message;
	bool returnValue = recv(&message);
	packet.ParseFromArray(message.data(), message.size());
	return returnValue;
}

size_t AIT::ABT_Socket::recvMessage(ABT_Message& packet) {
}

std::string AIT::ABT_Socket::getIP() {
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

