/*
 * fscam_cu135.cpp -- Handling special feature of seecamcu135 camera
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

#include "fscam_cu135.h"
#include "uvccamera.h"
#include "videostreaming.h"


FSCAM_CU135::FSCAM_CU135(QObject *parent) :
    QObject(parent)
{
}

// Added by Sankari: Mar 21, 2019
//To set number of frames to skip in preview
void FSCAM_CU135::setPreviewSkipCount(){
    uint previewFrameToSkip;
    previewFrameToSkip = 3;
    // send preview frame to skip values to fscamcu135.qml
    emit updatePreviewFrameToSkip(previewFrameToSkip);
}

bool FSCAM_CU135::setEffectMode(specialEffects specialEffect)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_FSCAM_CU135; /* set camera control code */
    g_out_packet_buf[2] = SET_SPECIAL_EFFECT_MODE_FSCAM_CU135; /* set special effect code */
    g_out_packet_buf[3] = specialEffect; /* set special effect */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
           return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_FSCAM_CU135 &&
            g_in_packet_buf[1]==SET_SPECIAL_EFFECT_MODE_FSCAM_CU135 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

bool FSCAM_CU135::getEffectMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_FSCAM_CU135; /* set camera control code */
    g_out_packet_buf[2] = GET_SPECIAL_EFFECT_MODE_FSCAM_CU135; /* get special effect code */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_FSCAM_CU135 &&
            g_in_packet_buf[1]==GET_SPECIAL_EFFECT_MODE_FSCAM_CU135 &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            emit effectModeValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

bool FSCAM_CU135::setSceneMode(sceneModes sceneMode)
{
    // hid validation

    if(uvccamera::hid_fd < 0)
    {

        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_FSCAM_CU135; /* Camera control id */
    g_out_packet_buf[2] = SET_SCENE_MODE_FSCAM_CU135; /* Set scene mode command */
    g_out_packet_buf[3] = sceneMode; /* Scene mode to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_FSCAM_CU135 &&
                  g_in_packet_buf[1]==SET_SCENE_MODE_FSCAM_CU135 &&
                  g_in_packet_buf[6]==SET_SUCCESS) {\
                 return true;
        }
    }
    return false;
}

bool FSCAM_CU135::getSceneMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_FSCAM_CU135; /* camera id */
    g_out_packet_buf[2] = GET_SCENE_MODE_FSCAM_CU135; /* get scene mode command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_FSCAM_CU135 &&
            g_in_packet_buf[1]==GET_SCENE_MODE_FSCAM_CU135 &&
            g_in_packet_buf[6]==GET_SUCCESS) {\

            emit sceneModeValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

bool FSCAM_CU135::setDenoiseValue(uint deNoiseVal)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_FSCAM_CU135; /* set camera control code */
    g_out_packet_buf[2] = SET_DENOISE_CONTROL_FSCAM_CU135; /* set denoise control code */
    g_out_packet_buf[3] = deNoiseVal; /* set denoise value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_FSCAM_CU135 &&
            g_in_packet_buf[1]==SET_DENOISE_CONTROL_FSCAM_CU135 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

bool FSCAM_CU135::getDenoiseValue()
{

   u_int8_t denoiseVal;
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_FSCAM_CU135; /* set camera control code */
   g_out_packet_buf[2] = GET_DENOISE_CONTROL_FSCAM_CU135; /* get denoise code */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[6]==GET_FAIL) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_FSCAM_CU135 &&
           g_in_packet_buf[1]==GET_DENOISE_CONTROL_FSCAM_CU135 &&
           g_in_packet_buf[6]==GET_SUCCESS) {\
           denoiseVal = g_in_packet_buf[2];
           emit denoiseValue(denoiseVal);
           return true;
       }
   }
   return false;

}

bool FSCAM_CU135::setQFactor(uint qfactor)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_FSCAM_CU135; /* camera id */
    g_out_packet_buf[2] = SET_Q_FACTOR_FSCAM_CU135; /* set qfactor command */
    g_out_packet_buf[3] = qfactor; /* qfactor value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_FSCAM_CU135 &&
            g_in_packet_buf[1]==SET_Q_FACTOR_FSCAM_CU135 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

bool FSCAM_CU135::getQFactor()
{
   u_int8_t qfactor;
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_FSCAM_CU135; /* camera id */
   g_out_packet_buf[2] = GET_Q_FACTOR_FSCAM_CU135; /* get qFactor value */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[6]==GET_FAIL) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_FSCAM_CU135 &&
           g_in_packet_buf[1]==GET_Q_FACTOR_FSCAM_CU135 &&
           g_in_packet_buf[6]==GET_SUCCESS) {\
           qfactor = g_in_packet_buf[2];
           emit qFactorValue(qfactor);
           return true;
       }
   }
   return false;

}

