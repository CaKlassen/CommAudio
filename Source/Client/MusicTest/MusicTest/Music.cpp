#include <iostream>
#include <fstream>
#include <Windows.h>
#include <mmsystem.h>
#include "Music.h"


using std::string;
using std::ifstream;


bool loadFile(string filename, char **buffer, int *length, WAVEFORMATEX *format)
{
	// Open the wav file in binary
	ifstream wavFile(filename, std::ios::in | std::ios::binary);

	// If the wav file was successfully found and opened
    if (wavFile.is_open())
    {
		// Four bytes to hold 'RIFF'
        char id[4];
		BYTE *sound_buffer;

		// 32 bit value to hold file size
        DWORD size;

		// 16 bit values
        short format_tag, channels, block_align, bits_per_sample;

		// 32 bit values
        DWORD format_length, sample_rate, avg_bytes_sec, data_size;

		// Read in the "RIFF" bytes
        wavFile.read(id, 4);

		// If the first 4 bytes are "RIFF"
        if (strncmp(id, "RIFF", 4) == 0)
		{
			// Read in a 32 bit size value
            wavFile.read((char*) &size, 4);
            
			// Read in another 4 byte identification string
			wavFile.read(id, 4);

			// If these four bytes are "WAVE"
            if (strncmp(id,"WAVE", 4) == 0)
            {
				// Read in "fmt "
                wavFile.read(id, 4);
                wavFile.read((char*) &format_length, 4);
                wavFile.read((char*) &format_tag, 2);
                wavFile.read((char*) &channels, 2);
                wavFile.read((char*) &sample_rate, 4);
                wavFile.read((char*) &avg_bytes_sec, 2);
                wavFile.read((char*) &block_align, 2);
                wavFile.read((char*) &bits_per_sample, 2);

				// Read in "data"
                wavFile.read((char*) &id, 2);
                wavFile.read((char*) &data_size, 4);

                (*buffer) = (char*) malloc(sizeof(char) * data_size);
                wavFile.read((char*) (*buffer), data_size);

				*length = data_size;
            }
            else
			{
				return false;
			}
        }
        else
		{
			return false;
		}
    }
	else
	{
		return false;
	}

	return true;
}

