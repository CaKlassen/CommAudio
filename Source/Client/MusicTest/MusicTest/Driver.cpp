#include <iostream>
#include <cstdlib>
#include <Windows.h>
#include <mmsystem.h>
#include <vlc/vlc.h>
#include <vlc/libvlc.h>

#include "Music.h"
#include "MusicBuffer.h"

/* Test reading and output thread */
DWORD WINAPI readDataThread(LPVOID lpArg);
void prepareRender(void* p_audio_data, uint8_t** pp_pcm_buffer , size_t size); 
void handleStream(void* p_audio_data, uint8_t* p_pcm_buffer, unsigned int channels, 
				  unsigned int rate, unsigned int nb_samples, unsigned int bits_per_sample, size_t size, int64_t pts);

using std::cout;
using std::cerr;
using std::endl;
using std::string;

/* Driver variables */
HWAVEOUT outputDevice;
char *mBuffer;
int mPos;

int main(void)
{
	mBuffer = new char[BUFFER_SIZE];
	ZeroMemory(mBuffer, BUFFER_SIZE);
	mPos = 0;

	// Create VLC objects
	libvlc_instance_t *inst;
	libvlc_media_player_t *mediaPlayer;
    char smem_options[256];

	sprintf(smem_options, "#transcode{acodec=s16l,samplerate=44100,channels=2}:smem{audio-postrender-callback=%lld,audio-prerender-callback=%lld}",
                (long long int)(intptr_t)(void*) &handleStream, (long long int)(intptr_t)(void*) &prepareRender);
                
	const char* const vlc_args[] = { "-I", "dummy", "--verbose=0", "--sout", smem_options };
	
	inst = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
	if (inst == NULL)
	{
		cerr << "Failed to create libvlc instance." << endl;
		exit (1);
	}
	
	// Load the media
	libvlc_media_t *media = libvlc_media_new_path(inst, "test5.wav");

	if (media == NULL)
	{
		cerr << "Failed to load music." << endl;
		exit(1);
	}

	mediaPlayer = libvlc_media_player_new_from_media(media);

	// Play the audio
	libvlc_media_release(media);
	libvlc_media_player_play(mediaPlayer);

	// Create the output thread
	CreateThread(NULL, 0, readDataThread, NULL, 0, NULL);

	/* Wait for user input before exiting */
	getchar();
	
	libvlc_media_player_release(mediaPlayer);
    libvlc_release(inst);

	free(mBuffer);

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
	LPWAVEHDR waveHeader2;
	LPWAVEHDR waveHeader3;

	// Set up the wave format
	temp.nSamplesPerSec = 44100;
	temp.wBitsPerSample = 16;
	temp.nChannels = 2;
	temp.cbSize = 0;
	temp.wFormatTag = WAVE_FORMAT_PCM;
	temp.nBlockAlign = temp.nChannels * (temp.wBitsPerSample / 8);
	temp.nAvgBytesPerSec = temp.nSamplesPerSec * temp.wBitsPerSample;

	// Open the output device
	if (waveOutOpen(&outputDevice, WAVE_MAPPER, &temp, (DWORD) WaveCallback, NULL, CALLBACK_FUNCTION) != MMSYSERR_NOERROR)
	{
		cerr << "Failed to open output device." << endl;
		return 1;
	}

	// Prepare the wave header
	waveHeader = (LPWAVEHDR) malloc(sizeof(WAVEHDR));
	ZeroMemory(waveHeader, sizeof(WAVEHDR));
	waveHeader2 = (LPWAVEHDR) malloc(sizeof(WAVEHDR));
	ZeroMemory(waveHeader2, sizeof(WAVEHDR));
	waveHeader3 = (LPWAVEHDR) malloc(sizeof(WAVEHDR));
	ZeroMemory(waveHeader3, sizeof(WAVEHDR));

	waveHeader->lpData = mBuffer;
	waveHeader->dwBufferLength = BUFFER_SIZE;
	waveHeader2->lpData = mBuffer;
	waveHeader2->dwBufferLength = BUFFER_SIZE;
	waveHeader3->lpData = mBuffer;
	waveHeader3->dwBufferLength = BUFFER_SIZE;

	// Create the wave out header
	if (waveOutPrepareHeader(outputDevice, waveHeader, sizeof(WAVEHDR)) != MMSYSERR_NOERROR ||
		waveOutPrepareHeader(outputDevice, waveHeader2, sizeof(WAVEHDR)) != MMSYSERR_NOERROR ||
		waveOutPrepareHeader(outputDevice, waveHeader3, sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
	{
		cerr << "Failed to create data header." << endl;
		return 1;
	}

	cout << "Ready to play music" << endl;
	waveOutWrite(outputDevice, waveHeader, sizeof(WAVEHDR));
	waveOutWrite(outputDevice, waveHeader2, sizeof(WAVEHDR));
	waveOutWrite(outputDevice, waveHeader3, sizeof(WAVEHDR));
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
	cout << "Done callback" << endl;
	if (uMsg == WOM_DONE)
	{
		if (waveOutWrite(outputDevice, (LPWAVEHDR) dw1, sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
		{
			cerr << "Failed to play audio." << endl;
			exit(1);
		}
	}
}


void prepareRender(void* p_audio_data, uint8_t** pp_pcm_buffer , size_t size)
{
	*pp_pcm_buffer = (uint8_t*) malloc(size);
	SecureZeroMemory(*pp_pcm_buffer, size);
}


void handleStream(void* p_audio_data, uint8_t* p_pcm_buffer, unsigned int channels, 
				  unsigned int rate, unsigned int nb_samples, unsigned int bits_per_sample, size_t size, int64_t pts )
{
	//char *buffer;
	int dataSize = size;
	//int messageSize;
	//int dataSent = 0;

	// While we have data to write
	/*while (dataSize > 0)
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
		


		//buffer = new char[dataSize];
		//memcpy(buffer, p_pcm_buffer + dataSent, messageSize);
		
		// TEMP - put data into the buffer
		//mBuffer.put(buffer, messageSize);
		dataSize -= messageSize;
		//dataSent += messageSize;

		delete [] buffer;
	}*/

	int mSpace = BUFFER_SIZE - mPos;

	if (dataSize > BUFFER_SIZE - mPos)
	{
		memcpy(mBuffer + mPos, p_pcm_buffer, mSpace);
		mPos = 0;

		memcpy(mBuffer, p_pcm_buffer + mSpace, dataSize - mSpace);
		mPos += dataSize - mSpace;
	}
	else
	{
		memcpy(mBuffer + mPos, p_pcm_buffer, dataSize);
		mPos += dataSize;
	}

	free(p_pcm_buffer);
}