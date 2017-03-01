/*
 * seecam_30.cpp -- Handling special feature of see3cam_30 camera
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

#include "see3cam_30.h"

See3CAM_30::See3CAM_30(QObject *parent) :
    QObject(parent)
{
}

/**
 * @brief See3CAM_30::setEffectMode - setting special effects to the camera
 * return true - success /false - failure
 */
bool See3CAM_30::setEffectMode(const specialEffects &specialEffect)
{
    if(uvccamera::hid_fd < 0)
    {        
        return false;
    }
    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;

    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* set camera control code */
    g_out_packet_buf[2] = SET_SPECIAL_EFFECT; /* set special effect code */
    g_out_packet_buf[3] = specialEffect; /* set special effect */

    ret = write(uvccamera::hid_fd, g_out_packet_buf, BUFFER_LENGTH);

    if (ret < 0) {        
        perror("write");
        return false;
    }

    /* Read the Status code from the device */
    start = uvc.getTickCount();


    while(timeout)
    {
        /* Get a report from the device */
        ret = read(uvccamera::hid_fd, g_in_packet_buf, BUFFER_LENGTH);
        if (ret < 0) {
            perror("read");
        } else {
            if (g_in_packet_buf[6]==SET_FAIL) {                
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
                g_in_packet_buf[1]==SET_SPECIAL_EFFECT &&
                g_in_packet_buf[2]==specialEffect &&
                g_in_packet_buf[6]==SET_SUCCESS) {                
                    timeout=false;
            }
        }
        end = uvc.getTickCount();
        if(end - start > SET_COMMAND_TIMEOUT)
        {            
            timeout = false;
            return false;
        }
    }
    return true;
}

/**
 * @brief See3CAM_30::setDenoiseValue - setting denoise value to the camera
 * @param deNoiseVal - denoise value
 * return true - success /false - failure
 */
bool See3CAM_30::setDenoiseValue(int deNoiseVal)
{
    if(uvccamera::hid_fd < 0)
    {        
        return false;
    }

    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;

    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    //Set the Report Number

    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* set camera control code */
    g_out_packet_buf[2] = SET_DENOISE_CONTROL; /* set denoise control code */
    g_out_packet_buf[3] = deNoiseVal; /* set denoise value */

    ret = write(uvccamera::hid_fd, g_out_packet_buf, BUFFER_LENGTH);

    if (ret < 0) {        
        perror("write");
        return false;
    }
    /* Read the Status code from the device */
    start = uvc.getTickCount();
    while(timeout)
    {
        /* Get a report from the device */
        ret = read(uvccamera::hid_fd, g_in_packet_buf, BUFFER_LENGTH);
        if (ret < 0) {
            //perror("read");
        } else {
            if (g_in_packet_buf[6]==SET_FAIL) {                
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
                g_in_packet_buf[1]==SET_DENOISE_CONTROL &&
                g_in_packet_buf[2]==deNoiseVal &&
                g_in_packet_buf[6]==SET_SUCCESS) {                
                    timeout=false;
            }
        }
        end = uvc.getTickCount();
        if(end - start > SET_COMMAND_TIMEOUT)
        {            
            timeout = false;
            return false;
        }
    }


    return true;
}

/**
 * @brief See3CAM_30::getEffectMode - get effect mode set in camera
 * return true - success /false - failure
 */
bool See3CAM_30::getEffectMode()
{    
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;

    uint effectMode;
    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* set camera control code */
    g_out_packet_buf[2] = GET_SPECIAL_EFFECT; /* get special effect code */

    ret = write(uvccamera::hid_fd, g_out_packet_buf, BUFFER_LENGTH);

    if (ret < 0) {
        perror("write");
        return false;
    }

    /* Read the Status code from the device */
    start = uvc.getTickCount();

    while(timeout)
    {
        /* Get a report from the device */
        ret = read(uvccamera::hid_fd, g_in_packet_buf, BUFFER_LENGTH);
        if (ret < 0) {
            //perror("read");
        } else {
            if (g_in_packet_buf[6]==GET_FAIL) {                
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
                g_in_packet_buf[1]==GET_SPECIAL_EFFECT &&
                g_in_packet_buf[6]==GET_SUCCESS) {                    
                    effectMode = g_in_packet_buf[2];
                    emit effectModeChanged(effectMode);
                    timeout = false;
            }
        }
        end = uvc.getTickCount();
        if(end - start > TIMEOUT)
        {            
            timeout = false;
            return false;
        }
    }
    return true;
}

