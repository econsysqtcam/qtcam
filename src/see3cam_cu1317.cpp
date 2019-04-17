/*
 * See3CAM_CU1317.cpp -- Handling special feature of seecamcu135 camera
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

#include "see3cam_cu1317.h"
#include "uvccamera.h"
#include "videostreaming.h"


See3CAM_CU1317::See3CAM_CU1317(QObject *parent) :
    QObject(parent)
{
}

// Added by Sankari: Mar 21, 2019
//To set number of frames to skip in preview
void See3CAM_CU1317::setPreviewSkipCount(){
    uint previewFrameToSkip;
    previewFrameToSkip = 3;
    // send preview frame to skip values to See3CAM_CU1317.qml
    emit updatePreviewFrameToSkip(previewFrameToSkip);
}

bool See3CAM_CU1317::setEffectMode(specialEffects specialEffect)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_See3CAM_CU1317; /* set camera control code */
    g_out_packet_buf[2] = SET_SPECIAL_EFFECT_MODE_See3CAM_CU1317; /* set special effect code */
    g_out_packet_buf[3] = specialEffect; /* set special effect */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
           return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_See3CAM_CU1317 &&
            g_in_packet_buf[1]==SET_SPECIAL_EFFECT_MODE_See3CAM_CU1317 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

bool See3CAM_CU1317::getEffectMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_See3CAM_CU1317; /* set camera control code */
    g_out_packet_buf[2] = GET_SPECIAL_EFFECT_MODE_See3CAM_CU1317; /* get special effect code */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_See3CAM_CU1317 &&
            g_in_packet_buf[1]==GET_SPECIAL_EFFECT_MODE_See3CAM_CU1317 &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            emit effectModeValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

bool See3CAM_CU1317::setSceneMode(sceneModes sceneMode)
{
    // hid validation

    if(uvccamera::hid_fd < 0)
    {

        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_See3CAM_CU1317; /* Camera control id */
    g_out_packet_buf[2] = SET_SCENE_MODE_See3CAM_CU1317; /* Set scene mode command */
    g_out_packet_buf[3] = sceneMode; /* Scene mode to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_See3CAM_CU1317 &&
                  g_in_packet_buf[1]==SET_SCENE_MODE_See3CAM_CU1317 &&
                  g_in_packet_buf[6]==SET_SUCCESS) {\
                 return true;
        }
    }
    return false;
}

bool See3CAM_CU1317::getSceneMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_See3CAM_CU1317; /* camera id */
    g_out_packet_buf[2] = GET_SCENE_MODE_See3CAM_CU1317; /* get scene mode command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_See3CAM_CU1317 &&
            g_in_packet_buf[1]==GET_SCENE_MODE_See3CAM_CU1317 &&
            g_in_packet_buf[6]==GET_SUCCESS) {\

            emit sceneModeValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

bool See3CAM_CU1317::setDenoiseValue(uint deNoiseVal)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_See3CAM_CU1317; /* set camera control code */
    g_out_packet_buf[2] = SET_DENOISE_CONTROL_See3CAM_CU1317; /* set denoise control code */
    g_out_packet_buf[3] = deNoiseVal; /* set denoise value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_See3CAM_CU1317 &&
            g_in_packet_buf[1]==SET_DENOISE_CONTROL_See3CAM_CU1317 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

bool See3CAM_CU1317::getDenoiseValue()
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
   g_out_packet_buf[1] = CAMERA_CONTROL_See3CAM_CU1317; /* set camera control code */
   g_out_packet_buf[2] = GET_DENOISE_CONTROL_See3CAM_CU1317; /* get denoise code */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[6]==GET_FAIL) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_See3CAM_CU1317 &&
           g_in_packet_buf[1]==GET_DENOISE_CONTROL_See3CAM_CU1317 &&
           g_in_packet_buf[6]==GET_SUCCESS) {\
           denoiseVal = g_in_packet_buf[2];
           emit denoiseValue(denoiseVal);
           return true;
       }
   }
   return false;

}

