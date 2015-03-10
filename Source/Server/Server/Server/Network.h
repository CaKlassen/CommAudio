#ifndef NETWORK_H_
#define NETWORK_H_

#include <winsock.h>
#include <vector>

struct Client
{
	SOCKET
		sock_tcp_control,
		sock_udp_stream,
		sock_tcp_download;
};

struct NetVars
{
	SOCKET sock_lisn;
	SOCKADDR_IN server;

	std::vector<Client> clients;
};

NetVars& GetNetVars();

bool openListener(unsigned short int port);
void acceptConnection();

#endif