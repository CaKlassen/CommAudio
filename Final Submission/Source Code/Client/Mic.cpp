/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: Mic.cpp - This file contains all mic input functionality.
--
-- PROGRAM: CommAudio.exe
--
-- FUNCTIONS:
--      Mic();
--      void setData(ClientState *cData);
--      void startSending();
--      void stopSending();
--      ~Mic();
--      void onNewState(QAudio::State state);
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
--      The functions in this file are part of a Mic class used to record
--      audio data and send it to a UDP port.
--
--      The functionality in this file is derived in part from various QT
--      QAudioInput tutorials.
----------------------------------------------------------------------------------------------------------------------*/


#include <QIODevice>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QDebug>
#include <iostream>
#include "Network.h"
#include "Mic.h"

using std::cerr;
using std::endl;


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: Mic
--
-- DATE: April 4, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: Mic();
--
-- PARAMETERS:
--
-- RETURNS: nothing
--
-- NOTES: 
--      This is the constructor for the Mic class. It sets up the audio format
--      and opens the devices needed for input.
----------------------------------------------------------------------------------------------------------------------*/
Mic::Mic() : QObject()
{    
    // Set up the audio format
    QAudioFormat format;
    format.setSampleRate(8000); 
    format.setChannelCount(1);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);
    
    // Create device info using format
    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if(!info.isFormatSupported(format))
    {
        qWarning() << "Default format not supported, finding next best...";
        format = info.nearestFormat(format);
    }
    
    // Open the output UDP socket
    micSocket = new QUdpSocket();
   
    // Open the input device
    input = new QAudioInput(format, this);
    connect(input, SIGNAL(stateChanged(QAudio::State)), this, SLOT(onNewState(QAudio::State)));
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
--      cData - the client state structure used for ip/port retrieval
--
-- RETURNS: void
--
-- NOTES: 
--      This function specifies the host to connect to for data sending.
----------------------------------------------------------------------------------------------------------------------*/
void Mic::setData(ClientState *cData)
{
    micSocket->disconnectFromHost();
    micSocket->connectToHost(QString::fromStdString(cData->ip), cData->port);
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: startSending
--
-- DATE: April 4, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: void startSending();
--
-- PARAMETERS:
--
-- RETURNS: void
--
-- NOTES: 
--      This function starts sending mic data to the UDP socket.
----------------------------------------------------------------------------------------------------------------------*/
void Mic::startSending()
{
    input->start(micSocket);
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: stopSending
--
-- DATE: April 4, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: void stopSending();
--
-- PARAMETERS:
--
-- RETURNS: void
--
-- NOTES: 
--      This function stops sending mic data to the UDP socket.
----------------------------------------------------------------------------------------------------------------------*/
void Mic::stopSending()
{
    input->stop();
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: ~Mic
--
-- DATE: April 4, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: ~Mic();
--
-- PARAMETERS:
--
-- RETURNS: nothing
--
-- NOTES: 
--      This function stops and deletes all mic members.
----------------------------------------------------------------------------------------------------------------------*/
Mic::~Mic()
{
    input->stop();
    micSocket->close();
    
    delete input;
    delete micSocket;
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
--      state - the current state of the audio device
--
-- RETURNS: void
--
-- NOTES: 
--      This function is a callback for the updated state of the audio device.
----------------------------------------------------------------------------------------------------------------------*/
void Mic::onNewState(QAudio::State state)
{
    switch(state)
    {
        case QAudio::StoppedState:
        {
            if(input->error() != QAudio::NoError)
            {
                // Error handling
                cerr << "Error with Mic: " << input->error() << endl;
                
            }
            else
            {
                // Finished recording
                cerr << "No more mic data to send" << endl;
                
            }
            break;
        }
        
        case QAudio::ActiveState:
        {
            // Started recording - read from IO device
            cerr << "Mic started" << endl;
            break;
        }    
    }
}
