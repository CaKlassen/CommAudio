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
