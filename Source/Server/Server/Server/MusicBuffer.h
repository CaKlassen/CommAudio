#ifndef MUSIC_BUFFER_H_
#define MUSIC_BUFFER_H_


/* The libvlc options string used to create an instance of the streamer */
#define VLC_OPTIONS "#transcode{acodec=s16l,samplerate=44100,channels=2}:smem{audio-postrender-callback=%lld,audio-prerender-callback=%lld}"

/* The number of output buffers for streaming audio */
#define NUM_OUTPUT_BUFFERS 3

/* The size of a message received from the socket */
#define MESSAGE_SIZE 512

/* The size of the circular audio buffer */
#define BUFFER_SIZE (MESSAGE_SIZE * 10)

/*
	This class is an encapsulation of a simple audio buffer. It is treated as a circular
	buffer for the purpose of audio playback.
*/
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
