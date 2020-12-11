/*
 * Audioinput.cpp -- Audio input settings
 * Copyright © 2015  e-con Systems India Pvt. Limited
 *
 * This file is part of Qtcam.
 *
 * Qtcam is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * Qtcam is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Qtcam. If not, see <http://www.gnu.org/licenses/>.
 */

#include <QList>
#include <QAudioDeviceInfo>
#include <QAudioInput>
#include <QDebug>

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

/* support for internationalization - i18n */
#include <locale.h>
#include <libintl.h>

#include <time.h>
#include <sys/time.h>
#include <libintl.h>
#include <math.h>
#include <QTimer>
#include <QAudioDeviceInfo>
#include <QAudioInput>

#include "audioinput.h"


struct card {
    struct card *next;
    char *indexstr;
    char *name;
    char *device_name;
};

struct card first_card;


static pa_context *pa_ctx = NULL;
static u_int32_t latency_ms = 15;
static pa_usec_t latency = 0;
static int sample_index = 0;
static audio_buff_t *audio_buffers = NULL; /*pointer to buffers list*/
static int buffer_read_index = 0; /*current read index of buffer list*/
static int buffer_write_index = 0;/*current write index of buffer list*/

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;



QStringListModel AudioInput::audioinputDeviceList;
QStringListModel AudioInput::audiosupportedFmtListModel;
QStringListModel AudioInput::audioChannelCountModel;
QList<int> AudioInput::cardNum;
QStringList AudioInput::audioDeviceList;
QMap<int, QString> AudioInput::audioDeviceMap;
QMap<QString, int> AudioInput::audioDeviceSampleRateMap;
QMap<QString, int> AudioInput::audioDeviceChannelsMap;
QMap<int, QString> AudioInput::audioCardMap;
//int AudioInfo::source_index;
uint AudioInput::devIndex;

static pthread_t my_read_thread;
static pa_stream *recordstream = NULL;


AudioInfo::AudioInfo(const QAudioFormat &format, QObject *parent)
    :   QIODevice(parent)
    ,   m_format(format)
    ,   m_maxAmplitude(0)
    ,   m_level(0.0)

{
    switch (m_format.sampleSize()) {
    case 8:
        switch (m_format.sampleType()) {
        case QAudioFormat::UnSignedInt:
            m_maxAmplitude = 255;
            break;
        case QAudioFormat::SignedInt:
            m_maxAmplitude = 127;
            break;
        default:
            break;
        }
        break;
    case 16:
        switch (m_format.sampleType()) {
        case QAudioFormat::UnSignedInt:
            m_maxAmplitude = 65535;
            break;
        case QAudioFormat::SignedInt:
            m_maxAmplitude = 32767;
            break;
        default:
            break;
        }
        break;

    case 32:
        switch (m_format.sampleType()) {
        case QAudioFormat::UnSignedInt:
            m_maxAmplitude = 0xffffffff;
            break;
        case QAudioFormat::SignedInt:
            m_maxAmplitude = 0x7fffffff;
            break;
        case QAudioFormat::Float:
            m_maxAmplitude = 0x7fffffff; // Kind of
        default:
            break;
        }
        break;

    default:
        break;
    }
}

AudioInfo::~AudioInfo()
{
}

void AudioInfo::start()
{
    open(QIODevice::WriteOnly);
}

void AudioInfo::stop()
{
    close();
}

qint64 AudioInfo::readData(char *data, qint64 maxlen)
{
    Q_UNUSED(data)
    Q_UNUSED(maxlen)

    return 0;
}

qint64 AudioInfo::writeData(const char *data, qint64 len)
{

}


AudioInput::AudioInput()
{
    audio_buff = NULL;
}

AudioInput::~AudioInput()
{

}

/*
 * clean up and disconnect
 * args:
 *    pa_ctx - pointer to pulse context
 *    pa_ml - pointer to pulse mainloop
 *
 * asserts:
 *    none
 *
 * returns:
 *    none
 */
 void AudioInput::finish(pa_context *pa_ctx, pa_mainloop *pa_ml)
{
    /* clean up and disconnect */
    pa_context_disconnect(pa_ctx);
    pa_context_unref(pa_ctx);
    pa_mainloop_free(pa_ml);
}

/*
 * This callback gets called when our context changes state.
 *  We really only care about when it's ready or if it has failed
 * args:
 *    c -pointer to pulse context
 *    data - pointer to user data
 *
 * asserts:
 *    none
 *
 * retuns: none
 */
static void pa_state_cb(pa_context *c, void *data)
{
    pa_context_state_t state;
    int *pa_ready = (int *)data;
    state = pa_context_get_state(c);
    switch  (state)
    {
        // These are just here for reference
        case PA_CONTEXT_UNCONNECTED:
        case PA_CONTEXT_CONNECTING:
        case PA_CONTEXT_AUTHORIZING:
        case PA_CONTEXT_SETTING_NAME:
        default:
            break;
        case PA_CONTEXT_FAILED:
        case PA_CONTEXT_TERMINATED:
            *pa_ready = 2;
            break;
        case PA_CONTEXT_READY:
            *pa_ready = 1;
            break;
    }
}

