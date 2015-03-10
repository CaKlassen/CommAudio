#ifndef CONTROL_CHANNEL_H_
#define CONTROL_CHANNEL_H_

#include "CMessage.h"

void createControlString(CMessage &cMsg, string &str);
void parseControlString(string str, CMessage *cMsg);
void handleControlMessage(CMessage *cMsg);

#endif