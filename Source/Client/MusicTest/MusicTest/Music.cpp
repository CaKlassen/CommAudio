#include <iostream>
#include <Windows.h>
#include "Music.h"
#include <Digitalv.h>

using std::string;


bool loadFile(string filename, MCIDEVICEID *deviceId)
{
	MCIDEVICEID device;
    MCI_WAVE_OPEN_PARMS fileParams;
    MCI_PLAY_PARMS play;
    MCIERROR mcierror;

	// Define file open parameters
    fileParams.lpstrDeviceType = "waveaudio";
    fileParams.lpstrElementName = filename.c_str();

	// Open the file
    if (mciSendCommand(0, MCI_OPEN, MCI_WAIT | MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD) &fileParams) != 0)
	{
		return false;
	}

	// Set the device
    *deviceId = fileParams.wDeviceID;

	

	// Play the song
	/*
    if (mciSendCommand(device, MCI_PLAY, MCI_NOTIFY, (DWORD) &play) != 0)
	{
		return false;
	}*/

    return true;
}


bool retrieveSoundData(MCIDEVICEID deviceId, LPMCI_DGV_COPY_PARMS *copyStruct)
{
	copyStruct->
}