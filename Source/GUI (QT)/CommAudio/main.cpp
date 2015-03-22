#include "mainwindow.h"
#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <WinSock2.h>

WSADATA stWSAData;

int main(int argc, char *argv[])
{
    // Start WinSock
    WSAStartup(0x0202, &stWSAData);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    // Stop WinSock
    WSACleanup();

    return a.exec();
}
