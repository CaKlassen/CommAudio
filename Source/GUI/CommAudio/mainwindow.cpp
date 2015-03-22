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

using std::string;
using std::vector;

// Client variables
string currentSong;
int songLength;
vector<string> tracklist;

ClientState cData;

QString IP = "default";
QString filePath = "c/";
int port = 4985;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set up the client data structure
    cData.ip = "127.0.0.1";
    cData.port = 0;
    cData.connected = false;
    cData.sMode = NOTHING;

    ui->lineEdit->setText(IP);
    ui->lineEdit_2->setText(QString::number(port));
    ui->lineEdit_3->setText(filePath);

    /* Populates the list widget  */
    for(int i= 1; i < 10; i++)
    {
        ui->listWidget->addItem("Song number " + QString::number(i));
        if (i == 1)
            ui->listWidget->setCurrentRow(0);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

//the play button
int starting = 0;
void MainWindow::on_pushButton_clicked()
{

    //This selects the item and then just make it blue
    QListWidgetItem *theItem = ui->listWidget->currentItem();

    theItem->setText("Selected --> " + theItem->text());
    theItem->setTextColor(Qt::blue);

    starting=starting+5; //once it reaches 100% it will stay as 100% even if "starting" is past 100
    ui->horizontalSlider->setValue(starting); // this is 50 as in 0%

}

//the download button

void MainWindow::on_pushButton_2_clicked()
{
    QListWidgetItem *theItem = ui->listWidget->currentItem();

    /* PopUp message */
    QMessageBox pop;
    pop.setText(theItem->text() + " is downloading");
    pop.exec();

}



/* This is for the microphone button  */

bool MicOn = true;

void MainWindow::on_pushButton_3_clicked()
{
    MicOn = !MicOn;
    if (MicOn) {
        ui->pushButton_3->setText("microphone ON");
    }
    else {
        ui->pushButton_3->setText("microphone OFF");
    }
}



void MainWindow::on_pushButton_4_clicked()
{

    QLabel *currentSong = ui->label_4;

    /* PopUp message */
    QMessageBox pop;
    pop.setText(currentSong->text() + " is playing");
    pop.exec();
}

void MainWindow::on_pushButton_5_clicked()
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
    ui->listWidget->clear();

    // Loop through and add each new song to the tracklist
    for (int i = 0; i < (int) songs->size(); i++)
    {
        tracklist.emplace_back(songs->at(i));

        // Update the tracklist GUI component
        ui->listWidget->addItem(QString::fromStdString(songs->at(i)));
    }

}



void MainWindow::on_actionDisconnect_triggered()
{
    //once disconnected all tabs are available again
    for (int n = 0; n < ui->tabWidget->count(); n++)
    {
            ui->tabWidget->setTabEnabled(n, true);
    }
}

void MainWindow::on_actionConnect_triggered()
{
    int mode = ui->tabWidget->currentIndex();

    //make sure connect does not work on the configuration
    if (ui->tabWidget->tabText(mode) != "Config.")
    {
        //check if its unicast
        if (cData.sMode == UNICAST && (mode==0 || mode==1))
        {
            for (int n = 0; n < ui->tabWidget->count(); n++)
            {
                if (n != 1)
                    ui->tabWidget->setTabEnabled(n, false);
            }
        }
        //check if it is multicast
        else if (cData.sMode == MULTICAST && (mode==0 || mode==1))
        {
            for (int n = 0; n < ui->tabWidget->count(); n++)
            {
                if (n != 0)
                    ui->tabWidget->setTabEnabled(n, false);
            }
        }
        //if its neither multicast or unicast
        else
        {
            for (int n = 0; n < ui->tabWidget->count(); n++)
            {
                if (n != mode)
                    ui->tabWidget->setTabEnabled(n, false);
            }
        }
    }
}

void MainWindow::on_pushButton_6_clicked()
{
    IP = ui->lineEdit->text();
    port = ui->lineEdit_2->text().toInt();
    filePath = ui->lineEdit_3->text();
}

void MainWindow::on_pushButton_7_clicked()
{
    ui->lineEdit->setText(IP);
    ui->lineEdit_2->setText(QString::number(port));
    ui->lineEdit_3->setText(filePath);

}
