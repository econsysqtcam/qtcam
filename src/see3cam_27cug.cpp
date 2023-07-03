/*
 * see3cam_27cug.cpp -- Handling special feature of see3cam_27cug camera
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
#include "see3cam_27cug.h"
#include "videostreaming.h"
SEE3CAM_27CUG::SEE3CAM_27CUG()
{
}

//Creating object for Videostreaming class to access CameraModeEnabled()
Videostreaming videostreamingObject;

/*
 * @brief SEE3CAM_27CUG::setOrientation - Setting orientation - set Normal/horizontal/vertical/Rotate180
 * @param - horizontal flip selection
 * @param - vertical flip selection
 * return true/false
*/
bool SEE3CAM_27CUG::setOrientation(bool horzModeSel, bool vertiModeSel)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_27CUG; /* camera id */
    g_out_packet_buf[2] = SET_ORIENTATION_SEE3CAM_27CUG; /* set orientation command  */

    if(horzModeSel && vertiModeSel){
        g_out_packet_buf[3] = ROTATE_180; /* both flip enable */
    }else if(horzModeSel && !vertiModeSel){
        g_out_packet_buf[3] = HORIZONTAL; /* horizontal flip only mode */
    }else if(!horzModeSel && vertiModeSel){
        g_out_packet_buf[3] = VERTICAL; /* vertical flip only mode */
    }else
        g_out_packet_buf[3] = NORMAL; /* both flip disable */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_27CUG &&
            g_in_packet_buf[1] == SET_ORIENTATION_SEE3CAM_27CUG &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_27CUG::getOrientation - getting flip mode from the camera
 * return true - success /false - failure
 */
bool SEE3CAM_27CUG::getOrientation()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_27CUG; /* camera id */
    g_out_packet_buf[2] = GET_ORIENTATION_SEE3CAM_27CUG; /* get orientation command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_27CUG &&
            g_in_packet_buf[1] == GET_ORIENTATION_SEE3CAM_27CUG &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit flipMirrorModeChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

bool SEE3CAM_27CUG::setCameraMode(cameraMode cameraModeValues)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_27CUG; /* camera id */
    g_out_packet_buf[2] = SET_CAMERA_MODE_SEE3CAM_27CUG;
    g_out_packet_buf[3] = cameraModeValues;  /* BOTH MODE DISABLE */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_27CUG &&
            g_in_packet_buf[1] == SET_CAMERA_MODE_SEE3CAM_27CUG &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool SEE3CAM_27CUG::getCameraMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_27CUG; /* camera id */
    g_out_packet_buf[2] = GET_CAMERA_MODE_SEE3CAM_27CUG; /* get camera mode command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_27CUG &&
            g_in_packet_buf[1] == GET_CAMERA_MODE_SEE3CAM_27CUG &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            videostreamingObject.cameraModeEnabled(g_in_packet_buf[2]);
            emit cameraModeChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;

}

bool SEE3CAM_27CUG::setStreamMode(streamModes streamModeValues)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_27CUG; /* camera id */
    g_out_packet_buf[2] = SET_STREAM_MODE_SEE3CAM_27CUG; /* set stream command  */
    g_out_packet_buf[3] = streamModeValues; /* pass strobe mode value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_27CUG &&
            g_in_packet_buf[1] == SET_STREAM_MODE_SEE3CAM_27CUG &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool SEE3CAM_27CUG::getStreamMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_27CUG; /* camera id */
    g_out_packet_buf[2] = GET_STREAM_MODE_SEE3CAM_27CUG; /* get stream mode command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL)
        {
            return false;
        }
        else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_27CUG &&
            g_in_packet_buf[1] == GET_STREAM_MODE_SEE3CAM_27CUG &&
            g_in_packet_buf[6] == GET_SUCCESS)
        {
            emit streamModeChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}


bool SEE3CAM_27CUG::setStrobeMode(strobeMode strobeValues)
{

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_27CUG; /* camera id */
    g_out_packet_buf[2] = SET_STROBE_MODE_SEE3CAM_27CUG; /* set strobe command  */
    g_out_packet_buf[3] = strobeValues; /* pass strobe mode value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_27CUG &&
            g_in_packet_buf[1] == SET_STROBE_MODE_SEE3CAM_27CUG &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_27CUG::getStrobeMode - get Strobe mode
 * @return true/false
 */