void AudioInput::pa_sinklist_cb(pa_context *c, const pa_sink_info *l, int eol, void *userdata)
{
//    audio_context_t *audio_ctx = (audio_context_t *) data;

//    /*
//     * If eol is set to a positive number,
//     * you're at the end of the list
//     */
//    if (eol > 0)
//        return;

//    sink_index++;

//    if(verbosity > 0)
//    {
//        printf("AUDIO: =======[ Output Device #%d ]=======\n", sink_index);
//        printf("       Description: %s\n", l->description);
//        printf("       Name: %s\n", l->name);
//        printf("       Index: %d\n", l->index);
//        printf("       Channels: %d\n", l->channel_map.channels);
//        printf("       SampleRate: %d\n", l->sample_spec.rate);
//        printf("       Latency: %llu (usec)\n", (long long unsigned) l->latency);
//        printf("       Configured Latency: %llu (usec)\n", (long long unsigned) l->configured_latency);
//        printf("       Card: %d\n", l->card);
//        printf("\n");
//    }
}


/*
 * pa_mainloop will call this function when it's ready to tell us
 *  about a source (input).
 *  Since we're not threading when listing devices,
 *  there's no need for mutexes on the devicelist structure
 * args:
 *    c - pointer to pulse context
 *    l - pointer to source info
 *    eol - end of list
 *    data - pointer to user data (audio context)
 *
 * asserts:
 *    none
 *
 * returns: none
 */
void AudioInput::pa_sourcelist_cb(pa_context *c, const pa_source_info *l, int eol, void *data)
{
    audio_context_t *audio_ctx = (audio_context_t *) data;

    int channels = 1;

     /*
     * If eol is set to a positive number,
     * you're at the end of the list
     */
    if (eol > 0)
        return;

    source_index++;

    if(l->sample_spec.channels <1)
        channels = 1;
    else
        channels = l->sample_spec.channels;

    double my_latency = 0.0;


/*    printf("AUDIO: =======[ Input Device #%d ]=======\n", source_index);
    printf("       Description: %s\n", l->description);
    printf("       Name: %s\n", l->name);
    printf("       Index: %d\n", l->index);
    printf("       Channels: %d (default to: %d)\n", l->sample_spec.channels, channels);
    printf("       SampleRate: %d\n", l->sample_spec.rate);
    printf("       Latency: %llu (usec)\n", (long long unsigned) l->latency);
    printf("       Configured Latency: %llu (usec)\n", (long long unsigned) l->configured_latency);
    printf("       Card: %d\n", l->card);

    printf("\n");

*/
    if(my_latency <= 0.0)
        my_latency = (double) latency_ms / 1000;

    if(l->monitor_of_sink == PA_INVALID_INDEX)
    {        
        audio_ctx->num_input_dev++;
        /*add device to list*/
        audio_ctx->list_devices = (audio_device_t *)realloc(audio_ctx->list_devices, audio_ctx->num_input_dev * sizeof(audio_device_t));
        if(audio_ctx->list_devices == NULL)
        {
            fprintf(stderr,"AUDIO: FATAL memory allocation failure (pa_sourcelist_cb): %s\n", strerror(errno));
            exit(-1);
        }

        devIndex++;

        /*fill device data*/
        audio_ctx->list_devices[audio_ctx->num_input_dev-1].id = l->index; /*saves dev id*/
        strncpy(audio_ctx->list_devices[audio_ctx->num_input_dev-1].name,  l->name, 511);
        strncpy(audio_ctx->list_devices[audio_ctx->num_input_dev-1].description, l->description, 255);
        audio_ctx->list_devices[audio_ctx->num_input_dev-1].channels = channels;
        audio_ctx->list_devices[audio_ctx->num_input_dev-1].samprate = l->sample_spec.rate;
        audio_ctx->list_devices[audio_ctx->num_input_dev-1].low_latency = my_latency; /*in seconds*/
        audio_ctx->list_devices[audio_ctx->num_input_dev-1].high_latency = my_latency; /*in seconds*/
        audioDeviceList.append(audio_ctx->list_devices[audio_ctx->num_input_dev-1].description);
        audioDeviceMap.insertMulti(devIndex, audio_ctx->list_devices[audio_ctx->num_input_dev-1].name);
        audioDeviceSampleRateMap.insertMulti(audio_ctx->list_devices[audio_ctx->num_input_dev-1].name, audio_ctx->list_devices[audio_ctx->num_input_dev-1].samprate);
        audioDeviceChannelsMap.insertMulti(audio_ctx->list_devices[audio_ctx->num_input_dev-1].name, audio_ctx->list_devices[audio_ctx->num_input_dev-1].channels);

    }
}


