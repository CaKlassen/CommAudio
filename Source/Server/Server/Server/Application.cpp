/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: Application.cpp - This file contains all the server logic.
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
--     This file contains all functions related to starting, closing and running the server.
----------------------------------------------------------------------------------------------------------------------*/

#include "Application.h"

#include <iostream>
#include <cstdlib>
#include <map>
#include <thread>
#include "dirent.h"

#include "Network.h"
#include "ControlChannel.h"

using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::map;
using std::string;

// Server variables
ServerMode sMode;
SOCKET listeningSocket;
int port;
vector<string> tracklist;
bool done = false;

// Socket variables
WSADATA wsaData;

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: main
--
-- DATE: March 7, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: int main(int argc, char* argv[]);
--
-- PARAMETERS:
--     argc - the number of command line arguments
--	   argv - a 2D array of the arguments
--
-- RETURNS: int - the exit code of the program
--
-- NOTES:
--     This is the main function of the server. It starts WinSock, creates all basic
--     structures, and calls the appropriate half of the server code.
----------------------------------------------------------------------------------------------------------------------*/
int main(int argc, char* argv[])
{
	// Validate and parse command line arguments
	if (argc != 3)
	{
		cerr << "Invalid command line arguments." << endl;
		cerr << "Usage: Server.exe [0 - multicast/1 - unicast] [port]" << endl << endl;

		return 1;
	}

	sMode = (ServerMode)atoi(argv[1]);
	port = atoi(argv[2]);

	// Start WinSock
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSAStartup(wVersionRequested, &wsaData);

	// Load the initial tracklist
	if (!loadTracklist(&tracklist, MUSIC_LOCATION))
	{
		cerr << "Unable to load music tracklist." << endl;
#ifndef DEBUG
		WSACleanup();

		return 1;
#endif
	}

	// Open the TCP listener
	if (!Server::openListener(listeningSocket, port))
	{
		cerr << "Failed to open TCP listener." << endl;
		WSACleanup();

		return 1;
	}

	// Enter the intended half of the program
	if (sMode == MULTICAST)
	{
		if (!startMulticast())
		{
			cerr << "Error with Multicast mode." << endl;
			WSACleanup();

			return 1;
		}
	}
	else
	{
		if (!startUnicast())
		{
			cerr << "Error with Unicast mode." << endl;
			WSACleanup();

			return 1;
		}
	}

	// Stop WinSock
	WSACleanup();

	return 0;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: loadTracklist
--
-- DATE: March 7, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: bool loadTracklist(vector<string> *tlist, string location);
--
-- PARAMETERS:
--     tlist - the destination for the tracklist
--     location - the relative path to search in
--
-- RETURNS: bool - whether or not the tracklist could be loaded
--
-- NOTES:
--     This function retrieves the music tracklist and loads it into the program.
--
--     This function uses structures and functions written by Toni Ronkko (dirent.h)
----------------------------------------------------------------------------------------------------------------------*/
bool loadTracklist(vector<string> *tlist, string location)
{
	DIR *dir;
	struct dirent *entry;

	// Clear the tracklist
	tlist->clear();

	// Open the music directory
	if ((dir = opendir(location.c_str())) != NULL)
	{

		// Find all files in the directory
		while ((entry = readdir(dir)) != NULL)
		{
			if (entry->d_type == DT_REG)
			{
				tlist->emplace_back(entry->d_name);
			}
		}

		// Close the music directory
		closedir(dir);
	}
	else
	{
		// Directory not found
		return false;
	}

	// Check if we have any songs
	if (tlist->empty())
	{
		return false;
	}

	return true;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: startMulticast
--
-- DATE: March 7, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: startMulticast();
--
-- PARAMETERS:
--
-- RETURNS: bool - whether or not there was an error in the multicast mode
--
-- NOTES:
--     This function handles the start of the multicast mode.
----------------------------------------------------------------------------------------------------------------------*/
bool startMulticast()
{
	// Open the multicast socket

	// Connect to the multicast group

	// Play music in a loop
	if (!playMulticast())
	{
		return false;
	}

	return true;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: playMulticast
--
-- DATE: March 14, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: playMulticast();
--
-- PARAMETERS:
--
-- RETURNS: bool - whether or not there was an error while playing multicast music
--
-- NOTES:
--     This function handles the multicast play loop.
----------------------------------------------------------------------------------------------------------------------*/
bool playMulticast()
{
	while (!done)
	{
		// Pick a random song
		int randSong = rand() % tracklist.size();

		// Open the song file

		// Start the Send Current Song thread
		std::thread tCurrentSong(sendCurrentSongMulti, randSong);
		tCurrentSong.detach();

		// While we are not done and there is data left to send
		while (!done)
		{
			// Send part of the song to the multicast socket
		}


		// Reload the tracklist
		if (!done)
		{
			if (!loadTracklist(&tracklist, MUSIC_LOCATION))
			{
				return false;
			}
		}
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
void sendCurrentSongMulti(int song)
{
	CMessage cMsg;
	cMsg.msgType = NOW_PLAYING;

	// Loop through all clients in the connected client list


}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: sendUnicast
--
-- DATE: March 14, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: void sendUnicast(string ip);
--
-- PARAMETERS:
--		ip - the requesting client's ip
--      song - the requested song
--
-- RETURNS: void
--
-- NOTES:
--     This function streams a song to a client via UDP
----------------------------------------------------------------------------------------------------------------------*/
void playUnicast(Client *c, string ip, string song)
{
	if (createSockAddrIn(c->sin_udp, ip, port + 1))
	{
		std::thread tSendUnicast(sendCurrentSongUni, c, song, false);
		tSendUnicast.detach();
	}
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: saveUnicast
--
-- DATE: March 14, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: void saveUnicast(string ip, string song);
--
-- PARAMETERS:
--		ip - the ip of the requesting client
--      song - the requested song
--
-- RETURNS: void
--
-- NOTES:
--     This function sends a song via TCP to a requesting client.
----------------------------------------------------------------------------------------------------------------------*/
void saveUnicast(Client *c, string ip, string song)
{
	std::thread tSendUnicast(sendCurrentSongUni, c, song, true);
	tSendUnicast.detach();
}

void sendCurrentSongUni(Client *c, string song, bool usingTCP)
{
	CMessage cMsg;
	cMsg.msgType = NOW_PLAYING;

	std::string msg;

	createControlString(cMsg, msg);

	if (usingTCP)
	{
		Server::send(c, msg);
	}
	else
	{
		Server::send(c, msg, &c->sin_udp);
	}
}

void thread_runserver()
{
	while (Server::isAlive())
	{
		Server::acceptConnection(listeningSocket);
	}
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: startUnicast
--
-- DATE: March 7, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: startUnicast();
--
-- PARAMETERS:
--
-- RETURNS: bool - whether or not there was an error in the unicast mode
--
-- NOTES:
--     This function handles the start of the unicast mode.
----------------------------------------------------------------------------------------------------------------------*/
bool startUnicast()
{
	Server::start();

	std::thread t_runserver(thread_runserver);
	t_runserver.detach();

	getchar();

	Server::tearDown();

	return true;
}
