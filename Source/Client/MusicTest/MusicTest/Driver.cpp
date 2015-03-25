#include <iostream>
#include <cstdlib>
#include <Windows.h>
#include <mmsystem.h>
#include <vlc/libvlc.h>

#include "Music.h"
#include "MusicBuffer.h"

/* Test reading and output thread */
DWORD WINAPI readDataThread(LPVOID lpArg);

using std::cout;
using std::cerr;
using std::endl;
using std::string;

/* Driver variables */
HWAVEOUT outputDevice;
MusicBuffer mBuffer;

int main(void)
{
	char *buffer;
	int bufferLength;
	WAVEFORMATEX format;

	// Load the data from a wav file
	if (!loadFile("test.wav", &buffer, &bufferLength, &format))
	{
		cerr << "Failed to load wav file." << endl;
		return 1;
	}

	/* Create the output thread */
	CreateThread(NULL, 0, readDataThread, NULL, 0, NULL);

	/* Loop until the song is done being put into the buffer */
	while (true)
	{
		
	}

	/* Wait for user input before exiting */
	getchar();

	return 0;
}


/*
	This is a simple test thread used to retrieve audio from the circular buffer
	and output it.
*/
DWORD WINAPI readDataThread(LPVOID lpArg)
{
	WAVEFORMATEX temp;
	LPWAVEHDR waveHeader;

	// Set up the wave format
	temp.nSamplesPerSec = 44100;
	temp.wBitsPerSample = 16;
	temp.nChannels = 2;
	temp.cbSize = 0;
	temp.wFormatTag = WAVE_FORMAT_PCM;
	temp.nBlockAlign = (temp.wBitsPerSample / 8) * temp.nChannels;
	temp.nAvgBytesPerSec = temp.nBlockAlign * temp.nSamplesPerSec;

	// Open the output device
	if (waveOutOpen(&outputDevice, WAVE_MAPPER, &temp, (DWORD) WaveCallback, NULL, CALLBACK_FUNCTION) != MMSYSERR_NOERROR)
	{
		cerr << "Failed to open output device." << endl;
		return 1;
	}

	// Prepare the wave header
	waveHeader = (LPWAVEHDR) malloc(sizeof(WAVEHDR));
	ZeroMemory(waveHeader, sizeof(WAVEHDR));
	waveHeader->lpData = mBuffer.getBuffer();
	waveHeader->dwBufferLength = BUFFER_SIZE;

	// Create the wave out header
	if (waveOutPrepareHeader(outputDevice, waveHeader, sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
	{
		cerr << "Failed to create data header." << endl;
		return 1;
	}

	// Play audio
	waveOutWrite(outputDevice, waveHeader, sizeof(WAVEHDR));
}



/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: AudioCallback
--
-- DATE: March 24, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: void CALLBACK AudioCallback(HWAVEOUT hWave, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
--
-- PARAMETERS:
--		hWave - a handle to the output device
--		uMsg - the message sent to the callback
--		dwUser - not used
--		dw1 - the wave header used for audio output
--		dw2 - not used
--
-- RETURNS: void
--
-- NOTES:
--     This callback is ued to properly stream audio without skipping.
----------------------------------------------------------------------------------------------------------------------*/
void CALLBACK WaveCallback(HWAVEOUT hWave, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	if (uMsg == WOM_DONE)
	{
		if (waveOutWrite(outputDevice, (LPWAVEHDR) dw1, sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
		{
			cerr << "Failed to play audio." << endl;
			exit(1);
		}
	}
}