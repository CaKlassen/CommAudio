#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <winsock2.h>
#include "Network.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setTracklist(std::vector<std::string> *songs);
    void disconnectIt();
    bool connectIt();
    void focusTab(int tabNumber);
    void errorMessage(QString message);
    void updateServerMode(ServerMode sMode);
    void updateMulticastSong(std::string title, std::string artist, std::string album);

private slots:
    void on_uPlayButton_clicked();

    void on_uDownloadButton_clicked();

    void on_micButton_clicked();

    void on_mPlayButton_clicked();

    void on_mVolumeButton_clicked();

    void on_actionConnectDisconnect_triggered();

    void on_cOKButton_clicked();

    void on_cCancelButton_clicked();

private:
    Ui::MainWindow *ui;

};

void outputAudio(MusicBuffer *buffer);
void CALLBACK WaveCallback(HWAVEOUT hWave, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
void sendMicrophone();
void endSong();

#endif // MAINWINDOW_H
