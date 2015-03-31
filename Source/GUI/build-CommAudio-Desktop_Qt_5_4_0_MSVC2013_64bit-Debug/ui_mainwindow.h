/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionConnectDisconnect;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tab_5;
    QGridLayout *gridLayout_4;
    QPushButton *mPlayButton;
    QPushButton *mVolumeButton;
    QLabel *mSongLabel;
    QLabel *mCurrentSongLabel;
    QWidget *tab;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QListWidget *uPlayList;
    QListWidget *uDownloadList;
    QListWidget *uSongList;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *uPlayButton;
    QSlider *uMusicProgressSlider;
    QPushButton *uDownloadButton;
    QWidget *tab_3;
    QGridLayout *gridLayout_5;
    QPushButton *micButton;
    QWidget *tab_2;
    QGridLayout *gridLayout_6;
    QLabel *cPortLabel;
    QLabel *cFilepathLabel;
    QLineEdit *cFilepathText;
    QLineEdit *cIPAddressText;
    QLabel *cIPAddressLabel;
    QLineEdit *cPortText;
    QPushButton *cOKButton;
    QPushButton *cCancelButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(674, 548);
        MainWindow->setMaximumSize(QSize(793, 548));
        actionConnectDisconnect = new QAction(MainWindow);
        actionConnectDisconnect->setObjectName(QStringLiteral("actionConnectDisconnect"));
        actionConnectDisconnect->setCheckable(true);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setMinimumSize(QSize(490, 341));
        tabWidget->setTabShape(QTabWidget::Rounded);
        tabWidget->setIconSize(QSize(16, 16));
        tabWidget->setUsesScrollButtons(true);
        tabWidget->setDocumentMode(false);
        tabWidget->setTabsClosable(false);
        tabWidget->setMovable(false);
        tabWidget->setTabBarAutoHide(false);
        tab_5 = new QWidget();
        tab_5->setObjectName(QStringLiteral("tab_5"));
        gridLayout_4 = new QGridLayout(tab_5);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        mPlayButton = new QPushButton(tab_5);
        mPlayButton->setObjectName(QStringLiteral("mPlayButton"));

        gridLayout_4->addWidget(mPlayButton, 3, 0, 1, 1);

        mVolumeButton = new QPushButton(tab_5);
        mVolumeButton->setObjectName(QStringLiteral("mVolumeButton"));

        gridLayout_4->addWidget(mVolumeButton, 3, 4, 1, 1);

        mSongLabel = new QLabel(tab_5);
        mSongLabel->setObjectName(QStringLiteral("mSongLabel"));

        gridLayout_4->addWidget(mSongLabel, 0, 0, 1, 1);

        mCurrentSongLabel = new QLabel(tab_5);
        mCurrentSongLabel->setObjectName(QStringLiteral("mCurrentSongLabel"));
        QFont font;
        font.setPointSize(32);
        mCurrentSongLabel->setFont(font);

        gridLayout_4->addWidget(mCurrentSongLabel, 0, 1, 2, 1);

        tabWidget->addTab(tab_5, QString());
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        verticalLayout_3 = new QVBoxLayout(tab);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        uPlayList = new QListWidget(tab);
        uPlayList->setObjectName(QStringLiteral("uPlayList"));
        uPlayList->setMinimumSize(QSize(30, 0));
        uPlayList->setMaximumSize(QSize(30, 16777215));
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        uPlayList->setFont(font1);
        uPlayList->setStyleSheet(QLatin1String("border-style:solid;\n"
"border-color: black;\n"
"border-top-width: 1px;\n"
"border-left-width: 1px;\n"
"border-bottom-width:1px;"));
        uPlayList->setSelectionMode(QAbstractItemView::NoSelection);
        uPlayList->setSelectionRectVisible(true);

        horizontalLayout->addWidget(uPlayList);

        uDownloadList = new QListWidget(tab);
        uDownloadList->setObjectName(QStringLiteral("uDownloadList"));
        uDownloadList->setMinimumSize(QSize(30, 0));
        uDownloadList->setMaximumSize(QSize(30, 16777215));
        uDownloadList->setFont(font1);
        uDownloadList->setStyleSheet(QLatin1String("border-style:solid;\n"
"border-color: black;\n"
"border-top-width: 1px;\n"
"border-bottom-width:1px;"));
        uDownloadList->setSelectionMode(QAbstractItemView::NoSelection);
        uDownloadList->setSelectionRectVisible(true);

        horizontalLayout->addWidget(uDownloadList);

        uSongList = new QListWidget(tab);
        uSongList->setObjectName(QStringLiteral("uSongList"));
        uSongList->setStyleSheet(QLatin1String("border-style:solid;\n"
"border-color: black;\n"
"border-top-width: 1px;\n"
"border-right-width: 1px;\n"
"border-bottom-width:1px;"));
        uSongList->setSelectionRectVisible(false);

        horizontalLayout->addWidget(uSongList);


        verticalLayout_3->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        uPlayButton = new QPushButton(tab);
        uPlayButton->setObjectName(QStringLiteral("uPlayButton"));

        horizontalLayout_2->addWidget(uPlayButton);

        uMusicProgressSlider = new QSlider(tab);
        uMusicProgressSlider->setObjectName(QStringLiteral("uMusicProgressSlider"));
        uMusicProgressSlider->setEnabled(false);
        uMusicProgressSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_2->addWidget(uMusicProgressSlider);

        uDownloadButton = new QPushButton(tab);
        uDownloadButton->setObjectName(QStringLiteral("uDownloadButton"));

        horizontalLayout_2->addWidget(uDownloadButton);


        verticalLayout_3->addLayout(horizontalLayout_2);

        tabWidget->addTab(tab, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        gridLayout_5 = new QGridLayout(tab_3);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        micButton = new QPushButton(tab_3);
        micButton->setObjectName(QStringLiteral("micButton"));
        micButton->setMinimumSize(QSize(214, 200));
        micButton->setMaximumSize(QSize(214, 200));

        gridLayout_5->addWidget(micButton, 0, 0, 1, 1);

        tabWidget->addTab(tab_3, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        gridLayout_6 = new QGridLayout(tab_2);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        cPortLabel = new QLabel(tab_2);
        cPortLabel->setObjectName(QStringLiteral("cPortLabel"));
        cPortLabel->setMinimumSize(QSize(85, 20));
        cPortLabel->setMaximumSize(QSize(85, 20));

        gridLayout_6->addWidget(cPortLabel, 1, 0, 1, 1);

        cFilepathLabel = new QLabel(tab_2);
        cFilepathLabel->setObjectName(QStringLiteral("cFilepathLabel"));
        cFilepathLabel->setMinimumSize(QSize(85, 20));
        cFilepathLabel->setMaximumSize(QSize(85, 20));

        gridLayout_6->addWidget(cFilepathLabel, 2, 0, 1, 1);

        cFilepathText = new QLineEdit(tab_2);
        cFilepathText->setObjectName(QStringLiteral("cFilepathText"));
        cFilepathText->setMinimumSize(QSize(327, 20));
        cFilepathText->setMaximumSize(QSize(327, 20));

        gridLayout_6->addWidget(cFilepathText, 2, 1, 1, 1);

        cIPAddressText = new QLineEdit(tab_2);
        cIPAddressText->setObjectName(QStringLiteral("cIPAddressText"));
        cIPAddressText->setMinimumSize(QSize(327, 20));
        cIPAddressText->setMaximumSize(QSize(327, 20));

        gridLayout_6->addWidget(cIPAddressText, 0, 1, 1, 1);

        cIPAddressLabel = new QLabel(tab_2);
        cIPAddressLabel->setObjectName(QStringLiteral("cIPAddressLabel"));
        cIPAddressLabel->setMinimumSize(QSize(85, 20));
        cIPAddressLabel->setMaximumSize(QSize(85, 20));

        gridLayout_6->addWidget(cIPAddressLabel, 0, 0, 1, 1);

        cPortText = new QLineEdit(tab_2);
        cPortText->setObjectName(QStringLiteral("cPortText"));
        cPortText->setMinimumSize(QSize(327, 20));
        cPortText->setMaximumSize(QSize(327, 20));

        gridLayout_6->addWidget(cPortText, 1, 1, 1, 1);

        cOKButton = new QPushButton(tab_2);
        cOKButton->setObjectName(QStringLiteral("cOKButton"));
        cOKButton->setMinimumSize(QSize(122, 23));
        cOKButton->setMaximumSize(QSize(122, 23));

        gridLayout_6->addWidget(cOKButton, 3, 2, 1, 1);

        cCancelButton = new QPushButton(tab_2);
        cCancelButton->setObjectName(QStringLiteral("cCancelButton"));
        cCancelButton->setMinimumSize(QSize(122, 23));
        cCancelButton->setMaximumSize(QSize(122, 23));

        gridLayout_6->addWidget(cCancelButton, 3, 3, 1, 1);

        tabWidget->addTab(tab_2, QString());

        gridLayout->addWidget(tabWidget, 1, 1, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 674, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        mainToolBar->setEnabled(true);
        mainToolBar->setMovable(false);
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        mainToolBar->addAction(actionConnectDisconnect);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionConnectDisconnect->setText(QApplication::translate("MainWindow", "Connect", 0));
        mPlayButton->setText(QApplication::translate("MainWindow", "Play", 0));
        mVolumeButton->setText(QApplication::translate("MainWindow", "Volume", 0));
        mSongLabel->setText(QApplication::translate("MainWindow", "                  Song:", 0));
        mCurrentSongLabel->setText(QApplication::translate("MainWindow", "Current Song ~", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_5), QApplication::translate("MainWindow", "Multicast", 0));
        uPlayButton->setText(QApplication::translate("MainWindow", "Play", 0));
        uDownloadButton->setText(QApplication::translate("MainWindow", "Download", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Unicast", 0));
        micButton->setText(QApplication::translate("MainWindow", "Microphone On", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MainWindow", "Microphone", 0));
        cPortLabel->setText(QApplication::translate("MainWindow", "Port", 0));
        cFilepathLabel->setText(QApplication::translate("MainWindow", "File Path", 0));
        cIPAddressLabel->setText(QApplication::translate("MainWindow", "IP Address", 0));
        cOKButton->setText(QApplication::translate("MainWindow", "OK", 0));
        cCancelButton->setText(QApplication::translate("MainWindow", "Cancel", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Config.", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