bool FSCAM_CU135::setiHDRMode(camiHDRMode iHDRMode, uint iHDRValue)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_FSCAM_CU135; /* set camera control code */
    g_out_packet_buf[2] = SET_HDR_MODE_FSCAM_CU135; /* set ihdr mode command code */
    g_out_packet_buf[3] = iHDRMode; /* iHdr value */
    if(iHDRMode == HdrManual){
        g_out_packet_buf[4] = iHDRValue;
    }
    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_FSCAM_CU135 &&
            g_in_packet_buf[1]==SET_HDR_MODE_FSCAM_CU135 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool FSCAM_CU135::getiHDRMode()
{
   uint iHDRMode, iHDRValue;
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_FSCAM_CU135; /* camera id */
   g_out_packet_buf[2] = GET_HDR_MODE_FSCAM_CU135; /* get hdr mode value */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[6]==GET_FAIL) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_FSCAM_CU135 &&
           g_in_packet_buf[1]==GET_HDR_MODE_FSCAM_CU135 &&
           g_in_packet_buf[6]==GET_SUCCESS) {\
           iHDRMode = g_in_packet_buf[2];
            iHDRValue = g_in_packet_buf[3];
           emit hdrModeValue(iHDRMode,iHDRValue);
           return true;
       }
   }
   return false;

}

bool FSCAM_CU135::setStreamMode(camStreamMode streamMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {

        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_FSCAM_CU135; /* set camera control code */
    g_out_packet_buf[2] = SET_STREAM_MODE_FSCAM_CU135; /* set stream mode code */
    g_out_packet_buf[3] = streamMode; /* actual stream mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {

            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_FSCAM_CU135 &&
            g_in_packet_buf[1]==SET_STREAM_MODE_FSCAM_CU135 &&
            g_in_packet_buf[6]==SET_SUCCESS) {

            return true;
        }
    }
    return false;
}

bool FSCAM_CU135::getStreamMode()
{
   uint streamMode;

   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_FSCAM_CU135;  /* set camera control code */
   g_out_packet_buf[2] = GET_STREAM_MODE_FSCAM_CU135; /* get stream mode code */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[6]==GET_FAIL) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_FSCAM_CU135 &&
           g_in_packet_buf[1]==GET_STREAM_MODE_FSCAM_CU135 &&
           g_in_packet_buf[6]==GET_SUCCESS) {
           streamMode = g_in_packet_buf[2];
           emit streamModeValue(streamMode);

           return true;
       }
   }
   return false;

}

