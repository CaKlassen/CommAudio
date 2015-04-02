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
#include <sstream>
#include <cstdlib>
#include <map>
#include <thread>
#include <WS2tcpip.h>

#include "dirent.h"

#include "Network.h"
#include "ControlChannel.h"
#include "MusicBuffer.h"

using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::map;
using std::string;
using std::stringstream;

// Server variables
ServerMode sMode;
SOCKET listeningSocket;
int port;
vector<string> tracklist;
bool done = false;

// Audio variable
libvlc_instance_t *inst;
libvlc_media_player_t *mediaPlayer;
libvlc_media_t *media;
AudioMetaData metaData;

// Socket variables
WSADATA wsaData;
SOCKET multicastSocket;
SOCKADDR_IN multicastInfo;
SOCKADDR_IN multicastDestInfo;
struct ip_mreq multicastInterface;

void threadCtrlSocketLoop()
{
	cout << "Listening... " << endl;

	while (Server::isAlive())
	{
		Server::acceptConnection(listeningSocket, sMode);
	}
}

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

	// Randomize the seed
	srand(time(NULL));

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

	// Start LibVLC
	startLibVLC();

	// Open the TCP listener
	if (!Server::openListener(listeningSocket, port))
	{
		cerr << "Failed to open TCP listener." << endl;
		WSACleanup();

		return 1;
	}

	Server::start();

	std::thread tCSL(threadCtrlSocketLoop);
	tCSL.detach();

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

	Server::tearDown();

	// Release LibVLC
	libvlc_release(inst);

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


bool getMetaData(AudioMetaData *metaData, libvlc_media_t *media)
{
	libvlc_media_parse(media);

	// Parse the metadata from the audio file
	metaData->title = libvlc_media_get_meta(media, libvlc_meta_Title);
	metaData->artist = libvlc_media_get_meta(media, libvlc_meta_Artist);
	metaData->album = libvlc_media_get_meta(media, libvlc_meta_Album);

	if (metaData->title == NULL || metaData->artist == NULL || metaData->album == NULL)
	{
		return false;
	}

	return true;
}