bool SEE3CAM_27CUG::getStrobeMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_27CUG; /* camera id */
    g_out_packet_buf[2] = GET_STROBE_MODE_SEE3CAM_27CUG; /* get strobe mode command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_27CUG &&
            g_in_packet_buf[1] == GET_STROBE_MODE_SEE3CAM_27CUG &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit strobeModeChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}


bool SEE3CAM_27CUG::setBrightnessForIR(uint brightness)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_27CUG; /* set camera control code */
    g_out_packet_buf[2] = SET_BRIGHTNESS_FOR_IR ;
    g_out_packet_buf[3] = brightness;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_27CUG &&
            g_in_packet_buf[1]==SET_BRIGHTNESS_FOR_IR  &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool SEE3CAM_27CUG::getBrightnessForIR()
{
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_27CUG;  /* set camera control code */
   g_out_packet_buf[2] = GET_BRIGHTNESS_FOR_IR;

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[6]==GET_FAIL) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_27CUG &&
           g_in_packet_buf[1]==GET_BRIGHTNESS_FOR_IR  &&
           g_in_packet_buf[6]==GET_SUCCESS) {
           emit brightnessChanged(g_in_packet_buf[2]);
           return true;
       }
   }
   return false;
}

bool SEE3CAM_27CUG::setContrastForIR(uint contrast)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_27CUG; /* set camera control code */
    g_out_packet_buf[2] = SET_CONTRAST_FOR_IR ;
    g_out_packet_buf[3] = contrast;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_27CUG &&
            g_in_packet_buf[1]==SET_CONTRAST_FOR_IR &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool SEE3CAM_27CUG::getContrastForIR()
{
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_27CUG;  /* set camera control code */
   g_out_packet_buf[2] = GET_CONTRAST_FOR_IR;

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[6]==GET_FAIL) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_27CUG &&
           g_in_packet_buf[1]==GET_CONTRAST_FOR_IR  &&
           g_in_packet_buf[6]==GET_SUCCESS) {
           emit contrastChanged(g_in_packet_buf[2]);
           return true;
       }
   }
   return false;
}

bool SEE3CAM_27CUG::setGainForIR(uint gain)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_27CUG; /* set camera control code */
    g_out_packet_buf[2] = SET_GAIN_FOR_IR ;
    g_out_packet_buf[3] = gain;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_27CUG &&
            g_in_packet_buf[1]==SET_GAIN_FOR_IR &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool SEE3CAM_27CUG::getGainForIR()
{
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();
   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_27CUG;  /* set camera control code */
   g_out_packet_buf[2] = GET_GAIN_FOR_IR ;
   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[6]==GET_FAIL){
           return false;
       }
       else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_27CUG &&
           g_in_packet_buf[1]==GET_GAIN_FOR_IR  &&
           g_in_packet_buf[6]==GET_SUCCESS) {
           emit gainChanged(g_in_packet_buf[2]);
           return true;
       }
   }
   return false;
}


/**
 * @brief ::setMinimumExposureCompensation - setting minimum exposure compensation
 * @return true/false
 */
bool SEE3CAM_27CUG::setMinimumExposureCompensation(unsigned int MinExpCompValue)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_27CUG;
    g_out_packet_buf[2] = SET_MINIMUM_EXPOSURE_COMPENSATION;
    g_out_packet_buf[3] = (u_int8_t)((MinExpCompValue & 0xFF000000) >> 24);
    g_out_packet_buf[4] = (u_int8_t)((MinExpCompValue & 0x00FF0000) >> 16);
    g_out_packet_buf[5] = (u_int8_t)((MinExpCompValue & 0x0000FF00) >> 8);
    g_out_packet_buf[6] = (u_int8_t)((MinExpCompValue & 0x000000FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            emit indicateCommandStatus("Failure", "Failed to set Minimum Exposure Compensation Value");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_27CUG &&
            g_in_packet_buf[1]==SET_MINIMUM_EXPOSURE_COMPENSATION &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            emit indicateCommandStatus("Success", "Minimum exposure compensation value is set successfully");
            return true;
        }
    }
    return false;
}

/**
 * @brief ::getMinimumExposureCompensation - getting minimum exposure compensation
 * @return true/false
 */
bool SEE3CAM_27CUG::getMinimumExposureCompensation(){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_27CUG;
    g_out_packet_buf[2] = GET_MINIMUM_EXPOSURE_COMPENSATION;

    unsigned int minExpCompValue;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_27CUG &&
            g_in_packet_buf[1]==GET_MINIMUM_EXPOSURE_COMPENSATION &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            minExpCompValue = (((u_int8_t)g_in_packet_buf[2]) << 24)
                                | (((u_int8_t)g_in_packet_buf[3]) << 16)
                                | (((u_int8_t)g_in_packet_buf[4]) << 8)
                                | (((u_int8_t)g_in_packet_buf[5]) << 0);
            emit minimumExposureReceive(minExpCompValue);
            return true;
        }
    }
    return false;
}

/**
 * @brief ::setMaximumExposureCompensation - setting maximum exposure compensation
 * @return true/false
 */