bool FSCAM_CU135::setOrientation(bool horzModeSel, bool vertiModeSel)
{
    u_int8_t flipMode;

    if(horzModeSel && vertiModeSel){
        flipMode = SetBothFlipEnable; /* both flip enable */
    }else if(horzModeSel && !vertiModeSel){
        flipMode = SetHorzFlip; /* horizontal flip only mode */
    }else if(!horzModeSel && vertiModeSel){
        flipMode = SetVertiFlip; /* vertical flip only mode */
    }else
        flipMode = SetBothFlipDisable; /* both flip disable */
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_FSCAM_CU135; /* camera control for see3cam_FSCAM_CU135 camera */
    g_out_packet_buf[2] = SET_ORIENTATION_FSCAM_CU135; /* set flip mode for storagecam_FSCAM_CU135 camera */
    g_out_packet_buf[3] = flipMode; /*flip mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_FSCAM_CU135 &&
            g_in_packet_buf[1]==SET_ORIENTATION_FSCAM_CU135 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;

  }

bool FSCAM_CU135::getOrientation()
{
    uint flipMode;
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_FSCAM_CU135; /* camera control id */
    g_out_packet_buf[2] = GET_ORIENTATION_FSCAM_CU135; /* Get orientation command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_FSCAM_CU135 &&
            g_in_packet_buf[1]==GET_ORIENTATION_FSCAM_CU135 &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            flipMode = (g_in_packet_buf[2]);
            emit flipMirrorModeChanged(flipMode);
            return true;
        }
    }
    return false;

}


bool FSCAM_CU135::setROIAutoExposure(camROIAutoExpMode see3camAutoexpROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //((Input - InputLow) / (InputHigh - InputLow)) * (OutputHigh - OutputLow) + OutputLow // map resolution width and height -  0 to 255

    double outputLow = 0;
    double outputHigh = 255;
    double inputXLow = 0;
    double inputXHigh = vidResolnWidth-1;
    double inputXCord = xCord;
    int outputXCord = ((inputXCord - inputXLow) / (inputXHigh - inputXLow)) * (outputHigh - outputLow) + outputLow;

    double inputYLow = 0;
    double inputYHigh = vidResolnHeight-1;
    double inputYCord = yCord;
    int outputYCord = ((inputYCord - inputYLow) / (inputYHigh - inputYLow)) * (outputHigh - outputLow) + outputLow;

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_FSCAM_CU135; /* camera id */
    g_out_packet_buf[2] = SET_EXP_ROI_MODE_FSCAM_CU135; /* set exposure ROI command */
    g_out_packet_buf[3] = see3camAutoexpROIMode; /* exposure ROI mode to set */

    if(see3camAutoexpROIMode == AutoExpManual){
        g_out_packet_buf[4] = outputXCord; // x cord
        g_out_packet_buf[5] = outputYCord; // y cord
        g_out_packet_buf[6] = winSize.toUInt();// window size
    }

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_FSCAM_CU135 &&
            g_in_packet_buf[1]==SET_EXP_ROI_MODE_FSCAM_CU135 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool FSCAM_CU135::getAutoExpROIModeAndWindowSize(){
   uint autoexpROImode, winSize;
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_FSCAM_CU135; /* camera id */
   g_out_packet_buf[2] = GET_EXP_ROI_MODE_FSCAM_CU135; /* get exposure ROI mode  */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[6]==GET_FAIL) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_FSCAM_CU135 &&
           g_in_packet_buf[1]==GET_EXP_ROI_MODE_FSCAM_CU135 &&
           g_in_packet_buf[6]==GET_SUCCESS) {
           autoexpROImode = g_in_packet_buf[2];
           winSize = g_in_packet_buf[5];
            emit roiAutoExpMode(autoexpROImode, winSize);
           return true;
       }
   }

   return false;
}

bool FSCAM_CU135::setFaceDetectionRect(bool enableFaceDetectRect, bool embedData, bool overlayRect){
     u_int8_t faceDetectMode, embedDataMode;
    if(enableFaceDetectRect)
        faceDetectMode = ENABLE_FACE_RECT_FSCAM_CU135; /* enable face detect */
    else
        faceDetectMode = DISABLE_FACE_RECT_FSCAM_CU135; /* disable face detect */

    if(embedData)
        embedDataMode = ENABLE_EMBED_DATA_FSCAM_CU135; /* enable embed data */
    else
        embedDataMode = DISABLE_EMBED_DATA_FSCAM_CU135; /* disable embed data */

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_FSCAM_CU135; /* camera id */
    g_out_packet_buf[2] = SET_FACE_DETECT_FSCAM_CU135; /* set face detect Rect command */
    g_out_packet_buf[3] = faceDetectMode; /* enable/disable face Detect */
    g_out_packet_buf[4] = embedData; /* enable/disable embed data */
    g_out_packet_buf[5] = overlayRect; /* enable/disable overlay rect */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_FSCAM_CU135 &&
            g_in_packet_buf[1]==SET_FACE_DETECT_FSCAM_CU135 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
   }

bool FSCAM_CU135::getFaceDetectMode(){
   uint faceDetectMode, faceDetectEmbedDataValue, faceOverlayRect;
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_FSCAM_CU135; /* camera id */
   g_out_packet_buf[2] = GET_FACE_DETECT_FSCAM_CU135; /* get face detect mode command */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[6]==GET_FAIL) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_FSCAM_CU135 &&
           g_in_packet_buf[1]==GET_FACE_DETECT_FSCAM_CU135 &&
           g_in_packet_buf[6]==GET_SUCCESS) {\

           faceDetectMode = g_in_packet_buf[2];
           faceDetectEmbedDataValue = g_in_packet_buf[3];
            faceOverlayRect = g_in_packet_buf[4];
            emit faceDetectModeValue(faceDetectMode, faceDetectEmbedDataValue, faceOverlayRect);
           return true;
       }
   }
   return false;

}

