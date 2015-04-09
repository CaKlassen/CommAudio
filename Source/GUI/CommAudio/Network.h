#ifndef NETWORK_H_
#define NETWORK_H_

#include <string>
#include <WinSock2.h>
#include "MusicBuffer.h"
#include "micoutput.h"

/* The Multicast IP to connect to */
#define MULTICAST_ADDR "234.5.6.7"

/* Control socket data size */
#define DATA_BUFSIZE 8192

/* The size of a packet for file saving */
#define SAVE_SIZE 512

class MicOutput;

/* An enum representing all possible server modes */
enum ServerMode { NOTHING, UNICAST, MULTICAST };
typedef enum ServerMode ServerMode;

/* A struct containing the complete state of the Client */
struct ClientState
{
    std::string ip;
    int port;
    std::string dlFilePath;
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
void disconnectAll();
bool connectMusic(ClientState *cData, MusicBuffer *musicBuffer);
void streamMusic(ClientState *cData, std::string &song, MusicBuffer *musicBuffer, bool *songDone);
void startMicrophone(ClientState *cData, MicOutput *micOutput);
void requestSaveSong(std::string controlString);

namespace ControlSocket
{
    bool recv(SocketInfo* si);
    bool send(SocketInfo* si, std::string msg, sockaddr_in* sin = nullptr);
}

#endif
