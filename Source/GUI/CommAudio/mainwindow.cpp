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
--
-- PROGRAMMER: Jonathan Chu
--             Chris Klassen
--
-- NOTES:
--      This file contains functionality to handle GUI interaction and display.
----------------------------------------------------------------------------------------------------------------------*/


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSlider>
#include <vector>
#include "Network.h"
#include <stdio.h>
#include <winsock2.h>
#include <errno.h>

#define BUFSIZE 8192

using std::string;
using std::vector;

// Client variables
string currentSong;
int songLength;
vector<string> tracklist;

ClientState cData;

//default settings as of now
QString IP = "default";
QString filePath = "c:/";
int port = 4985;

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

    // Set up the client data structure
    cData.ip = "127.0.0.1";
    cData.port = 7000;
    cData.connected = false;
    cData.sMode = UNICAST;//NOTHING;

    IP = QString::fromStdString(cData.ip);
    port = cData.port;

    ui->cIPAddressText->setText(IP);
    ui->cPortText->setText(QString::number(port));
    ui->cFilepathText->setText(filePath);
    /* Populates the list widget  */
    for(int i= 1; i < 10; i++)
    {
        ui->uSongList->addItem("   Song number " + QString::number(i));
        if (i == 1)
            ui->uSongList->setCurrentRow(0);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

//the play button
int starting = 0;
int currentMusic = -1;
void MainWindow::on_uPlayButton_clicked()
{
    //This selects the item and then just make it blue
    QListWidgetItem *theItem = ui->uSongList->currentItem();

    if (theItem->text().left(3) == "   ")
    {
        theItem->setText( " > " + theItem->text().mid(3));
        theItem->setTextColor(Qt::red);

        currentMusic = ui->uSongList->currentRow();
    }

    //all items on the list will be reverted back to normal
    if (true)
    {
        for(int i = 0; i < ui->uSongList->count(); ++i)
        {
            QListWidgetItem *allItems = ui->uSongList->item(i);
            if (allItems->text().left(3) == " > " && currentMusic != i)
            {
                allItems->setText( "   " + theItem->text().mid(3));
                allItems->setTextColor(Qt::black);
            }

        }
    }

    starting = starting + 5; //once it reaches 100% it will stay as 100% even if "starting" is past 100
    ui->uMusicProgressSlider->setValue(starting); // this is 50 as in 0%
}

//the download button

void MainWindow::on_uDownloadButton_clicked()
{
    QListWidgetItem *theItem = ui->uSongList->currentItem();

    /* PopUp message */
    QMessageBox pop;
    pop.setText(theItem->text() + " is downloading");
    pop.exec();

}



/* This is for the microphone button  */

bool MicOn = true;

void MainWindow::on_micButton_clicked()
{
    MicOn = !MicOn;
    if (MicOn) {
        ui->micButton->setText("microphone ON");
    }
    else {
        ui->micButton->setText("microphone OFF");
    }
}

void MainWindow::on_mPlayButton_clicked()
{
    QLabel *currentSong = ui->mCurrentSongLabel;

    /* PopUp message */
    QMessageBox pop;
    pop.setText(currentSong->text() + " is playing");
    pop.exec();
}

void MainWindow::on_mVolumeButton_clicked()
{

    /* PopUp message */
    QMessageBox pop;
    pop.setText("Display the volume");
    pop.exec();
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

    // Loop through and add each new song to the tracklist
    for (int i = 0; i < (int) songs->size(); i++)
    {
        tracklist.emplace_back(songs->at(i));

        // Update the tracklist GUI component
        ui->uSongList->addItem("   " + QString::fromStdString(songs->at(i)));
    }

}



void MainWindow::on_actionConnectDisconnect_triggered()
{
    if (ui->tabWidget->tabText(ui->tabWidget->currentIndex()) != "Config.")
    {
        if (!ui->actionConnectDisconnect->isChecked())
        {
            disconnectIt();
        }
        else
        {
            connectIt();
        }
    }
    else
    {
        //if its the connect is being pressed while on the config tab
        ui->actionConnectDisconnect->setChecked(false);
    }
}

//this is the button Ok on the config tab
void MainWindow::on_cOKButton_clicked()
{
    IP = ui->cIPAddressText->text();
    port = ui->cPortText->text().toInt();
    filePath = ui->cFilepathText->text();
}

//this is the button cancel on the config tab
void MainWindow::on_cCancelButton_clicked()
{
    ui->cIPAddressText->setText(IP);
    ui->cPortText->setText(QString::number(port));
    ui->cFilepathText->setText(filePath);

}

void MainWindow::connectIt()
{
    int mode = ui->tabWidget->currentIndex();

    //make sure connect does not work on the configuration
    if (ui->tabWidget->tabText(mode) != "Config.")
    {
        //check if its unicast
        if (cData.sMode == UNICAST && (mode == 0 || mode==1))
        {
            focusTab(1);

            int n, ns, bytes_to_read;
            int port, err;
            SOCKET sd;
            struct hostent	*hp;
            struct sockaddr_in server;
            char  *host, *bp, rbuf[BUFSIZE], sbuf[BUFSIZE], **pptr;
            WSADATA WSAData;
            WORD wVersionRequested;

            host = "localhost";
            port = cData.port;

            wVersionRequested = MAKEWORD(2, 2);
            err = WSAStartup(wVersionRequested, &WSAData);
            if (err != 0) //No usable DLL
            {
                errorMessage("DLL not found!\n");
                exit(1);
            }

            // Create the socket
            if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
            {
                errorMessage("Cannot create socket");
                exit(1);
            }

            // Initialize and set up the address structure
            memset((char *)&server, 0, sizeof(struct sockaddr_in));
            server.sin_family = AF_INET;
            server.sin_port = htons(port);
            if ((hp = gethostbyname(host)) == NULL)
            {
                fprintf(stderr, "Unknown server address\n");
                errorMessage("Unknown server address");
                exit(1);
            }

            // Copy the server address
            memcpy((char *)&server.sin_addr, hp->h_addr, hp->h_length);

            // Connecting to the server
            if (::connect(sd, (struct sockaddr *)&server, sizeof(server)) == -1)
            {
                fprintf(stderr, "Can't connect to server\n");
                errorMessage("Can't connect to server");
                return;//exit(1);
            }
            printf("Connected:    Server Name: %s\n", hp->h_name);
            pptr = hp->h_addr_list;
            printf("\t\tIP Address: %s\n", inet_ntoa(server.sin_addr));

            for (;;)
            {
                printf("Transmiting:\n");
                memset((char *)sbuf, 0, sizeof(sbuf));
                gets(sbuf); // get user's text

                // Transmit data through the socket
                ns = send(sd, sbuf, BUFSIZE, 0);
                printf("Receive:\n");
                bp = rbuf;
                bytes_to_read = BUFSIZE;
/*
                // client makes repeated calls to recv until no more data is expected to arrive.
                while ((n = recv(sd, bp, bytes_to_read, 0)) < BUFSIZE)
                {
                    bp += n;
                    bytes_to_read -= n;
                    if (n == 0)
                        break;
                }
                */
                printf("%s\n\n", rbuf);
            }

            closesocket(sd);
            WSACleanup();
        }

        //check if it is multicast
        else if (cData.sMode == MULTICAST && (mode==0 || mode==1))
        {
            focusTab(0);
        }
        //if its neither multicast or unicast
        else
        {
            focusTab(mode);
        }
    }
}

void MainWindow::disconnectIt()
{
    //once disconnected all tabs are available again
    focusTab(-1);
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