/**
 * @brief See3CAM_30::getDenoiseValue - get denoise value from camera
 * return true - success /false - failure
 */
bool See3CAM_30::getDenoiseValue()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;

    uint denoiseValue;
    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* set camera control code */
    g_out_packet_buf[2] = GET_DENOISE_CONTROL; /* get denoise code */

    ret = write(uvccamera::hid_fd, g_out_packet_buf, BUFFER_LENGTH);

    if (ret < 0) {
        perror("write");
        return false;
    }

    /* Read the Status code from the device */
    start = uvc.getTickCount();

    while(timeout)
    {
        /* Get a report from the device */
        ret = read(uvccamera::hid_fd, g_in_packet_buf, BUFFER_LENGTH);

        if (ret < 0) {
            //perror("read");
        } else {
            if (g_in_packet_buf[6]==GET_FAIL) {                
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
                g_in_packet_buf[1]==GET_DENOISE_CONTROL &&
                g_in_packet_buf[6]==GET_SUCCESS) {                
                    denoiseValue = g_in_packet_buf[2];
                    emit denoiseValueChanged(denoiseValue);
                    timeout = false;
            }
        }
        end = uvc.getTickCount();
        if(end - start > TIMEOUT)
        {            
            timeout = false;
            return false;
        }
    }
    return true;
}

/**
 * @brief See3CAM_30::setAutoFocusMode - set auto focus mode in camera
 * @param afMode - enum Auto focus mode
 * return true - success /false - failure
 */
bool See3CAM_30::setAutoFocusMode(camAfMode afMode){

    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;

    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* see3cam_30 camera control  */
    g_out_packet_buf[2] = SET_AF_MODE_30; /* Set Auto Focus mode command */
    g_out_packet_buf[3] = afMode; /* Auto focus mode */

    ret = write(uvccamera::hid_fd, g_out_packet_buf, BUFFER_LENGTH);

    if (ret < 0) {
        perror("write");
        return false;
    }

    /* Read the Status code from the device */
    start = uvc.getTickCount();


    while(timeout)
    {
        /* Get a report from the device */
        ret = read(uvccamera::hid_fd, g_in_packet_buf, BUFFER_LENGTH);
        if (ret < 0) {
            //perror("read");
        } else {
            if (g_in_packet_buf[6]==SET_FAIL) {
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
                g_in_packet_buf[1]==SET_AF_MODE_30 &&
                g_in_packet_buf[2]==afMode &&
                g_in_packet_buf[6]==SET_SUCCESS) {
                    timeout=false;
            }
        }
        end = uvc.getTickCount();
        if(end - start > TIMEOUT)
        {
            timeout = false;
            return false;
        }
    }
    return true;
}

/**
 * @brief See3CAM_30::getAutoFocusMode - get auto focus mode from camera
 * return true - success /false - failure
 */
bool See3CAM_30::getAutoFocusMode()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;

    uint autoFocusMode;
    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* camera control id for see3cam_30 */
    g_out_packet_buf[2] = GET_AF_MODE_30; /* get focus mode */

    ret = write(uvccamera::hid_fd , g_out_packet_buf, BUFFER_LENGTH);

    if (ret < 0) {
        perror("write");
        return false;
    }

    /* Read the Status code from the device */
    start = uvc.getTickCount();

    while(timeout)
    {
        /* Get a report from the device */
        ret = read(uvccamera::hid_fd, g_in_packet_buf, BUFFER_LENGTH);        
        if (ret < 0) {
            //perror("read");
        } else {
            if (g_in_packet_buf[6]==GET_FAIL) {
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
                g_in_packet_buf[1]==GET_AF_MODE_30 &&
                g_in_packet_buf[6]==GET_SUCCESS) {
                    autoFocusMode = g_in_packet_buf[2];
                    emit afModeChanged(autoFocusMode);
                    timeout = false;
            }
        }
        end = uvc.getTickCount();
        if(end - start > TIMEOUT)
        {            
            timeout = false;
            return false;
        }
    }
    return true;
}