bool FSCAM_CU135::setSmileDetection(bool enableSmileDetect, bool embedData){
    u_int8_t smileDetectMode, embedDataMode;
    if(enableSmileDetect)
        smileDetectMode = ENABLE_SMILE_DETECT_FSCAM_CU135; /* enable smile detect */
    else
        smileDetectMode = DISABLE_SMILE_DETECT_FSCAM_CU135; /* disable smile detect */

    if(embedData)
        embedDataMode = ENABLE_EMBED_DATA_FSCAM_CU135; /* enable embed data */
    else
        embedDataMode = DISABLE_EMBED_DATA_FSCAM_CU135; /* disable embed data */

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_FSCAM_CU135; /* camera id */
    g_out_packet_buf[2] = SET_SMILE_DETECTION_FSCAM_CU135; /* set face detect Rect command */
    g_out_packet_buf[3] = smileDetectMode; /* disable smile detect */
    g_out_packet_buf[4] = embedData; /* disable embed data */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {

            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_FSCAM_CU135 &&
            g_in_packet_buf[1]==SET_SMILE_DETECTION_FSCAM_CU135 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\

            return true;
        }
    }
    return false;
}

bool FSCAM_CU135::getSmileDetectMode(){
   uint smileDetectMode, smileDetectEmbedDataValue;
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_FSCAM_CU135; /* Camera control id */
   g_out_packet_buf[2] = GET_SMILE_DETECTION_FSCAM_CU135; /* Get smile detection */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[6]==GET_FAIL) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_FSCAM_CU135 &&
           g_in_packet_buf[1]==GET_SMILE_DETECTION_FSCAM_CU135 &&
           g_in_packet_buf[6]==GET_SUCCESS) {\

           smileDetectMode = g_in_packet_buf[2];
           smileDetectEmbedDataValue = g_in_packet_buf[3];

        emit smileDetectModeValue(smileDetectMode, smileDetectEmbedDataValue);
           return true;
       }
   }
   return false;
}

bool FSCAM_CU135::setFrameRateCtrlValue(uint frameRateCtrl)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_FSCAM_CU135; /* set camera control code */
    g_out_packet_buf[2] = SET_FRAME_RATE_FSCAM_CU135; /* set framerate control code */
    g_out_packet_buf[3] = frameRateCtrl; /* set framerate value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_FSCAM_CU135 &&
            g_in_packet_buf[1]==SET_FRAME_RATE_FSCAM_CU135 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

bool FSCAM_CU135::getFrameRateCtrlValue()
{
   uint frameRateVal;
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_FSCAM_CU135; /* set camera control code */
   g_out_packet_buf[2] = GET_FRAME_RATE_FSCAM_CU135; /* get frame rate code */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[6]==GET_FAIL) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_FSCAM_CU135 &&
           g_in_packet_buf[1]==GET_FRAME_RATE_FSCAM_CU135 &&
           g_in_packet_buf[6]==GET_SUCCESS) {\
           frameRateVal = g_in_packet_buf[2];
           emit frameRateCtrlValue(frameRateVal);
           return true;
       }
   }
   return false;

}

bool FSCAM_CU135::setExposureCompensation(uint exposureCompValue)
{
 /*  if(EXPOSURECOMP_MIN > exposureCompValue || EXPOSURECOMP_MAX < exposureCompValue){
        emit indicateExposureValueRangeFailure("Failure", "Given exposure compensation value is invalid");
        return false;
    }*/
   if(EXPOSURECOMP_MIN > exposureCompValue || EXPOSURECOMP_MAX < exposureCompValue){
       return false;
   }

   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_FSCAM_CU135; /* camera id */
   g_out_packet_buf[2] = SET_EXPOSURE_COMPENSATION_FSCAM_CU135; /* set exposure compensation command */
   g_out_packet_buf[3] = (u_int8_t)((exposureCompValue & 0xFF000000) >> 24);
   g_out_packet_buf[4] = (u_int8_t)((exposureCompValue & 0x00FF0000) >> 16);
   g_out_packet_buf[5] = (u_int8_t)((exposureCompValue & 0x0000FF00) >> 8);
   g_out_packet_buf[6] = (u_int8_t)((exposureCompValue & 0x000000FF) >> 0);

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[6]==SET_FAIL) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_FSCAM_CU135 &&
           g_in_packet_buf[1]==SET_EXPOSURE_COMPENSATION_FSCAM_CU135 &&
           g_in_packet_buf[6]==SET_SUCCESS) {\
           emit indicateCommandStatus("Success", "Exposure compensation value is set successfully");
           return true;
       }
   }
  else{
	emit indicateCommandStatus("Failure", "Failed to set exposure compensation value");
	
   }
	return false;
}

