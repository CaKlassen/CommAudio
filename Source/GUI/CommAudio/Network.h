#ifndef NETWORK_H_
#define NETWORK_H_

#include <string>
#include <WinSock2.h>
#include "MusicBuffer.h"

/* The Multicast IP to connect to */
#define MULTICAST_ADDR "234.5.6.7"

/* Control socket data size */
#define DATA_BUFSIZE 8192

/* An enum representing all possible server modes */
enum ServerMode { NOTHING, UNICAST, MULTICAST };
typedef enum ServerMode ServerMode;

/* A struct containing the complete state of the Client */
struct ClientState
{
    std::string ip;
    int port;
    bool connected;
    ServerMode sMode;
};
typedef struct ClientState ClientState;

struct SocketInfo
{
    OVERLAPPED overlapped;
    SOCKET socket;
    CHAR buffer[DATA_BUFSIZE];
    WSABUF dataBuf;
};

class MainWindow;

namespace Network
{
    void setGUIHandle(MainWindow *window);
}

bool connectControlChannel(ClientState *cData);
void disconnectControlChannel();
bool connectMusic(ClientState *cData, MusicBuffer *musicBuffer);
void streamMusic(ClientState *cData, std::string &song, MusicBuffer *musicBuffer);

namespace ControlSocket
{
    bool recv(SocketInfo* si);
    bool send(SocketInfo* si, std::string msg, sockaddr_in* sin = nullptr);
}

#endif
