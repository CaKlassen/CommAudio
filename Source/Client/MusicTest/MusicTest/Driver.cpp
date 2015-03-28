#include <iostream>
#include <cstdlib>
#include <Windows.h>
#include <mmsystem.h>
#include <vlc/vlc.h>
#include <vlc/libvlc.h>

#include "MusicBuffer.h"

// Prototypes 
DWORD WINAPI readDataThread(LPVOID lpArg);
void CALLBACK WaveCallback(HWAVEOUT hWave, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
void prepareRender(void* p_audio_data, uint8_t** pp_pcm_buffer , size_t size); 
void handleStream(void* p_audio_data, uint8_t* p_pcm_buffer, unsigned int channels, 
				  unsigned int rate, unsigned int nb_samples, unsigned int bits_per_sample, size_t size, int64_t pts);

using std::cout;
using std::cerr;
using std::endl;
using std::string;

// Driver variables
HWAVEOUT outputDevice;
MusicBuffer mBuffer;


int main(void)
{
	// Create VLC objects
	libvlc_instance_t *inst;
	libvlc_media_player_t *mediaPlayer;
    char smem_options[256];

	// Write the command line string to the char array
	sprintf(smem_options, VLC_OPTIONS, (long long int)(intptr_t)(void*) &handleStream, (long long int)(intptr_t)(void*) &prepareRender);

	const char* const vlc_args[] = { "-I", "dummy", "--verbose=0", "--sout", smem_options };

	// Create an instance of libvlc
	if ((inst = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args)) == NULL)
	{
		cerr << "Failed to create libvlc instance." << endl;
		exit (1);
	}
	
	// Load the media
	libvlc_media_t *media = libvlc_media_new_path(inst, "test8.mp3");

	if (media == NULL)
	{
		cerr << "Failed to load music." << endl;
		exit(1);
	}

	// Create the media player
	mediaPlayer = libvlc_media_player_new_from_media(media);

	// Play the audio
	libvlc_media_release(media);
	libvlc_media_player_play(mediaPlayer);

	// Create the output thread
	CreateThread(NULL, 0, readDataThread, NULL, 0, NULL);

	// Wait for user input before exiting
	getchar();
	
	libvlc_media_player_release(mediaPlayer);
    libvlc_release(inst);

	return 0;
}


/*
	This is a simple test thread used to retrieve audio from the circular buffer
	and output it.
*/
DWORD WINAPI readDataThread(LPVOID lpArg)
{
	WAVEFORMATEX format;

	LPWAVEHDR audioBuffers[NUM_OUTPUT_BUFFERS];

	// Set up the wave format
	format.nSamplesPerSec = 44100;
	format.wBitsPerSample = 16;
	format.nChannels = 2;
	format.cbSize = 0;
	format.wFormatTag = WAVE_FORMAT_PCM;
	format.nBlockAlign = format.nChannels * (format.wBitsPerSample / 8);
	format.nAvgBytesPerSec = format.nSamplesPerSec * format.wBitsPerSample;

	// Open the output device
	if (waveOutOpen(&outputDevice, WAVE_MAPPER, &format, (DWORD) WaveCallback, NULL, CALLBACK_FUNCTION) != MMSYSERR_NOERROR)
	{
		cerr << "Failed to open output device." << endl;
		return 1;
	}

	// Prepare the wave headers
	for (int i = 0; i < NUM_OUTPUT_BUFFERS; i++)
	{
		audioBuffers[i] = (LPWAVEHDR) malloc(sizeof(WAVEHDR));
		ZeroMemory(audioBuffers[i], sizeof(WAVEHDR));

		audioBuffers[i]->lpData = mBuffer.getBuffer();
		audioBuffers[i]->dwBufferLength = BUFFER_SIZE;

		// Create the header
		if (waveOutPrepareHeader(outputDevice, audioBuffers[i], sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
		{
			cerr << "Failed to create output header." << endl;
			exit(1);
		}
	}

	// Write audio to the buffer
	cout << "Ready to play music" << endl;
	
	for (int i = 0; i < NUM_OUTPUT_BUFFERS; i++)
	{
		waveOutWrite(outputDevice, audioBuffers[i], sizeof(WAVEHDR));
	}
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
void prepareRender(void* p_audio_data, uint8_t** pp_pcm_buffer , size_t size)
{
	// Allocate memory to the buffer
	*pp_pcm_buffer = (uint8_t*) malloc(size);
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
				  unsigned int rate, unsigned int nb_samples, unsigned int bits_per_sample, size_t size, int64_t pts )
{
	char *buffer;
	int dataSize = size;
	int messageSize;
	int dataSent = 0;

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

		// Write the data to the circular buffer
		buffer = new char[dataSize];
		memcpy(buffer, p_pcm_buffer + dataSent, messageSize);
		
		mBuffer.put(buffer, messageSize);
		dataSize -= messageSize;
		dataSent += messageSize;

		delete [] buffer;
	}

	// Free the temporary stream buffer
	free(p_pcm_buffer);
}