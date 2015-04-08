/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: mainwindow.cpp - This file handles all GUI interaction.
--
-- PROGRAM: CommAudio.exe
--
-- FUNCTIONS:
--
-- DATE: March 9, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Jonathan Chu
--           Chris Klassen
--           Melvin Loho
--
-- PROGRAMMER: Jonathan Chu
--             Chris Klassen
--             Melvin Loho
--
-- NOTES:
--      This file contains functionality to handle GUI interaction and display.
----------------------------------------------------------------------------------------------------------------------*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSlider>
#include <vector>
#include <iostream>
#include <thread>
#include <stdio.h>
#include <errno.h>
#include <QAudioDeviceInfo>
#include <QAudioInput>

#include "Network.h"
#include "MusicBuffer.h"
#include "Mic.h"
#include "micoutput.h"

#define BUFSIZE 8192

using std::string;
using std::vector;
using std::cerr;
using std::endl;

// Client variables
MainWindow *mWin;
string currentSong;
int songLength;
vector<string> tracklist;
QString filePath = "C:/";

MusicBuffer musicBuffer;
HWAVEOUT outputDevice;
LPWAVEHDR audioBuffers[NUM_OUTPUT_BUFFERS];

// Microphone variables
Mic *mic;
MicOutput *micOutput;

ClientState cData;


//the play button
int starting = 0;
int currentMusic = -1;

//the characters to be shown when the song is being downloaded or played
QString downloadAscii = "D";
QString playAscii = ">";

//colors that are used to depict the state of the song
QColor playColor = Qt::darkGreen;
QColor downloadColor = Qt::blue;
QColor bothColor = Qt::darkMagenta;
QColor defaultColor = Qt::black;


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:
--
-- DATE:        March 18, 2015
--
-- REVISIONS:   (Date and Description)
--
-- DESIGNER:
--
-- PROGRAMMER:
--
-- INTERFACE:
--
-- PARAMETERS:
--
--
-- RETURNS:     void
--
-- NOTES:
--
----------------------------------------------------------------------------------------------------------------------*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mWin = this;
    
    // Set up the client data structure
    cData.ip = "127.0.0.1";
    cData.port = 9000;
    cData.connected = false;
    cData.sMode = NOTHING;

    ui->cIPAddressText->setText(QString::fromStdString(cData.ip));
    ui->cPortText->setText(QString::number(cData.port));
    ui->cFilepathText->setText(filePath);

    ui->uSongList->setFrameShape(QFrame::NoFrame);
    ui->uPlayList->setFrameShape(QFrame::NoFrame);
    ui->uDownloadList->setFrameShape(QFrame::NoFrame);

    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    updateMulticastSong("", "", "");
    
    
    mic = new Mic();
    micOutput = new MicOutput();
}

