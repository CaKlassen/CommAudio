#ifndef MICINFO
#define MICINFO

#include <QAudioInput>
#include <QByteArray>

class MicInfo : public QIODevice
{
    public:
        MicInfo(const QAudioFormat &format);
        ~MicInfo();
        void start();
        void stop();
        qint64 readData(char *data, qint64 maxlen);
        qint64 writeData(const char *data, qint64 len);
private:
        const QAudioFormat micFormat;
        quint32 micMaxAmplitude;
        qreal micLevel;
};

#endif // MICINFO