/**
 * @brief See3CAM_30::setQFactor - set QFactor in camera
 * @param qFactor.
 * return true - success /false - failure
 */
bool See3CAM_30::setQFactor(uint qFactor){

    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;

    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* camera control id for see3cam_30 */
    g_out_packet_buf[2] = SET_Q_FACTOR_30; /* set Qfactor */
    g_out_packet_buf[3] = qFactor; /* Qfactor value */
    ret = write(uvccamera::hid_fd, g_out_packet_buf, BUFFER_LENGTH);

    if (ret < 0) {
        perror("write");
        return false;
    }

    /* Read the Status code from the device */
    start = uvc.getTickCount();


    while(timeout)
    {
        /* Get a report from the device */
        ret = read(uvccamera::hid_fd, g_in_packet_buf, BUFFER_LENGTH);
        if (ret < 0) {
            //perror("read");
        } else {
            if (g_in_packet_buf[6]==SET_FAIL) {
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
                g_in_packet_buf[1]==SET_Q_FACTOR_30 &&
                g_in_packet_buf[2]==qFactor &&
                g_in_packet_buf[6]==SET_SUCCESS) {
                    timeout=false;
            }
        }
        end = uvc.getTickCount();
        if(end - start > TIMEOUT)
        {
            timeout = false;
            return false;
        }
    }
    return true;
}

/**
 * @brief See3CAM_30::getQFactor - get QFactor from camera
 * return true - success /false - failure
 */
bool See3CAM_30::getQFactor()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;

    uint qFactor;
    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* camera control id for see3cam_30 */
    g_out_packet_buf[2] = GET_Q_FACTOR_30; /* get Qfactor value */

    ret = write(uvccamera::hid_fd , g_out_packet_buf, BUFFER_LENGTH);

    if (ret < 0) {
        perror("write");
        return false;
    }

    /* Read the Status code from the device */
    start = uvc.getTickCount();

    while(timeout)
    {
        /* Get a report from the device */
        ret = read(uvccamera::hid_fd, g_in_packet_buf, BUFFER_LENGTH);
        if (ret < 0) {
            //perror("read");
        } else {
            if (g_in_packet_buf[6]==GET_FAIL) {
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
                g_in_packet_buf[1]==GET_Q_FACTOR_30 &&
                g_in_packet_buf[6]==GET_SUCCESS) {
                    qFactor = g_in_packet_buf[2];
                    emit qFactorChanged(qFactor);
                    timeout = false;
            }
        }
        end = uvc.getTickCount();
        if(end - start > TIMEOUT)
        {
            timeout = false;
            return false;
        }
    }
    return true;
}


/**
 * @brief See3CAM_30::getOrientation - getting flip mode from the camera
 * return true - success /false - failure
 */
bool See3CAM_30::getOrientation()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;

    uint flipMirrorMode;
    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* Report Number */
    g_out_packet_buf[2] = GET_ORIENTATION_30; /* Report Number */

    ret = write(uvccamera::hid_fd , g_out_packet_buf, BUFFER_LENGTH);

    if (ret < 0) {
        perror("write");
        return false;
    }

    /* Read the Status code from the device */
    start = uvc.getTickCount();

    while(timeout)
    {
        /* Get a report from the device */
        ret = read(uvccamera::hid_fd, g_in_packet_buf, BUFFER_LENGTH);
        if (ret < 0) {
            //perror("read");
        } else {
            if (g_in_packet_buf[6]==GET_FAIL) {
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
                g_in_packet_buf[1]==GET_ORIENTATION_30 &&
                g_in_packet_buf[6]==GET_SUCCESS) {
                    flipMirrorMode = g_in_packet_buf[2];
                    emit flipMirrorModeChanged(flipMirrorMode);
                    timeout = false;
            }
        }
        end = uvc.getTickCount();
        if(end - start > TIMEOUT)
        {
            timeout = false;
            return false;
        }
    }
    return true;
}


