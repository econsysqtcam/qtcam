/*
 * h264decoder.cpp -- decode h264.
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
#include "h264decoder.h"
#include <QDebug>

using namespace std;

static enum AVPixelFormat get_format(AVCodecContext *ctx,const enum AVPixelFormat *pi_fmt)
{
    H264Decoder* this_pointer = static_cast<H264Decoder*>(ctx->opaque);
    return this_pointer->get_format_real(ctx, pi_fmt);
}

H264Decoder::H264Decoder()
{
    initVars();
    av_register_all();    
}

H264Decoder::~H264Decoder()
{
    closeFile();
}

enum AVPixelFormat H264Decoder::get_format_real(AVCodecContext *ctx, const AVPixelFormat *pi_fmt)
{
    enum AVPixelFormat swfmt = avcodec_default_get_format(ctx, pi_fmt);
    return swfmt;

}

bool H264Decoder::initH264Decoder(unsigned width, unsigned height)
{
    // find the decoder
#if !LIBAVCODEC_VER_AT_LEAST(54, 25)
    pH264Codec = avcodec_find_decoder(CODEC_ID_H264);
#else
    pH264Codec = avcodec_find_decoder(AV_CODEC_ID_H264);
#endif
    if (!pH264Codec)
    {        
        return false;
    }

#if LIBAVCODEC_VER_AT_LEAST(53,6)
    pH264CodecCtx = avcodec_alloc_context3(pH264Codec);
    avcodec_get_context_defaults3 (pH264CodecCtx, pH264Codec);
#else
    pH264CodecCtx = avcodec_alloc_context();
    avcodec_get_context_defaults(pH264CodecCtx);
#endif

#if !LIBAVCODEC_VER_AT_LEAST(58,0)
	pH264CodecCtx->flags2 |= CODEC_FLAG2_FAST;
#else
	pH264CodecCtx->flags2 |= AV_CODEC_FLAG2_FAST;
#endif

#if !LIBAVCODEC_VER_AT_LEAST(54, 25)
    pH264CodecCtx->pix_fmt = PIX_FMT_YUV420P;
#else
    pH264CodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
#endif

    pH264CodecCtx->width = width;
    pH264CodecCtx->height = height;
    pH264CodecCtx->thread_count = 10;
    pH264CodecCtx->thread_safe_callbacks = true;
    pH264CodecCtx->workaround_bugs = FF_BUG_AUTODETECT;
    pH264CodecCtx->err_recognition = 1;
    pH264CodecCtx->thread_type = 3;
    pH264CodecCtx->opaque = this;

    pH264CodecCtx->get_format = get_format;
#if LIBAVCODEC_VER_AT_LEAST(53,6)
    if (avcodec_open2(pH264CodecCtx, pH264Codec, NULL) < 0)
#else
    if (avcodec_open(pH264CodecCtx, pH264Codec) < 0)
#endif
    {        
        return false;
    }

#if LIBAVCODEC_VER_AT_LEAST(55,28)
    pH264picture = av_frame_alloc();
#else
    pH264picture = avcodec_alloc_frame();
#endif
    if(pH264picture==0)
        return false;

#if LIBAVCODEC_VER_AT_LEAST(55,28)
    av_frame_unref(pH264picture);
#else
    avcodec_get_frame_defaults(pH264picture);
#endif

#if LIBAVUTIL_VER_AT_LEAST(54,6)
    h264PictureSize = av_image_get_buffer_size(pH264CodecCtx->pix_fmt, width, height, 1);
#else
    h264PictureSize = avpicture_get_size(pH264CodecCtx->pix_fmt, width, height);
#endif
    pH264CodecCtx->width = width;
    pH264CodecCtx->height = height;

    h264pictureBuf = new uint8_t[h264PictureSize];
    if(h264pictureBuf==0)
    {
        av_free(pH264picture);
        pH264picture=0;
        return false;
    }    
    return true;
}


void H264Decoder::yu12_to_yuyv(u_int8_t *out, u_int8_t *in, int width, int height)
{
    uint8_t *py;
    uint8_t *pu;
    uint8_t *pv;

    int linesize = width * 2;
    int uvlinesize = width / 2;

    py=in;
    pu=py+(width*height);
    pv=pu+(width*height/4);

    int h=0;
    int huv=0;

    for(h=0;h<height;h+=2)
    {
        int wy = 0;
        int wuv = 0;
        int offset = h * linesize;
        int offset1 = (h + 1) * linesize;
        int offsety = h * width;
        int offsety1 = (h + 1) * width;
        int offsetuv = huv * uvlinesize;
        int w = 0;

        for(w=0;w<linesize;w+=4)
        {
            /*y00*/
            out[w + offset] = py[wy + offsety];
            /*u0*/
            out[(w + 1) + offset] = pu[wuv + offsetuv];
            /*y01*/
            out[(w + 2) + offset] = py[(wy + 1) + offsety];
            /*v0*/
            out[(w + 3) + offset] = pv[wuv + offsetuv];

            /*y10*/
            out[w + offset1] = py[wy + offsety1];
            /*u0*/
            out[(w + 1) + offset1] = pu[wuv + offsetuv];
            /*y11*/
            out[(w + 2) + offset1] = py[(wy + 1) + offsety1];
            /*v0*/
            out[(w + 3) + offset1] = pv[wuv + offsetuv];

            wuv++;
            wy+=2;
        }
        huv++;
    }
}