bool See3CAM_CU1317::setQFactor(uint qfactor)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_See3CAM_CU1317; /* camera id */
    g_out_packet_buf[2] = SET_Q_FACTOR_See3CAM_CU1317; /* set qfactor command */
    g_out_packet_buf[3] = qfactor; /* qfactor value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_See3CAM_CU1317 &&
            g_in_packet_buf[1]==SET_Q_FACTOR_See3CAM_CU1317 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

bool See3CAM_CU1317::getQFactor()
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
   g_out_packet_buf[1] = CAMERA_CONTROL_See3CAM_CU1317; /* camera id */
   g_out_packet_buf[2] = GET_Q_FACTOR_See3CAM_CU1317; /* get qFactor value */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[6]==GET_FAIL) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_See3CAM_CU1317 &&
           g_in_packet_buf[1]==GET_Q_FACTOR_See3CAM_CU1317 &&
           g_in_packet_buf[6]==GET_SUCCESS) {\
           qfactor = g_in_packet_buf[2];
           emit qFactorValue(qfactor);
           return true;
       }
   }
   return false;

}

bool See3CAM_CU1317::setiHDRMode(camiHDRMode iHDRMode, uint iHDRValue)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_See3CAM_CU1317; /* set camera control code */
    g_out_packet_buf[2] = SET_HDR_MODE_See3CAM_CU1317; /* set ihdr mode command code */
    g_out_packet_buf[3] = iHDRMode; /* iHdr value */
    if(iHDRMode == HdrManual){
        g_out_packet_buf[4] = iHDRValue;
    }
    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_See3CAM_CU1317 &&
            g_in_packet_buf[1]==SET_HDR_MODE_See3CAM_CU1317 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool See3CAM_CU1317::getiHDRMode()
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
   g_out_packet_buf[1] = CAMERA_CONTROL_See3CAM_CU1317; /* camera id */
   g_out_packet_buf[2] = GET_HDR_MODE_See3CAM_CU1317; /* get hdr mode value */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[6]==GET_FAIL) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_See3CAM_CU1317 &&
           g_in_packet_buf[1]==GET_HDR_MODE_See3CAM_CU1317 &&
           g_in_packet_buf[6]==GET_SUCCESS) {\
           iHDRMode = g_in_packet_buf[2];
            iHDRValue = g_in_packet_buf[3];
           emit hdrModeValue(iHDRMode,iHDRValue);
           return true;
       }
   }
   return false;

}

bool See3CAM_CU1317::setStreamMode(camStreamMode streamMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {

        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_See3CAM_CU1317; /* set camera control code */
    g_out_packet_buf[2] = SET_STREAM_MODE_See3CAM_CU1317; /* set stream mode code */
    g_out_packet_buf[3] = streamMode; /* actual stream mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {

            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_See3CAM_CU1317 &&
            g_in_packet_buf[1]==SET_STREAM_MODE_See3CAM_CU1317 &&
            g_in_packet_buf[6]==SET_SUCCESS) {

            return true;
        }
    }
    return false;
}

bool See3CAM_CU1317::getStreamMode()
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
   g_out_packet_buf[1] = CAMERA_CONTROL_See3CAM_CU1317;  /* set camera control code */
   g_out_packet_buf[2] = GET_STREAM_MODE_See3CAM_CU1317; /* get stream mode code */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[6]==GET_FAIL) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_See3CAM_CU1317 &&
           g_in_packet_buf[1]==GET_STREAM_MODE_See3CAM_CU1317 &&
           g_in_packet_buf[6]==GET_SUCCESS) {
           streamMode = g_in_packet_buf[2];
           emit streamModeValue(streamMode);

           return true;
       }
   }
   return false;

}

