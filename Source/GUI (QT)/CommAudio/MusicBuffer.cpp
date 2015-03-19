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
    curPosition = 0;
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: get
--
-- DATE: March 18, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: char get();
--
-- PARAMETERS:
--
-- RETURNS: char - the next char in the buffer
--
-- NOTES:
--     This function is responsible for initiating the connection process when
--     the connect button is pressed on the Music tab.
----------------------------------------------------------------------------------------------------------------------*/
char MusicBuffer::get()
{   
    if (curPosition == BUFFER_SIZE - 1)
    {
        curPosition = 0;
        return buffer[BUFFER_SIZE - 1];
    }
    else
    {
        curPosition++;
        return buffer[curPosition - 1];
    }
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
-- FUNCTION: getCurrentPosition
--
-- DATE: March 18, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: int getCurrentPosition();
--
-- PARAMETERS:
--
-- RETURNS: int - the current reading position in the buffer.
--
-- NOTES:
--     This function returns the current reading position in the buffer.
----------------------------------------------------------------------------------------------------------------------*/
int MusicBuffer::getCurrentPosition()
{
    return curPosition;
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
    
    curPosition = 0;
    endPosition = 0;
}
    
    
