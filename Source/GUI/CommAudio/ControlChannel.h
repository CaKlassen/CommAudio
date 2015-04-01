#ifndef CONTROL_CHANNEL_H_
#define CONTROL_CHANNEL_H_

#include "CMessage.h"
#include "mainwindow.h"

void createControlString(CMessage *cMsg, string *str);
void parseControlString(string str, CMessage *cMsg);
void handleControlMessage(CMessage *cMsg);
void setGUIHandle(MainWindow *window);

#endif