/*
 * iterate the main loop until all devices are listed
 * args:
 *    audio_ctx - pointer to audio context
 *
 * asserts:
 *    audio_ctx is not null
 *
 * returns: error code
 */
 int AudioInput::pa_get_devicelist(audio_context_t *audio_ctx)
{
    audioDeviceList.append("--Select device--");
    /*assertions*/
    assert(audio_ctx != NULL);

    /* Define our pulse audio loop and connection variables */
    pa_mainloop *pa_ml;
    pa_mainloop_api *pa_mlapi;
    pa_operation *pa_op = NULL;
    pa_context *pa_ctx;

    /* We'll need these state variables to keep track of our requests */
    int state = 0;
    int pa_ready = 0;

    /* Create a mainloop API and connection to the default server */
    pa_ml = pa_mainloop_new();
    pa_mlapi = pa_mainloop_get_api(pa_ml);
    pa_ctx = pa_context_new(pa_mlapi, "getDevices");

    /* This function connects to the pulse server */
    if(pa_context_connect(pa_ctx, NULL, (pa_context_flags_t)0x0000U, NULL) < 0)
    {
        fprintf(stderr,"AUDIO: PULSE - unable to connect to server: pa_context_connect failed\n");
        finish(pa_ctx, pa_ml);
        return -1;
    }

    /*
     * This function defines a callback so the server will tell us
     * it's state.
     * Our callback will wait for the state to be ready.
     * The callback will modify the variable to 1 so we know when we
     * have a connection and it's ready.
     * If there's an error, the callback will set pa_ready to 2
     */
    pa_context_set_state_callback(pa_ctx, pa_state_cb, &pa_ready);

    /*
     * Now we'll enter into an infinite loop until we get the data
     * we receive or if there's an error
     */
    for (;;)
    {
        /*
         * We can't do anything until PA is ready,
         * so just iterate the mainloop and continue
         */
        if (pa_ready == 0)
        {
            pa_mainloop_iterate(pa_ml, 1, NULL);
            continue;
        }
        /* We couldn't get a connection to the server, so exit out */
        if (pa_ready == 2)
        {
            finish(pa_ctx, pa_ml);
            return -1;
        }
        /*
         * At this point, we're connected to the server and ready
         * to make requests
         */
        switch (state)
        {
            /* State 0: we haven't done anything yet */
            case 0:
                /*
                 * This sends an operation to the server.
                 * pa_sinklist_cb is our callback function and a pointer
                 * o our devicelist will be passed to the callback
                 * (audio_ctx) The operation ID is stored in the
                 * pa_op variable
                 */
                pa_op = pa_context_get_sink_info_list(
                          pa_ctx,
                          pa_sinklist_cb,
                          (void *) audio_ctx);

                /* Update state for next iteration through the loop */
                state++;
                break;
            case 1:
                /*
                 * Now we wait for our operation to complete.
                 * When it's complete our pa_output_devicelist is
                 * filled out, and we move along to the next state
                 */
                if (pa_operation_get_state(pa_op) == PA_OPERATION_DONE)
                {
                    pa_operation_unref(pa_op);

                    /*
                     * Now we perform another operation to get the
                     * source(input device) list just like before.
                     * This time we pass a pointer to our input structure
                     */
                    pa_op = pa_context_get_source_info_list(
                              pa_ctx,
                              pa_sourcelist_cb,
                              (void *) audio_ctx);
                    /* Update the state so we know what to do next */
                    state++;
                }
                break;
            case 2:
                if (pa_operation_get_state(pa_op) == PA_OPERATION_DONE)
                {
                    /*
                     * Now we're done,
                     * clean up and disconnect and return
                     */
                    pa_operation_unref(pa_op);
                    finish(pa_ctx, pa_ml);
                    return 0;
                }
                break;
            default:
                /* We should never see this state */
                printf("AUDIO: Pulseaudio in state %d\n", state);
                return -1;
        }
        /*
         * Iterate the main loop and go again.  The second argument is whether
         * or not the iteration should block until something is ready to be
         * done.  Set it to zero for non-blocking.
         */
        pa_mainloop_iterate(pa_ml, 1, NULL);
    }

    return 0;
}

/*
 * update pulseaudio latency
 * args:
 *    s - pointer to pa_stream
 *
 * asserts:
 *    none
 *
 * returns:none
 */
 void AudioInput::get_latency(pa_stream *s)
{
    pa_usec_t l;
    int negative;

    pa_stream_get_timing_info(s);

    if (pa_stream_get_latency(s, &l, &negative) != 0)
    {
        fprintf(stderr, "AUDIO: Pulseaudio pa_stream_get_latency() failed\n");
        return;
    }

    //latency = l * (negative?-1:1);
    latency = l; /*can only be negative in monitoring streams*/

}


void AudioInput::audio_fill_buffer(audio_context_t *audio_ctx, int64_t ts, void *arg)
{
    AudioInput *audioInput = static_cast<AudioInput *>(arg);
     /*assertions*/
     assert(audio_ctx != NULL);

     /*in nanosec*/
     uint64_t frame_length = NSEC_PER_SEC / audio_ctx->samprate;
     uint64_t buffer_length = frame_length * (audio_ctx->capture_buff_size / audio_ctx->channels);

     audio_ctx->current_ts += buffer_length; /*buffer end time*/

     audio_ctx->ts_drift = audio_ctx->current_ts - ts;

     /*get the current write indexed buffer flag*/
     pthread_mutex_lock(&mutex);
     int flag = audio_buffers[buffer_write_index].flag;
     pthread_mutex_unlock(&mutex);
     if(flag == AUDIO_BUFF_USED)
     {
         fprintf(stderr, "AUDIO: write buffer(%i) is still in use - dropping data\n", buffer_write_index);
         return;
     }

     /*write max_frames and fill a buffer*/
     memcpy(audio_buffers[buffer_write_index].data,
         audio_ctx->capture_buff,
         audio_ctx->capture_buff_size * sizeof(sample_t));
     /*buffer begin time*/
     audio_buffers[buffer_write_index].timestamp = audio_ctx->current_ts - buffer_length;

     audio_buffers[buffer_write_index].level_meter[0] = audio_ctx->capture_buff_level[0];
     audio_buffers[buffer_write_index].level_meter[1] = audio_ctx->capture_buff_level[1];

     pthread_mutex_lock(&mutex);
     audio_buffers[buffer_write_index].flag = AUDIO_BUFF_USED;
     NEXT_IND(buffer_write_index, AUDBUFF_NUM);
     pthread_mutex_unlock(&mutex);

     audioInput->captureAudio();

 }