bool See3CAM_CU1317::setOrientation(bool horzModeSel, bool vertiModeSel)
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
    g_out_packet_buf[1] = CAMERA_CONTROL_See3CAM_CU1317; /* camera control for see3cam_See3CAM_CU1317 camera */
    g_out_packet_buf[2] = SET_ORIENTATION_See3CAM_CU1317; /* set flip mode for storagecam_See3CAM_CU1317 camera */
    g_out_packet_buf[3] = flipMode; /*flip mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_See3CAM_CU1317 &&
            g_in_packet_buf[1]==SET_ORIENTATION_See3CAM_CU1317 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;

  }

bool See3CAM_CU1317::getOrientation()
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
    g_out_packet_buf[1] = CAMERA_CONTROL_See3CAM_CU1317; /* camera control id */
    g_out_packet_buf[2] = GET_ORIENTATION_See3CAM_CU1317; /* Get orientation command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_See3CAM_CU1317 &&
            g_in_packet_buf[1]==GET_ORIENTATION_See3CAM_CU1317 &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            flipMode = (g_in_packet_buf[2]);
            emit flipMirrorModeChanged(flipMode);
            return true;
        }
    }
    return false;

}


bool See3CAM_CU1317::setROIAutoExposure(camROIAutoExpMode see3camAutoexpROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize)
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
    g_out_packet_buf[1] = CAMERA_CONTROL_See3CAM_CU1317; /* camera id */
    g_out_packet_buf[2] = SET_EXP_ROI_MODE_See3CAM_CU1317; /* set exposure ROI command */
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
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_See3CAM_CU1317 &&
            g_in_packet_buf[1]==SET_EXP_ROI_MODE_See3CAM_CU1317 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool See3CAM_CU1317::getAutoExpROIModeAndWindowSize(){
   uint autoexpROImode, winSize;
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_See3CAM_CU1317; /* camera id */
   g_out_packet_buf[2] = GET_EXP_ROI_MODE_See3CAM_CU1317; /* get exposure ROI mode  */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[6]==GET_FAIL) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_See3CAM_CU1317 &&
           g_in_packet_buf[1]==GET_EXP_ROI_MODE_See3CAM_CU1317 &&
           g_in_packet_buf[6]==GET_SUCCESS) {
           autoexpROImode = g_in_packet_buf[2];
           winSize = g_in_packet_buf[5];
            emit roiAutoExpMode(autoexpROImode, winSize);
           return true;
       }
   }

   return false;
}

bool See3CAM_CU1317::setFaceDetectionRect(bool enableFaceDetectRect, bool embedData, bool overlayRect){
     u_int8_t faceDetectMode, embedDataMode;
    if(enableFaceDetectRect)
        faceDetectMode = ENABLE_FACE_RECT_See3CAM_CU1317; /* enable face detect */
    else
        faceDetectMode = DISABLE_FACE_RECT_See3CAM_CU1317; /* disable face detect */

    if(embedData)
        embedDataMode = ENABLE_EMBED_DATA_See3CAM_CU1317; /* enable embed data */
    else
        embedDataMode = DISABLE_EMBED_DATA_See3CAM_CU1317; /* disable embed data */

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_See3CAM_CU1317; /* camera id */
    g_out_packet_buf[2] = SET_FACE_DETECT_See3CAM_CU1317; /* set face detect Rect command */
    g_out_packet_buf[3] = faceDetectMode; /* enable/disable face Detect */
    g_out_packet_buf[4] = embedData; /* enable/disable embed data */
    g_out_packet_buf[5] = overlayRect; /* enable/disable overlay rect */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_See3CAM_CU1317 &&
            g_in_packet_buf[1]==SET_FACE_DETECT_See3CAM_CU1317 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
   }

bool See3CAM_CU1317::getFaceDetectMode(){
   uint faceDetectMode, faceDetectEmbedDataValue, faceOverlayRect;
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_See3CAM_CU1317; /* camera id */
   g_out_packet_buf[2] = GET_FACE_DETECT_See3CAM_CU1317; /* get face detect mode command */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[6]==GET_FAIL) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_See3CAM_CU1317 &&
           g_in_packet_buf[1]==GET_FACE_DETECT_See3CAM_CU1317 &&
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

bool See3CAM_CU1317::setSmileDetection(bool enableSmileDetect, bool embedData){
    u_int8_t smileDetectMode, embedDataMode;
    if(enableSmileDetect)
        smileDetectMode = ENABLE_SMILE_DETECT_See3CAM_CU1317; /* enable smile detect */
    else
        smileDetectMode = DISABLE_SMILE_DETECT_See3CAM_CU1317; /* disable smile detect */

    if(embedData)
        embedDataMode = ENABLE_EMBED_DATA_See3CAM_CU1317; /* enable embed data */
    else
        embedDataMode = DISABLE_EMBED_DATA_See3CAM_CU1317; /* disable embed data */

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_See3CAM_CU1317; /* camera id */
    g_out_packet_buf[2] = SET_SMILE_DETECTION_See3CAM_CU1317; /* set face detect Rect command */
    g_out_packet_buf[3] = smileDetectMode; /* disable smile detect */
    g_out_packet_buf[4] = embedData; /* disable embed data */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {

            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_See3CAM_CU1317 &&
            g_in_packet_buf[1]==SET_SMILE_DETECTION_See3CAM_CU1317 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\

            return true;
        }
    }
    return false;
}

