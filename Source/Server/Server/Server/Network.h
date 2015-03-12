#ifndef NETWORK_H_
#define NETWORK_H_

#include <WinSock2.h>
#include <vector>

struct Client
{
	SOCKET
		sock_tcp_control,
		sock_udp_stream,
		sock_tcp_download;

	std::string buffer;

	/* DON'T TOUCH BELOW */

	WSAOVERLAPPED overlapped;
	WSABUF dataBuf;
};

enum ClientSocket { CONTROL, STREAM, DOWNLOAD };

struct NetVars
{
	SOCKET sock_lisn;
	SOCKADDR_IN server;
};

NetVars& getNetVars();

bool openListener(unsigned short int port);
void acceptConnection();

void serverStart();
void serverTearDown();
Client& serverCreateClient();
bool serverSend(Client& c, ClientSocket cs);
bool serverRecv(Client& c, ClientSocket cs);

#endif