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

void CALLBACK completionRoutine(
	DWORD dwError,
	DWORD cbTransferred,
	LPWSAOVERLAPPED lpOverlapped,
	DWORD dwFlags
	);

vector<Client> clients;
bool isAlive = false;

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: getNetVars
--
-- DATE: March 9, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Melvin Loho
--
-- PROGRAMMER: Melvin Loho
--
-- INTERFACE: NetVars& getNetVars();
--
-- PARAMETERS:
--
-- RETURNS: NetVars - The "global" NetVars structure.
--
-- NOTES:
--     This function retrieves the global NetVars structure that is used to store networking variables.
----------------------------------------------------------------------------------------------------------------------*/
NetVars& getNetVars()
{
	static NetVars nv;
	return nv;
}

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
	return isAlive;
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
bool Server::openListener(unsigned short int port)
{
	if ((getNetVars().sock_lisn = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		cerr << "Failed to create TCP listen socket!" << endl;
		return false;
	}

	getNetVars().server.sin_family = AF_INET;
	getNetVars().server.sin_addr.s_addr = htonl(INADDR_ANY);
	getNetVars().server.sin_port = htons(port);

	if (bind(getNetVars().sock_lisn, (struct sockaddr*)&getNetVars().server, sizeof(getNetVars().server)) == SOCKET_ERROR)
	{
		cerr << "Failed to bind the TCP listen socket!" << endl;
		return false;
	}

	if (listen(getNetVars().sock_lisn, 5))
	{
		cerr << "Failed to start listening!" << endl;
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
void Server::acceptConnection()
{
	Client& c = createClient();
	string startConnMsg;

	c.sock_tcp_control = accept(getNetVars().sock_lisn, NULL, NULL);
	createControlString(CMessage{ START_CONNECTION }, c.buffer);

	send(c, CONTROL);
}

Client& Server::createClient()
{
	clients.emplace_back();
	return clients.back();
}

bool Server::send(Client& c, ClientSocket cs)
{
	DWORD bytesSent = 0;

	c.overlapped = { 0 };
	c.dataBuf.len = c.buffer.length();
	c.dataBuf.buf = &c.buffer.at(0);

	if (WSASend(
		cs == CONTROL ? c.sock_tcp_control :
		cs == STREAM ? c.sock_udp_stream :
		cs == DOWNLOAD ? c.sock_tcp_download :
		NULL,
		&(c.dataBuf), 1, &bytesSent, 0,
		&(c.overlapped), completionRoutine
		) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			cerr << "WSASend() failed with error " << WSAGetLastError() << endl;
			return false;
		}
	}

	return true;
}

bool Server::recv(Client& c, ClientSocket cs)
{
	DWORD bytesReceived = 0;
	DWORD Flags = 0;

	c.overlapped = { 0 };
	c.dataBuf.len = c.buffer.length();
	c.dataBuf.buf = &c.buffer.at(0);

	if (WSARecv(
		cs == CONTROL ? c.sock_tcp_control :
		cs == STREAM ? c.sock_udp_stream :
		cs == DOWNLOAD ? c.sock_tcp_download :
		NULL,
		&(c.dataBuf), 1, &bytesReceived, &Flags,
		&(c.overlapped), completionRoutine) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			cout << "WSARecv() failed with error " << WSAGetLastError() << endl;
			return false;
		}
	}

	return true;
}

void CALLBACK completionRoutine(
	DWORD dwError,
	DWORD cbTransferred,
	LPWSAOVERLAPPED lpOverlapped,
	DWORD dwFlags
	)
{

}