void AudioInput::recordAudio(){
    captureAudio();
}

/*
 * audio record callback
 * args:
 *   s - pointer to pa_stream
 *   length - buffer length
 *   data - pointer to user data
 *
 * asserts:
 *   none
 *
 * returns: none
 */
 void AudioInput::stream_request_cb(pa_stream *s, size_t length, void *arg)
{
//    audio_context_t *audio_ctx = (audio_context_t *) data;

     AudioInput *audioInput = static_cast<AudioInput *>(arg);

    if(audioInput->audio_context->channels == 0)
    {
        fprintf(stderr, "AUDIO: (pulseaudio) stream_request_cb failed: channels = 0\n");
        return;
    }

    if(audioInput->audio_context->samprate == 0)
    {
        fprintf(stderr, "AUDIO: (pulseaudio) stream_request_cb failed: samprate = 0\n");
        return;
    }

    uint64_t frame_length = NSEC_PER_SEC / audioInput->audio_context->samprate; /*in nanosec*/
    int64_t ts = 0;
    int64_t buff_ts = 0;
    uint32_t i = 0;

    while (pa_stream_readable_size(s) > 0)
    {        
        const void *inputBuffer;
        size_t length;

        /*read from stream*/
        if (pa_stream_peek(s, &inputBuffer, &length) < 0)
        {
            fprintf(stderr, "AUDIO: (pulseaudio) pa_stream_peek() failed\n");
            return;
        }

        if(length == 0)
        {
            fprintf(stderr, "AUDIO: (pulseaudio) empty buffer!\n");
            return; /*buffer is empty*/
        }

        get_latency(s);

        struct timespec now;

        if(clock_gettime(CLOCK_MONOTONIC, &now) != 0)
        {
            fprintf(stderr, "V4L2_CORE: ns_time_monotonic (clock_gettime) error: %s\n", strerror(errno));
        }

        u_int64_t nanosecs = ((u_int64_t)now.tv_sec * NSEC_PER_SEC + (u_int64_t) now.tv_nsec);


        ts = nanosecs - (latency * 1000);

        if(audioInput->audio_context->last_ts <= 0)
            audioInput->audio_context->last_ts = ts;


        uint32_t numSamples = (uint32_t) length / sizeof(sample_t);

        const sample_t *rptr = (const sample_t*) inputBuffer;
        sample_t *capture_buff = (sample_t *) audioInput->audio_context->capture_buff;        

        int chan = 0;
        /*store capture samples or silence if inputBuffer == NULL (hole)*/
        for( i = 0; i < numSamples; ++i )
        {
            capture_buff[sample_index] = inputBuffer ? *rptr++ : 0;
            sample_index++;

            /*store peak value*/
            if(audioInput->audio_context->capture_buff_level[chan] < capture_buff[sample_index])
                audioInput->audio_context->capture_buff_level[chan] = capture_buff[sample_index];
            chan++;
            if(chan >= audioInput->audio_context->channels)
                chan = 0;

            if(sample_index >= audioInput->audio_context->capture_buff_size)
            {
                buff_ts = ts + ( i / audioInput->audio_context->channels ) * frame_length;

                audio_fill_buffer(audioInput->audio_context, buff_ts, (void *) arg);
                /*reset*/
                audioInput->audio_context->capture_buff_level[0] = 0;
                audioInput->audio_context->capture_buff_level[1] = 0;
                sample_index = 0;
            }
        }

        pa_stream_drop(s); /*clean the samples*/
    }

}

