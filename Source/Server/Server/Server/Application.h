#ifndef APPLICATION_H_
#define APPLICATION_H_

#define DEBUG

#include <vector>

#define MUSIC_LOCATION "music"

enum ServerMode { MULTICAST, UNICAST };
typedef enum ServerMode ServerMode;

bool loadTracklist(std::vector<std::string> *tlist, std::string location);
bool startMulticast();
bool startUnicast();

bool playMulticast();
void sendCurrentSong(int song);

void playUnicast(std::string ip, std::string song);
void saveUnicast(std::string ip, std::string song);

#endif