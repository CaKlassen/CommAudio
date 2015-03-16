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
	DWORD bytesSend;
	DWORD bytesRecv;
};

struct Client
{
	SOCKET_INFORMATION socketinfo;
};

namespace Server
{
	void start();
	void tearDown();
	bool isAlive();
	bool openListener(SOCKET& listenSocket, unsigned short int port);
	bool acceptConnection(SOCKET listenSocket);
	Client& createClient();
	bool send(Client& c, std::string msg);
	bool recv(Client& c, std::string msg);
	void disconnectClient(std::string ip);
}

#endif