int H264Decoder::libav_decode(AVCodecContext *avctx, AVFrame *frame, int *got_frame, AVPacket *pkt)
{
#if LIBAVCODEC_VER_AT_LEAST(57,64)
    int ret;

    *got_frame = 0;

    int frameHeight,frameWidth;

    if (pkt)
    {
            frameHeight = avctx->height;
            frameWidth = avctx->width;
            ret = avcodec_send_packet(avctx, pkt);
            // In particular, we don't expect AVERROR(EAGAIN), because we read all
            // decoded frames with avcodec_receive_frame() until done.
            if(frameHeight != avctx->height || frameWidth != avctx->width)
            {
              //  emit openDialogBox();
            }
            if (ret < 0)
                    return ret == AVERROR_EOF ? 0 : ret;
    }

    ret = avcodec_receive_frame(avctx, frame);
    if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF)
            return ret;
    if (ret >= 0)
            *got_frame = 1;

    return ret;

#else
    return avcodec_decode_video2(avctx ,frame, got_frame, pkt);

#endif
}

int H264Decoder::decodeH264(u_int8_t *outBuf, u_int8_t *inBuf, int bufSize)
{
    AVPacket avpkt;

    av_init_packet(&avpkt);

    avpkt.size = bufSize;
    avpkt.data = inBuf;

    int gotPicture = 0;

    int len = libav_decode(pH264CodecCtx,pH264picture,&gotPicture,&avpkt);
    if(len < 0)
    {
        return len;
    }

    if(gotPicture)
    {
#if LIBAVUTIL_VER_AT_LEAST(54,6)
        av_image_copy_to_buffer(outBuf, h264PictureSize,
                                     (const unsigned char * const*) pH264picture->data, pH264picture->linesize,
                                     pH264CodecCtx->pix_fmt, pH264CodecCtx->width, pH264CodecCtx->height, 1);
#else
        avpicture_layout((AVPicture *) pH264picture, pH264CodecCtx->pix_fmt
            ,pH264CodecCtx->width, pH264CodecCtx->height, outBuf, h264PictureSize);
#endif
        return len;
    }
    else
        return 0;
}


void H264Decoder::closeFile()
{
    avcodec_close(pH264CodecCtx);
    freeFrame();
}


void H264Decoder::initVars()
{
    pH264CodecCtx = NULL;
    pH264Codec = NULL;
    pH264picture = NULL;
}


void H264Decoder::freeFrame()
{
    if(h264pictureBuf)
    {
        delete[] h264pictureBuf;
        h264pictureBuf = NULL;
    }
    if(pH264picture)
    {
        av_free(pH264picture);
        pH264picture = NULL;
    }
}