/**
 * @brief See3CAM_30::setROIAutoFocus - Set ROI auto focus to camera
 * @param camROIAfMode - ROI mode
 * @param vidResolnWidth - preview resolution width
 * @param vidResolnHeight - preview resolution height
 * @param xCord - mouse xCordinate from preview
 * @param yCord - mouse yCordinate from preview
 * @param winSize - ROI window size
 * return true - success /false - failure
 */
bool See3CAM_30::setROIAutoFoucs(camROIAfMode see3camAfROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize)
{
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

    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;

    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* camera control id for camera see3cam_30 */
    g_out_packet_buf[2] = SET_AF_ROI_MODE_30; /* set Auto focus ROI mode command  */
    g_out_packet_buf[3] = see3camAfROIMode; /*  ROI mode which is need to set */

    if(see3camAfROIMode == AFManual){
        g_out_packet_buf[4] = outputXCord;
        g_out_packet_buf[5] = outputYCord;
        g_out_packet_buf[6] = winSize.toUInt();
    }

    ret = write(uvccamera::hid_fd, g_out_packet_buf, BUFFER_LENGTH);

    if (ret < 0) {
        perror("write");
        return false;
    }

    /* Read the Status code from the device */
    start = uvc.getTickCount();


    while(timeout)
    {
        /* Get a report from the device */
        ret = read(uvccamera::hid_fd, g_in_packet_buf, BUFFER_LENGTH);
        if (ret < 0) {
            //perror("read");
        } else {
            if (g_in_packet_buf[6]==SET_FAIL) {
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
                g_in_packet_buf[1]== SET_AF_ROI_MODE_30 &&
                g_in_packet_buf[2]==see3camAfROIMode &&
                g_in_packet_buf[6]==SET_SUCCESS) {
                    timeout=false;
            }
        }
        end = uvc.getTickCount();
        if(end - start > TIMEOUT)
        {
            timeout = false;
            return false;
        }
    }
    return true;
}


/**
 * @brief See3CAM_30::getAutoFocusROIModeAndWindowSize - get ROI auto focus mode and window size
 * return true - success /false - failure
 */
bool See3CAM_30::getAutoFocusROIModeAndWindowSize(){
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;
    uint roiMode, windowSize = 0;

    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* Report Number */
    g_out_packet_buf[2] = GET_AF_ROI_MODE_30; /* Report Number */

    ret = write(uvccamera::hid_fd , g_out_packet_buf, BUFFER_LENGTH);

    if (ret < 0) {
        perror("write");
        return false;
    }

    /* Read the Status code from the device */
    start = uvc.getTickCount();

    while(timeout)
    {
        /* Get a report from the device */
        ret = read(uvccamera::hid_fd, g_in_packet_buf, BUFFER_LENGTH);
        if (ret < 0) {
            //perror("read");
        } else {
            if (g_in_packet_buf[6]==GET_FAIL) {
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
                g_in_packet_buf[1]==GET_AF_ROI_MODE_30 &&
                g_in_packet_buf[6]==GET_SUCCESS) {
                    roiMode = g_in_packet_buf[2];
                    if(roiMode == AFManual){
                        windowSize = g_in_packet_buf[5];
                    }
                    emit roiAfModeChanged(roiMode, windowSize);
                    timeout = false;
            }
        }
        end = uvc.getTickCount();
        if(end - start > TIMEOUT)
        {
            timeout = false;
            return false;
        }
    }
    return true;

}

/**
 * @brief See3CAM_30::setROIAutoExposure - Set ROI auto exposure to camera
 * @param camROIAutoExposureMode - ROI mode
 * @param vidResolnWidth - preview resolution width
 * @param vidResolnHeight - preview resolution height
 * @param xCord - mouse xCordinate from preview
 * @param yCord - mouse yCordinate from preview
 * @param winSize - ROI window size
 * return true - success /false - failure
 */
