/*
 * h264decoder.h -- Handling special feature of decoding h264
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
#ifndef H264DECODER_H
#define H264DECODER_H


/* checking version compatibility */
#define LIBAVCODEC_VER_AT_LEAST(major,minor)  (LIBAVCODEC_VERSION_MAJOR > major || \
                                              (LIBAVCODEC_VERSION_MAJOR == major && \
                                               LIBAVCODEC_VERSION_MINOR >= minor))

//#if !LIBAVCODEC_VER_AT_LEAST(54,25)
//    #define AV_CODEC_ID_H264 CODEC_ID_H264
//#endif

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/mathematics.h"
#include "libavutil/rational.h"
#include "libavutil/avstring.h"
#include "libswscale/swscale.h"
#include "libavutil/intreadwrite.h"
#include "libavutil/dict.h"
}


class H264Decoder
{

public:
   H264Decoder();
   ~H264Decoder();
   void closeFile();
   bool initH264Decoder(unsigned width,unsigned height);
   int decodeH264(uint8_t *out_buf, uint8_t *in_buf, int buf_size);

protected:
      AVCodecContext *pH264CodecCtx;
      AVCodec *pH264Codec;
      AVFrame *pH264picture;
      uint8_t *h264pictureBuf;
      int h264PictureSize;

      void initVars();
      bool initCodec();

      //free a frame
      void freeFrame();

};

#endif // H264DECODER_H
