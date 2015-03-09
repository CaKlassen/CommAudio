#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSlider>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);




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
