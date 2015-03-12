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

	OVERLAPPED overlapped;
	WSABUF dataBuf;
	DWORD bytesSend;
	DWORD bytesRecv;
};
enum ClientSocket { CONTROL, STREAM, DOWNLOAD };

struct NetVars
{
	SOCKET sock_lisn;
	SOCKADDR_IN server;
};
NetVars& getNetVars();

namespace Server
{
	void start();
	void tearDown();
	bool isAlive();
	bool openListener(unsigned short int port);
	void acceptConnection();
	Client& createClient();
	bool send(Client& c, ClientSocket cs);
	bool recv(Client& c, ClientSocket cs);
}

#endif