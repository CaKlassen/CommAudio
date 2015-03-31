#-------------------------------------------------
#
# Project created by QtCreator 2015-03-08T21:32:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CommAudio
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ControlChannel.cpp \
    Network.cpp \
    MusicBuffer.cpp

HEADERS  += mainwindow.h \
    ControlChannel.h \
    Network.h \
    CMessage.h \
    MusicBuffer.h

FORMS    += mainwindow.ui

LIBS += -lws2_32 -lwinmm

CONFIG += c++11
