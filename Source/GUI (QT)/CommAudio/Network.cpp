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
#include <WinSock2.h>
#include <ws2tcpip.h>
#include "Network.h"

using std::cerr;
using std::endl;

// Network Variables
SOCKET controlSocket;
char musicBuffer[BUFFER_SIZE];

// Unicast Variables

// Multicast Variables
SOCKET multicastSocket;
SOCKADDR_IN multicastInfo;
SOCKADDR_IN multicastServerInfo;
struct ip_mreq multicastInterface;

// Microphone Variables



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
void connectMusic(ClientState *cData)
{
    // While we are waiting for the server to tell us what state it is in
    while (cData->sMode == NOTHING)
    {
        // Do nothing
    }

    if (cData->sMode == UNICAST)
    {
        // Our functionality exists based on GUI elements and callbacks;
        // we don't need to be here
        return;
    }

    // Open the multicast socket
    if ((multicastSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
    {
        // The socket failed to br created
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
    multicastInfo.sin_port = htons(cData->port);

    if (bind(multicastSocket, (struct sockaddr*) &multicastInfo, sizeof(multicastInfo)) == SOCKET_ERROR)
    {
        cerr << "Failed to bind multicast socket" << endl;
        WSACleanup();
        exit(1);
    }

    // Join the Multicast Session
    multicastInterface.imr_multiaddr.s_addr = inet_addr(cData->ip.c_str());
    multicastInterface.imr_interface.s_addr = INADDR_ANY;

    if (setsockopt(multicastSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP,
        (char *) &multicastInterface, sizeof(multicastInterface)) == SOCKET_ERROR)
    {
        cerr << "Failed to join multicast session." << endl;
        WSACleanup();
        exit(1);
    }

    while (cData->connected)
    {
        // Receive data from the server
        int infoSize = sizeof(struct sockaddr_in);

        if (recvfrom(multicastSocket, musicBuffer, MESSAGE_SIZE,
            0, (struct sockaddr*) &multicastServerInfo, &infoSize) < 0)
        {
            cerr << "Error reading data from multicast socket." << endl;
            continue;
        }
    }
}