bool FSCAM_CU135::getExposureCompensation()
{
   uint expComp;
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_FSCAM_CU135; /* set camera control code */
   g_out_packet_buf[2] = GET_EXPOSURE_COMPENSATION_FSCAM_CU135; /* get exposure compensation code */


   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[6]==GET_FAIL) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_FSCAM_CU135 &&
           g_in_packet_buf[1]==GET_EXPOSURE_COMPENSATION_FSCAM_CU135 &&
           g_in_packet_buf[6]==GET_SUCCESS) {\
           expComp = (((u_int8_t)g_in_packet_buf[2]) << 24)
                               | (((u_int8_t)g_in_packet_buf[3]) << 16)
                               | (((u_int8_t)g_in_packet_buf[4]) << 8)
                   | (((u_int8_t)g_in_packet_buf[5]) << 0);

         emit exposureCompValue(expComp);

           return true;
       }
   }
   return false;

}

bool FSCAM_CU135::setToDefault()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_FSCAM_CU135; /* camera id */
    g_out_packet_buf[2] = SET_TO_DEFAULT_FSCAM_CU135; /* set to default command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_FSCAM_CU135 &&
            g_in_packet_buf[1]==SET_TO_DEFAULT_FSCAM_CU135 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            emit titleTextChanged(_title,_text);
            return true;
        }
    }
    return false;
}

bool FSCAM_CU135::storePreviewFrame()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_FSCAM_CU135;// camera control id
    g_out_packet_buf[2] = STORE_FRAME; // store frame
    g_out_packet_buf[3] = STORE_PREV_FRAME; // store previous frame

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]== GET_SUCCESS) {
           return true;
        }
    }
    return false;
}

bool FSCAM_CU135::storeStillFrame(uint stillformatId, uint stillresolutionId)
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_FSCAM_CU135;// camera control id
    g_out_packet_buf[2] = STORE_FRAME; // store frame
    g_out_packet_buf[3] = STORE_STILL_FRAME; // store still frame
    g_out_packet_buf[4] = stillformatId; // still format id
    g_out_packet_buf[5] = stillresolutionId; // still resolution id

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]== GET_SUCCESS) {
            emit storeStillFrameSucceess();
            return true;
        }
    }
    return false;
}

bool FSCAM_CU135::getNumberOfFramesCanStore(uint stillformatId, uint stillresolutionId)
{
   uint numberOfFrames;

   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

  //Initialize buffers
   initializeBuffers();

   //Set the Report Number
   g_out_packet_buf[1] = CAMERA_CONTROL_FSCAM_CU135;// camera control id
   g_out_packet_buf[2] = GET_NUM_OF_FRAMES_STORE; // get number of frames can store
   g_out_packet_buf[3] = stillformatId; // still format id
   g_out_packet_buf[4] = stillresolutionId; // still resolution id

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
   if (g_in_packet_buf[6]== SET_SUCCESS) {
       numberOfFrames = g_in_packet_buf[4];
       emit numberOfframesStoreCapacity(numberOfFrames);
           return true;
   }
   }
   return false;

}

bool FSCAM_CU135::setStillResolution(uint stillformatId, uint stillresolutionId)
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

   //Initialize buffers
    initializeBuffers();

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_FSCAM_CU135;// camera control id
    g_out_packet_buf[2] = SET_STILL_RESOLUTION; // set still resolution
    g_out_packet_buf[3] = stillformatId; // still format id
    g_out_packet_buf[4] = stillresolutionId; // still resolution id

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]== GET_SUCCESS) {      
            return true;
        }

            return true;
    }
    return false;
}
bool FSCAM_CU135::grabStillFrame(uint frameIndex, uint stillformatId, uint stillresolutionId)
{

    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_FSCAM_CU135; // camera control id
    g_out_packet_buf[2] = GRAB_STILL_FRAME; // query still frame
    g_out_packet_buf[3] = stillformatId; // still format id
    g_out_packet_buf[4] = stillresolutionId; // still resolution id
    g_out_packet_buf[5] = frameIndex; // still frame index

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]== GET_SUCCESS) {
            emit grabStillFrameStatus(true);
            return true;
        }
    }

    return false;
   }

bool FSCAM_CU135::grabPreviewFrame()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_FSCAM_CU135; // camera control id
    g_out_packet_buf[2] = GRAB_PREVIEW_FRAME; // query frame
    g_out_packet_buf[3] = QUERY_NEXT_FRAME; // query next frame - 0x01 , query prev frame - 0x02

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]== GET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool FSCAM_CU135::getStillResolution()
{
   uint stillformatId, stillResolutionId;
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

  //Initialize buffers
   initializeBuffers();

   //Set the Report Number
   g_out_packet_buf[1] = CAMERA_CONTROL_FSCAM_CU135;// camera control id
   g_out_packet_buf[2] = GET_STILL_RESOLUTION; // store frame

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[6]== SET_SUCCESS) {
       stillformatId = g_in_packet_buf[2];
       stillResolutionId = g_in_packet_buf[3];
           emit stillsettingsReceived(stillformatId, stillResolutionId);
           return true;
           }
   }
   return false;
}


void FSCAM_CU135::initializeBuffers(){

    //Initialize input and output buffers
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));
}
bool FSCAM_CU135::initFSCAM_CU135(char *hidDeviceName)
{
    initializeBuffers();
    if(hidDeviceName == NULL)
    {
        return false;
    }

    if(uvccamera::hid_fd > 0)
    {
        close(uvccamera::hid_fd);
    }

    uvccamera::hid_fd = open(hidDeviceName, O_RDWR|O_NONBLOCK);

    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

//    int ret, desc_size = 0;
//    char buf[256];
//    struct hidraw_devinfo info;
//    struct hidraw_report_descriptor rpt_desc;

//    /* Get Report Descriptor */
//    rpt_desc.size = desc_size;
//    ret = ioctl(uvccamera::hid_fd, HIDIOCGRDESC, &rpt_desc);
//    if (ret < 0) {
//        perror("HIDIOCGRDESC");
//        return false;
//    }

