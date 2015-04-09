/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: main.cpp - This file handles the start of the program.
--
-- PROGRAM: CommAudio.exe
--
-- FUNCTIONS:
--      int main(int argc, char *argv[])
--
-- DATE: February 23, 2015
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
--      This file contains the jumping off point for the application.
----------------------------------------------------------------------------------------------------------------------*/


#include "mainwindow.h"
#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <WinSock2.h>

#include "ControlChannel.h"
#include "Network.h"

WSADATA stWSAData;


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:    main
--
-- DATE:        February 23, 2015
--
-- REVISIONS:   (Date and Description)
--
-- DESIGNER:    Jonathan Chu
--
-- PROGRAMMER:  Jonathan Chu
--              Chris Klassen
--
-- INTERFACE:   int main(int argc, char *argv[]);
--
-- NOTES:
--          The main function of the application.
----------------------------------------------------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    // Start WinSock
    if (WSAStartup(0x0202, &stWSAData) != 0)
    {
        exit(1);
    }

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    ControlChannel::setGUIHandle(&w);
    Network::setGUIHandle(&w);
    
    return a.exec();
}
