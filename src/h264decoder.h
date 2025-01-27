/*
 * h264decoder.h -- Handling special feature of decoding h264
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
#ifndef H264DECODER_H
#define H264DECODER_H
#include "common.h"
#include <QObject>

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
}

static enum AVPixelFormat get_format(AVCodecContext *ctx, const enum AVPixelFormat *pi_fmt);

class H264Decoder : public QObject
{
    Q_OBJECT
public:
   H264Decoder();
   ~H264Decoder();
   void closeFile();
   bool initH264Decoder(unsigned width,unsigned height);
   int decodeH264(uint8_t *out_buf, uint8_t *in_buf, int buf_size);
   void yu12_to_yuyv(u_int8_t *out, u_int8_t *in, int width, int height);
    int libav_decode(AVCodecContext *avctx, AVFrame *frame, int *got_frame, AVPacket *pkt);
   enum AVPixelFormat get_format_real(AVCodecContext *ctx, const enum AVPixelFormat *pi_fmt);
signals:
    void openDialogBox();
protected:
      AVCodecContext *pH264CodecCtx;
      const AVCodec *pH264Codec;
      AVFrame *pH264picture;
      uint8_t *h264pictureBuf;
      int h264PictureSize;

      void initVars();
      bool initCodec();

      //free a frame
      void freeFrame();

};

#endif // H264DECODER_H
