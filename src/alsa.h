#ifndef ALSA
#define ALSA
#include <QDebug>
#include <QObject>

#include <alsa/asoundlib.h>
#include <alsa/mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/asoundlib.h>


class Alsa : public QObject
{

private:
    snd_mixer_t *mixer;
    snd_mixer_elem_t *cap_elem;

public:
    Alsa();
    ~Alsa();

    int initializeMixer(QString cardName);
    int closeMixer();


public slots:
    int setAlsaVolume(long volume);
    int getAlsaVolume(long *min, long *max);
    int setAlsaMute(bool mute);
    int getMuteStatus();

};
#endif // ALSA