//    /* Get Raw Name */
//    ret = ioctl(uvccamera::hid_fd, HIDIOCGRAWNAME(256), buf);
//    if (ret < 0) {
//        perror("HIDIOCGRAWNAME");
//        return false;
//    }

//    /* Get Physical Location */
//    ret = ioctl(uvccamera::hid_fd, HIDIOCGRAWPHYS(256), buf);
//    if (ret < 0) {
//        perror("HIDIOCGRAWPHYS");
//        return false;
//    }

//    /* Get Raw Info */
//    ret = ioctl(uvccamera::hid_fd, HIDIOCGRAWINFO, &info);
//    if (ret < 0) {
//        perror("HIDIOCGRAWINFO");
//        return false;
//    }
    return true;
}
void FSCAM_CU135::deinitFSCAM_CU135()
{
    close(uvccamera::hid_fd);
}
bool FSCAM_CU135::readFirmwareVersion(uint *pMajorVersion, uint *pMinorVersion1, uint *sdkVersion, uint *svnVersion)
{

    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    *pMajorVersion = 0;
    *pMinorVersion1 = 0;
    *sdkVersion = 0;
    *svnVersion = 0;

    initializeBuffers();

    //Set the Report Number
    g_out_packet_buf[1] = READFIRMWAREVERSION; 	/* Report Number */

    unsigned short int sdk_ver=0, svn_ver=0;

      // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if(g_in_packet_buf[0] == READFIRMWAREVERSION){
             sdk_ver = (g_in_packet_buf[3]<<8)+g_in_packet_buf[4];
             svn_ver = (g_in_packet_buf[5]<<8)+g_in_packet_buf[6];

             *pMajorVersion = g_in_packet_buf[1];
             *pMinorVersion1 = g_in_packet_buf[2];
             *sdkVersion = sdk_ver;
             *svnVersion = svn_ver;
             return true;
        }
    }
    return false;

}

