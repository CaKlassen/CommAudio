#ifndef MIC_H_
#define MIC_H_

#include <QAudioInput>
#include <QAudio>
#include <QUdpSocket>
#include <QBuffer>
#include "Network.h"

class Mic : QObject
{
    Q_OBJECT
    
    public:
        Mic();
        ~Mic();
        void setData(ClientState *cData);
        void startSending();
        void stopSending();
        
    public slots:
        void onNewState(QAudio::State state);
        
    private:
        QAudioInput *input;
        QUdpSocket *micSocket;
};

#endif
