#ifndef NETWORK_H_
#define NETWORK_H_

#include <WinSock2.h>
#include <vector>

#define MULTICAST_ADDR "234.5.6.7"
#define DATA_BUFSIZE 8192

enum ServerMode { MULTICAST, UNICAST };
typedef enum ServerMode ServerMode;

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
	sockaddr_in sin_udp;
};

bool createSockAddrIn(sockaddr_in& sin, std::string ip, unsigned short port);

namespace Server
{
	void start();
	void tearDown();
	bool isAlive();
	bool openListener(SOCKET& listenSocket, unsigned short int port);
	bool acceptConnection(SOCKET listenSocket, ServerMode sMode);
	Client* createClient();
	bool recv(Client* c);
	bool send(Client* c, std::string msg, sockaddr_in* sin = nullptr);
	void disconnectClient(std::string ip);
}

#endif