bool See3CAM_CU1317::getSmileDetectMode(){
   uint smileDetectMode, smileDetectEmbedDataValue;
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_See3CAM_CU1317; /* Camera control id */
   g_out_packet_buf[2] = GET_SMILE_DETECTION_See3CAM_CU1317; /* Get smile detection */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[6]==GET_FAIL) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_See3CAM_CU1317 &&
           g_in_packet_buf[1]==GET_SMILE_DETECTION_See3CAM_CU1317 &&
           g_in_packet_buf[6]==GET_SUCCESS) {\

           smileDetectMode = g_in_packet_buf[2];
           smileDetectEmbedDataValue = g_in_packet_buf[3];

        emit smileDetectModeValue(smileDetectMode, smileDetectEmbedDataValue);
           return true;
       }
   }
   return false;
}

bool See3CAM_CU1317::setFrameRateCtrlValue(uint frameRateCtrl)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_See3CAM_CU1317; /* set camera control code */
    g_out_packet_buf[2] = SET_FRAME_RATE_See3CAM_CU1317; /* set framerate control code */
    g_out_packet_buf[3] = frameRateCtrl; /* set framerate value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_See3CAM_CU1317 &&
            g_in_packet_buf[1]==SET_FRAME_RATE_See3CAM_CU1317 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

bool See3CAM_CU1317::getFrameRateCtrlValue()
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
   g_out_packet_buf[1] = CAMERA_CONTROL_See3CAM_CU1317; /* set camera control code */
   g_out_packet_buf[2] = GET_FRAME_RATE_See3CAM_CU1317; /* get frame rate code */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[6]==GET_FAIL) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_See3CAM_CU1317 &&
           g_in_packet_buf[1]==GET_FRAME_RATE_See3CAM_CU1317 &&
           g_in_packet_buf[6]==GET_SUCCESS) {\
           frameRateVal = g_in_packet_buf[2];
           emit frameRateCtrlValue(frameRateVal);
           return true;
       }
   }
   return false;

}

bool See3CAM_CU1317::setExposureCompensation(uint exposureCompValue)
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
   g_out_packet_buf[1] = CAMERA_CONTROL_See3CAM_CU1317; /* camera id */
   g_out_packet_buf[2] = SET_EXPOSURE_COMPENSATION_See3CAM_CU1317; /* set exposure compensation command */
   g_out_packet_buf[3] = (u_int8_t)((exposureCompValue & 0xFF000000) >> 24);
   g_out_packet_buf[4] = (u_int8_t)((exposureCompValue & 0x00FF0000) >> 16);
   g_out_packet_buf[5] = (u_int8_t)((exposureCompValue & 0x0000FF00) >> 8);
   g_out_packet_buf[6] = (u_int8_t)((exposureCompValue & 0x000000FF) >> 0);

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[6]==SET_FAIL) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_See3CAM_CU1317 &&
           g_in_packet_buf[1]==SET_EXPOSURE_COMPENSATION_See3CAM_CU1317 &&
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

bool See3CAM_CU1317::getExposureCompensation()
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
   g_out_packet_buf[1] = CAMERA_CONTROL_See3CAM_CU1317; /* set camera control code */
   g_out_packet_buf[2] = GET_EXPOSURE_COMPENSATION_See3CAM_CU1317; /* get exposure compensation code */


   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[6]==GET_FAIL) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_See3CAM_CU1317 &&
           g_in_packet_buf[1]==GET_EXPOSURE_COMPENSATION_See3CAM_CU1317 &&
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

