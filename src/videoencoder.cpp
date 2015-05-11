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

bool VideoEncoder::createFile(QString fileName,AVCodecID encodeType, unsigned width,unsigned height,unsigned fpsDenominator, unsigned fpsNumerator, unsigned bitrate)
{
    // If we had an open video, close it.
    closeFile();

    Width=width;
    Height=height;
    Bitrate=bitrate;

    if(!isSizeValid())
    {
        printf("Invalid size\n");
        return false;
    }

    pOutputFormat = av_guess_format(NULL, fileName.toStdString().c_str(), NULL);
    if (!pOutputFormat) {
        printf("Could not deduce output format from file extension: using MPEG.\n");
        pOutputFormat = av_guess_format("mpeg", NULL, NULL);
    }

    pOutputFormat->video_codec = (AVCodecID)encodeType;
    pFormatCtx= avformat_alloc_context();
    if(!pFormatCtx)
    {
        printf("Error allocating format context\n");
        return false;
    }
    pFormatCtx->oformat = pOutputFormat;

    snprintf(pFormatCtx->filename, sizeof(pFormatCtx->filename), "%s", fileName.toStdString().c_str());

    // find the video encoder

    if(pOutputFormat->video_codec != AV_CODEC_ID_NONE) {
        pCodec = avcodec_find_encoder(pOutputFormat->video_codec);
        if (!pCodec)
        {
            printf("codec not found\n");
            return false;
        }
        // Add the video stream

        pVideoStream = avformat_new_stream(pFormatCtx,pCodec);
        if(!pVideoStream )
        {
            printf("Could not allocate stream\n");
            return false;
        }

        pCodecCtx=pVideoStream->codec;
        // some formats want stream headers to be separate
        if(pFormatCtx->oformat->flags & AVFMT_GLOBALHEADER)
            pCodecCtx->flags |= CODEC_FLAG_GLOBAL_HEADER;


        pCodecCtx->codec_id = pOutputFormat->video_codec;

        if(encodeType == AV_CODEC_ID_RAWVIDEO)
            pCodecCtx->pix_fmt =  PIX_FMT_YUYV422;//AV_PIX_FMT_YUV444P;//AV_PIX_FMT_YUV422P;//PIX_FMT_YUYV422;//PIX_FMT_YUV420P;
        else if(encodeType == AV_CODEC_ID_MJPEG)
            pCodecCtx->pix_fmt =  PIX_FMT_YUVJ420P;
        else {
            pCodecCtx->pix_fmt =  PIX_FMT_YUV420P;
        }
        pCodecCtx->bit_rate = Bitrate;
        pCodecCtx->width = getWidth();
        pCodecCtx->height = getHeight();
        pVideoStream->time_base.den = fpsDenominator;
        pVideoStream->time_base.num = fpsNumerator;
        pCodecCtx->gop_size = fpsDenominator/fpsNumerator;
        pCodecCtx->time_base.den = fpsDenominator;
        pCodecCtx->time_base.num = fpsNumerator;
        tempExtensionCheck = fileName.mid(fileName.length()-3);
        if(pOutputFormat->video_codec == AV_CODEC_ID_H264 || pOutputFormat->video_codec == AV_CODEC_ID_VP8) {
            pCodecCtx->qmin = 15; // qmin = 10*
            pCodecCtx->qmax = 30; //qmax = 51 **
        }
        // open the codec
        if (avcodec_open2(pCodecCtx, pCodec,NULL) < 0)
        {
            printf("could not open codec\n");
            return false;
        }

        //Allocate memory for output
        if(!initOutputBuf())
        {
            printf("Can't allocate memory for output bitstream\n");
            return false;
        }
        // Allocate the YUV frame
        if(!initFrame())
        {
            printf("Can't init frame\n");
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
        printf("Unable to record video...");
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
        fprintf(stderr, "Error encoding a video frame\n");
        exit(1);
    }
    if (got_packet) {
        if (pCodecCtx->coded_frame->pts != AV_NOPTS_VALUE)
            av_packet_rescale_ts(&pkt, pCodecCtx->time_base, pVideoStream->time_base);
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
    ppicture = av_frame_alloc();
    if(ppicture==0)
        return false;

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
        printf("Wrong image size!\n");
        return false;
    }
    if(img.format()!=QImage::Format_RGB32 && img.format() != QImage::Format_ARGB32)
    {
        printf("Wrong image format\n");
        return false;
    }

    img_convert_ctx = sws_getCachedContext(img_convert_ctx,getWidth(),getHeight(),PIX_FMT_RGB32,getWidth(),getHeight(),pCodecCtx->pix_fmt,SWS_FAST_BILINEAR, NULL, NULL, NULL);

    if (img_convert_ctx == NULL)
    {
        printf("Cannot initialize the conversion context\n");
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



