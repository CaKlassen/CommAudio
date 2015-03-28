#ifndef APPLICATION_H_
#define APPLICATION_H_

#define DEBUG

#include <WinSock2.h>
#include <vector>
#include "Network.h"

#define MUSIC_LOCATION "music"

enum ServerMode { MULTICAST, UNICAST };
typedef enum ServerMode ServerMode;

bool loadTracklist(std::vector<std::string> *tlist, std::string location);
bool startMulticast();
bool startUnicast();

bool playMulticast();
void sendCurrentSongMulti(int song);

void playUnicast(Client *c, std::string ip, std::string song);
void saveUnicast(Client *c, std::string ip, std::string song);

void sendCurrentSongUni(Client *c, std::string song, bool usingTCP);

#endif
