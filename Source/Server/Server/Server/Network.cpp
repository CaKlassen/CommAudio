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
#include <sstream>
#include "ControlChannel.h"

using namespace std;

/// FUNCTION PROTOTYPES

void CALLBACK onRecv(DWORD error, DWORD bytesTransferred, LPWSAOVERLAPPED overlapped, DWORD inFlags);

/// VARIABLES

vector<Client*> clients;
bool isAlive = false;

/// GENERAL STUFF

bool createSockAddrIn(sockaddr_in& sin, std::string ip, unsigned short port)
{
	sin = {};
	sin.sin_family = AF_INET;   // Specify the Internet (TCP/IP) Address family
	sin.sin_port = htons(port); // Convert to network byte order

	// Ensure that the IP string is a legitimate address (dotted decimal)
	if ((sin.sin_addr.s_addr = inet_addr(ip.c_str())) == INADDR_NONE)
	{
		cerr << "Invalid IP address" << endl;
		return false;
	}

	return true;
}

/// SERVER STUFF

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
-- DATE: March 16, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Melvin Loho
--
-- PROGRAMMER: Melvin Loho
--
-- INTERFACE: bool Server::openListener(SOCKET& listenSocket, unsigned short int port)
--
-- PARAMETERS:
--		listenSocket - The socket to be assigned as the listening socket
--		port - the port number to listen on
--
-- RETURNS: bool - whether or not the listener was opened successfully
--
-- NOTES:
--     This function opens a TCP listener socket.
----------------------------------------------------------------------------------------------------------------------*/
bool Server::openListener(SOCKET& listenSocket, unsigned short int port)
{
	if ((listenSocket = WSASocketW(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED))
		== INVALID_SOCKET)
	{
		cerr << "Failed to create the listening socket. Error " << WSAGetLastError() << endl;
		return false;
	}

	SOCKADDR_IN server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(port);

	if (bind(listenSocket, (SOCKADDR*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		cerr << "Failed to bind the listening socket. Error " << WSAGetLastError() << endl;
		return false;
	}

	if (listen(listenSocket, 5))
	{
		cerr << "Failed to listen on the listening socket. Error " << WSAGetLastError() << endl;
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
--			   Chris Klassen
--
-- INTERFACE: bool Server::acceptConnection(SOCKET listenSocket, ServerMode sMode)
--
-- PARAMETERS:
--		listenSocket - the socket to accept connections from
--		sMode - the server's current mode
--
-- RETURNS: bool - whether the "start connection" message was sent successfuly to the client after their acceptance
--
-- NOTES:
--     This function accepts an incoming client connection request.
----------------------------------------------------------------------------------------------------------------------*/
bool Server::acceptConnection(SOCKET listenSocket, ServerMode sMode)
{
	Client* c = nullptr;
	string startConnMsg;
	SOCKET acceptedSocket;

	acceptedSocket = accept(listenSocket, NULL, NULL);
	c = createClient();
	c->socketinfo.socket = acceptedSocket;

	CMessage cMsg;
	cMsg.msgType = START_CONNECTION;
	stringstream ss;
	ss << sMode;
	cMsg.msgData.emplace_back(ss.str());

	createControlString(cMsg, startConnMsg);

	bool success = send(c, startConnMsg);

	if (success)
	{
		cout << "CLIENT CONNECTED" << endl;
	}

	recv(c); // start the recursion

	return success;
}

Client* Server::createClient()
{
	clients.emplace_back(new Client());
	return clients.back();
}

bool Server::recv(Client* c)
{
	DWORD bytesReceived = 0;
	DWORD flags = 0;

	c->socketinfo.overlapped = {};
	c->socketinfo.dataBuf.len = DATA_BUFSIZE;
	c->socketinfo.dataBuf.buf = c->socketinfo.buffer;

	if (WSARecv(c->socketinfo.socket,
		&(c->socketinfo.dataBuf), 1, &bytesReceived, &flags,
		&(c->socketinfo.overlapped), onRecv
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

bool Server::send(Client* c, std::string msg, sockaddr_in* sin)
{
	DWORD bytesSent = 0;

	c->socketinfo.overlapped = {};
	c->socketinfo.dataBuf.len = DATA_BUFSIZE;
	strcpy_s(c->socketinfo.buffer, msg.c_str());
	c->socketinfo.dataBuf.buf = c->socketinfo.buffer;

	if (sin)
	{
		if (WSASendTo(c->socketinfo.socket,
			&(c->socketinfo.dataBuf), 1, &bytesSent, 0,
			(sockaddr*)sin, sizeof(*sin),
			&(c->socketinfo.overlapped), NULL
			) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				cerr << "WSASend() failed. Error " << WSAGetLastError() << endl;
				return false;
			}
		}
		cout << "send udp> " << c->socketinfo.buffer << endl;
	}
	else
	{
		if (WSASend(c->socketinfo.socket,
			&(c->socketinfo.dataBuf), 1, &bytesSent, 0,
			&(c->socketinfo.overlapped), NULL
			) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				cerr << "WSASend() failed. Error " << WSAGetLastError() << endl;
				return false;
			}
		}
		cout << "send tcp> " << c->socketinfo.buffer << endl;
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

void CALLBACK onRecv(DWORD error, DWORD bytesTransferred, LPWSAOVERLAPPED overlapped, DWORD inFlags)
{
	CMessage cm;

	Client* C = (Client*)overlapped;

	if (error != 0)
	{
		cerr << "I/O operation failed. Error " << error << endl;
	}

	if (bytesTransferred == 0)
	{
		cout << "Closing socket " << C->socketinfo.socket << endl;
	}

	if (error != 0 || bytesTransferred == 0)
	{
		closesocket(C->socketinfo.socket);
		delete C;
		return;
	}

	cout << "recv> " << C->socketinfo.buffer << endl;

	parseControlString(std::string(C->socketinfo.buffer), &cm);
	handleControlMessage(&cm, C);

	Server::recv(C);
}
