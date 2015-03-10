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

NetVars& GetNetVars()
{
	static NetVars nv;
	return nv;
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
bool openListener(unsigned short int port)
{
	if ((GetNetVars().sock_lisn = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		cerr << "Failed to create TCP listen socket!" << endl;
		return false;
	}

	GetNetVars().server.sin_family = AF_INET;
	GetNetVars().server.sin_addr.s_addr = htonl(INADDR_ANY);
	GetNetVars().server.sin_port = htons(port);

	if (bind(GetNetVars().sock_lisn, (struct sockaddr*)&GetNetVars().server, sizeof(GetNetVars().server)) == SOCKET_ERROR)
	{
		cerr << "Failed to bind the TCP listen socket!" << endl;
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
-- INTERFACE: void acceptConnection();
--
-- PARAMETERS:
--
-- RETURNS: void.
--
-- NOTES:
--     This function accepts an incoming connection request from a client.
----------------------------------------------------------------------------------------------------------------------*/
void acceptConnection()
{
	string startConnMsg;

	GetNetVars().clients.emplace_back(Client{});

	GetNetVars().clients.back().sock_tcp_control
		= accept(GetNetVars().sock_lisn, NULL, NULL);

	createControlString(CMessage{ START_CONNECTION }, startConnMsg);

	send(
		GetNetVars().clients.back().sock_tcp_control
		, startConnMsg.c_str()
		, startConnMsg.length() + 1
		, 0
		);
}