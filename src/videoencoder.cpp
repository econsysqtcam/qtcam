/*
 * videoencoder.cpp -- encode the video frame based on the selected encoder
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


#include "videoencoder.h"
#include <sys/time.h>
#include <QDebug>
#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

using namespace std;
static int64_t audioPts = 0;

#if LIBAVCODEC_VER_AT_LEAST(58, 54)
    #define CODEC_FLAG_GLOBAL_HEADER AV_CODEC_FLAG_GLOBAL_HEADER
#endif
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

    // Added by Sankari: Mar 4 2019.  Initialize AVpacket to NULL
    pkt.data = NULL;
    pkt.size = 0;
    videoPacketReceived = false;
    m_recStop = false;
}

//Retrives the current system time in milliseconds
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


/*
 * Creating a video file for writing.
 * Initializes the required video and audio codecs.
 * Sets the output format.
 * Opens the file for writing.
*/
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
    //Format can handle media streams where the frame rate is not constant throughout the duration of the content
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
        if(encodeType == CODEC_ID_MJPEG)
            pCodecCtx->pix_fmt =  PIX_FMT_YUVJ420P;
        else {
            pCodecCtx->pix_fmt =  PIX_FMT_YUV420P;
        }
#else
        if(encodeType == AV_CODEC_ID_MJPEG)
            pCodecCtx->pix_fmt =  AV_PIX_FMT_YUVJ420P;
        else {
            pCodecCtx->pix_fmt =  AV_PIX_FMT_YUV420P;
        }
#endif
        // Added by Sankari: Mar 20, 2019
        // Edited by Navya : Jan 28, 2020
        // If fps is >=112 means, bitrate is very low. So "avcodec_open2" is failed in H264 encoder. So make it as 60.
        unsigned supportedFpsDen;
        if(fpsDenominator == 120 || fpsDenominator == 112 || fpsDenominator == 180 || fpsDenominator == 145){
            supportedFpsDen = 60;
        }else{
            supportedFpsDen = fpsDenominator;
        }
        pCodecCtx->bit_rate =  getWidth() / 3.0f * getHeight() * fpsNumerator / supportedFpsDen;
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

        pCodecCtx->gop_size = 12; // mjpg
        pVideoStream->time_base.den = pCodecCtx->time_base.den;
        pVideoStream->time_base.num = pCodecCtx->time_base.num;


        tempExtensionCheck = fileName.mid(fileName.length()-3);
