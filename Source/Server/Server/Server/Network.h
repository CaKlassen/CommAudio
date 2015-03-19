#ifndef NETWORK_H_
#define NETWORK_H_

#include <WinSock2.h>
#include <vector>

#define DATA_BUFSIZE 8192

struct SOCKET_INFORMATION
{
	OVERLAPPED overlapped;
	SOCKET socket;
	CHAR buffer[DATA_BUFSIZE];
	WSABUF dataBuf;
};

struct Client
{
	SOCKET_INFORMATION socketinfo;
};

bool createSockAddrIn(sockaddr_in& sin, std::string ip, unsigned short port);

namespace Server
{
	void start();
	void tearDown();
	bool isAlive();
	bool openListener(SOCKET& listenSocket, unsigned short int port);
	bool acceptConnection(SOCKET listenSocket);
	Client* createClient();
	bool recv(Client* c);
	bool send(Client* c, std::string msg, sockaddr_in* sin = nullptr);
	void disconnectClient(std::string ip);
}

#endif
