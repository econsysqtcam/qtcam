#ifndef ALSAMASTER
#define ALSAMASTER
#include <QDebug>
#include <QObject>
#include <QStringListModel>
#include <pulse/pulseaudio.h>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <alsa/asoundlib.h>
#include <alsa/mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/asoundlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/ioctl.h>


class AlsaMaster : public QObject
{
public:
    AlsaMaster();

public slots:
    int SetAlsaMasterVolume(long volume);
    int GetAlsaMasterVolume();
    int SetAlsaMasterMute();
    int SetAlsaMasterUnMute();

};
#endif // ALSAMASTER