/*
 * Iterate the main loop while recording is on.
 * This function runs under it's own thread called by audio_pulse_start
 * args:
 *   data - pointer to user data (audio context)
 *
 * asserts:
 *   data is not null
 *
 * returns: pointer to error code
 */
 void *AudioInput::pulse_read_audio(void *arg)
{
     AudioInput *audioInput = static_cast<AudioInput *>(arg);    
//    audio_context_t *audio_ctx = (audio_context_t *) data;
//    /*assertions*/
//    assert(audio_ctx != NULL);


    printf("AUDIO: (pulseaudio) read thread started\n");
    pa_mainloop *pa_ml;
    pa_mainloop_api *pa_mlapi;
    pa_buffer_attr bufattr;
    pa_sample_spec ss;
    pa_stream_flags_t flags = PA_STREAM_NOFLAGS;
    int r;
    int pa_ready = 0;

    /* Create a mainloop API and connection to the default server */
    pa_ml = pa_mainloop_new();
    pa_mlapi = pa_mainloop_get_api(pa_ml);
    pa_ctx = pa_context_new(pa_mlapi, "Qtcam Pulse API");

    if(pa_context_connect(pa_ctx, NULL, (pa_context_flags_t)0x0000U, NULL) < 0)
    {
        fprintf(stderr,"AUDIO: PULSE - unable to connect to server: pa_context_connect failed\n");
        //finish(pa_ctx, pa_ml);
        return ((void *) -1);
    }

    /*
     * This function defines a callback so the server will tell us it's state.
     * Our callback will wait for the state to be ready.  The callback will
     * modify the variable to 1 so we know when we have a connection and it's
     * ready.
     * If there's an error, the callback will set pa_ready to 2
     */
    pa_context_set_state_callback(pa_ctx, pa_state_cb, &pa_ready);

    /*
     * This function defines a time event callback (called every TIME_EVENT_USEC)
     */
    //pa_context_rttime_new(pa_ctx, pa_rtclock_now() + TIME_EVENT_USEC, time_event_callback, NULL);

    /*
     * We can't do anything until PA is ready, so just iterate the mainloop
     * and continue
     */
    while (pa_ready == 0)
    {
        pa_mainloop_iterate(pa_ml, 1, NULL);
    }
    if (pa_ready == 2)
    {
        //finish(pa_ctx, pa_ml);
        return ((void *) -1);
    }

    /* set the sample spec (frame rate, channels and format) */
    ss.rate = audioInput->audio_context->samprate;
    ss.channels = audioInput->audio_context->channels;    
    ss.format = PA_SAMPLE_FLOAT32LE; /*for PCM -> PA_SAMPLE_S16LE*/

    recordstream = pa_stream_new(pa_ctx, "Record", &ss, NULL);
    if (!recordstream)
        fprintf(stderr, "AUDIO: (pulseaudio) pa_stream_new failed (chan:%d rate:%d)\n",
            ss.channels, ss.rate);

    /* define the callbacks */
    pa_stream_set_read_callback(recordstream, stream_request_cb, (void *) arg);

    // Set properties of the record buffer
    pa_zero(bufattr);
    /* optimal value for all is (uint32_t)-1   ~= 2 sec */
    bufattr.maxlength = (uint32_t) -1;
    bufattr.prebuf = (uint32_t) -1;
    bufattr.minreq = (uint32_t) -1;

    if (audioInput->audio_context->latency > 0)
    {
      bufattr.fragsize = bufattr.tlength = pa_usec_to_bytes((audioInput->audio_context->latency * 1000) * PA_USEC_PER_MSEC, &ss);
      flags = static_cast<pa_stream_flags> (flags | PA_STREAM_ADJUST_LATENCY);
    }
    else
      bufattr.fragsize = bufattr.tlength = (uint32_t) -1;

    flags = static_cast<pa_stream_flags> (flags | PA_STREAM_INTERPOLATE_TIMING);
    flags = static_cast<pa_stream_flags> (flags | PA_STREAM_AUTO_TIMING_UPDATE);

    char * dev = audioInput->audio_context->list_devices[audioInput->audio_context->device].name;

    printf("AUDIO: (pulseaudio) connecting to device %s\n\t (channels %d rate %d)\n",
            dev, ss.channels, ss.rate);
    r = pa_stream_connect_record(recordstream, dev, &bufattr, flags);
    if (r < 0)
    {
        fprintf(stderr, "AUDIO: (pulseaudio) skip latency adjustment\n");
        /*
         * Old pulse audio servers don't like the ADJUST_LATENCY flag,
         * so retry without that
         */
        r = pa_stream_connect_record(recordstream, dev, &bufattr,
                                     static_cast<pa_stream_flags>(PA_STREAM_INTERPOLATE_TIMING|
                                     PA_STREAM_AUTO_TIMING_UPDATE));
    }
    if (r < 0)
    {
        fprintf(stderr, "AUDIO: (pulseaudio) pa_stream_connect_record failed\n");
        finish(pa_ctx, pa_ml);
        return ((void *) -1);
    }

    get_latency(recordstream);

    /*
     * Iterate the main loop while streaming.  The second argument is whether
     * or not the iteration should block until something is ready to be
     * done.  Set it to zero for non-blocking.
     */
    while (audioInput->audio_context->stream_flag == AUDIO_STRM_ON)
    {
        pa_mainloop_iterate(pa_ml, 1, NULL);
    }


    printf("AUDIO: (pulseaudio) stream terminated(%i)\n", audioInput->audio_context->stream_flag);

    pa_stream_disconnect (recordstream);
    pa_stream_unref (recordstream);
    finish(pa_ctx, pa_ml);
    return ((void *) 0);
}

bool AudioInput::audio_init()
{    
    audioDeviceList.clear();
    audio_context = audio_init_pulseaudio();
    if(audio_context == NULL){
        return false;
    }
    audioinputDeviceList.setStringList(audioDeviceList);
    /*
         * alloc the buffer after audio_start
         * otherwise capture_buff_size may not
         * be correct
         * allocated data is big enough for float samples (32 bit)
         * although it may contain int16 samples (16 bit)
         */
    return true;
}

int AudioInput::getCards(void)
{
    int count, number, err;
    snd_ctl_t *ctl;
    snd_ctl_card_info_t *info;
    char buf[16];
    struct card *card, *prev_card;

    first_card.indexstr = "-";
    first_card.name = ("(default)");
    first_card.device_name = "default";
    count = 1;

    snd_ctl_card_info_alloca(&info);
    prev_card = &first_card;
    number = -1;
    for (;;) {
        err = snd_card_next(&number);
        if (err < 0)
            qDebug()<<"cannot enumerate";
        if (number < 0)
            break;
        sprintf(buf, "hw:%d", number);
        err = snd_ctl_open(&ctl, buf, 0);
        if (err < 0)
            continue;
        err = snd_ctl_card_info(ctl, info);
        snd_ctl_close(ctl);
        if (err < 0)
            continue;
        card = (struct card *)calloc(1, sizeof *card);
        sprintf(buf, "%d", number);
        card->indexstr = strdup(buf);
        card->name = strdup(snd_ctl_card_info_get_name(info));
        sprintf(buf, "hw:%d", number);
        card->device_name = strdup(buf);        

        prev_card->next = card;
        prev_card = card;

        audioCardMap.insertMulti(count, card->device_name); // map device count and  device name
        ++count;
    }

    card = (struct card *)calloc(1, sizeof *card);
    card->indexstr = strdup(" ");
    card->name = strdup(("enter device name..."));
    prev_card->next = card;
    ++count;

    return count;
}


