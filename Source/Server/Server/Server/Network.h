#ifndef NETWORK_H_
#define NETWORK_H_

#include <WinSock2.h>
#include <vector>

#define MULTICAST_ADDR "234.5.6.7"
#define DATA_BUFSIZE 8192

enum ServerMode { MULTICAST, UNICAST };
typedef enum ServerMode ServerMode;

/* A struct containing metadata for a song */
struct AudioMetaData
{
	char *title;
	char *artist;
	char *album;
};

typedef struct AudioMetaData AudioMetaData;

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
	SOCKET unicastSocket;
	sockaddr_in sin_udp;
	sockaddr_in cInfo;
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

void sendCurrentSongMulti(int song, AudioMetaData *metaData);

#endif
