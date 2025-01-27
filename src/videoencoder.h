/*
 * videoencoder.h -- encode the video frame based on the selected encoder
 * Copyright © 2024  e-con Systems India Pvt. Limited
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
#ifndef __VideoEncoder_H
#define __VideoEncoder_H

#include <QIODevice>
#include <QFile>
#include <QImage>
#include <QDateTime>


#include "common.h"
/* checking version compatibility */

#define LIBAVUTIL_VER_AT_LEAST(major,minor)  (LIBAVUTIL_VERSION_MAJOR > major || \
                                              (LIBAVUTIL_VERSION_MAJOR == major && \
                                               LIBAVUTIL_VERSION_MINOR >= minor))

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/mathematics.h"
#include "libavutil/rational.h"
#include "libavutil/avstring.h"
#include "libswscale/swscale.h"
#include "libavutil/intreadwrite.h"
#include "libavutil/dict.h"
#include "libavutil/avutil.h"
#include "libavutil/imgutils.h"
#include "libavutil/opt.h"
//#include "libavcodec/version.h"
#if LIBAVUTIL_VER_AT_LEAST(52,2)
    #include "libavutil/channel_layout.h"
#endif

}
//audio
#define MAX_DELAYED_FRAMES 50
#define VIDEO_FRAME_RATE_HEADER_OFFSET 112

struct AVIStreamHeader {
    uint32_t  fccType;
    uint32_t  fccHandler;
    uint32_t  dwFlags;
    uint32_t  dwPriority;
    uint32_t  dwInitialFrames;
    uint32_t  dwScale;
    uint32_t  dwRate;
    uint32_t  dwStart;
    uint32_t  dwLength;
    uint32_t  dwSuggestedBufferSize;
    uint32_t  dwQuality;
    uint32_t  dwSampleSize;
} __attribute__((__packed__));


class VideoEncoder
{

public:
   VideoEncoder();
   virtual ~VideoEncoder();

   AVOutputFormat *pOutputFormat;
   bool ok;
   AVCodecContext *pCodecCtx,*pAudioCodecCtx;

   QTime dateTime1;
   QTime dateTime2;

   QTime time1;
   QTime time2;

   bool videoPacketReceived;
   int64_t pts_prev =0;

   bool m_recStop;


   // Added by Navya - 25 Nov 2019 -- To configure source formats in "convertImage_sws" API according to the format Selected"
   typedef enum
   {
       RGB_BUFFER = 0,
       YUYV_BUFFER = 1,
       UYVY_BUFFER,
       Y8_BUFFER,
       Y16_BUFFER
   }bufferType;

   Q_ENUMS(bufferType)

#if LIBAVCODEC_VER_AT_LEAST(54,25)
   bool createFile(QString filename, AVCodecID encodeType, unsigned width,unsigned height,unsigned fpsDenominator, unsigned fpsNumerator, unsigned bitRate,  int audioDeviceIndex, int sampleRate, int channels);
#else
   bool createFile(QString filename, CodecID encodeType, unsigned width,unsigned height,unsigned fpsDenominator, unsigned fpsNumerator, unsigned bitRate,  int audioDeviceIndex, int sampleRate, int channels);
#endif

    // Opens the audio stream for encoding.
    int open_audio(AVStream *st);

    unsigned int getTickCount();

#if LIBAVCODEC_VER_AT_LEAST(54,25)
    AVStream* add_audio_stream(AVFormatContext *oc, enum AVCodecID codec_id, int sampleRate, int channels); //Adds an audio stream to the output file.
#else
    AVStream* add_audio_stream(AVFormatContext *oc, enum CodecID codec_id, int sampleRate, int channels);
#endif
    static int check_sample_fmt(const AVCodec *codec, enum AVSampleFormat sample_fmt);
    int encodeAudio(void *);

   bool closeFile();

   //Encodes a video frame given a buffer and buffer type.
   int encodeImage(uint8_t *buffer, uint8_t bufferType);

   //Encodes a video packet given a buffer and buffer type.
   int encodePacket(uint8_t *buffer, uint8_t bufferType);

   //Checks if the video encoder is in a valid state.
   bool isOk();

// Added by Sankari : 8 Oct 2018
/**
   \brief Write h264 one frame

   @param : buffer - raw h264 buffer
   @param : bytesused - bytes in buffer
**/
   int writeH264Image(void *buffer, int bytesUsed);

   int encodeH264Packet(void *buffer, int bytesused);

protected:
    unsigned Width,Height;
    unsigned Bitrate;
    unsigned Gop;
    int i, j;

    unsigned int recordStartTimeInMilliSec;

    int frameCount;

    // FFmpeg stuff
    AVFormatContext *pFormatCtx;

    AVStream *pVideoStream, *pAudioStream;
    const AVCodec *pCodec;

    // Frame data
    AVFrame *ppicture;
    uint8_t *picture_buf;

    // Compressed data
    int outbuf_size;
    uint8_t* outbuf;
    uint8_t* finalBuf;

    // Conversion
    SwsContext *img_convert_ctx;

    // Packet
    AVPacket *pkt, *audioPkt;

    QString fileName;
    QString tempExtensionCheck;

    //audio
    int audio_outbuf_size;
    int audio_input_frame_size;
    u_int8_t *samples;
    u_int8_t *audio_outbuf;

    const AVCodec *paudioCodec;
    AVFrame *pAudioFrame;

    unsigned getWidth();
    unsigned getHeight();
    bool isSizeValid();

    void initVars();

    // Alloc/free the output buffer
    bool initOutputBuf();
    void freeOutputBuf();

    // Alloc/free a frame
    bool initFrame();
    void freeFrame();

    // Frame conversion
    bool convertImage(const QImage &img);
    bool convertImage_sws(uint8_t *buffer, uint8_t bufferType);
};
#endif // VideoEncoder_H