bool See3CAM_30::setROIAutoExposure(camROIAutoExpMode see3camAutoexpROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize)
{
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

    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;

    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_30;   /*  */
    g_out_packet_buf[2] = SET_EXP_ROI_MODE_30; /* Report Number */
    g_out_packet_buf[3] = see3camAutoexpROIMode;/* Report Number */

    if(see3camAutoexpROIMode == AutoExpManual){
        g_out_packet_buf[4] = outputXCord;
        g_out_packet_buf[5] = outputYCord;
        g_out_packet_buf[6] = winSize.toUInt();
    }

    ret = write(uvccamera::hid_fd, g_out_packet_buf, BUFFER_LENGTH);

    if (ret < 0) {
        perror("write");
        return false;
    }

    /* Read the Status code from the device */
    start = uvc.getTickCount();


    while(timeout)
    {
        /* Get a report from the device */
        ret = read(uvccamera::hid_fd, g_in_packet_buf, BUFFER_LENGTH);
        if (ret < 0) {
            //perror("read");
        } else {
            if (g_in_packet_buf[6]==SET_FAIL) {
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
                g_in_packet_buf[1]== SET_EXP_ROI_MODE_30 &&
                g_in_packet_buf[2]==see3camAutoexpROIMode &&
                g_in_packet_buf[6]==SET_SUCCESS) {
                    timeout=false;
            }
        }
        end = uvc.getTickCount();
        if(end - start > TIMEOUT)
        {
            timeout = false;
            return false;
        }
    }
    return true;
}


/**
 * @brief See3CAM_30::getAutoExpROIModeAndWindowSize - get ROI auto exposure mode and window size
 * return true - success /false - failure
 */
bool See3CAM_30::getAutoExpROIModeAndWindowSize(){
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;
    uint roiMode, windowSize = 0;

    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* Report Number */
    g_out_packet_buf[2] = GET_EXP_ROI_MODE_30; /* Report Number */

    ret = write(uvccamera::hid_fd , g_out_packet_buf, BUFFER_LENGTH);

    if (ret < 0) {
        perror("write");
        return false;
    }

    /* Read the Status code from the device */
    start = uvc.getTickCount();

    while(timeout)
    {
        /* Get a report from the device */
        ret = read(uvccamera::hid_fd, g_in_packet_buf, BUFFER_LENGTH);
        if (ret < 0) {
            //perror("read");
        } else {
            if (g_in_packet_buf[6]==GET_FAIL) {
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
                g_in_packet_buf[1]==GET_EXP_ROI_MODE_30 &&
                g_in_packet_buf[6]==GET_SUCCESS) {
                    roiMode = g_in_packet_buf[2];
                    if(roiMode == AutoExpManual){
                        windowSize = g_in_packet_buf[5];
                    }
                    emit roiAutoExpModeChanged(roiMode, windowSize);
                    timeout = false;
            }
        }
        end = uvc.getTickCount();
        if(end - start > TIMEOUT)
        {
            timeout = false;
            return false;
        }
    }
    return true;
}


/**
 * @brief See3CAM_30::enableDisableAFRectangle - disable RF rectangle before capture image and enable back after capturing image
 * @param enableRFRect - true / false
 * @return true/false
 */
bool See3CAM_30::enableDisableAFRectangle(bool enableRFRect){

    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;
    uint inputRFRectMode = ENABLE_AF_RECTANGLE_30;

    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* camera control id for cam_30 */
    g_out_packet_buf[2] = SET_AF_RECT_MODE_30; /* Set AF Rect mode for cam_30 */
    if(enableRFRect)
        g_out_packet_buf[3] = ENABLE_AF_RECTANGLE_30; /* Enable AF Rectangle */
    else
        g_out_packet_buf[3] = DISABLE_AF_RECTANGLE_30; /* Disable AF Rectangle */

    inputRFRectMode = g_out_packet_buf[3];

    ret = write(uvccamera::hid_fd, g_out_packet_buf, BUFFER_LENGTH);

    if (ret < 0) {
        perror("write");
        return false;
    }

    /* Read the Status code from the device */
    start = uvc.getTickCount();

    while(timeout)
    {
        /* Get a report from the device */
        ret = read(uvccamera::hid_fd, g_in_packet_buf, BUFFER_LENGTH);

        if (ret < 0) {
            //perror("read");
        } else {
            if (g_in_packet_buf[6]==SET_FAIL) {
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
                g_in_packet_buf[1]==SET_AF_RECT_MODE_30 &&
                g_in_packet_buf[2]==inputRFRectMode &&
                g_in_packet_buf[6]==SET_SUCCESS) {
                    timeout=false;
            }
        }
        end = uvc.getTickCount();
        if(end - start > TIMEOUT)
        {
            timeout = false;
            return false;
        }
    }
    return true;
}

