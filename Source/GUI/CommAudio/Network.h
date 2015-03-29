#ifndef NETWORK_H_
#define NETWORK_H_

#include <string>


/* The Multicast IP to connect to */
#define MULTICAST_ADDR "234.5.6.7"

/* The size of a data message */
#define MESSAGE_SIZE 512

/* The size of the circular sound buffer */
#define BUFFER_SIZE (MESSAGE_SIZE * 10)


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


void connectControlChannel(ClientState *cData);
void streamMusic(ClientState *cData);
void connectMusic(ClientState *cData);


#endif