bool AudioInput::updateSupportedInfo(uint currentIndex)
{   
    getCards();
    QString cardName;
    QMap<int, QString>::iterator cardNameIterator;
    for (cardNameIterator = audioCardMap.begin(); cardNameIterator != audioCardMap.end(); ++cardNameIterator)
    {
        if(cardNameIterator.key() == currentIndex){
            cardName = cardNameIterator.value();
            break;
        }
    }

    // close mixer
    alsa.closeMixer();

    // opne, attach, load mixer
    alsa.initializeMixer(cardName);

    QString audioDeviceName;
    QMap<int, QString>::iterator audioDeviceNameIterator;
    for (audioDeviceNameIterator = audioDeviceMap.begin(); audioDeviceNameIterator != audioDeviceMap.end(); ++audioDeviceNameIterator)
    {
        if(audioDeviceNameIterator.key() == currentIndex){
            audioDeviceName = audioDeviceNameIterator.value();
            break;
        }
    }

    samplerateStringList.clear();
    channelCountStringList.clear();
    QMap<QString, int>::iterator audioDeviceSampleRateIterator;
    for (audioDeviceSampleRateIterator = audioDeviceSampleRateMap.begin(); audioDeviceSampleRateIterator != audioDeviceSampleRateMap.end(); ++audioDeviceSampleRateIterator)
    {
        if(audioDeviceSampleRateIterator.key().contains(audioDeviceName)){
            samplerateStringList.append(QString::number(audioDeviceSampleRateIterator.value()));
            break;
        }
    }
    QMap<QString, int>::iterator audioDeviceChannelsIterator;
    for (audioDeviceChannelsIterator = audioDeviceChannelsMap.begin(); audioDeviceChannelsIterator != audioDeviceSampleRateMap.end(); ++audioDeviceChannelsIterator)
    {
        if(audioDeviceChannelsIterator.key().contains(audioDeviceName)){
            channelCountStringList.append(QString::number(audioDeviceChannelsIterator.value()));
            break;
        }
    }

    long min, max;

    // get mute status
    int muteState = alsa.getMuteStatus();
    emit muteStateChanged(muteState);

    // Using Alsa, get volume
    qreal volume = alsa.getAlsaVolume(&min, &max); // min - 0 , max 6 -> in see3cam_cu38 camera

    // while setting mic volume , range is (1-7), while getting mic volume , range is (min:0, max:6),
    // So here max+1 is used.(Ex: 6+1=7)
    uint currentVolume = uint((volume*100/max+1));
    emit volumeChanged(currentVolume);

    audiosupportedFmtListModel.setStringList(samplerateStringList);
    audioChannelCountModel.setStringList(channelCountStringList);
}

bool AudioInput::setVolume(int micVolume){
    alsa.setAlsaVolume(micVolume); // mic volume [ While setting mic volume, In slider (Range: 1-100), In camera (Range 1-7) ]
}

bool AudioInput::setMuteState(bool mute){
    if(mute)
    {
        alsa.setAlsaMute(true);
    }
    else
    {
         alsa.setAlsaMute(false);
    }
}
void AudioInput::setSampleRate(int sampleRate){
    assert(audio_context != NULL);    
    audio_context->samprate = sampleRate;    
}

void AudioInput::setChannelCount(uint index){
    int channels = 0;   
    assert(audio_context != NULL);
    switch(index)
    {
        case 0:
            channels =  audio_context->list_devices[audio_context->device].channels;
            break;
        case 1:
            channels =  1;
            break;
        default:
        case 2:
            channels = 2;
            break;
    }
    if(channels > audio_context->list_devices[audio_context->device].channels || index == 0){
        audio_context->channels = audio_context->list_devices[audio_context->device].channels;
    }else{
        audio_context->channels = channels;
    }

    if(audio_context->channels > 2)
        audio_context->channels = 2; /*limit to stereo*/    
}

audio_context_t* AudioInput::audio_init_pulseaudio()
{  
    audio_context_t *audio_ctx = (audio_context_t *)calloc(1, sizeof(audio_context_t));

    devIndex = 0;

    if(audio_ctx == NULL)
    {
        fprintf(stderr,"AUDIO: FATAL memory allocation failure (audio_init_pulseaudio): %s\n", strerror(errno));
        exit(-1);
    }

    if (pa_get_devicelist(audio_ctx) < 0)
    {
        fprintf(stderr, "AUDIO: Pulseaudio failed to get audio device list from PULSE server\n");
        free(audio_ctx);
        return NULL;
    }
    audio_context = audio_ctx;
    return audio_ctx;
}

/*
 * set audio device
 * args:
 *   audio_context - pointer to audio context data
 *   index - device index to set
 *
 * asserts:
 *   audio_context is not null
 *
 * returns: none
 */
