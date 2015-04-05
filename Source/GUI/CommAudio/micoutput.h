#ifndef MICOUTPUT
#define MICOUTPUT

#include <QAudioOutput>
#include <QAudio>
#include <QBuffer>
#include <QUdpSocket>
#include "Network.h"

struct ClientState;

class MicOutput : QObject
{
    Q_OBJECT
    
    public:
        MicOutput();
        ~MicOutput();
        void setData(ClientState *cData);
        void startListening();
        void stopListening();
        
    public slots:
        void onNewState(QAudio::State state);
        void play();
        
    private:
        QAudioOutput *output;
        QIODevice *speakers;
        QUdpSocket *micSocket;
        
};

#endif // MICOUTPUT