bool SEE3CAM_27CUG::setMaximumExposureCompensation(unsigned int MaxExpCompValue)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_27CUG;
    g_out_packet_buf[2] = SET_MAXIMUM_EXPOSURE_COMPENSATION;
    g_out_packet_buf[3] = (u_int8_t)((MaxExpCompValue & 0xFF000000) >> 24);
    g_out_packet_buf[4] = (u_int8_t)((MaxExpCompValue & 0x00FF0000) >> 16);
    g_out_packet_buf[5] = (u_int8_t)((MaxExpCompValue & 0x0000FF00) >> 8);
    g_out_packet_buf[6] = (u_int8_t)((MaxExpCompValue & 0x000000FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            emit indicateCommandStatus("Failure", "Failed to set exposure compensation value");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_27CUG &&
            g_in_packet_buf[1]==SET_MAXIMUM_EXPOSURE_COMPENSATION &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            emit indicateCommandStatus("Success", " Maximum exposure compensation Value is set successfully");
            return true;
        }
    }
    return false;
}

/**
 * @brief ::getMaximumExposureCompensation - getting maximum exposure compensation
 * @return true/false
 */
bool SEE3CAM_27CUG::getMaximumExposureCompensation()
{

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_27CUG;
    g_out_packet_buf[2] = GET_MAXIMUM_EXPOSURE_COMPENSATION;

    unsigned int maxExpCompValue;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_27CUG &&
            g_in_packet_buf[1]==GET_MAXIMUM_EXPOSURE_COMPENSATION &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            maxExpCompValue = (((u_int8_t)g_in_packet_buf[2]) << 24)
                                | (((u_int8_t)g_in_packet_buf[3]) << 16)
                                | (((u_int8_t)g_in_packet_buf[4]) << 8)
                                | (((u_int8_t)g_in_packet_buf[5]) << 0);
            emit maximumExposureReceive(maxExpCompValue);
            return true;
        }
    }
    return false;
}



bool SEE3CAM_27CUG::get27CugSerialNumber()
{
    QString lsb = "";
    QString msb = "";

    bool timeout = true;
    int ret = 0;
    unsigned int start, end = 0;

    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = GET27CUG_UNIQUE_ID_1; 	/* Report Number */

    ret = write(uvccamera::hid_fd, g_out_packet_buf, BUFFER_LENGTH);
    if (ret < 0) {
        _text = tr("Device not available");
        return false;
    }

    /* Read the Unique id from the device */
    start = uvc.getTickCount();
    while(timeout)
    {
        /* Get a report from the device */
        ret = read(uvccamera::hid_fd, g_in_packet_buf, BUFFER_LENGTH);
        if (ret < 0)
        {
        }
        else
        {
            if((g_in_packet_buf[0] == GET27CUG_UNIQUE_ID_1))
            {
                lsb.sprintf("%02x%02x%02x%02x",g_in_packet_buf[1],g_in_packet_buf[2],g_in_packet_buf[3],g_in_packet_buf[4]);

                //Initialize the buffer
                memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

                //Set the Report Number
                g_out_packet_buf[1] = GET27CUG_UNIQUE_ID_1;
                g_out_packet_buf[2] = GET27CUG_UNIQUE_ID_2;

                ret = write(uvccamera::hid_fd, g_out_packet_buf, BUFFER_LENGTH);
                if (ret < 0) {
                    _text = tr("Device not available");
                    return false;
                }
                start = uvc.getTickCount();

                while(timeout)
                {
                    /* Get a report from the device */
                    ret = read(uvccamera::hid_fd, g_in_packet_buf, BUFFER_LENGTH);
                    if (ret < 0)
                    {
                    }
                    else
                    {
                        if((g_in_packet_buf[0] == GET27CUG_UNIQUE_ID_1))
                        {
                            msb.sprintf("%02x%02x%02x%02x",g_in_packet_buf[2],g_in_packet_buf[3],g_in_packet_buf[4],g_in_packet_buf[5]);
                            timeout = false;
                        }
                    }
                }

            }
        }
        end = uvc.getTickCount();
        if(end - start > TIMEOUT)
        {
            timeout = false;
            return false;
        }
    }
    _text.clear();
    _text.append(lsb+msb);
    emit titleTextChanged(_title,_text);
    return true;
}



/**
 * @brief SEE3CAM_27CUG::setToDefaultValues - set all the values to default in camera
 * @return true/false
 */
bool SEE3CAM_27CUG::setToDefaultValues()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_27CUG; /* camera id */
    g_out_packet_buf[2] = SET_DEFAULT_SEE3CAM_27CUG; /* set to default command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH))
    {
        if (g_in_packet_buf[6]==SET_FAIL)
        {
            return false;
        }
        else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_27CUG &&
            g_in_packet_buf[1]==SET_DEFAULT_SEE3CAM_27CUG &&
            g_in_packet_buf[6]==SET_SUCCESS){
            return true;
        }
    }
    return false;
}

/*
 *
 * @brief SEE3CAM_27CUG::initializeBuffers - Initialize input and output buffers
 */
void SEE3CAM_27CUG::initializeBuffers(){
    //Initialize input and output buffers
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));
}
