#ifndef MUSIC_BUFFER_H_
#define MUSIC_BUFFER_H_

#define BUFFER_SIZE 1024

class MusicBuffer
{
    public:
        MusicBuffer();
        char* getBuffer();
        void put(char *data, int dataSize);
		int size();
        int getEndPosition();
        void clear();
    private:
        char buffer[BUFFER_SIZE];
        int endPosition;
};


#endif
