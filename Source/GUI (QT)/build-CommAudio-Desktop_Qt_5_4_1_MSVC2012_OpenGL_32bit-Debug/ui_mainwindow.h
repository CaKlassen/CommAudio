/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
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
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
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
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tab_5;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QLabel *label_3;
    QLabel *label_4;
    QWidget *tab;
    QVBoxLayout *verticalLayout_3;
    QListWidget *listWidget;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton;
    QSlider *horizontalSlider;
    QPushButton *pushButton_2;
    QWidget *tab_3;
    QPushButton *pushButton_3;
    QWidget *tab_4;
    QLabel *label_2;
    QWidget *tab_2;
    QLabel *label;
    QMenuBar *menuBar;
    QMenu *menuTest1;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(820, 609);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab_5 = new QWidget();
        tab_5->setObjectName(QStringLiteral("tab_5"));
        pushButton_4 = new QPushButton(tab_5);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(220, 390, 75, 23));
        pushButton_5 = new QPushButton(tab_5);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setGeometry(QRect(530, 390, 75, 23));
        label_3 = new QLabel(tab_5);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(210, 150, 47, 13));
        label_4 = new QLabel(tab_5);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(230, 200, 401, 111));
        QFont font;
        font.setPointSize(32);
        label_4->setFont(font);
        tabWidget->addTab(tab_5, QString());
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        verticalLayout_3 = new QVBoxLayout(tab);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        listWidget = new QListWidget(tab);
        listWidget->setObjectName(QStringLiteral("listWidget"));

        verticalLayout_3->addWidget(listWidget);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        pushButton = new QPushButton(tab);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout_2->addWidget(pushButton);

        horizontalSlider = new QSlider(tab);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setEnabled(false);
        horizontalSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_2->addWidget(horizontalSlider);

        pushButton_2 = new QPushButton(tab);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout_2->addWidget(pushButton_2);


        verticalLayout_3->addLayout(horizontalLayout_2);

        tabWidget->addTab(tab, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        pushButton_3 = new QPushButton(tab_3);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(310, 160, 161, 161));
        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        label_2 = new QLabel(tab_4);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(240, 220, 251, 61));
        QFont font1;
        font1.setPointSize(25);
        label_2->setFont(font1);
        tabWidget->addTab(tab_4, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        label = new QLabel(tab_2);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(270, 210, 251, 61));
        label->setFont(font1);
        tabWidget->addTab(tab_2, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 820, 21));
        menuTest1 = new QMenu(menuBar);
        menuTest1->setObjectName(QStringLiteral("menuTest1"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuTest1->menuAction());

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);
        listWidget->setCurrentRow(-1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        pushButton_4->setText(QApplication::translate("MainWindow", "Play", 0));
        pushButton_5->setText(QApplication::translate("MainWindow", "Volume", 0));
        label_3->setText(QApplication::translate("MainWindow", "Song:", 0));
        label_4->setText(QApplication::translate("MainWindow", "Current Song ~", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_5), QApplication::translate("MainWindow", "Multicast", 0));
        pushButton->setText(QApplication::translate("MainWindow", "Play", 0));
        pushButton_2->setText(QApplication::translate("MainWindow", "Download", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Unicast", 0));
        pushButton_3->setText(QApplication::translate("MainWindow", "Microphone On", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MainWindow", "Microphone", 0));
        label_2->setText(QApplication::translate("MainWindow", "Nothing here yet as of now !", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("MainWindow", "Icon", 0));
        label->setText(QApplication::translate("MainWindow", "Nothing here yet as of now !", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Config.", 0));
        menuTest1->setTitle(QApplication::translate("MainWindow", "test1", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
