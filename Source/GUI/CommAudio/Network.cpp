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
#include <WS2tcpip.h>

#include "Network.h"
#include "mainwindow.h"
#include "MusicBuffer.h"
#include "ControlChannel.h"
#include "micoutput.h"

using std::cerr;
using std::cout;
using std::endl;

namespace Network
{
    MainWindow *GUI;
}

// Network Variables
SOCKET controlSocket;
SOCKADDR_IN controlInfo;
SocketInfo* serverCtrlSockInfo;

// Unicast Variables
SOCKET unicastStreamSocket;
SOCKADDR_IN unicastStreamInfo;
SOCKADDR_IN unicastServerInfo;

// Multicast Variables
SOCKET multicastSocket;
SOCKADDR_IN multicastInfo;
SOCKADDR_IN multicastServerInfo;
struct ip_mreq multicastInterface;


// Functions

void Network::setGUIHandle(MainWindow *window)
{
    GUI = window;
}

void CALLBACK onRecv(DWORD error, DWORD bytesTransferred, LPWSAOVERLAPPED overlapped, DWORD inFlags);

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
bool connectControlChannel(ClientState *cData)
{
    int port;
    struct hostent *hp;
    char *host;

    host = (char *) cData->ip.c_str();
    port = cData->port;

    // Create the socket
    if ((controlSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        cerr << "Failed to open control socket." << endl;
        return false;
    }

    // Initialize and set up the address structure
    memset((char *)&controlInfo, 0, sizeof(struct sockaddr_in));
    controlInfo.sin_family = AF_INET;
    controlInfo.sin_port = htons(port);
    if ((hp = gethostbyname(host)) == NULL)
    {
        cerr << "Failed to identify server host." << endl;
        return false;
    }

    // Copy the server address
    memcpy((char *)&controlInfo.sin_addr, hp->h_addr, hp->h_length);

    // Connecting to the server
    if (::connect(controlSocket, (struct sockaddr *)&controlInfo, sizeof(controlInfo)) == -1)
    {
        cerr << "Failed to connect to the server." << endl;
        return false;
    }

    serverCtrlSockInfo = new SocketInfo();
    serverCtrlSockInfo->socket = controlSocket;

    ControlSocket::recv(serverCtrlSockInfo);

    cout << "Connected!" << endl;

    return true;
}

void disconnectControlChannel()
{
    closesocket(controlSocket);
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: requestSaveSong
--
-- DATE: April 7, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: bool requestSaveSong(string controlString);
--
-- PARAMETERS:
--      controlString - the control string to send to the server
--
-- RETURNS: void
--
-- NOTES:
--     This function sends the server a request to save a file.
----------------------------------------------------------------------------------------------------------------------*/
void requestSaveSong(string controlString)
{
    ControlSocket::send(serverCtrlSockInfo, controlString);
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
bool connectMusic(ClientState *cData, MusicBuffer *musicBuffer)
{
    // Open the multicast socket
    if ((multicastSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
    {
        // The socket failed to be created
        cerr << "Failed to create multicast socket." << endl;
        return false;
    }

    // Set the REUSEADDR flag
    bool optionFlag = true;
    if (setsockopt(multicastSocket, SOL_SOCKET, SO_REUSEADDR, (char *) &optionFlag, sizeof(optionFlag)) == SOCKET_ERROR)
    {
        cerr << "Failed to set REUSEADDR flag on multicast socket." << endl;
        return false;
    }

    // Bind the socket
    multicastInfo.sin_family = AF_INET;
    multicastInfo.sin_addr.s_addr = htonl(INADDR_ANY);
    multicastInfo.sin_port = htons((cData->port - 1));

    if (bind(multicastSocket, (struct sockaddr*) &multicastInfo, sizeof(multicastInfo)) == SOCKET_ERROR)
    {
        cerr << "Failed to bind multicast socket" << endl;
        return false;
    }

    // Join the Multicast Session
    multicastInterface.imr_multiaddr.s_addr = inet_addr(MULTICAST_ADDR);
    multicastInterface.imr_interface.s_addr = INADDR_ANY;

    if (setsockopt(multicastSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP,
        (char *) &multicastInterface, sizeof(multicastInterface)) == SOCKET_ERROR)
    {
        cerr << "Failed to join multicast session." << endl;
        return false;
    }

    // Start streaming the audio
    std::thread startAudioOutputThread(outputAudio, musicBuffer);
    // TODO constrain the above thread creation to only happen if the WaveCallback() have been stopped
    // TODO or else there will be multiple outputs if the user spams the connect/disconnect button.
    
    // While we are still connected
    while (cData->connected)
    {
        // Receive data from the server
        int infoSize = sizeof(struct sockaddr_in);

        int numReceived = 0;
        char tempBuffer[MESSAGE_SIZE];
        
        numReceived = recvfrom(multicastSocket, tempBuffer, MESSAGE_SIZE,
            0, (struct sockaddr*) &multicastServerInfo, &infoSize);
        
        // Add the data to the buffer
        musicBuffer->put(tempBuffer, numReceived);
    }

    startAudioOutputThread.join(); // wait for the thread to finish

    closesocket(multicastSocket);

    return true;
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
-- INTERFACE: void streamMusic(ClientState *cData, string &song);
--
-- PARAMETERS:
--      cData - the struct containing the client state info
--      song - the song name to request
--
-- RETURNS: void
--
-- NOTES:
--     This function begins listening for a Unicast music stream from the
--     server and plays it.
----------------------------------------------------------------------------------------------------------------------*/
void streamMusic(ClientState *cData, string &song, MusicBuffer *musicBuffer, bool *songDone)
{
    // Send the server a request
    CMessage cMsg;
    cMsg.msgType = PLAY_SONG;
    cMsg.msgData.emplace_back(song);
    
    string controlString;
    createControlString(&cMsg, &controlString);
    
    ControlSocket::send(serverCtrlSockInfo, controlString);
    
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
    
    std::thread startAudioOutputThread(outputAudio, musicBuffer);
    
    // While we are still connected
    while(!(*songDone) && cData->connected)
    {
        int serverInfoSize = sizeof(unicastServerInfo);
        int numReceived = 0;
        char tempBuffer[MESSAGE_SIZE];
        
        if (!(*songDone) && (numReceived = recvfrom(unicastStreamSocket, tempBuffer, MESSAGE_SIZE, 0,(struct sockaddr *) &unicastServerInfo, &serverInfoSize)) < 0)
        {
            // The socket data failed to be read
            cerr << "Failed to read from unicast socket." << endl;
        }
                
        // Add the data to the buffer
        musicBuffer->put(tempBuffer, numReceived);
    }
    
    startAudioOutputThread.join(); // wait for the thread to finish
    cout << "Finished unicast stream" << endl;
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: disconnectUnicast
--
-- DATE: April 6, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: void disconnectUnicast();
--
-- PARAMETERS:
--
-- RETURNS: void
--
-- NOTES:
--     This function closes the unicast socket once a song has been streamed.
----------------------------------------------------------------------------------------------------------------------*/
void disconnectUnicast()
{
    closesocket(unicastStreamSocket);
}


bool ControlSocket::recv(SocketInfo* si)
{
    DWORD bytesReceived = 0;
    DWORD flags = 0;

    //si->overlapped = {};
    si->dataBuf.len = DATA_BUFSIZE;
    si->dataBuf.buf = si->buffer;

    if (WSARecv(si->socket,
        &(si->dataBuf), 1, &bytesReceived, &flags,
        &(si->overlapped), onRecv
        ) == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSA_IO_PENDING)
        {
            cerr << "WSARecv() failed. Error " << WSAGetLastError() << endl;
            return false;
        }
    }

    return true;
}

bool ControlSocket::send(SocketInfo* si, std::string msg, sockaddr_in* sin)
{
    DWORD bytesSent = 0;

    //si->overlapped = {};
    si->dataBuf.len = DATA_BUFSIZE;
    strcpy_s(si->buffer, msg.c_str());
    si->dataBuf.buf = si->buffer;

    if (sin)
    {
        if (WSASendTo(si->socket,
            &(si->dataBuf), 1, &bytesSent, 0,
            (sockaddr*)sin, sizeof(*sin),
            &(si->overlapped), NULL
            ) == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSA_IO_PENDING)
            {
                cerr << "WSASend() failed. Error " << WSAGetLastError() << endl;
                return false;
            }
        }
        cout << "send udp> " << si->buffer << endl;
    }
    else
    {
        if (WSASend(si->socket,
            &(si->dataBuf), 1, &bytesSent, 0,
            &(si->overlapped), NULL
            ) == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSA_IO_PENDING)
            {
                cerr << "WSASend() failed. Error " << WSAGetLastError() << endl;
                return false;
            }
        }
        cout << "send tcp> " << si->buffer << endl;
    }

    return true;
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: startMicrophone
--
-- DATE: April 2, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: void startMicrophone(ClientState *cData);
--
-- PARAMETERS:
--
-- RETURNS: void
--
-- NOTES:
--     This function opens a receiving and sending socket for client to client
--     microphone transfer.
----------------------------------------------------------------------------------------------------------------------*/
void startMicrophone(ClientState *cData, MicOutput *micOutput)
{
    cData->connected = true;
    
    // Start sending
    std::thread sendMic(sendMicrophone);
    sendMic.detach();
    
    micOutput->setData(cData);
    micOutput->startListening();
}


void CALLBACK onRecv(DWORD error, DWORD bytesTransferred, LPWSAOVERLAPPED overlapped, DWORD inFlags)
{
    CMessage cm;

    SocketInfo* SI = (SocketInfo*)overlapped;

    if (bytesTransferred == 0 || error == 10054)
    {
        cout << "Disconnected!" << endl;
    }
    else if (error != 0)
    {
        cerr << "I/O operation failed. Error " << error << endl;
    }

    if (error != 0 || bytesTransferred == 0)
    {
        Network::GUI->disconnectIt();

        closesocket(SI->socket);
        delete SI;
        return;
    }

    cout << "recv> " << SI->buffer << endl;

    parseControlString(std::string(SI->buffer), &cm);
    handleControlMessage(&cm);

    // TODO Check if (cdata->connected): if true then recurse
    ControlSocket::recv(SI);
}
