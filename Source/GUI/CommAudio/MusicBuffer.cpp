/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: MusicBuffer.cpp - This file contains all MusicBuffer functions.
--
-- PROGRAM: CommAudio.exe
--
-- FUNCTIONS:
--      MusicBuffer();
--      char* getBuffer();
--      void put(char *data, int dataSize);
--      bool ready();
--      int getEndPosition();
--      void clear();
--
-- DATE: March 18, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- NOTES:
--      This file contains the implementation of the MusicBuffer class.
----------------------------------------------------------------------------------------------------------------------*/

#include "MusicBuffer.h"


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: MusicBuffer
--
-- DATE: March 18, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: MusicBuffer();
--
-- PARAMETERS:
--
-- RETURNS: nothing
--
-- NOTES:
--     This is the default constructor for the MusicBuffer.
----------------------------------------------------------------------------------------------------------------------*/
MusicBuffer::MusicBuffer()
{
    endPosition = 0;
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: getBuffer
--
-- DATE: March 24, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: char* getBuffer();
--
-- PARAMETERS:
--
-- RETURNS: char* - the music buffer to retrieve
--
-- NOTES:
--     This function returns the music buffer.
----------------------------------------------------------------------------------------------------------------------*/
char* MusicBuffer::getBuffer()
{
	return buffer;
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: put
--
-- DATE: March 18, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: void put(char *data, int dataSize, int volume);
--
-- PARAMETERS:
--      data - the data to insert
--      dataSize - the amount of data (in bytes)
--      volume - the volume of the audio
--
-- RETURNS: void
--
-- NOTES:
--     This function inserts data into the circular buffer.
----------------------------------------------------------------------------------------------------------------------*/
void MusicBuffer::put(char *data, int dataSize, int volume)
{
    for (int i = 0; i < dataSize; i++)
    {
        buffer[endPosition] = data[i] * volume / 100;
        
        // If we are at the end of the buffer
        if (endPosition == BUFFER_SIZE - 1)
        {
            endPosition = 0;
        }
        else
        {
            endPosition++;
        }
    }
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: size
--
-- DATE: March 24, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: bool ready();
--
-- PARAMETERS:
--
-- RETURNS: bool - whether or not the buffer is ready to start streaming
--
-- NOTES:
--     This function returns the ready state of the buffer.
----------------------------------------------------------------------------------------------------------------------*/
bool MusicBuffer::ready()
{
	if (endPosition >= MESSAGE_SIZE * 10)
	{
		return true;
	}

	return false;
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: getEndPosition
--
-- DATE: March 18, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: int getEndPosition();
--
-- PARAMETERS:
--
-- RETURNS: int - the position of the writer in the buffer
--
-- NOTES:
--     This function returns the position of the writer in the buffer.
----------------------------------------------------------------------------------------------------------------------*/
int MusicBuffer::getEndPosition()
{
    return endPosition;
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: clear
--
-- DATE: March 18, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: void clear();
--
-- PARAMETERS:
--
-- RETURNS: void
--
-- NOTES:
--     This function empties the circular buffer.
----------------------------------------------------------------------------------------------------------------------*/
void MusicBuffer::clear()
{
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        buffer[i] = '\0';
    }
    
    endPosition = 0;
}
    
    
