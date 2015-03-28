#ifndef MUSIC_BUFFER_H_
#define MUSIC_BUFFER_H_

#define MESSAGE_SIZE 4096
#define BUFFER_SIZE (MESSAGE_SIZE * 1)

class MusicBuffer
{
    public:
        MusicBuffer();
        char* getBuffer();
        void put(char *data, int dataSize);
		bool ready();
        int getEndPosition();
        void clear();
    private:
        char buffer[BUFFER_SIZE];
        int endPosition;
};


#endif