/**
 * @brief See3CAM_30::getAFRectMode - get AF rectangle mode[ disable/enable ] from camera
 * return true - success /false - failure
 */
bool See3CAM_30::getAFRectMode()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;

    uint afRectMode;
    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* camera control id - camera 30 */
    g_out_packet_buf[2] = GET_AF_RECT_MODE_30; /* get AF rect mode */

    ret = write(uvccamera::hid_fd , g_out_packet_buf, BUFFER_LENGTH);

    if (ret < 0) {
        perror("write");
        return false;
    }

    /* Read the Status code from the device */
    start = uvc.getTickCount();

    while(timeout)
    {
        /* Get a report from the device */
        ret = read(uvccamera::hid_fd, g_in_packet_buf, BUFFER_LENGTH);
        if (ret < 0) {
            //perror("read");
        } else {
            if (g_in_packet_buf[6]==GET_FAIL) {
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
                g_in_packet_buf[1]==GET_AF_RECT_MODE_30 &&
                g_in_packet_buf[6]==GET_SUCCESS) {
                    afRectMode = g_in_packet_buf[2];
                    emit afRectModeChanged(afRectMode);
                    timeout = false;
            }
        }
        end = uvc.getTickCount();
        if(end - start > TIMEOUT)
        {
            timeout = false;
            return false;
        }
    }
    return true;
}

/**
 * @brief See3CAM_30::setBurstLength - set burst length in camera
 * @param burstLength - burst length - no of images to be taken.
 * return true - success /false - failure
 */
bool See3CAM_30::setBurstLength(uint burstLength){

    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;

    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* camera control id for 30 camera */
    g_out_packet_buf[2] = SET_BURST_LENGTH_30; /* set burst length */
    g_out_packet_buf[3] = burstLength; /* Report Number */
    ret = write(uvccamera::hid_fd, g_out_packet_buf, BUFFER_LENGTH);

    if (ret < 0) {
        perror("write");
        return false;
    }

    /* Read the Status code from the device */
    start = uvc.getTickCount();


    while(timeout)
    {
        /* Get a report from the device */
        ret = read(uvccamera::hid_fd, g_in_packet_buf, BUFFER_LENGTH);
        if (ret < 0) {
            //perror("read");
        } else {
            if (g_in_packet_buf[6]==SET_FAIL) {
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
                g_in_packet_buf[1]==SET_BURST_LENGTH_30 &&
                g_in_packet_buf[2]==burstLength &&
                g_in_packet_buf[6]==SET_SUCCESS) {
                    timeout=false;
            }
        }
        end = uvc.getTickCount();
        if(end - start > TIMEOUT)
        {
            timeout = false;
            return false;
        }
    }
    return true;
}

/**
 * @brief See3CAM_30::getBurstLength - get burst length from camera
 * return true - success /false - failure
 */
bool See3CAM_30::getBurstLength()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;

    uint burstLength = 1;
    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* camera control id for 30 camera */
    g_out_packet_buf[2] = GET_BURST_LENGTH_30; /*  get burst length */

    ret = write(uvccamera::hid_fd , g_out_packet_buf, BUFFER_LENGTH);

    if (ret < 0) {
        perror("write");
        return false;
    }

    /* Read the Status code from the device */
    start = uvc.getTickCount();

    while(timeout)
    {
        /* Get a report from the device */
        ret = read(uvccamera::hid_fd, g_in_packet_buf, BUFFER_LENGTH);
        if (ret < 0) {
            //perror("read");
        } else {
            if (g_in_packet_buf[6]==GET_FAIL) {
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
                g_in_packet_buf[1]==GET_BURST_LENGTH_30 &&
                g_in_packet_buf[6]==GET_SUCCESS) {
                    burstLength = g_in_packet_buf[2];
                    emit burstLengthChanged(burstLength);
                    timeout = false;
            }
        }
        end = uvc.getTickCount();
        if(end - start > TIMEOUT)
        {
            timeout = false;
            return false;
        }
    }
    return true;
}


