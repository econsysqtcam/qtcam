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


#include <QPainter>
#include "videoencoder.h"
#include <QDebug>

using namespace std;

/**
  gop: maximal interval in frames between keyframes
**/
VideoEncoder::VideoEncoder()
{
    initVars();
    initCodec();
}

VideoEncoder::~VideoEncoder()
{
    closeFile();
}

#if LIBAVCODEC_VER_AT_LEAST(54,25)
bool VideoEncoder::createFile(QString fileName,AVCodecID encodeType, unsigned width,unsigned height,unsigned fpsDenominator, unsigned fpsNumerator, unsigned bitrate)
#else
bool VideoEncoder::createFile(QString fileName,CodecID encodeType, unsigned width,unsigned height,unsigned fpsDenominator, unsigned fpsNumerator, unsigned bitrate)
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
        pCodecCtx->bit_rate = Bitrate;
        pCodecCtx->width = getWidth();
        pCodecCtx->height = getHeight();
        pVideoStream->time_base.den = fpsDenominator;
        pVideoStream->time_base.num = fpsNumerator;
        pCodecCtx->gop_size = fpsDenominator/fpsNumerator;
        pCodecCtx->time_base.den = fpsDenominator;
        pCodecCtx->time_base.num = fpsNumerator;
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
    return true;
}

bool VideoEncoder::closeFile()
{
    if(!isOk())
        return false;

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
int VideoEncoder::encodeImage(const QImage &img)
{
    if(!isOk())
        return -1;
    convertImage_sws(img);     // SWS conversion

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
    if (got_packet) {
        if (pCodecCtx->coded_frame->pts != AV_NOPTS_VALUE)
            pkt.pts = av_rescale_q(pCodecCtx->coded_frame->pts, pCodecCtx->time_base, pVideoStream->time_base);
        pkt.stream_index = pVideoStream->index;
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
int VideoEncoder::encodeImage(const QImage &img)
{
    int out_size, ret;

    if(!isOk())
        return -1;

    convertImage_sws(img);     // SWS conversion
    /* encode the image */
    out_size = avcodec_encode_video(pCodecCtx, outbuf, outbuf_size, ppicture);

    if(out_size < 0){
        fprintf(stderr, "Error encoding a video frame\n");
	    return -1;        
    }
    /* if zero size, it means the image was buffered */
    if (out_size > 0) {
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
}

bool VideoEncoder::convertImage_sws(const QImage &img)
{
    // Check if the image matches the size
    if((unsigned)img.width()!=getWidth() || (unsigned)img.height()!=getHeight())
    {
        return false;
    }
    if(img.format()!=QImage::Format_RGB32 && img.format() != QImage::Format_ARGB32)
    {
        return false;
    }

#if !LIBAVCODEC_VER_AT_LEAST(54, 25)
    img_convert_ctx = sws_getCachedContext(img_convert_ctx,getWidth(),getHeight(),PIX_FMT_RGB32,getWidth(),getHeight(),pCodecCtx->pix_fmt,SWS_FAST_BILINEAR, NULL, NULL, NULL);
#else
    img_convert_ctx = sws_getCachedContext(img_convert_ctx,getWidth(),getHeight(),AV_PIX_FMT_RGB32,getWidth(),getHeight(),pCodecCtx->pix_fmt,SWS_FAST_BILINEAR, NULL, NULL, NULL);
#endif

    if (img_convert_ctx == NULL)
    {
        return false;
    }

    uint8_t *srcplanes[3];
    srcplanes[0]=(uint8_t*)img.bits();
    srcplanes[1]=0;
    srcplanes[2]=0;

    int srcstride[3];
    srcstride[0]=img.bytesPerLine();
    srcstride[1]=0;
    srcstride[2]=0;

    sws_scale(img_convert_ctx, srcplanes, srcstride,0, getHeight(), ppicture->data, ppicture->linesize);

    return true;
}


