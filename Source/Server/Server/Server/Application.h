#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <vector>

#define MUSIC_LOCATION "music"

enum ServerMode { MULTICAST, UNICAST };
typedef enum ServerMode ServerMode;

bool loadTracklist(std::vector<std::string> *tlist, std::string location);
bool startMulticast();
bool startUnicast();

#endif