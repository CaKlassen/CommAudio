/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: ControlChannel.cpp - a collection of functions used to handle server-client control messages.
--
-- PROGRAM: 
--
-- FUNCTIONS:
--
-- DATE: February 23, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- NOTES:
--     This file contains functions used to create and parse control strings sent across sockets.
--
--     Messages are defined in CMessage.h, and have two components: type and data.
----------------------------------------------------------------------------------------------------------------------*/


#include <cstdlib>
#include <sstream>
#include "ControlChannel.h"
#include "mainwindow.h"

using std::stringstream;

namespace ControlChannel
{
    MainWindow *GUI;
}

void ControlChannel::setGUIHandle(MainWindow *window)
{
    GUI = window;
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: createControlString
--
-- DATE: February 23, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: void createControlString(CMessage *cMsg, string *str);
--
-- PARAMETERS:
--     cMsg - the Control Message to create the string from
--	   str - the string to put the data into
--
-- RETURNS: void
--
-- NOTES:
--     This function creates a control string from a Control Message.
----------------------------------------------------------------------------------------------------------------------*/
void createControlString(CMessage *cMsg, string *str)
{
	stringstream ss;
	ss << cMsg->msgType << "~";

	// Loop through all strings in the message data array
	for (int i = 0; i < (int) cMsg->msgData.size(); i++)
	{
		ss << cMsg->msgData[i] << "`";
	}

	*str = ss.str();
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: parseControlString
--
-- DATE: February 23, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: void parseControlString(string str, CMessage *cMsg);
--
-- PARAMETERS:
--	   str - the string to parse
--     cMsg - the Control Message to put the parsed data into
--
-- RETURNS: void
--
-- NOTES:
--     This function populates a Control Message from a control string.
----------------------------------------------------------------------------------------------------------------------*/
void parseControlString(string str, CMessage *cMsg)
{
	// Parse the message type
	int typeEnd = str.find('~');
	cMsg->msgType = static_cast<MessageType> (atoi(str.substr(0, typeEnd).c_str()));
	
	str = str.substr(typeEnd + 1, str.length());

	// Parse the message data
	while(str.length() > 0)
	{
		int dataEnd = str.find('`');
		cMsg->msgData.emplace_back(str.substr(0, dataEnd));

		str = str.substr(dataEnd + 1, str.length());
	}
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: handleControlMessage
--
-- DATE: February 23, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: void handleControlMessage(CMessage *cMsg);
--
-- PARAMETERS:
--     cMsg - the Control Message read from
--
-- RETURNS: void
--
-- NOTES:
--     This function handles a control message.
----------------------------------------------------------------------------------------------------------------------*/
void handleControlMessage(CMessage *cMsg)
{
	// Switch on the message's type
	switch(cMsg->msgType)
	{
		case START_CONNECTION:
		{
            // Determine the current server mode
            if (cMsg->msgData[0].compare("0") == 0)
            {
                ControlChannel::GUI->updateServerMode(MULTICAST);
            }
            else
            {
                ControlChannel::GUI->updateServerMode(UNICAST);
            }
            
			break;
		}

        case END_CONNECTION:
        {
            // The server is shutting down
            break;
        }

        case TRACK_LIST:
        {
            // The server's current tracklist
            ControlChannel::GUI->setTracklist(&cMsg->msgData);
            break;
        }

        case NOW_PLAYING:
        {
            // The currently playing multicast song
            ControlChannel::GUI->updateMulticastSong(cMsg->msgData[0], cMsg->msgData[1], cMsg->msgData[2]);
            break;
        }

        case SONG_PIECE:
        {
            // A piece of a song to save to a file
            saveSongPiece((BYTE *) cMsg->msgData[0].c_str(), strlen(cMsg->msgData[0].c_str()));
            break;
        }
        
        case SAVE_SONG:
        {
            // The song requested to be saved has completed sending
            doneSavingSong();
            break;
        }

        case END_SONG:
        {
            // The currently playing song has ended
            endSong();
            break;
        }
	}
}