bool See3CAM_CU1317::setToDefault()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_See3CAM_CU1317; /* camera id */
    g_out_packet_buf[2] = SET_TO_DEFAULT_See3CAM_CU1317; /* set to default command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_See3CAM_CU1317 &&
            g_in_packet_buf[1]==SET_TO_DEFAULT_See3CAM_CU1317 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            emit titleTextChanged(_title,_text);
            return true;
        }
    }
    return false;
}

bool See3CAM_CU1317::storePreviewFrame()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_See3CAM_CU1317;// camera control id
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

bool See3CAM_CU1317::storeStillFrame(uint stillformatId, uint stillresolutionId)
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_See3CAM_CU1317;// camera control id
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

bool See3CAM_CU1317::getNumberOfFramesCanStore(uint stillformatId, uint stillresolutionId)
{
   uint numberOfFrames;

   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

  //Initialize buffers
   initializeBuffers();

   //Set the Report Number
   g_out_packet_buf[1] = CAMERA_CONTROL_See3CAM_CU1317;// camera control id
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

bool See3CAM_CU1317::setStillResolution(uint stillformatId, uint stillresolutionId)
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

   //Initialize buffers
    initializeBuffers();

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_See3CAM_CU1317;// camera control id
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
bool See3CAM_CU1317::grabStillFrame(uint frameIndex, uint stillformatId, uint stillresolutionId)
{

    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_See3CAM_CU1317; // camera control id
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

bool See3CAM_CU1317::grabPreviewFrame()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_See3CAM_CU1317; // camera control id
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

bool See3CAM_CU1317::getStillResolution()
{
   uint stillformatId, stillResolutionId;
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

  //Initialize buffers
   initializeBuffers();

   //Set the Report Number
   g_out_packet_buf[1] = CAMERA_CONTROL_See3CAM_CU1317;// camera control id
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


void See3CAM_CU1317::initializeBuffers(){

    //Initialize input and output buffers
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));
}
bool See3CAM_CU1317::initStoreCAM1335(char *hidDeviceName)
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
void See3CAM_CU1317::deinitStoreCAM1335()
{
    close(uvccamera::hid_fd);
}
bool See3CAM_CU1317::readFirmwareVersion(uint *pMajorVersion, uint *pMinorVersion1, uint *sdkVersion, uint *svnVersion)
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

bool See3CAM_CU1317::getLedControl()
{
   bool ledstatus,blueledstatus,greenledstatus,redledstatus;

   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_See3CAM_CU1317;  /* set camera control code */
   g_out_packet_buf[2] = GET_LED_CONTROL_See3CAM_CU1317; /* get LED control code */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[6]==GET_FAIL) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_See3CAM_CU1317 &&
           g_in_packet_buf[1]==GET_LED_CONTROL_See3CAM_CU1317 &&
           g_in_packet_buf[6]==GET_SUCCESS) {
           ledstatus = g_in_packet_buf[3];
           blueledstatus=g_in_packet_buf[4];
           greenledstatus=g_in_packet_buf[5];
           redledstatus=g_in_packet_buf[6];

           emit ledControlStatus(ledstatus,blueledstatus,greenledstatus,redledstatus);

           return true;
       }
   }
   return false;

}

/** breif description for setLedControl
 * ledstatus     -  whether ON/OFF
 * blueledstatus -  status of blue led
 * greenledstatus-  status of green led
 * redledstatus  -  status of red led     **/

bool See3CAM_CU1317::setLedControl(bool ledstatus,bool blueledstatus,bool greenledstatus,bool redledstatus)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {

        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_See3CAM_CU1317; /* set camera control code */
    g_out_packet_buf[2] = SET_LED_CONTROL_See3CAM_CU1317; /* set led control code */
    g_out_packet_buf[3] = ledstatus; /* actual led status */
    g_out_packet_buf[4] = blueledstatus;
    g_out_packet_buf[5] = greenledstatus;
    g_out_packet_buf[6] = redledstatus;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {

            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_See3CAM_CU1317 &&
            g_in_packet_buf[1]==SET_LED_CONTROL_See3CAM_CU1317 &&
            g_in_packet_buf[6]==SET_SUCCESS) {

            return true;
        }
    }
    return false;
}
bool See3CAM_CU1317::resetTimeStamp()
{

    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_See3CAM_CU1317; // camera control id
    g_out_packet_buf[2] = RESET_TIME_STAMP; // reset time stamp

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]== GET_SUCCESS) {
            return true;
        }
    }

    return false;
}
