#include <QAudioDeviceInfo>
#include <QAudioInput>
#include <qendian.h>

#include "micinfo.h"

MicInfo::MicInfo(const QAudioFormat &format) :   
    micFormat(format), micMaxAmplitude(0), micLevel(0.0)
{
    switch (micFormat.sampleSize()) {
        case 8:
            switch (micFormat.sampleType()) {
            case QAudioFormat::UnSignedInt:
                micMaxAmplitude = 255;
                break;
            case QAudioFormat::SignedInt:
                micMaxAmplitude = 127;
                break;
            default:
                break;
            }
            break;
        case 16:
            switch (micFormat.sampleType()) {
            case QAudioFormat::UnSignedInt:
                micMaxAmplitude = 65535;
                break;
            case QAudioFormat::SignedInt:
                micMaxAmplitude = 32767;
                break;
            default:
                break;
            }
            break;
    
        case 32:
            switch (micFormat.sampleType()) {
            case QAudioFormat::UnSignedInt:
                micMaxAmplitude = 0xffffffff;
                break;
            case QAudioFormat::SignedInt:
                micMaxAmplitude = 0x7fffffff;
                break;
            case QAudioFormat::Float:
                micMaxAmplitude = 0x7fffffff; // Kind of
            default:
                break;
            }
         break;

        default:
            break;
    }
}


MicInfo::~MicInfo()
{
    // Empty
}


void MicInfo::start()
{
    open(QIODevice::WriteOnly);
}


void MicInfo::stop()
{
    close();
}

qint64 MicInfo::readData(char *data, qint64 maxlen)
{
    Q_UNUSED(data)
    Q_UNUSED(maxlen)

    return 0;
}


qint64 MicInfo::writeData(const char *data, qint64 len)
{
    if (micMaxAmplitude) {
        Q_ASSERT(micFormat.sampleSize() % 8 == 0);
        const int channelBytes = micFormat.sampleSize() / 8;
        const int sampleBytes = micFormat.channelCount() * channelBytes;
        Q_ASSERT(len % sampleBytes == 0);
        const int numSamples = len / sampleBytes;

        quint32 maxValue = 0;
        const unsigned char *ptr = reinterpret_cast<const unsigned char *>(data);

        for (int i = 0; i < numSamples; ++i) {
            for (int j = 0; j < micFormat.channelCount(); ++j) {
                quint32 value = 0;

                if (micFormat.sampleSize() == 8 && micFormat.sampleType() == QAudioFormat::UnSignedInt) {
                    value = *reinterpret_cast<const quint8*>(ptr);
                } else if (micFormat.sampleSize() == 8 && micFormat.sampleType() == QAudioFormat::SignedInt) {
                    value = qAbs(*reinterpret_cast<const qint8*>(ptr));
                } else if (micFormat.sampleSize() == 16 && micFormat.sampleType() == QAudioFormat::UnSignedInt) {
                    if (micFormat.byteOrder() == QAudioFormat::LittleEndian)
                        value = qFromLittleEndian<quint16>(ptr);
                    else
                        value = qFromBigEndian<quint16>(ptr);
                } else if (micFormat.sampleSize() == 16 && micFormat.sampleType() == QAudioFormat::SignedInt) {
                    if (micFormat.byteOrder() == QAudioFormat::LittleEndian)
                        value = qAbs(qFromLittleEndian<qint16>(ptr));
                    else
                        value = qAbs(qFromBigEndian<qint16>(ptr));
                } else if (micFormat.sampleSize() == 32 && micFormat.sampleType() == QAudioFormat::UnSignedInt) {
                    if (micFormat.byteOrder() == QAudioFormat::LittleEndian)
                        value = qFromLittleEndian<quint32>(ptr);
                    else
                        value = qFromBigEndian<quint32>(ptr);
                } else if (micFormat.sampleSize() == 32 && micFormat.sampleType() == QAudioFormat::SignedInt) {
                    if (micFormat.byteOrder() == QAudioFormat::LittleEndian)
                        value = qAbs(qFromLittleEndian<qint32>(ptr));
                    else
                        value = qAbs(qFromBigEndian<qint32>(ptr));
                } else if (micFormat.sampleSize() == 32 && micFormat.sampleType() == QAudioFormat::Float) {
                    value = qAbs(*reinterpret_cast<const float*>(ptr) * 0x7fffffff); // assumes 0-1.0
                }

                maxValue = qMax(value, maxValue);
                ptr += channelBytes;
            }
        }

        maxValue = qMin(maxValue, micMaxAmplitude);
        micLevel = qreal(maxValue) / micMaxAmplitude;
    }

    emit update();
    return len;
}
