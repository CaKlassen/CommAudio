/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: ControlChannel.cpp - a collection of functions used to handle server-client control messages.
--
-- PROGRAM: Server.exe
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
#include "Application.h"
#include "Network.h"

using std::stringstream;


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
-- INTERFACE: void createControlString(CMessage &cMsg, string &str);
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
void createControlString(CMessage &cMsg, string &str)
{
	stringstream ss;
	ss << cMsg.msgType << "~";

	// Loop through all strings in the message data array
	for (int i = 0; i < (int)cMsg.msgData.size(); i++)
	{
		ss << cMsg.msgData[i] << "`";
	}

	str = ss.str();
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
	while (str.length() > 0)
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
void handleControlMessage(CMessage *cMsg, Client *c)
{
	// Switch on the message's type
	switch (cMsg->msgType)
	{
	case END_CONNECTION:
	{
		// Client disconnecting
		Server::disconnectClient(cMsg->msgData[0]);
		break;
	}

	case PLAY_SONG:
	{
		// Client requesting song over UDP
		playUnicast(c, cMsg->msgData[0], cMsg->msgData[1]);
		break;
	}

	case SAVE_SONG:
	{
		// Client requesting song over TCP
		saveUnicast(cMsg->msgData[0], cMsg->msgData[1]);

		break;
	}
	}
}
