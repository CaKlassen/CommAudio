/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: Network.cpp - all functions related to socket interfacing.
--
-- PROGRAM: Server.exe
--
-- FUNCTIONS:
--
-- DATE: March 7, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- NOTES:
--     This file contains functions used to interface with the WinSock socket API.
----------------------------------------------------------------------------------------------------------------------*/

#include "Network.h"

#include <iostream>
#include "ControlChannel.h"

using namespace std;

void CALLBACK onRecv(DWORD Error, DWORD BytesTransferred, LPWSAOVERLAPPED overlapped, DWORD InFlags);

vector<Client> clients;
bool isAlive = false;

void Server::start()
{
	::isAlive = true;
}

void Server::tearDown()
{
	::isAlive = false;
}

bool Server::isAlive()
{
	return ::isAlive;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: openListener
--
-- DATE: March 9, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Melvin Loho
--
-- PROGRAMMER: Melvin Loho
--
-- INTERFACE: bool openListener();
--
-- PARAMETERS:
--
-- RETURNS: bool - whether or not the listener was opened successfully.
--
-- NOTES:
--     This function opens a TCP listener socket.
----------------------------------------------------------------------------------------------------------------------*/
bool Server::openListener(SOCKET& listenSocket, unsigned short int port)
{
	if ((listenSocket = WSASocketW(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED))
		== INVALID_SOCKET)
	{
		cerr << "Failed to create the listening socket. Error: " << WSAGetLastError() << endl;
		return false;
	}

	SOCKADDR_IN server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(port);

	if (bind(listenSocket, (SOCKADDR*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		cerr << "Failed to bind the listening socket. Error: " << WSAGetLastError() << endl;
		return false;
	}

	if (listen(listenSocket, 5))
	{
		cerr << "Failed to listen on the listening socket. Error: " << WSAGetLastError() << endl;
		return false;
	}

	return true;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: acceptConnection
--
-- DATE: March 9, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Melvin Loho
--
-- PROGRAMMER: Melvin Loho
--
-- INTERFACE: Client acceptConnection();
--
-- PARAMETERS:
--
-- RETURNS: void.
--
-- NOTES:
--     This function accepts an incoming client connection request.
----------------------------------------------------------------------------------------------------------------------*/
bool Server::acceptConnection(SOCKET listenSocket)
{
	Client& c = createClient();
	string startConnMsg;

	c.socketinfo.socket = accept(listenSocket, NULL, NULL);
	createControlString(CMessage{ START_CONNECTION }, startConnMsg);

	send(c, startConnMsg);
}

Client& Server::createClient()
{
	clients.emplace_back();
	return clients.back();
}

bool Server::send(Client& c, std::string msg)
{
	DWORD bytesSent = 0;

	c.socketinfo.overlapped = { 0 };
	c.socketinfo.dataBuf.len = DATA_BUFSIZE;
	msg.copy(c.socketinfo.buffer, DATA_BUFSIZE);
	c.socketinfo.dataBuf.buf = c.socketinfo.buffer;

	if (WSASend(c.socketinfo.socket,
		&(c.socketinfo.dataBuf), 1, &bytesSent, 0,
		&(c.socketinfo.overlapped), NULL
		) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			cerr << "Failed to WSASend(). Error " << WSAGetLastError() << endl;
			return false;
		}
	}

	return true;
}

bool Server::recv(Client& c, std::string msg)
{
	DWORD bytesReceived = 0;
	DWORD Flags = 0;

	c.socketinfo.overlapped = { 0 };
	c.socketinfo.dataBuf.len = DATA_BUFSIZE;
	c.socketinfo.dataBuf.buf = c.socketinfo.buffer;

	if (WSARecv(c.socketinfo.socket,
		&(c.socketinfo.dataBuf), 1, &bytesReceived, &Flags,
		&(c.socketinfo.overlapped), onRecv
		) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			cout << "WSARecv() failed with error " << WSAGetLastError() << endl;
			return false;
		}
	}

	return true;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: disconnectClient
--
-- DATE: March 14, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: void disconnectClient(int socket);
--
-- PARAMETERS:
--		socket - the socket number for the disconnecting client
--
-- RETURNS: void
--
-- NOTES:
--     This function removes a client from the list of connected clients.
----------------------------------------------------------------------------------------------------------------------*/
void Server::disconnectClient(string ip)
{
	// Close the connection

	// Remove the client from the list of clients
}

void CALLBACK onRecv(DWORD Error, DWORD BytesTransferred, LPWSAOVERLAPPED overlapped, DWORD InFlags)
{

}