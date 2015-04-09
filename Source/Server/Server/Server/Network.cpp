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
#include <algorithm>
#include "Application.h"
#include "ControlChannel.h"

using namespace std;

/// FUNCTION PROTOTYPES

void CALLBACK onRecv(DWORD error, DWORD bytesTransferred, LPWSAOVERLAPPED overlapped, DWORD inFlags);

/// VARIABLES

vector<Client*> clients;
deque<Client*> pendingUnicastClients;
bool isAlive = false;
AudioMetaData *mData;

/// GENERAL STUFF

bool createSockAddrIn(sockaddr_in& sin, std::string ip, unsigned short port)
{
	//sin = {};
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
-- FUNCTION: sendCurrentSongMulti
--
-- DATE: March 14, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: sendCurrentSongMulti(int song);
--
-- PARAMETERS:
--		song - the song number in the tracklist to send
--
-- RETURNS: void
--
-- NOTES:
--     This function sends the current song to all connected clients.
----------------------------------------------------------------------------------------------------------------------*/
void sendCurrentSongMulti(int song, AudioMetaData *metaData)
{
	mData = metaData;

	CMessage cMsg;
	cMsg.msgType = NOW_PLAYING;

	if (metaData->title == NULL)
	{
		cMsg.msgData.emplace_back("Unknown");
	}
	else
	{
		cMsg.msgData.emplace_back(metaData->title);
	}

	if (metaData->artist == NULL)
	{
		cMsg.msgData.emplace_back("Unknown");
	}
	else
	{
		cMsg.msgData.emplace_back(metaData->artist);
	}

	if (metaData->album == NULL)
	{
		cMsg.msgData.emplace_back("Unknown");
	}
	else
	{
		cMsg.msgData.emplace_back(metaData->album);
	}

	string controlString;
	createControlString(cMsg, controlString);

	// Loop through all clients in the connected client list
	vector<Client*>::iterator it;

	for (it = clients.begin(); it != clients.end(); it++)
	{
		Server::send(*it, controlString);
	}
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
	c->controlSI.socket = acceptedSocket;

	CMessage cMsg;
	cMsg.msgType = START_CONNECTION;
	stringstream ss;
	ss << sMode;
	cMsg.msgData.emplace_back(ss.str());

	createControlString(cMsg, startConnMsg);

	bool success = send(c, startConnMsg);

	if (success)
	{
		cout << "Client connected [" << c->controlSI.socket << "]" << endl;

		// Retrieve the client IP
		int size = sizeof(c->sinTCP);
		if (getpeername(acceptedSocket, (sockaddr *)&c->sinTCP, &size) != 0)
		{
			cerr << "Failed to get peer name." << endl;
		}
		else
		{
			cout << "Peer name: " << inet_ntoa(c->sinTCP.sin_addr) << endl;
		}

		if (sMode == MULTICAST)
		{
			// Send current song
			CMessage cMsg;
			cMsg.msgType = NOW_PLAYING;

			if (mData->title == NULL)
			{
				cMsg.msgData.emplace_back("Unknown");
			}
			else
			{
				cMsg.msgData.emplace_back(mData->title);
			}

			if (mData->artist == NULL)
			{
				cMsg.msgData.emplace_back("Unknown");
			}
			else
			{
				cMsg.msgData.emplace_back(mData->artist);
			}

			if (mData->album == NULL)
			{
				cMsg.msgData.emplace_back("Unknown");
			}
			else
			{
				cMsg.msgData.emplace_back(mData->album);
			}

			string controlString;
			createControlString(cMsg, controlString);

			Server::send(c, controlString);
		}
		else
		{
			// Send tracklist
			CMessage cMsg;
			cMsg.msgType = TRACK_LIST;
			vector<string>* tlist = getTracklist();

			vector<string>::iterator it;
			for (it = tlist->begin(); it != tlist->end(); it++)
			{
				cMsg.msgData.emplace_back(*it);
			}

			string controlString;
			createControlString(cMsg, controlString);

			Server::send(c, controlString);
		}

		recv(c); // start the recursion
	}

	return success;
}

Client* Server::createClient()
{
	clients.emplace_back(new Client());
	return clients.back();
}

std::deque<Client*>& Server::getPendingUnicastClients()
{
	return pendingUnicastClients;
}

bool Server::recv(Client* c)
{
	DWORD bytesReceived = 0;
	DWORD flags = 0;

	//c->controlSI.overlapped = {};
	c->controlSI.dataBuf.len = DATA_BUFSIZE;
	c->controlSI.dataBuf.buf = c->controlSI.buffer;

	if (WSARecv(c->controlSI.socket,
		&(c->controlSI.dataBuf), 1, &bytesReceived, &flags,
		&(c->controlSI.overlapped), onRecv
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

	//c->controlSI.overlapped = {};
	c->controlSI.dataBuf.len = DATA_BUFSIZE;
	strcpy_s(c->controlSI.buffer, msg.c_str());
	c->controlSI.dataBuf.buf = c->controlSI.buffer;

	if (sin)
	{
		if (WSASendTo(c->controlSI.socket,
			&(c->controlSI.dataBuf), 1, &bytesSent, 0,
			(sockaddr*)sin, sizeof(*sin),
			&(c->controlSI.overlapped), NULL
			) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				cerr << "WSASend() failed. Error " << WSAGetLastError() << endl;
				return false;
			}
		}
		cout << "send udp> " << c->controlSI.buffer << endl;
	}
	else
	{
		if (WSASend(c->controlSI.socket,
			&(c->controlSI.dataBuf), 1, &bytesSent, 0,
			&(c->controlSI.overlapped), NULL
			) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				cerr << "WSASend() failed. Error " << WSAGetLastError() << endl;
				return false;
			}
		}
		cout << "send tcp> " << c->controlSI.buffer << endl;
	}

	return true;
}


void CALLBACK onRecv(DWORD error, DWORD bytesTransferred, LPWSAOVERLAPPED overlapped, DWORD inFlags)
{
	CMessage cm;

	Client* C = (Client*)overlapped;

	if (bytesTransferred == 0 || error == 10054)
	{
		cout << "Client disconnected [" << C->controlSI.socket << "]" << endl;
	}
	else if (error != 0)
	{
		cerr << "I/O operation failed. Error " << error << endl;
	}

	if (error != 0 || bytesTransferred == 0)
	{
		closesocket(C->controlSI.socket);
		clients.erase(std::remove(clients.begin(), clients.end(), C), clients.end());
		pendingUnicastClients.erase(std::remove(pendingUnicastClients.begin(), pendingUnicastClients.end(), C), pendingUnicastClients.end());
		delete C; C = nullptr;
		return;
	}

	cout << "recv> " << C->controlSI.buffer << endl;

	parseControlString(std::string(C->controlSI.buffer), &cm);
	handleControlMessage(&cm, C);

	Server::recv(C);
}