/**
 * @brief See3CAM_30::setToDefault - set all the values to default in camera
 * @return true/false
 */
bool See3CAM_30::setToDefault(){

    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;

    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* camera control id for 30 camera */
    g_out_packet_buf[2] = SET_TO_DEFAULT_30; /* set to default */

    ret = write(uvccamera::hid_fd, g_out_packet_buf, BUFFER_LENGTH);

    if (ret < 0) {
        perror("write");
        return false;
    }

    /* Read the Status code from the device */
    start = uvc.getTickCount();


    while(timeout)
    {

        /* Get a report from the device */
        ret = read(uvccamera::hid_fd, g_in_packet_buf, BUFFER_LENGTH);

        if (ret < 0) {
            //perror("read");
        } else {
            if (g_in_packet_buf[6]==SET_FAIL) {
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
                g_in_packet_buf[1]==SET_TO_DEFAULT_30 &&
                g_in_packet_buf[6]==SET_SUCCESS) {
                    timeout=false;
            }
        }
        end = uvc.getTickCount();
        if(end - start > TIMEOUT)
        {
            timeout = false;
            return false;
        }
    }
    return true;
}

/**
 * @brief See3CAM_30::setOrientation - Setting orientation
*/
void See3CAM_30::setOrientation(bool horzModeSel, bool vertiModeSel)
{
    QtConcurrent::run(setOrientationBackgrndFn, this, horzModeSel, vertiModeSel);
}


/**
 * @brief See3CAM_30::setOrientationBackgrndFn - Setting orientation
 * @param see3cam30obj object of the class
 * @param horzModeSel
 * @param vertiModeSel
 * @return true/false
 */
bool See3CAM_30::setOrientationBackgrndFn(See3CAM_30 *see3cam30obj, bool horzModeSel, bool vertiModeSel){
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];

    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;

    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* camera control for see3cam_30 camera */
    g_out_packet_buf[2] = SET_ORIENTATION_30; /* set flip mode for 30 camera */
    if(horzModeSel && vertiModeSel){
        g_out_packet_buf[3] = SET_ORIENTATION_BOTHFLIP_ENABLE_30; /* both flip enable */
    }else if(horzModeSel && !vertiModeSel){
        g_out_packet_buf[3] = SET_ORIENTATION_HORZFLIP_30; /* horizontal flip only mode */
    }else if(!horzModeSel && vertiModeSel){
        g_out_packet_buf[3] = SET_ORIENTATION_VERTFLIP_30; /* vertical flip only mode */
    }else
        g_out_packet_buf[3] = SET_ORIENTATION_BOTHFLIP_DISABLE_30; /* both flip disable */

    ret = write(uvccamera::hid_fd, g_out_packet_buf, BUFFER_LENGTH);


    if (ret < 0) {
        perror("write");
        return false;
    }

    /* Read the Status code from the device */
    start = see3cam30obj->uvc.getTickCount();

    while(timeout)
    {
        /* Get a report from the device */
        ret = read(uvccamera::hid_fd, g_in_packet_buf, BUFFER_LENGTH);
        if (ret < 0) {
            //perror("read");
        } else {
            if (g_in_packet_buf[6]==SET_FAIL) {
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
                      g_in_packet_buf[1]==SET_ORIENTATION_30 &&
                      g_in_packet_buf[3]==SET_SUCCESS) {
                timeout = false;
            }
        }
        end = see3cam30obj->uvc.getTickCount();
        if(end - start > TIMEOUT)
        {
            timeout = false;
            return false;
        }
    }
    return true;
}