void AudioInput::audio_set_pulseaudio_device(int index)
{    
    /*assertions*/
    assert(audio_context != NULL);

    if(index >= audio_context->num_input_dev)
        audio_context->device = audio_context->num_input_dev - 1;
    else if(index > 0 )
        audio_context->device = index;

    audio_context->latency = audio_context->list_devices[audio_context->device].high_latency;

    audio_context->channels = audio_context->list_devices[audio_context->device].channels;
    if(audio_context->channels > 2)
        audio_context->channels = 2;/*limit it to stereo input*/
    audio_context->samprate = audio_context->list_devices[audio_context->device].samprate;   
}


/*
 * alloc audio buffers
 * args:
 *    audio_ctx - pointer to audio context data
 *
 * asserts:
 *    none
 *
 * returns: error code
 */
int AudioInput::audio_init_buffers(audio_context_t *audio_ctx)
{   
    if(!audio_ctx)
        return -1;

    int i = 0;

    /*set the buffers size*/
    if(!audio_ctx->capture_buff_size)
        audio_ctx->capture_buff_size = audio_ctx->channels * AUDBUFF_FRAMES;   

    if(audio_ctx->capture_buff)
        free(audio_ctx->capture_buff);

    audio_ctx->capture_buff = (sample_t *)calloc(
        audio_ctx->capture_buff_size, sizeof(sample_t));
    if(audio_ctx->capture_buff == NULL)
    {
        fprintf(stderr,"AUDIO: FATAL memory allocation failure (audio_init_buffers): %s\n", strerror(errno));
        exit(-1);
    }

    /*free audio_buffers (if any)*/
    audio_free_buffers;

    audio_buffers = (audio_buff_t *)calloc(AUDBUFF_NUM, sizeof(audio_buff_t));
    if(audio_buffers == NULL)
    {
        fprintf(stderr,"AUDIO: FATAL memory allocation failure (audio_init_buffers): %s\n", strerror(errno));
        exit(-1);
    }

    for(i = 0; i < AUDBUFF_NUM; ++i)
    {
        audio_buffers[i].data = calloc(
            audio_ctx->capture_buff_size, sizeof(sample_t));
        if(audio_buffers[i].data == NULL)
        {
            fprintf(stderr,"AUDIO: FATAL memory allocation failure (audio_init_buffers): %s\n", strerror(errno));
            exit(-1);
        }
        audio_buffers[i].flag = AUDIO_BUFF_FREE;
    }

    return 0;
}

int AudioInput::audio_free_buffers()
{   
    buffer_read_index = 0;
    buffer_write_index = 0;

    /*return if no buffers set*/
    if(!audio_buffers)
    {
        fprintf(stderr,"AUDIO: can't free audio buffers (audio_free_buffers): audio_buffers is null\n");
        return 0;
    }

    int i = 0;

    for(i = 0; i < AUDBUFF_NUM; ++i)
    {
        if(audio_buffers[i].data)
        {
            free(audio_buffers[i].data);
            audio_buffers[i].data = NULL;
        }
    }
    if(audio_buffers)
    free(audio_buffers);
    audio_buffers = NULL;
}


/*
 * Launch the main loop iteration thread
 * args:
 *   audio_ctx - pointer to audio context data
 *
 * asserts:
 *   audio_ctx is not null
 *
 * returns: error code
 */
int AudioInput::audio_start_pulseaudio(VideoEncoder videoEncoder)
{  
    /*assertions*/
    assert(audio_context != NULL);

    /*alloc the ring buffer*/
    audio_init_buffers(audio_context);

    /*reset timestamp values*/
    audio_context->current_ts = 0;
    audio_context->last_ts = 0;
    audio_context->snd_begintime = 0;
    audio_context->ts_drift = 0;

    audio_context->stream_flag = AUDIO_STRM_ON;

    /* start audio capture thread */
    if(pthread_create(&my_read_thread, NULL, pulse_read_audio, this))
    {
        fprintf(stderr, "AUDIO: (pulseaudio) read thread creation failed\n");
        audio_context->stream_flag = AUDIO_STRM_OFF;
        return (-1);
    }

    timer = new QTimer();
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(recordAudio()));
    timer->start(0);

    return 0;
}

/*
 * stop and join the main loop iteration thread
 * args:
 *   audio_ctx - pointer to audio context data
 *
 * asserts:
 *   audio_ctx is not null
 *
 * returns: error code
 */
int AudioInput::audio_stop_pulseaudio()
{   
    timer->stop();

    /*assertions*/
    assert(audio_context != NULL);

    audio_context->stream_flag = AUDIO_STRM_OFF;

    pthread_join(my_read_thread, NULL);

    printf("AUDIO: (pulseaudio) read thread joined\n");

    return 0;
}

/*
 * close and clean audio context for pulseaudio api
 * args:
 *   audio_ctx - pointer to audio context data
 *
 * asserts:
 *   none
 *
 * returns: none
 */
void AudioInput::audio_close_pulseaudio()
{  
    if(audio_context == NULL)
        return;

    if(audio_context->stream_flag == AUDIO_STRM_ON)
        audio_stop_pulseaudio();

    if(audio_context->list_devices != NULL)
        free(audio_context->list_devices);
    audio_context->list_devices = NULL;

    if(audio_context->capture_buff)
        free(audio_context->capture_buff);

    free(audio_context);
}

