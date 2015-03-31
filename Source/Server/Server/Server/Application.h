#ifndef APPLICATION_H_
#define APPLICATION_H_

#define DEBUG

#include <WinSock2.h>
#include <vector>
#include <vlc/vlc.h>
#include <vlc/libvlc.h>
#include "Network.h"

#define MUSIC_LOCATION "music"

bool loadTracklist(std::vector<std::string> *tlist, std::string location);
bool startMulticast();
bool startUnicast();

bool playMulticast();
void sendCurrentSongMulti(int song);

void playUnicast(Client *c, std::string ip, std::string song);
void saveUnicast(Client *c, std::string ip, std::string song);

void sendCurrentSongUni(Client *c, std::string song, bool usingTCP);

void startLibVLC();
void prepareRender(void* p_audio_data, uint8_t** pp_pcm_buffer, size_t size);
void handleStream(void* p_audio_data, uint8_t* p_pcm_buffer, unsigned int channels,
	unsigned int rate, unsigned int nb_samples, unsigned int bits_per_sample, size_t size, int64_t pts);

#endif
