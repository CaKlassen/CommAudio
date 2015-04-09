#ifndef NETWORK_H_
#define NETWORK_H_

#include <WinSock2.h>
#include <vector>
#include <deque>

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
	SOCKET_INFORMATION controlSI;
	SOCKET unicastSocket;
	sockaddr_in sinTCP, sinUDP;
	bool requestFinished;
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
	std::deque<Client*>& getPendingUnicastClients();
	bool recv(Client* c);
	bool send(Client* c, std::string msg, sockaddr_in* sin = nullptr);
}

void sendCurrentSongMulti(int song, AudioMetaData *metaData);
void endCurrentSong();

#endif
