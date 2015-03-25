/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: MusicBuffer.cpp - This file contains all MusicBuffer functions.
--
-- PROGRAM: CommAudio.exe
--
-- FUNCTIONS:
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
-- INTERFACE: void put(char *data, int dataSize);
--
-- PARAMETERS:
--      data - the data to insert
--      dataSize - the amount of data (in bytes)
--
-- RETURNS: void
--
-- NOTES:
--     This function inserts data into the circular buffer.
----------------------------------------------------------------------------------------------------------------------*/
void MusicBuffer::put(char *data, int dataSize)
{
    for (int i = 0; i < dataSize; i++)
    {
        buffer[endPosition] = data[i];
        
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
-- INTERFACE: int size();
--
-- PARAMETERS:
--
-- RETURNS: int - the current size of the buffer
--
-- NOTES:
--     This function returns the size of the circular buffer.
----------------------------------------------------------------------------------------------------------------------*/
int MusicBuffer::size()
{
	/*
	if (endPosition < curPosition)
	{
		// Our end has wrapped around and our current position has not
		return (BUFFER_SIZE - curPosition + endPosition);
	}
	else
	{
		return endPosition - curPosition;
	}
	*/
	return 1;
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
    
    