MainWindow::~MainWindow()
{
    WSACleanup();
    
    delete mic;
    delete micOutput;
    delete ui;
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: on_uPlayButton_clicked
--
-- DATE: March 31, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: 
--      Jonathan Chu
--      Chris Klassen
--
-- PROGRAMMER: 
--      Jonathan Chu
--      Chris Klassen
--
-- INTERFACE: void on_uPlayButton_clicked();
--
-- PARAMETERS:
--
-- RETURNS: void
--
-- NOTES:
--     This function requests a song from the server to be streamed.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::on_uPlayButton_clicked()
{
    //This selects the item and then just make it blue
    QListWidgetItem *theItem = ui->uSongList->currentItem();
    QListWidgetItem *playingIcon = ui->uPlayList->item(ui->uSongList->currentRow());

    if (!theItem || !playingIcon) return;

    if (playingIcon->text() == "")
    {
        playingIcon->setText(playAscii);

        if (theItem->textColor() == downloadColor)
            theItem->setTextColor(bothColor);
        else
            theItem->setTextColor(playColor);


        currentMusic = ui->uSongList->currentRow();
    }

    //all items on the list will be reverted back to normal
    if (true)
    {
        for(int i = 0; i < ui->uSongList->count(); ++i)
        {
            QListWidgetItem *allItems = ui->uSongList->item(i);
            QListWidgetItem *playingIcons = ui->uPlayList->item(i);
            if (playingIcons->text() == playAscii && currentMusic != i)
            {
                playingIcons->setText("");

                if (allItems->textColor() == bothColor)
                    allItems->setTextColor(downloadColor);
                else
                    allItems->setTextColor(defaultColor);
            }
        }
    }

    // Start receiving audio
    string song;
    song = ui->uSongList->item(currentMusic)->text().toStdString();
    std::thread streamThread(streamMusic, &cData, song, &musicBuffer);
    streamThread.detach();
    
    //starting = starting + 5; //once it reaches 100% it will stay as 100% even if "starting" is past 100
    //ui->uMusicProgressSlider->setValue(starting); // this is 50 as in 0%
}

//the download button

void MainWindow::on_uDownloadButton_clicked()
{
    //This selects the item and then just make it blue
    QListWidgetItem *theItem = ui->uSongList->currentItem();
    QListWidgetItem *downloadIcon = ui->uDownloadList->item(ui->uSongList->currentRow());

    if (!theItem || !downloadIcon) return;

    if (downloadIcon->text() == "")
    {
        downloadIcon->setText(downloadAscii);

        if (theItem->textColor() == playColor)
            theItem->setTextColor(bothColor);
        else
            theItem->setTextColor(downloadColor);

        currentMusic = ui->uSongList->currentRow();
    }

    //all items on the list will be reverted back to normal
    if (true)
    {
        for(int i = 0; i < ui->uSongList->count(); ++i)
        {
            QListWidgetItem *allItems = ui->uSongList->item(i);
            QListWidgetItem *downloadingIcons = ui->uDownloadList->item(i);
            if (downloadingIcons->text() == downloadAscii && currentMusic != i)
            {
                downloadingIcons->setText("");


                if (allItems->textColor() == bothColor)
                    allItems->setTextColor(playColor);
                else
                    allItems->setTextColor(defaultColor);
            }
        }
    }
}


/* This is for the microphone button  */

bool MicOn = true;

void MainWindow::on_micButton_clicked()
{
    MicOn = !MicOn;
    if (MicOn) {
        mic->startSending();
        ui->micButton->setText("microphone ON");
    }
    else {
        mic->stopSending();
        ui->micButton->setText("microphone OFF");
    }
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: updateMulticastSong
--
-- DATE: March 31, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: void updateMulticastSong(string title, string artist, string album);
--
-- PARAMETERS:
--      title - the title of the current song
--      artist - the artist of the current song
--      album - the album of the current song
--
-- RETURNS: void
--
-- NOTES:
--     This function updates the current multicast song on the GUI.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::updateMulticastSong(string title, string artist, string album)
{
    ui->mCurrentSongLabel->setText(QString::fromStdString(title));
    ui->mCurrentArtistLabel->setText(QString::fromStdString(artist));
    ui->mCurrentAlbumLabel->setText(QString::fromStdString(album));
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: setTracklist
--
-- DATE: March 18, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: void setTracklist(vector<string> *songs);
--
-- PARAMETERS:
--      songs - the list of songs from the server
--
-- RETURNS: void
--
-- NOTES:
--     This function updates the current tracklist from the server.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::setTracklist(vector<string> *songs)
{
    tracklist.clear();
    ui->uSongList->clear();
    ui->uPlayList->clear();
    ui->uDownloadList->clear();

    // Loop through and add each new song to the tracklist
    for (int i = 0; i < (int) songs->size(); i++)
    {
        tracklist.emplace_back(songs->at(i));

        // Update the tracklist GUI component
        ui->uSongList->addItem(QString::fromStdString(songs->at(i)));
        ui->uPlayList->addItem("");
        ui->uPlayList->item(tracklist.size() - 1)->setTextColor(playColor);
        ui->uDownloadList->addItem("");
        ui->uDownloadList->item(tracklist.size() - 1)->setTextColor(downloadColor);
        
    }
    
    ui->uSongList->setCurrentRow(0);
}


void MainWindow::on_actionConnectDisconnect_triggered()
{
    int mode = ui->tabWidget->currentIndex();

    //make sure we're in an appropriate tab
    if (mode < 0 || mode > 2) return;

    if (mode == 0 || mode == 1)
    {
        // Server-based
        if (cData.connected)
        {
            disconnectIt();
        }
        else
        {
            if (!connectIt()) return;
    
            cData.connected = true;
        }
    }
    else
    {
        // Microphone
        if (!cData.connected)
        {
            startMicrophone(&cData, micOutput);
        }
        else
        {
            // Disable microphone input and output
            mic->stopSending();
            micOutput->stopListening();
            
            cData.connected = false;
        }
    }
}


//this is the button Ok on the config tab
void MainWindow::on_cOKButton_clicked()
{
    filePath = ui->cFilepathText->text();
    
    cData.ip = ui->cIPAddressText->text().toStdString();
    cData.port = ui->cPortText->text().toInt();
}

//this is the button cancel on the config tab
void MainWindow::on_cCancelButton_clicked()
{
    ui->cIPAddressText->setText(QString::fromStdString(cData.ip));
    ui->cPortText->setText(QString::number(cData.port));
    ui->cFilepathText->setText(filePath);
}

bool MainWindow::connectIt()
{
    musicBuffer.clear();

    // Connect to the control channel
    return connectControlChannel(&cData);
}

void MainWindow::disconnectIt()
{
    // Clear the buffer and free audio structures
    musicBuffer.clear();
    
    waveOutPause(outputDevice);
    waveOutClose(outputDevice);
    
    
    for (int i = 0; i < NUM_OUTPUT_BUFFERS; i++)
    {
        waveOutUnprepareHeader(outputDevice, audioBuffers[i], sizeof(WAVEHDR)); 
    }
       
    //once disconnected all tabs are available again
    focusTab(-1);

    disconnectControlChannel();

    cData.connected = false;

    updateMulticastSong("", "", "");
}

void MainWindow::focusTab(int tabNumber)
{
    //there is no focus on a specific tab, so all are enabled
    if (tabNumber < 0)
    {
        ui->actionConnectDisconnect->setText("Connect");
        for (int n = 0; n < ui->tabWidget->count(); n++)
        {
            ui->tabWidget->setTabEnabled(n, true);
        }
    }
    else
    {
        ui->actionConnectDisconnect->setText("Disconnect");
        for (int n = 0; n < ui->tabWidget->count(); n++)
        {
            if (n != tabNumber)
                ui->tabWidget->setTabEnabled(n, false);
        }
    }
}

void MainWindow::errorMessage(QString message)
{
    QMessageBox::warning(
        this,
        tr("Error Message!"),
        message );
    if (ui->actionConnectDisconnect->isChecked())
    {
        focusTab(-1);
    }
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: updateServerMode
--
-- DATE: March 28, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: void updateServerMode(ServerMode sMode);
--
-- PARAMETERS:
--      sMode - the mode of the server
--
-- RETURNS: void
--
-- NOTES:
--     This function updates the client mode based on the server.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::updateServerMode(ServerMode sMode)
{
    cData.sMode = sMode;

    //check if its unicast
    if (cData.sMode == UNICAST)
    {
        focusTab(1);
    }
    //check if it is multicast
    else if (cData.sMode == MULTICAST)
    {
        focusTab(0);
        std::thread streamThread(connectMusic, &cData, &musicBuffer);
        streamThread.detach();
    }
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: outputAudio
--
-- DATE: March 30, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: void outputAudio(MusicBuffer *buffer);
--
-- PARAMETERS:
--      buffer - a pointer to the music buffer to stream from
--
-- RETURNS: void
--
-- NOTES:
--     This function uses WAVEOUT structures to play audio from a buffer.
----------------------------------------------------------------------------------------------------------------------*/
void outputAudio(MusicBuffer *buffer)
{
    WAVEFORMATEX format;
    
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
        exit(1);
    }

    // Prepare the wave headers
    for (int i = 0; i < NUM_OUTPUT_BUFFERS; i++)
    {
        audioBuffers[i] = (LPWAVEHDR) malloc(sizeof(WAVEHDR));
        ZeroMemory(audioBuffers[i], sizeof(WAVEHDR));

        audioBuffers[i]->lpData = buffer->getBuffer();
        audioBuffers[i]->dwBufferLength = BUFFER_SIZE;

        // Create the header
        if (waveOutPrepareHeader(outputDevice, audioBuffers[i], sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
        {
            cerr << "Failed to create output header." << endl;
            exit(1);
        }
    }

    // Wait for the buffer to be ready to stream
    while (!buffer->ready());
    
    for (int i = 0; i < NUM_OUTPUT_BUFFERS; i++)
    {
        waveOutWrite(outputDevice, audioBuffers[i], sizeof(WAVEHDR));
    }
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: WaveCallback
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
--     This callback is used to properly stream audio without skipping.
----------------------------------------------------------------------------------------------------------------------*/
void CALLBACK WaveCallback(HWAVEOUT hWave, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
    if (cData.connected)
    {
        if (uMsg == WOM_DONE)
        {
            // Wait for the buffer to be ready to stream
            while (!musicBuffer.ready());

            if (waveOutWrite(outputDevice, (LPWAVEHDR) dw1, sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
            {
                cerr << "Failed to play audio." << endl;
            }
        }
    }
    else
    {
        free((LPWAVEHDR) dw1);
    }
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: sendMicrophone
--
-- DATE: April 3, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Chris Klassen
--
-- PROGRAMMER: Chris Klassen
--
-- INTERFACE: void sendMicrophone(SOCKET micSocket);
--
-- PARAMETERS:
--		micSocket - the socket to send data to
--
-- RETURNS: void
--
-- NOTES:
--     This function starts the microphone input.
----------------------------------------------------------------------------------------------------------------------*/
void sendMicrophone()
{
    // Open the mic input
    mic->setData(&cData);
    mic->startSending();
    
    // Wait until we are not connected
    while (cData.connected);
}