/* Added by Navya -08 Apr 2019
 * Inorder to control Led's  */

bool FSCAM_CU135::getLedControl()
{
   bool ledstatus,powerctl,stream,trigger;

   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[0] = CAMERA_CONTROL_FSCAM_CU135;  /* set camera control code */
   g_out_packet_buf[1] = GET_LED_CONTROL_FSCAM_CU135; /* get LED control code */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[6]==GET_FAIL) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_FSCAM_CU135 &&
           g_in_packet_buf[1]==GET_LED_CONTROL_FSCAM_CU135 &&
           g_in_packet_buf[6]==GET_SUCCESS) {
           ledstatus = g_in_packet_buf[2];
           powerctl=g_in_packet_buf[3];
           stream=g_in_packet_buf[4];
           trigger=g_in_packet_buf[5];
           emit ledControlStatus(ledstatus,powerctl,stream,trigger);
           return true;
       }
   }
   return false;

}

//Modified by Navya - 5th June 2019

/** breif description for setLedControl
 * ledstatus       -  whether ON/OFF
 * powerctl(red)   -  status of Power ON/OFF
 * stream (blue)   -  status of Streaming
 * trigger (green) -  status of Trigger Acknowledgement
 * @return true/false
 **/

bool FSCAM_CU135::setLedControl(bool ledstatus,bool powerctl,bool stream,bool trigger)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {

        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[0] = CAMERA_CONTROL_FSCAM_CU135; /* set camera control code */
    g_out_packet_buf[1] = SET_LED_CONTROL_FSCAM_CU135; /* set led control code */
    if(ledstatus)
        g_out_packet_buf[2] = ENABLE_LED_CONTROL_FSCAM_CU135;
    else
        g_out_packet_buf[2] = DISABLE_LED_CONTROL_FSCAM_CU135;

    if(powerctl)
        g_out_packet_buf[5] = ENABLE_POWERON_CONTROL_FSCAM_CU135;
    else
        g_out_packet_buf[5] = DISABLE_POWERON_CONTROL_FSCAM_CU135;

    if(stream)
        g_out_packet_buf[3] = ENABLE_STREAMING_CONTROL_FSCAM_CU135;
    else
        g_out_packet_buf[3] = DISABLE_STREAMING_CONTROL_FSCAM_CU135;

    if(trigger)
        g_out_packet_buf[4] = ENABLE_TRIGGERACK_CONTROL_FSCAM_CU135;
    else
        g_out_packet_buf[4] = DISABLE_TRIGGERACK_CONTROL_FSCAM_CU135;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {

            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_FSCAM_CU135 &&
                  g_in_packet_buf[1]==SET_LED_CONTROL_FSCAM_CU135 &&
                  g_in_packet_buf[6]==SET_SUCCESS) {

            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_130::setFlickerDetection - setting the flicker control.
 * @param flickerMode - mode we set for camera
 * @return true/false
 */

bool FSCAM_CU135::setFlickerDetection(camFlickerMode flickerMode){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_FSCAM_CU135; /* camera id */
    g_out_packet_buf[2] = SET_FLICKER_DETECTION; /* set flicker detection command */
    g_out_packet_buf[3] = flickerMode; /* flicker detection mode to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_FSCAM_CU135 &&
            g_in_packet_buf[1]==SET_FLICKER_DETECTION &&
            g_in_packet_buf[6]==SET_SUCCESS) {

            return true;
        }
    }
    return false;
}

/**
 * @brief FSCAM_CU135::getFlickerDetection - getting the flicker control we set,from the camera.
 * @return true/false
 */

bool FSCAM_CU135::getFlickerDetection()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_FSCAM_CU135; /* camera id */
    g_out_packet_buf[2] = GET_FLICKER_DETECTION; /* get flicker detection command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_FSCAM_CU135 &&
            g_in_packet_buf[1]==GET_FLICKER_DETECTION &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit flickerDetectionMode(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

bool FSCAM_CU135::resetTimeStamp()
{

    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_FSCAM_CU135; // camera control id
    g_out_packet_buf[2] = RESET_TIME_STAMP; // reset time stamp

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]== GET_SUCCESS) {
            return true;
        }
    }

    return false;
}