void freeMetaData(AudioMetaData *metaData)
{
	libvlc_free(metaData->artist);
	libvlc_free(metaData->album);
	libvlc_free(metaData->title);
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
	if ((multicastSocket = socket(AF_INET, SOCK_DGRAM, 0)) == SOCKET_ERROR)
	{
		cerr << "Failed to open multicast socket." << endl;
		return false;
	}

	multicastInfo.sin_addr.s_addr = htonl(INADDR_ANY);
	multicastInfo.sin_family = AF_INET;
	multicastInfo.sin_port = 0;

	if (bind(multicastSocket, (struct sockaddr *) &multicastInfo, sizeof(multicastInfo)) == SOCKET_ERROR)
	{
		cerr << "Failed to bind multicast socket." << endl;
		return false;
	}

	// Connect to the multicast group
	multicastInterface.imr_multiaddr.s_addr = inet_addr(MULTICAST_ADDR);
	multicastInterface.imr_interface.s_addr = INADDR_ANY;

	if (setsockopt(multicastSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&multicastInterface, sizeof(multicastInterface)) == SOCKET_ERROR)
	{
		cerr << "Failed to create multicast group." << endl;
		return false;
	}

	u_long ttl = 10;
	if (setsockopt(multicastSocket, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&ttl, sizeof(ttl)) == SOCKET_ERROR)
	{
		cerr << "Failed to set time to live." << endl;
		return false;
	}

	bool loopback = false;
	if (setsockopt(multicastSocket, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&loopback, sizeof(loopback)) == SOCKET_ERROR)
	{
		cerr << "Failed to set loopback." << endl;
		return false;
	}

	multicastDestInfo.sin_addr.s_addr = inet_addr(MULTICAST_ADDR);
	multicastDestInfo.sin_family = AF_INET;
	multicastDestInfo.sin_port = htons((port - 1));

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
		stringstream ss;
		ss << MUSIC_LOCATION << "/" << tracklist[randSong];
		libvlc_media_t *media = libvlc_media_new_path(inst, ss.str().c_str());

		if (media == NULL)
		{
			cerr << "Failed to load music." << endl;
			return false;
		}

		// Retrieve the metadata
		freeMetaData(&metaData);
		if (getMetaData(&metaData, media))
		{
			cout << "Meta Data: " << metaData.artist << ", " << metaData.title << ", " << metaData.album << endl;
		}

		// Play the audio
		mediaPlayer = libvlc_media_player_new_from_media(media);
		libvlc_media_release(media);
		libvlc_media_player_play(mediaPlayer);

		// Start the Send Current Song thread
		std::thread tCurrentSong(sendCurrentSongMulti, randSong, &metaData);
		tCurrentSong.detach();
		
		while (!libvlc_media_player_is_playing(mediaPlayer))
		{
			// Wait for the song to start
		}

		cout << "Multicasting..." << endl;

		// While we are not done and there is data left to send
		while (!done && libvlc_media_player_is_playing(mediaPlayer))
		{
			cout << "Sending data in " << MESSAGE_SIZE << " byte messages..." << endl;

			Sleep(1000);
		}

		libvlc_media_player_release(mediaPlayer);

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
void playUnicast(Client *c, string song, string ip)
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
void saveUnicast(Client *c, string song)
{
	std::thread tSendUnicast(sendCurrentSongUni, c, song, true);
	tSendUnicast.detach();
}


void sendCurrentSongUni(Client *c, string song, bool usingTCP)
{
	// loop until all of the song is sent
	cout << "Unicasting..." << endl;

	if (!usingTCP)
	{
		SOCKET cSock;

		if ((cSock = socket(PF_INET, SOCK_DGRAM, 0)) == SOCKET_ERROR)
		{
			cerr << "Failed to open client stream socket." << endl;
			return;
		}

		char test[MESSAGE_SIZE];

		for (int i = 0; i < MESSAGE_SIZE; i++)
		{
			test[i] = 'T';
		}

		if (sendto(cSock, test, MESSAGE_SIZE, 0, (sockaddr *)&(c->sin_udp), sizeof(c->sin_udp)) <= 0)
		{
			cerr << "Failed to send stream data." << endl;
			return;
		}

		cout << "Sent stream data." << endl;

		closesocket(cSock);
	}
	else
	{
		// send song via tcp control socket (download)
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
	getchar();

	return true;
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: startLibVLC
--
-- DATE: March 29, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: void startLibVLC();
--
-- PARAMETERS:
--
-- RETURNS: void
--
-- NOTES:
--     This function initializes LibVLC.
----------------------------------------------------------------------------------------------------------------------*/
void startLibVLC()
{
	char smem_options[256];

	// Write the command line string to the char array
	sprintf(smem_options, VLC_OPTIONS, (long long int)(intptr_t)(void*)&handleStream, (long long int)(intptr_t)(void*)&prepareRender);

	const char* const vlc_args[] = { "-I", "dummy", "--verbose=0", "--sout", smem_options };

	// Create an instance of libvlc
	if ((inst = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args)) == NULL)
	{
		cerr << "Failed to create libvlc instance." << endl;
		exit(1);
	}
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: prepareRender
--
-- DATE: March 26, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: void prepareRender(void* p_audio_data, uint8_t** pp_pcm_buffer , size_t size);
--
-- PARAMETERS:
--		p_audio_data - not used
--		pp_pcm_buffer - a pointer to the location of the audio stream
--		size - the size of the required buffer
--
--
-- RETURNS: void
--
-- NOTES:
--     This function is called before audio is streamed to allocate memory for the buffer.
----------------------------------------------------------------------------------------------------------------------*/
void prepareRender(void* p_audio_data, uint8_t** pp_pcm_buffer, size_t size)
{
	// Allocate memory to the buffer
	*pp_pcm_buffer = (uint8_t*)malloc(size);
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: handleStream
--
-- DATE: March 26, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: void handleStream(void* p_audio_data, uint8_t* p_pcm_buffer, unsigned int channels,
--				  unsigned int rate, unsigned int nb_samples, unsigned int bits_per_sample, size_t size, int64_t pts )
--
-- PARAMETERS:
--		p_audio_data - not used
--		p_pcm_buffer - a pointer to the location of the stream buffer
--		channels - the number of channels used
--		rate - the bitrate of the audio
--		nb_samples - the number of samples per second for the audio
--		bits_per_sample - the number of bits per audio sample (bit depth)
--		size - the length of the buffer
--		pts - not used
--
-- RETURNS: void
--
-- NOTES:
--     This function is called after audio is streamed and is used to write data to the buffer.
----------------------------------------------------------------------------------------------------------------------*/
void handleStream(void* p_audio_data, uint8_t* p_pcm_buffer, unsigned int channels,
	unsigned int rate, unsigned int nb_samples, unsigned int bits_per_sample, size_t size, int64_t pts)
{
	char *buffer;
	int dataSize = size;
	int messageSize;
	int dataSent = 0;

	//cout << p_pcm_buffer << endl;

	// While we have data to write
	while (dataSize > 0)
	{
		// Set the size of the next message to send
		if (dataSize > MESSAGE_SIZE)
		{
			messageSize = MESSAGE_SIZE;
		}
		else
		{
			messageSize = dataSize;
		}

		// Write the data to the socket
		buffer = new char[dataSize];
		memcpy(buffer, p_pcm_buffer + dataSent, messageSize);

		sendto(multicastSocket, buffer, MESSAGE_SIZE, 0, (struct sockaddr *) &multicastDestInfo, sizeof(multicastDestInfo));

		dataSize -= messageSize;
		dataSent += messageSize;

		delete[] buffer;
	}

	// Free the temporary stream buffer
	free(p_pcm_buffer);
}


vector<string>* getTracklist()
{
	return &tracklist;
}