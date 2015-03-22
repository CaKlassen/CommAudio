#ifndef MUSIC_BUFFER_H_
#define MUSIC_BUFFER_H_

#include "Network.h"

class MusicBuffer
{
    public:
        MusicBuffer();
        char get();
        void put(char *data, int dataSize);
        int getCurrentPosition();
        int getEndPosition();
        void clear();
    private:
        char buffer[BUFFER_SIZE];
        int curPosition;
        int endPosition;
};


#endif
