/*
 * videoencoder.cpp -- encode the video frame based on the selected encoder
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


#include "videoencoder.h"
#include <sys/time.h>
#include <QDebug>
#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

using namespace std;
static int64_t audioPts = 0;


/**
  gop: maximal interval in frames between keyframes
**/
VideoEncoder::VideoEncoder()
{
    initVars();
    initCodec();
    frameCount = 0;
    pAudioFrame = 0;
    samples = 0;
}

unsigned int VideoEncoder::getTickCount()
{
    struct timeval tv;
    if(gettimeofday(&tv, NULL) != 0)
        return 0;

    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

VideoEncoder::~VideoEncoder()
{
    closeFile();

}

#if LIBAVCODEC_VER_AT_LEAST(54,25)
bool VideoEncoder::createFile(QString fileName,AVCodecID encodeType, unsigned width,unsigned height,unsigned fpsDenominator, unsigned fpsNumerator, unsigned bitrate, int audioDeviceIndex, int sampleRate, int channels)
#else
bool VideoEncoder::createFile(QString fileName,CodecID encodeType, unsigned width,unsigned height,unsigned fpsDenominator, unsigned fpsNumerator, unsigned bitrate, int audioDeviceIndex, int sampleRate, int channels)
#endif
{
    // If we had an open video, close it.
    closeFile();

    Width=width;
    Height=height;
    Bitrate=bitrate;

#if 0
    if(!isSizeValid())
    {
        return false;
    }
#endif
    pOutputFormat = av_guess_format(NULL, fileName.toStdString().c_str(), NULL);
    if (!pOutputFormat) {
        pOutputFormat = av_guess_format("mpeg", NULL, NULL);
    }
#if LIBAVCODEC_VER_AT_LEAST(54,25)
    pOutputFormat->video_codec = (AVCodecID)encodeType;
#else
    pOutputFormat->video_codec = (CodecID)encodeType;
#endif
    pFormatCtx= avformat_alloc_context();
    if(!pFormatCtx)
    {
        return false;
    }
    pOutputFormat->flags |= AVFMT_VARIABLE_FPS; // need to check

    pFormatCtx->oformat = pOutputFormat;

    snprintf(pFormatCtx->filename, sizeof(pFormatCtx->filename), "%s", fileName.toStdString().c_str());


    // find the video encoder

#if !LIBAVCODEC_VER_AT_LEAST(54, 25)
    if(pOutputFormat->video_codec != CODEC_ID_NONE) {
#else
    if(pOutputFormat->video_codec != AV_CODEC_ID_NONE) {
#endif
        pCodec = avcodec_find_encoder(pOutputFormat->video_codec);
        if (!pCodec)
        {
            return false;
        }

        // Add the video stream
        pVideoStream = avformat_new_stream(pFormatCtx, pCodec);
        if(!pVideoStream )
        {
            return false;
        }

        pCodecCtx=pVideoStream->codec;
        // some formats want stream headers to be separate
        if(pFormatCtx->oformat->flags & AVFMT_GLOBALHEADER)
            pCodecCtx->flags |= CODEC_FLAG_GLOBAL_HEADER;

        pCodecCtx->codec_id = pOutputFormat->video_codec;
#if !LIBAVCODEC_VER_AT_LEAST(54, 25)
        if(encodeType == CODEC_ID_RAWVIDEO){
            pCodecCtx->pix_fmt =  PIX_FMT_YUYV422;//AV_PIX_FMT_YUV444P;//AV_PIX_FMT_YUV422P;//PIX_FMT_YUYV422;//PIX_FMT_YUV420P;
        }
        else if(encodeType == CODEC_ID_MJPEG)
            pCodecCtx->pix_fmt =  PIX_FMT_YUVJ420P;
        else {
            pCodecCtx->pix_fmt =  PIX_FMT_YUV420P;
        }
#else
        if(encodeType == AV_CODEC_ID_RAWVIDEO){
            pCodecCtx->pix_fmt =  AV_PIX_FMT_YUYV422;//AV_PIX_FMT_YUV444P;//AV_PIX_FMT_YUV422P;//PIX_FMT_YUYV422;//PIX_FMT_YUV420P;
        }
        else if(encodeType == AV_CODEC_ID_MJPEG)
            pCodecCtx->pix_fmt =  AV_PIX_FMT_YUVJ420P;
        else {
            pCodecCtx->pix_fmt =  AV_PIX_FMT_YUV420P;
        }
#endif

        pCodecCtx->bit_rate =  getWidth() / 3.0f * getHeight() * fpsNumerator / fpsDenominator;
        pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
        pCodecCtx->width = getWidth();
        pCodecCtx->height = getHeight();

        if(fpsDenominator >= 5){
            pCodecCtx->time_base = (AVRational){fpsNumerator, fpsDenominator};
        }else{
            pCodecCtx->time_base = (AVRational){1, 15};
        }

        pCodecCtx->qmax = 4;
        pCodecCtx->qmin = 1;

        pCodecCtx->gop_size = 12; // mjpeg
        pVideoStream->time_base.den = pCodecCtx->time_base.den;
        pVideoStream->time_base.num = pCodecCtx->time_base.num;


        tempExtensionCheck = fileName.mid(fileName.length()-3);
#if !LIBAVCODEC_VER_AT_LEAST(54, 25)
        if(pOutputFormat->video_codec == CODEC_ID_H264 || pOutputFormat->video_codec == CODEC_ID_VP8) {
#else
        if(pOutputFormat->video_codec == AV_CODEC_ID_H264 || pOutputFormat->video_codec == AV_CODEC_ID_VP8) {
#endif
            pCodecCtx->qmin = 15; // qmin = 10*
            pCodecCtx->qmax = 30; //qmax = 51 **
        }

#if !LIBAVCODEC_VER_AT_LEAST(53,6)
        /* set the output parameters (must be done even if no
                parameters). */
        if (av_set_parameters(pFormatCtx, NULL) < 0) {
            return false;
        }
#endif

#if LIBAVCODEC_VER_AT_LEAST(53,6)
        if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
#else
        if (avcodec_open(pCodecCtx, pCodec) < 0)
#endif
        {
            return false;
        }

        //Allocate memory for output
        if(!initOutputBuf())
        {
            return false;
        }
        // Allocate the YUV frame
        if(!initFrame())
        {
            return false;
        }
    }
    if(audioDeviceIndex-1 >= 0){
        #if !LIBAVCODEC_VER_AT_LEAST(54, 25)
            pFormatCtx->audio_codec_id = CODEC_ID_MP2;
            pOutputFormat->audio_codec = CODEC_ID_MP2;
        #else
            pFormatCtx->audio_codec_id = AV_CODEC_ID_MP2;
            pOutputFormat->audio_codec = AV_CODEC_ID_MP2;
        #endif

        #if !LIBAVCODEC_VER_AT_LEAST(54, 25)
            if (pOutputFormat->audio_codec != CODEC_ID_NONE)
        #else
            if(pOutputFormat->audio_codec != AV_CODEC_ID_NONE)
        #endif
            {
                  pAudioStream = add_audio_stream(pFormatCtx, pOutputFormat->audio_codec, sampleRate, channels);
            }

            if(pAudioStream == NULL)
                return false;
            else
                open_audio(pAudioStream);
    }


    if (!(pOutputFormat->flags & AVFMT_NOFILE)) {
        if (avio_open(&pFormatCtx->pb, fileName.toStdString().c_str(), AVIO_FLAG_WRITE) < 0) {
            fprintf(stderr, "Could not open '%s'\n", fileName.toStdString().c_str());
            return 1;
        }
    }

    int ret = avformat_write_header(pFormatCtx,NULL);
    if(ret<0) {
        return false;
    }
    ok=true;

    frameCount = 0; // recording frame count - initialization
    return true;
}
/* read or die! */
static void hdr_read(const char *tag, int fd, void *hdr, size_t bytes)
{
    ssize_t r = read(fd, hdr, bytes);
    if (bytes != r) {
       return void();
    }
}

/* write or die! */
static void hdr_write(const char *tag, int fd, const void *hdr, size_t bytes)
{
    ssize_t w = write(fd, hdr, bytes);
    if (bytes != w) {
       return void();
    }
}


bool VideoEncoder::closeFile()
{
    if(!isOk())
        return false;

    // calculate average fps during recording
    recordStopTimeInMilliSec = getTickCount();
    double recordTimeDurationInMilliSec = recordStopTimeInMilliSec - recordStartTimeInMilliSec;
    double recordTimeDurationInSec = recordTimeDurationInMilliSec/1000;

    long int fps = (frameCount)/ recordTimeDurationInSec;
    av_write_trailer(pFormatCtx);

    // close_video
    avcodec_close(pVideoStream->codec);
    freeFrame();
    freeOutputBuf();

    /* free the streams */
    for(unsigned int i = 0; i < pFormatCtx->nb_streams; i++)
    {
        av_freep(&pFormatCtx->streams[i]->codec);
        av_freep(&pFormatCtx->streams[i]);
    }

    sws_freeContext(img_convert_ctx);

    // Close file
    avio_close(pFormatCtx->pb);

    if(tempExtensionCheck == "avi"){ // avi file format - write avg frame rate in header
        struct AVIStreamHeader vsh;
        long vh_off = 0;

        int fd = open(pFormatCtx->filename, O_RDWR);
        if (fd < 0) {
           perror("err in opening avi file");
        }
        vh_off = VIDEO_FRAME_RATE_HEADER_OFFSET;
        lseek(fd, vh_off-4, SEEK_SET);
        hdr_read("video fps", fd, &vsh, sizeof(vsh));

        vsh.dwRate  = (long)fps;
        vsh.dwScale = (long)1;

        lseek(fd, vh_off-4, SEEK_SET);
        hdr_write("video fps", fd, &vsh, sizeof(vsh));

        int err = close(fd);
        if (err) {
           qDebug()<<"err in close avi file.";
        }
    }

    /*if(tempExtensionCheck == "mkv"){ // mkv file format - write avg frame rate in header
	float time = float(1)/float(fps);
	QProcess process;
	QString cmd = "mkvpropedit --edit track:v1 --set default-duration=";
	long long fpsInNanoSec = time*1000000000;
	cmd.append(QString::number(fpsInNanoSec, 10)).append(" ");
	cmd.append(pFormatCtx->filename);
	process.start(cmd);
	process.waitForFinished(-1);
    }*/

    // Free the stream
    av_free(pFormatCtx);

    initVars();

    return true;
}

/**
   \brief Encode one frame

   The frame must be of the same size as specifie
**/
#if LIBAVCODEC_VER_AT_LEAST(54,01)
int VideoEncoder::encodeImage(uint8_t *buffer, bool rgbBufferformat)
{
    if(!isOk())
        return -1;

    convertImage_sws(buffer, rgbBufferformat);     /* rgba format means, true */
                                                    /* other than rgba format means, false */

    int got_packet = 0;
    int out_size = 0;
    pkt.data = NULL;
    pkt.size = 0;
    av_init_packet(&pkt);
    pkt.pts = pkt.dts = ppicture->pts;

    /* encode the image */
    int ret = avcodec_encode_video2(pCodecCtx, &pkt, ppicture, &got_packet);
    if (ret < 0) {
        char errText[999]="";
        av_strerror(ret, errText, 999);
        fprintf(stderr, "Error encoding a video frame\n");
        return -1;
    }

    pkt.stream_index = pVideoStream->index;

    if (got_packet) {
        if(frameCount == 0){
            recordStartTimeInMilliSec = getTickCount();
        }
        frameCount++;
        if (pCodecCtx->coded_frame->pts != AV_NOPTS_VALUE){
            pkt.pts = av_rescale_q(pCodecCtx->coded_frame->pts, pCodecCtx->time_base, pVideoStream->time_base);
        }

        if((tempExtensionCheck) == "mkv") {
            i++;
            pkt.pts = (i*(1000/pCodecCtx->gop_size));
            pkt.dts = pkt.pts;
        }
        if(pCodecCtx->coded_frame->key_frame)
            pkt.flags |= AV_PKT_FLAG_KEY;

        /* Write the compressed frame to the media file. */
        out_size = av_write_frame(pFormatCtx, &pkt);
        av_free_packet(&pkt);
    }

    return out_size;
}
#else
int VideoEncoder::encodeImage(uint8_t *buffer, bool rgbBufferformat)
{
    int out_size, ret;

    if(!isOk())
        return -1;

    convertImage_sws(buffer, rgbBufferformat);     /* rgba format means, true */
                                                    /* other than rgba format means, false */
    /* encode the image */
    out_size = avcodec_encode_video(pCodecCtx, outbuf, outbuf_size, ppicture);

    if(out_size < 0){
        fprintf(stderr, "Error encoding a video frame\n");
        return -1;
    }
    /* if zero size, it means the image was buffered */
    if (out_size > 0) {
        if(frameCount == 0){
            recordStartTimeInMilliSec = getTickCount(); // get initial record time
        }
       frameCount++;
       av_init_packet(&pkt);
       if(pCodecCtx->coded_frame->pts != (int64_t)AV_NOPTS_VALUE)
            pkt.pts= av_rescale_q(pCodecCtx->coded_frame->pts, pCodecCtx->time_base, pVideoStream->time_base);
       if(pCodecCtx->coded_frame->key_frame)
            pkt.flags |= AV_PKT_FLAG_KEY;
       pkt.stream_index = pVideoStream->index;
       pkt.data = outbuf;
       pkt.size = out_size;
       if((tempExtensionCheck) == "mkv"){
           i++;
           pkt.pts = (i*(1000/pCodecCtx->gop_size));
           pkt.dts = pkt.pts;
       }
       /* write the compressed frame in the media file */
       ret = av_write_frame(pFormatCtx, &pkt);
       av_free_packet(&pkt);
    } else {
       ret = 0;
    }
    //Added by Nithyesh
    //No value was returned previously
    return ret;
}
#endif


void VideoEncoder::initVars()
{
    ok=false;
    pFormatCtx=0;
    pOutputFormat=0;
    pCodecCtx=0;
    pVideoStream=0;
    pCodec=0;
    ppicture=0;
    outbuf=0;
    picture_buf=0;
    img_convert_ctx=0;
    i = 0;
}

bool VideoEncoder::initCodec()
{
    av_register_all();
    return true;
}

bool VideoEncoder::isSizeValid()
{
    if(getWidth()%8)
        return false;
    if(getHeight()%8)
        return false;
    return true;
}

unsigned VideoEncoder::getWidth()
{
    return Width;
}

unsigned VideoEncoder::getHeight()
{
    return Height;
}

bool VideoEncoder::isOk()
{
    return ok;
}

bool VideoEncoder::initOutputBuf()
{
    outbuf_size = getWidth()*getHeight()*3;
    outbuf = new uint8_t[outbuf_size];
    if(outbuf==0)
        return false;
    return true;
}

void VideoEncoder::freeOutputBuf()
{
    if(outbuf)
    {
        delete[] outbuf;
        outbuf=0;
    }
}

bool VideoEncoder::initFrame()
{
#if LIBAVCODEC_VER_AT_LEAST(55,28)
    ppicture = av_frame_alloc();
#else
    ppicture = avcodec_alloc_frame();
#endif
    if(ppicture==0){
        return false;
    }

    int size = avpicture_get_size(pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height);
    picture_buf = new uint8_t[size];
    if(picture_buf==0)
    {
        av_free(ppicture);
        ppicture=0;
        return false;
    }

    // Setup the planes
    avpicture_fill((AVPicture *)ppicture, picture_buf,pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height);

    return true;
}
void VideoEncoder::freeFrame()
{
    if(picture_buf)
    {
        delete[] picture_buf;
        picture_buf=0;
    }
    if(ppicture)
    {
        av_free(ppicture);
        ppicture=0;
    }
    //audio
    if(samples)
    {
       delete[] samples;
       samples=0;
    }
    if(pAudioFrame)
    {
       av_free(pAudioFrame);
       pAudioFrame=0;
    }
}

bool VideoEncoder::convertImage_sws(uint8_t *buffer, bool rgbBufferformat)
{
    if(rgbBufferformat){
#if !LIBAVCODEC_VER_AT_LEAST(54, 25)
        img_convert_ctx = sws_getCachedContext(img_convert_ctx,getWidth(),getHeight(), PIX_FMT_RGBA,getWidth(),getHeight(),pCodecCtx->pix_fmt,SWS_FAST_BILINEAR, NULL, NULL, NULL);
#else
        img_convert_ctx = sws_getCachedContext(img_convert_ctx,getWidth(),getHeight(),AV_PIX_FMT_RGBA,getWidth(),getHeight(),pCodecCtx->pix_fmt,SWS_FAST_BILINEAR, NULL, NULL, NULL);
#endif
    }else{
#if !LIBAVCODEC_VER_AT_LEAST(54, 25)
        img_convert_ctx = sws_getCachedContext(img_convert_ctx,getWidth(),getHeight(), PIX_FMT_YUYV422,getWidth(),getHeight(),pCodecCtx->pix_fmt,SWS_FAST_BILINEAR, NULL, NULL, NULL);
#else
        img_convert_ctx = sws_getCachedContext(img_convert_ctx,getWidth(),getHeight(),AV_PIX_FMT_YUYV422,getWidth(),getHeight(),pCodecCtx->pix_fmt,SWS_FAST_BILINEAR, NULL, NULL, NULL);
#endif
    }

    if (img_convert_ctx == NULL)
    {
        return false;
    }

    uint8_t *srcplanes[3];
    srcplanes[0]=buffer;
    srcplanes[1]=0;
    srcplanes[2]=0;

    int srcstride[3];
    if(rgbBufferformat){
        srcstride[0]=getWidth()*4; // RGBA  - 32 bit.  so  4 is used
    }else{
        srcstride[0]=getWidth()*2; // YUYV  - 16 bit.  so  2 is used
    }
    srcstride[1]=0;
    srcstride[2]=0;

    sws_scale(img_convert_ctx, srcplanes, srcstride,0, getHeight(), ppicture->data, ppicture->linesize);

    return true;
}

// Added by Sankari : 8 Oct 2018
/**
   \brief Write h264 one frame
   @param : buffer - raw h264 buffer
   @param : bytesused - bytes in buffer
**/
int VideoEncoder::writeH264Image(void *buffer, int bytesused){
    if(!isOk())
        return -1;

    int out_size = 0;
    pkt.data = (u_int8_t *) buffer;
    pkt.size = bytesused;

    av_init_packet(&pkt);

    if (pCodecCtx->coded_frame->pts != AV_NOPTS_VALUE){
        pkt.pts = av_rescale_q(pCodecCtx->coded_frame->pts, pCodecCtx->time_base, pVideoStream->time_base);
    }
    pkt.stream_index = pVideoStream->index;
    if((tempExtensionCheck) == "mkv") {
        i++;
        pkt.pts = (i*(1000/pCodecCtx->gop_size));
        pkt.dts = pkt.pts;
    }
    if(pCodecCtx->coded_frame->key_frame)
        pkt.flags |= AV_PKT_FLAG_KEY;

    if(frameCount == 0){
        recordStartTimeInMilliSec = getTickCount();
    }
    frameCount++;

    /* Write the compressed frame to the media file. */
    out_size = av_write_frame(pFormatCtx, &pkt);
    av_free_packet(&pkt);

    return out_size;
}

int VideoEncoder::check_sample_fmt(AVCodec *codec, enum AVSampleFormat sample_fmt)
{
    const enum AVSampleFormat *p = codec->sample_fmts;

    while (*p != AV_SAMPLE_FMT_NONE) {
        if (*p == sample_fmt)
            return 1;
        p++;
    }
    return 0;
}

#if LIBAVCODEC_VER_AT_LEAST(54,25)
AVStream* VideoEncoder::add_audio_stream(AVFormatContext *oc, AVCodecID codec_id, int sampleRate, int channels)
#else
AVStream* VideoEncoder::add_audio_stream(AVFormatContext *oc, CodecID codec_id, int sampleRate, int channels)
#endif
{
    AVCodecContext *c;
    AVStream *st;


    index_of_df = -1;
    flushed_buffers = 0;
    flush_delayed_frames = 0;
    flush_done = 0;
#if !LIBAVCODEC_VER_AT_LEAST(54, 25)
    paudioCodec = avcodec_find_encoder(CODEC_ID_MP2);
#else
    paudioCodec = avcodec_find_encoder(AV_CODEC_ID_MP2);
#endif
    if (!paudioCodec)
    {
        printf("codec not found\n");
        return NULL;
    }
    st = avformat_new_stream(oc, paudioCodec);
    if (!st) {
        fprintf(stderr, "Could not alloc stream\n");
        return NULL;
    }

    c = st->codec;
    c->codec_id = codec_id;
    c->codec_type = AVMEDIA_TYPE_AUDIO;

    /* put sample parameters */
    c->sample_fmt = AV_SAMPLE_FMT_S16;
    if (!check_sample_fmt(paudioCodec, c->sample_fmt)) {
        fprintf(stderr, "Encoder does not support sample format %s",
        av_get_sample_fmt_name(c->sample_fmt));
        return NULL;
    }

    c->bit_rate = 160000;
    c->sample_rate = sampleRate;
    c->time_base = (AVRational){1, c->sample_rate};
    c->channels    = channels;
    if (c->channels < 2)
        c->channel_layout = AV_CH_LAYOUT_MONO;
    else
        c->channel_layout = AV_CH_LAYOUT_STEREO;

    // some formats want stream headers to be separate
    if(oc->oformat->flags & AVFMT_GLOBALHEADER)
        c->flags |= CODEC_FLAG_GLOBAL_HEADER;

    st->time_base = c->time_base;

    return st;
}

int VideoEncoder::open_audio(AVStream *st)
{
    pAudioCodecCtx = st->codec;
   /* open it */
#if LIBAVCODEC_VER_AT_LEAST(53,6)
    if (avcodec_open2(pAudioCodecCtx, paudioCodec, NULL) < 0)
#else
    if (avcodec_open(pAudioCodecCtx, paudioCodec) < 0)
#endif
    {
        fprintf(stderr, "could not open codec\n");
        return false;
    }

    /* frame containing input raw audio */
#if LIBAVCODEC_VER_AT_LEAST(55,28)
    pAudioFrame = av_frame_alloc();
#else
    pAudioFrame = avcodec_alloc_frame();
#endif
    if (!pAudioFrame) {
        fprintf(stderr, "Could not allocate audio frame\n");
        return false;
    }

    int frameSize = pAudioCodecCtx->frame_size;
    if(frameSize <= 0){
        frameSize = 1152; // default
    }

    audio_outbuf_size = 240000;
    samples = (u_int8_t *)av_malloc(audio_outbuf_size);
    if (!samples) {
        fprintf(stderr, "Could not allocate %d bytes for samples buffer\n",
                                       audio_outbuf_size);
        return false;
    }
    pAudioFrame->nb_samples     = frameSize;
    pAudioFrame->format         = pAudioCodecCtx->sample_fmt;
    return true;
}


#if LIBAVCODEC_VER_AT_LEAST(53,34)
int VideoEncoder::encodeAudio(void *data){
    int got_packet = 0;
    int out_size = 0;
    int ret = 0;
    audioPkt.data = NULL;
    audioPkt.size = 0;
    av_init_packet(&audioPkt);
    audioPkt.pts = pAudioFrame->pts;

    if(flush_delayed_frames){
        if(!flushed_buffers){
            avcodec_flush_buffers(pAudioCodecCtx);
            flushed_buffers = 1;
        }

        ret = avcodec_encode_audio2(
                pAudioCodecCtx,
                &audioPkt,
                NULL, /*NULL flushes the encoder buffers*/
                &got_packet);


    }else{

        #if LIBAVUTIL_VER_AT_LEAST(51,23)
            int align = 0;
        #else
            int align = 1; /*otherwise it causes a SIGFPE*/
        #endif
        int bufferSize = av_samples_get_buffer_size(NULL, pAudioCodecCtx->channels, pAudioCodecCtx->frame_size,
                                                            pAudioCodecCtx->sample_fmt, align);
        if(bufferSize <= 0)
        {
            fprintf(stderr, "ENCODER: (encoder_encode_audio) av_samples_get_buffer_size error (%d): chan(%d) samp_fmt(%d)\n",
                bufferSize,
                pAudioCodecCtx->channels, pAudioCodecCtx->sample_fmt);
            return out_size;
        }

        /*set the data pointers in frame*/
        ret = avcodec_fill_audio_frame(pAudioFrame, pAudioCodecCtx->channels, pAudioCodecCtx->sample_fmt,
                                       (u_int8_t *) data, bufferSize, align);

        if(ret < 0)
        {
            fprintf(stderr, "ENCODER: (encoder_encode_audio) av_samples_get_buffer_size error (%d): chan(%d) samp_fmt(%d)\n",
                bufferSize,
                pAudioCodecCtx->channels, pAudioCodecCtx->sample_fmt);
            return out_size;
        }

        /* encode the audio */
        ret = avcodec_encode_audio2(pAudioCodecCtx, &audioPkt, pAudioFrame, &got_packet);
        if (ret < 0) {
            fprintf(stderr, "Error encoding a audio frame\n");
            av_free_packet(&audioPkt);
            return -1;
        }
    }
    if (got_packet) {
        audioPkt.stream_index = pAudioStream->index;
#if LIBAVCODEC_VER_AT_LEAST(56,1)
        av_packet_rescale_ts(&audioPkt,
                                  pAudioCodecCtx->time_base,
                                  pAudioStream->time_base);
#else
        audioPkt.pts = av_rescale_q(pAudioCodecCtx->coded_frame->pts, pAudioCodecCtx->time_base, pAudioStream->time_base);
#endif

        pAudioCodecCtx->gop_size = pCodecCtx->gop_size;

        if(pAudioCodecCtx->coded_frame->key_frame)
            audioPkt.flags |= AV_PKT_FLAG_KEY;

        out_size = audioPkt.size;

        if(flush_delayed_frames && out_size == 0 ){
            flush_done = 1;
        }else if(out_size == 0 && index_of_df < 0){
            delayed_pts[delayed_frames] = tempPts;
            delayed_frames++;
            if(delayed_frames > MAX_DELAYED_FRAMES)
            {
                delayed_frames = MAX_DELAYED_FRAMES;
                printf("ENCODER: Maximum of %i delayed audio frames reached...\n", MAX_DELAYED_FRAMES);
            }


        }else{
            if(delayed_frames > 0){
                if(index_of_df > 0){
                    index_of_df = 0;
                }
                int64_t my_pts = tempPts;
                tempPts = delayed_pts[index_of_df];
                delayed_pts[index_of_df] = my_pts;
                index_of_df++;
                if(index_of_df >= delayed_frames){
                    index_of_df = 0;
                }
            }
        }

        /* Write the compressed frame to the media file. */
        out_size = av_write_frame(pFormatCtx, &audioPkt);
        av_free_packet(&audioPkt);
    }
    return out_size;
}
#else
int VideoEncoder::encodeAudio(void *data)
{
    int out_size, ret;
    if(!isOk())
        return -1;

   out_size = avcodec_encode_audio(pAudioCodecCtx, samples, audio_outbuf_size, (short*)data);
   if(out_size < 0){
       fprintf(stderr, "Error encoding a audio frame\n");
       return -1;
   }

   if(out_size > 0){
       av_init_packet(&audioPkt);

       if(pAudioCodecCtx->coded_frame && pAudioCodecCtx->coded_frame->pts != (int64_t)AV_NOPTS_VALUE)
           audioPkt.pts = av_rescale_q(pAudioCodecCtx->coded_frame->pts, pAudioCodecCtx->time_base, pAudioStream->time_base);
       if(pAudioCodecCtx->coded_frame->key_frame)
           audioPkt.flags |= AV_PKT_FLAG_KEY;
       audioPkt.stream_index = pAudioStream->index;
       audioPkt.data = samples;
       audioPkt.size = out_size;
       pAudioCodecCtx->gop_size = pCodecCtx->gop_size;
       if((tempExtensionCheck) == "mkv")
       {
         j++;
         audioPkt.pts = (j*(1000/pAudioCodecCtx->gop_size));
         audioPkt.dts = audioPkt.pts;
       }
       /* write the compressed frame in the media file */
          ret = av_write_frame(pFormatCtx, &audioPkt);
          av_free_packet(&audioPkt);
   } else
   {
          ret = 0;
   }
   return ret;
}
#endif
