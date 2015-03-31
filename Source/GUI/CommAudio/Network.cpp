/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: Network.cpp - This file contains all networking functionality.
--
-- PROGRAM: CommAudio.exe
--
-- FUNCTIONS:
--
-- DATE: March 17, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- NOTES:
--      The functions in this file are primarily triggered by GUI elements and by
--      messages forwarded to the Control Message Handler.
----------------------------------------------------------------------------------------------------------------------*/

#include <cstdlib>
#include <iostream>
#include <thread>
#include <ws2tcpip.h>

#include "mainwindow.h"
#include "Network.h"
#include "MusicBuffer.h"

using std::cerr;
using std::cout;
using std::endl;

// Network Variables
SOCKET controlSocket;
SOCKADDR_IN controlInfo;

// Unicast Variables
SOCKET unicastStreamSocket;
SOCKADDR_IN unicastStreamInfo;
SOCKADDR_IN unicastServerInfo;

// Multicast Variables
SOCKET multicastSocket;
SOCKADDR_IN multicastInfo;
SOCKADDR_IN multicastServerInfo;
struct ip_mreq multicastInterface;

// Microphone Variables


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: connectContolChannel
--
-- DATE: March 28, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: void connectControlChannel(ClientState *cData);
--
-- PARAMETERS:
--      cData - the struct containing all client state information
--
-- RETURNS: void
--
-- NOTES:
--     This function is responsible for connecting the client to the server
--     via a TCP control channel.
----------------------------------------------------------------------------------------------------------------------*/
void connectControlChannel(ClientState *cData)
{
    int port;
    struct hostent *hp;
    char *host;

    host = (char *) cData->ip.c_str();
    port = cData->port;
    
//    WSADATA stWSAData;
//    if (WSAStartup(0x0202, &stWSAData) != 0)
//    {
//        cerr << "Failed to start WinSock." << endl;
//        exit(1);
//    }
    
    // Create the socket
    if ((controlSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        cerr << "Failed to open control socket." << endl;
        WSACleanup();
        exit(1);
    }

    // Initialize and set up the address structure
    memset((char *)&controlInfo, 0, sizeof(struct sockaddr_in));
    controlInfo.sin_family = AF_INET;
    controlInfo.sin_port = htons(port);
    if ((hp = gethostbyname(host)) == NULL)
    {
        cerr << "Failed to identify server host." << endl;
        WSACleanup();
        exit(1);
    }

    // Copy the server address
    memcpy((char *)&controlInfo.sin_addr, hp->h_addr, hp->h_length);

    // Connecting to the server
    if (::connect(controlSocket, (struct sockaddr *)&controlInfo, sizeof(controlInfo)) == -1)
    {
        cerr << "Failed to connect to the server." << endl;
        WSACleanup();
        return;
    }
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: connectMusic
--
-- DATE: March 17, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: void connectMusic(ClientState *cData);
--
-- PARAMETERS:
--      cData - the struct containing all client state information
--
-- RETURNS: void
--
-- NOTES:
--     This function is responsible for initiating the connection process when
--     the connect button is pressed on the Music tab.
----------------------------------------------------------------------------------------------------------------------*/
void connectMusic(ClientState *cData, MusicBuffer *musicBuffer)
{ 
    if (cData->sMode == UNICAST)
    {
        // Our functionality exists based on GUI elements and callbacks;
        // we don't need to be here
        return;
    }
   
    // Open the multicast socket
    if ((multicastSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
    {
        // The socket failed to be created
        cerr << "Failed to create multicast socket." << endl;
        WSACleanup();
        exit(1);
    }

    // Set the REUSEADDR flag
    bool optionFlag = true;
    if (setsockopt(multicastSocket, SOL_SOCKET, SO_REUSEADDR, (char *) &optionFlag, sizeof(optionFlag)) == SOCKET_ERROR)
    {
        cerr << "Failed to set REUSEADDR flag on multicast socket." << endl;
        WSACleanup();
        exit(1);
    }

    // Bind the socket
    multicastInfo.sin_family = AF_INET;
    multicastInfo.sin_addr.s_addr = htonl(INADDR_ANY);
    multicastInfo.sin_port = htons((cData->port - 1));

    if (bind(multicastSocket, (struct sockaddr*) &multicastInfo, sizeof(multicastInfo)) == SOCKET_ERROR)
    {
        cerr << "Failed to bind multicast socket" << endl;
        WSACleanup();
        exit(1);
    }

    // Join the Multicast Session
    multicastInterface.imr_multiaddr.s_addr = inet_addr(MULTICAST_ADDR);
    multicastInterface.imr_interface.s_addr = INADDR_ANY;

    if (setsockopt(multicastSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP,
        (char *) &multicastInterface, sizeof(multicastInterface)) == SOCKET_ERROR)
    {
        cerr << "Failed to join multicast session." << endl;
        WSACleanup();
        exit(1);
    }

    // Start streaming the audio
    std::thread streamThread(outputAudio, musicBuffer);
    streamThread.detach();
    
    while (!cData->connected)
    {
        // Receive data from the server
        int infoSize = sizeof(struct sockaddr_in);

        int numReceived = 0;
        char tempBuffer[MESSAGE_SIZE];
        
        if ((numReceived = recvfrom(multicastSocket, tempBuffer, MESSAGE_SIZE,
            0, (struct sockaddr*) &multicastServerInfo, &infoSize)) < 0)
        {
            cerr << "Error reading data from multicast socket." << endl;
            continue;
        }
        
        // Add the data to the buffer
        musicBuffer->put(tempBuffer, numReceived);
    }
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: streamMusic
--
-- DATE: March 18, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: void streamMusic(ClientState *cData);
--
-- PARAMETERS:
--      cData - the struct containing the client state info
--
-- RETURNS: void
--
-- NOTES:
--     This function begins listening for a Unicast music stream from the
--     server and plays it.
----------------------------------------------------------------------------------------------------------------------*/
void streamMusic(ClientState *cData)
{
    // Open a UDP listener
    if ((unicastStreamSocket = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
    {
        // The socket failed to be created
        cerr << "Failed to create unicast socket." << endl;
        WSACleanup();
        exit(1);
    }

    // Bind the UDP listener
    unicastStreamInfo.sin_family = AF_INET;
    unicastStreamInfo.sin_port = htons(cData->port + 1);
    unicastStreamInfo.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(unicastStreamSocket, (struct sockaddr *) &unicastStreamInfo, sizeof(unicastStreamInfo)) == -1)
    {
        // The socket failed to be bound
        cerr << "Failed to bind unicast socket." << endl;
        WSACleanup();
        exit(1);
    }

    // While we are still connected
    while(cData->connected)
    {
        int serverInfoSize = sizeof(unicastServerInfo);
        char buffer[MESSAGE_SIZE];
        if (recvfrom(unicastStreamSocket, buffer, MESSAGE_SIZE, 0,(struct sockaddr *) &unicastServerInfo, &serverInfoSize) < 0)
        {
            // The socket data failed to be read
            cerr << "Failed to read from unicast socket." << endl;
            WSACleanup();
            exit(1);
        }
    }
}

