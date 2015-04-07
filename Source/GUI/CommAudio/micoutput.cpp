/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: micoutput.cpp - This file contains all mic outputs functionality.
--
-- PROGRAM: CommAudio.exe
--
-- FUNCTIONS:
--
-- DATE: April 4, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- NOTES:
--      The functions in this file are part of a MicOutput class used to retrieve
--      audio data from a UDP port and output it to the speakers.
--
--      The functionality in this file is derived in part from various QT
--      QAudioInput tutorials.
----------------------------------------------------------------------------------------------------------------------*/

#include "micoutput.h"

#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QIODevice>
#include <QByteArray>
#include <QDebug>
#include <iostream>

using std::cerr;
using std::endl;


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: MicOutput
--
-- DATE: April 4, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: MicOutput();
--
-- PARAMETERS:
--
-- RETURNS: nothing
--
-- NOTES: 
--      This is the constructor for the MicOutput class. It sets up the audio
--      format and opens the necessary socket and audio device.
----------------------------------------------------------------------------------------------------------------------*/
MicOutput::MicOutput()
{
    // Set the output format
    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);
    
    // Set up the device info with the format
    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format)) 
    {
       qWarning() << "Raw audio format not supported by backend, cannot play audio.";
       return;
    }
    
    // Open the receiving socket
    micSocket = new QUdpSocket();
    
    // Start the audio output
    output = new QAudioOutput(format, this);
    connect(output, SIGNAL(stateChanged(QAudio::State)), this, SLOT(onNewState(QAudio::State)));
    
    connect(micSocket, SIGNAL(readyRead()), this, SLOT(play()));
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: ~MicOutput
--
-- DATE: April 4, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: ~MicOutput();
--
-- PARAMETERS:
--
-- RETURNS: nothing
--
-- NOTES: 
--      This destructor closes and deletes the MicOutput members.
----------------------------------------------------------------------------------------------------------------------*/
MicOutput::~MicOutput()
{
    output->stop();
    micSocket->close();
    
    delete output;
    delete micSocket;
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: setData
--
-- DATE: April 4, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: void setData(ClientState *cData);
--
-- PARAMETERS:
--      cData - the client state used for port retrieval
--
-- RETURNS: void
--
-- NOTES: 
--      This function ssets the port for the receiving socket.
----------------------------------------------------------------------------------------------------------------------*/
void MicOutput::setData(ClientState *cData)
{
    micSocket->bind(cData->port);
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: startListening
--
-- DATE: April 4, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: void startListening();
--
-- PARAMETERS:
--
-- RETURNS: void
--
-- NOTES: 
--      This function starts receiving data from the inbound UDP socket.
----------------------------------------------------------------------------------------------------------------------*/
void MicOutput::startListening()
{
    speakers = output->start();
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: stopListening
--
-- DATE: April 4, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: void stopListening();
--
-- PARAMETERS:
--
-- RETURNS: void
--
-- NOTES: 
--      This function stops receiving data from the UDP socket.
----------------------------------------------------------------------------------------------------------------------*/
void MicOutput::stopListening()
{
    output->stop();
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: onNewState
--
-- DATE: April 4, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: void onNewState(QAudio::State state);
--
-- PARAMETERS:
--      state - the updated state of the audio device.
--
-- RETURNS: void
--
-- NOTES: 
--      This function is a callback for the updated state of the audio device.
----------------------------------------------------------------------------------------------------------------------*/
void MicOutput::onNewState(QAudio::State state)
{
    switch(state) 
    {
        case QAudio::ActiveState:
        {
            cerr << "Mic output started" << endl;
            break;
        }
            
        case QAudio::IdleState:
        {
            cerr << "No mic data to play" << endl;
            break;
        }
        
        case QAudio::StoppedState:
        {
            if (output->error() != QAudio::NoError) 
            {
                cerr << "Error with Mic Output" << endl;
            }
            
            break;
        }
    }
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: play
--
-- DATE: April 4, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: void play();
--
-- PARAMETERS:
--
-- RETURNS: void
--
-- NOTES: 
--      This function is automatically called when new data is received. It plays
--      audio to the output device.
----------------------------------------------------------------------------------------------------------------------*/
void MicOutput::play()
{
    while (micSocket->hasPendingDatagrams())
    {
        QByteArray data;
        data.resize(micSocket->pendingDatagramSize());
        micSocket->readDatagram(data.data(), data.size());
        speakers->write(data.data(), data.size());
    }
}