/*
 * alloc a single audio buffer
 * args:
 *    audio_ctx - pointer to audio context data
 *
 * asserts:
 *    none
 *
 * returns: pointer to newly allocate audio buffer or NULL on error
 *   must be freed with audio_delete_buffer
 * data is allocated for float(32 bit) samples but can also store
 * int16 (16 bit) samples
 */
audio_buff_t *AudioInput::audio_get_buffer()
{
    if(audio_context->capture_buff_size <= 0)
    {
        fprintf(stderr, "AUDIO: (get_buffer) invalid capture_buff_size(%i)\n",
            audio_context->capture_buff_size);
        return NULL;
    }

    audio_buff_t *audio_buff = (audio_buff_t *)calloc(1, sizeof(audio_buff_t));
    if(audio_buff == NULL)
    {
        fprintf(stderr,"AUDIO: FATAL memory allocation failure (audio_get_buffer): %s\n", strerror(errno));
        exit(-1);
    }
    audio_buff->data = calloc(audio_context->capture_buff_size, sizeof(sample_t));
    if(audio_buff->data == NULL)
    {
        fprintf(stderr,"AUDIO: FATAL memory allocation failure (audio_get_buffer): %s\n", strerror(errno));
        exit(-1);
    }

    return audio_buff;
}


int16_t AudioInput::clip_int16 (float in)
{
    //int16_t out = (int16_t) (in < -32768) ? -32768 : (in > 32767) ? 32767 : in;

    long lout =  lroundf(in);
    int16_t out = (lout < -32768) ? -32768 : (lout > 32767) ? 32767: (int16_t) lout;
    return (out);
}

/*
 * get the next used buffer from the ring buffer
 * args:
 *   audio_ctx - pointer to audio context
 *   buff - pointer to an allocated audio buffer
 *   type - type of data (SAMPLE_TYPE_[INT16|FLOAT])
 *   mask - audio fx mask
 *
 * asserts:
 *   none
 *
 * returns: error code
 */
int AudioInput::audio_get_next_buffer(audio_buff_t *buff)
{  
    pthread_mutex_lock(&mutex);
    int flag = audio_buffers[buffer_read_index].flag;
    pthread_mutex_unlock(&mutex);

    if(flag == AUDIO_BUFF_FREE)
        return 1; /*all done*/

//    /*aplly fx*/
//    audio_fx_apply(audio_context, (sample_t *) audio_buffers[buffer_read_index].data, mask);

    /*copy data into requested format type*/
    int i = 0;
    int type = GV_SAMPLE_TYPE_INT16;  // hardcoded - need to change
    switch(type)
    {
        case GV_SAMPLE_TYPE_FLOAT:
        {
            sample_t *my_data = (sample_t *) buff->data;
            memcpy( my_data, audio_buffers[buffer_read_index].data,
                audio_context->capture_buff_size * sizeof(sample_t));
            break;
        }
        case GV_SAMPLE_TYPE_INT16:
        {
            int16_t *my_data = (int16_t *) buff->data;
            sample_t *buff_p = (sample_t *) audio_buffers[buffer_read_index].data;
            for(i = 0; i < audio_context->capture_buff_size; ++i)
            {
                my_data[i] = clip_int16( (buff_p[i]) * 32767);
            }
            break;
        }
        case GV_SAMPLE_TYPE_FLOATP:
        {
            int j=0;

            float *my_data[audio_context->channels];
            sample_t *buff_p = (sample_t *) audio_buffers[buffer_read_index].data;

            for(j = 0; j < audio_context->channels; ++j)
                my_data[j] = (float *) (((float *) buff->data) +
                    (j * audio_context->capture_buff_size/audio_context->channels));

            for(i = 0; i < audio_context->capture_buff_size/audio_context->channels; ++i)
                for(j = 0; j < audio_context->channels; ++j)
                {
                    my_data[j][i] = *buff_p++;
                }
            break;
        }
        case GV_SAMPLE_TYPE_INT16P:
        {
            int j=0;

            int16_t *my_data[audio_context->channels];
            sample_t *buff_p = (sample_t *) audio_buffers[buffer_read_index].data;

            for(j = 0; j < audio_context->channels; ++j)
                my_data[j] = (int16_t *) (((int16_t *) buff->data) +
                    (j * audio_context->capture_buff_size/audio_context->channels));

            for(i = 0; i < audio_context->capture_buff_size/audio_context->channels; ++i)
                for(j = 0; j < audio_context->channels; ++j)
                {
                    my_data[j][i] = clip_int16((*buff_p++) * 32767);
                }
            break;
        }
    }

    buff->timestamp = audio_buffers[buffer_read_index].timestamp;

    buff->level_meter[0] = audio_buffers[buffer_read_index].level_meter[0];
    buff->level_meter[1] = audio_buffers[buffer_read_index].level_meter[1];

    pthread_mutex_lock(&mutex);
    audio_buffers[buffer_read_index].flag = AUDIO_BUFF_FREE;
    NEXT_IND(buffer_read_index, AUDBUFF_NUM);
    pthread_mutex_unlock(&mutex);
    return 0;
}


void AudioInput::audio_delete_buffer(audio_buff_t *audio_buff)
{    
    audio_free_buffers();

    if(!audio_buff)
        return;

    if(audio_buff->data)
        free(audio_buff->data);

    free(audio_buff);
}


