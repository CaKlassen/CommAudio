#ifndef NETWORK_H_
#define NETWORK_H_

#include <winsock.h>
#include <vector>

struct Client
{
	SOCKET
		sock_ctrl,
		sock_data;
};

struct NetVars
{
	SOCKET sock_lisn;
	SOCKADDR_IN server;

	std::vector<Client> clients;
};

bool openListener(unsigned short int port);
void acceptConnection();

#endif