#if !LIBAVCODEC_VER_AT_LEAST(54, 25)
        if(pOutputFormat->video_codec == CODEC_ID_H264) {
#else
        if(pOutputFormat->video_codec == AV_CODEC_ID_H264) {
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
	// Added by Sankari: Mar 11, 2019
	// Dictionary options are set to make audio video sync(especially in H264 encoder)
        AVDictionary *opts = NULL;
        av_dict_set(&opts, "tune", "zerolatency", 0);
        av_dict_set(&opts, "preset","ultrafast",0);
#if LIBAVCODEC_VER_AT_LEAST(53,6)
        if (avcodec_open2(pCodecCtx, pCodec,&opts) < 0)
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

/*
 * Closing the video file.
 * Perform necessary cleanup operations.
*/
bool VideoEncoder::closeFile()
{
    if(!isOk())
        return false;

    av_write_trailer(pFormatCtx);

    // close_video
    avcodec_close(pVideoStream->codec);

    freeFrame();

    if(pkt.data != NULL && pkt.size != 0){
    	av_free_packet(&pkt);
        pkt.data = NULL;
        pkt.size = 0;
    }

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

    // Free the stream
    av_free(pFormatCtx);

    initVars();

    return true;
}


//Encodes the frame using the selected codec and writes the encoded packet to the output file.
/**
   \brief Encode one frame
    /* buffer - input buffer to encode
     * bufferType - 0 if rgbabuffer passes
     *              - 1 if yuyv  or other buffer passes
     *              - 2 if uyvy buffer passes
**/
#if LIBAVCODEC_VER_AT_LEAST(54,01)
int VideoEncoder::encodeImage(uint8_t *buffer,uint8_t bufferType)
{
    int ret = -1;
    ret = encodePacket(buffer, bufferType);
    return ret;
}

/**
 * @brief VideoEncoder::encodePacket
 * @param buffer - input buffer to encode
     * bufferType - 0 if rgbabuffer passes
     *              - 1 if yuyv  or other buffer passes
     *              - 2 if uyvy buffer passes
 * @return 0 if success/ -ve if failure
 */
int VideoEncoder::encodePacket(uint8_t *buffer, uint8_t bufferType){
    double fps, recordTimeDurationInSec, millisecondsDiff;
    if(frameCount == 0){
        time1  = QTime::currentTime();
        fps = pCodecCtx->time_base.den;
    }else{
        time2  = QTime::currentTime(); // get current time  - time2

        millisecondsDiff = time1.msecsTo(time2); // find millisecond diff from beginning to current

        recordTimeDurationInSec = millisecondsDiff/1000; // convert millisec to sec

        fps = (frameCount)/ recordTimeDurationInSec; // calculate fps

    }

    if(!isOk())
        return -1;

    convertImage_sws(buffer,bufferType);

    int got_packet = 0;
    int out_size = 0;

    if(pkt.data != NULL && pkt.size != 0){
       av_free_packet(&pkt);
       pkt.data = NULL;
       pkt.size = 0;
   }

    // Init packet
    av_init_packet(&pkt);

    pkt.pts = pkt.dts = ppicture->pts;

    //Takes an input video frame, encodes it, and produces a compressed output packet
    int ret = avcodec_encode_video2(pCodecCtx, &pkt, ppicture, &got_packet);
    if (ret < 0) {
        char errText[999]="";
        av_strerror(ret, errText, 999);
        fprintf(stderr, "Error encoding a video frame\n");
        av_free_packet(&pkt);
        pkt.data = NULL;
        pkt.size = 0;
        return -1;
    }

    pkt.stream_index = pVideoStream->index;
    if (got_packet) {

        // increment frame count
          frameCount++;

        // https://stackoverflow.com/questions/48440670/how-to-set-pts-and-dts-of-avpacket-from-rtp-timestamps-while-muxing-vp8-rtp-stre
            //frameDuration = video_st->time_base.den / video_fps; // i.e. 25
            //frameTime     = frame_count * frameDuration;
            //pkt->pts      = frameTime / video_st->time_base.num;
            //pkt->duration = frameDuration;

        pkt.duration = pCodecCtx->time_base.den/fps;
        // The above calculation can be shortly gives as below
        pkt.pts  = (frameCount*(pCodecCtx->time_base.den/fps)) / pCodecCtx->time_base.num;
        pkt.dts = pkt.pts;

        // Added by Navya -- 18 Sep 2019
        // Adjusted timestamps inorder to avoid glitches in recorded video for h264 encoder.

        if(pts_prev == pkt.pts | pkt.pts < pts_prev){
            pkt.pts = pts_prev+1;  // Incremented the timestamp value,as pkt.pts is maintaining the same value,leading to av_write_interleaved_frame failure.
            pkt.dts = pkt.pts;
        }
        pts_prev = pkt.pts;
        if(pCodecCtx->coded_frame->key_frame)
            pkt.flags |= AV_PKT_FLAG_KEY;
        /* Write the compressed frame to the media file. */
        out_size = av_interleaved_write_frame(pFormatCtx, &pkt);
        if(out_size == 0){
            videoPacketReceived = true;
            m_recStop = false;
        }
    }
    return out_size;
}
#else
int VideoEncoder::encodeImage(uint8_t *buffer,uint8_t bufferType)
{
    int out_size, ret;

    double fps, recordTimeDurationInSec, millisecondsDiff;
    if(frameCount == 0){
        time1  = QTime::currentTime();
        fps = pCodecCtx->time_base.den;
    }else{
        time2  = QTime::currentTime(); // get current time  - time2

        millisecondsDiff = time1.msecsTo(time2); // find millisecond diff from beginning to current

        recordTimeDurationInSec = millisecondsDiff/1000; // convert millisec to sec

        fps = (frameCount)/ recordTimeDurationInSec; // calculate fps
    }

    if(!isOk())
        return -1;

    convertImage_sws(buffer, bufferType);

    /* encode the image */
    out_size = avcodec_encode_video(pCodecCtx, outbuf, outbuf_size, ppicture);


    if(out_size < 0){
        fprintf(stderr, "Error encoding a video frame\n");
        return -1;
    }
    /* if zero size, it means the image was buffered */
    if (out_size > 0) {
	if(pkt.data != NULL && pkt.size != 0){
          av_free_packet(&pkt);
          pkt.data = NULL;
          pkt.size = 0;
      }

       frameCount++;
       av_init_packet(&pkt);

       pkt.pts  = (frameCount*(pCodecCtx->time_base.den/fps)) / pCodecCtx->time_base.num;


       if(pCodecCtx->coded_frame->key_frame)
            pkt.flags |= AV_PKT_FLAG_KEY;
       pkt.stream_index = pVideoStream->index;
       pkt.data = outbuf;
       pkt.size = out_size;
       /* write the compressed frame in the media file */
       ret = av_write_frame(pFormatCtx, &pkt);

       if(ret == 0){
        videoPacketReceived = true;
       }

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

bool VideoEncoder::convertImage_sws(uint8_t *buffer,uint8_t bufferType)
{
    if(bufferType == RGB_BUFFER){
#if !LIBAVCODEC_VER_AT_LEAST(54, 25)
        img_convert_ctx = sws_getCachedContext(img_convert_ctx,getWidth(),getHeight(), PIX_FMT_RGBA,getWidth(),getHeight(),pCodecCtx->pix_fmt,SWS_FAST_BILINEAR, NULL, NULL, NULL);
#else
        img_convert_ctx = sws_getCachedContext(img_convert_ctx,getWidth(),getHeight(),AV_PIX_FMT_RGBA,getWidth(),getHeight(),pCodecCtx->pix_fmt,SWS_FAST_BILINEAR, NULL, NULL, NULL);
#endif
    }else if(bufferType == YUYV_BUFFER){
#if !LIBAVCODEC_VER_AT_LEAST(54, 25)
        img_convert_ctx = sws_getCachedContext(img_convert_ctx,getWidth(),getHeight(), PIX_FMT_YUYV422,getWidth(),getHeight(),pCodecCtx->pix_fmt,SWS_FAST_BILINEAR, NULL, NULL, NULL);
#else
        img_convert_ctx = sws_getCachedContext(img_convert_ctx,getWidth(),getHeight(),AV_PIX_FMT_YUYV422,getWidth(),getHeight(),pCodecCtx->pix_fmt,SWS_FAST_BILINEAR, NULL, NULL, NULL);
#endif
    }
    /** Added by Navya:29 Nov 2019
      * Previously UYVY/Y8 data are converted to YUVY and given for recording and hence the source format is configured to YUYV422.
      * Now these data are directly given for recording,hence configuring source Format to the corresponding formatTypes.
     **/
    else if(bufferType == UYVY_BUFFER){
#if !LIBAVCODEC_VER_AT_LEAST(54, 25)
        img_convert_ctx = sws_getCachedContext(img_convert_ctx,getWidth(),getHeight(), PIX_FMT_UYVY422,getWidth(),getHeight(),pCodecCtx->pix_fmt,SWS_FAST_BILINEAR, NULL, NULL, NULL);
#else
        img_convert_ctx = sws_getCachedContext(img_convert_ctx,getWidth(),getHeight(),AV_PIX_FMT_UYVY422,getWidth(),getHeight(),pCodecCtx->pix_fmt,SWS_FAST_BILINEAR, NULL, NULL, NULL);
#endif
    } else if(bufferType == Y8_BUFFER){
#if !LIBAVCODEC_VER_AT_LEAST(54, 25)
        img_convert_ctx = sws_getCachedContext(img_convert_ctx,getWidth(),getHeight(), PIX_FMT_GRAY8,getWidth(),getHeight(),pCodecCtx->pix_fmt,SWS_FAST_BILINEAR, NULL, NULL, NULL);
#else
        img_convert_ctx = sws_getCachedContext(img_convert_ctx,getWidth(),getHeight(),AV_PIX_FMT_GRAY8,getWidth(),getHeight(),pCodecCtx->pix_fmt,SWS_FAST_BILINEAR, NULL, NULL, NULL);
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
    if(bufferType == RGB_BUFFER){
        srcstride[0]=getWidth()*4; // RGBA  - 32 bit.  so  4 is used
    }
    /** Added by Navya -29 Nov 2019
        * Giving 8 bit data directly for Video Record,hence making the srcStride as "getwidth()*1"
        * previously we are giving 16 bit data for recording,hence it was "getwidth()*2".
        * */
    else if(bufferType == Y8_BUFFER){
        srcstride[0]=getWidth(); // Y8  - 8 bit.  so  1 is used
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
    int ret = -1;
    ret = encodeH264Packet(buffer, bytesused);
    return ret;
}


int VideoEncoder::encodeH264Packet(void *buffer, int bytesused){

    double fps, recordTimeDurationInSec, millisecondsDiff;
    if(frameCount == 0){
        time1  = QTime::currentTime();
        fps = pCodecCtx->time_base.den;
    }else{
        time2  = QTime::currentTime(); // get current time  - time2

        millisecondsDiff = time1.msecsTo(time2); // find millisecond diff from beginning to current

        recordTimeDurationInSec = millisecondsDiff/1000; // convert millisec to sec

        fps = (frameCount)/ recordTimeDurationInSec; // calculate fps
    }

    if(!isOk())
        return -1;

    int out_size = 0;
    pkt.data = (u_int8_t *) buffer;
    pkt.size = bytesused;

    av_init_packet(&pkt);

    pkt.stream_index = pVideoStream->index;

    if(pCodecCtx->coded_frame->key_frame)
        pkt.flags |= AV_PKT_FLAG_KEY;

    pkt.pts  = (frameCount*(pCodecCtx->time_base.den/fps)) / pCodecCtx->time_base.num;
    pkt.dts = pkt.pts;

    // Added by Navya -- 18 Oct 2019
    // Adjusted timestamps inorder to avoid glitches in recorded video for h264 encoder.

    if(pts_prev == pkt.pts | pkt.pts < pts_prev){
        pkt.pts = pts_prev+1;  // Incremented the timestamp value,as pkt.pts is maintaining the same value,leading to av_write_interleaved_frame failure.
        pkt.dts = pkt.pts;
    }
    pts_prev = pkt.pts;
    /* Write the compressed frame to the media file. */

    out_size = av_write_frame(pFormatCtx, &pkt);
    if(out_size == 0){
         frameCount++;
         videoPacketReceived = true;
    }

     if(pkt.data != NULL && pkt.size != 0){
        av_free_packet(&pkt);
        pkt.data = NULL;
        pkt.size = 0;
    }

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
    c->time_base =(AVRational){1,c->sample_rate};
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


#if LIBAVCODEC_VER_AT_LEAST(56,60)
int VideoEncoder::encodeAudio(void *data){

    // start audio if video recorded - gotpacket true
    if(!videoPacketReceived){
        return -1;
    }

    int got_packet = 0;
    int out_size = 0;
    int ret = 0;
    audioPkt.data = NULL;
    audioPkt.size = 0;
    av_init_packet(&audioPkt);
    audioPkt.pts = pAudioFrame->pts;

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

        if(!m_recStop){
            /* Write the compressed frame to the media file. */
            out_size = av_write_frame(pFormatCtx, &audioPkt);
        }

        av_free_packet(&audioPkt);
    }
    return out_size;
}
#else
int VideoEncoder::encodeAudio(void *data)
{
    // start audio if video recorded - gotpacket true
    if(!videoPacketReceived){
        return -1;
    }

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
