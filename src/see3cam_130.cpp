/*
 * seecam_130.cpp -- Handling special feature of seecam130 camera
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

#include "see3cam_130.h"

See3CAM_130::See3CAM_130(QObject *parent) :
    QObject(parent)
{
}

/**
 * @brief See3CAM_130::getSceneMode - getting scene mode from the camera *
 * return true - success /false - failure
 */
bool See3CAM_130::getSceneMode()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;

    uint sceneMode = 0;
    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = GET_SCENEMODE_130; /* get scene mode command */

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
            if (g_in_packet_buf[6] == GET_FAIL) {
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
                g_in_packet_buf[1]==GET_SCENEMODE_130 &&
                g_in_packet_buf[6]==GET_SUCCESS) {
                    sceneMode = g_in_packet_buf[2];                    
                    emit sceneModeValue(sceneMode);
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
 * @brief See3CAM_130::getEffectMode - getting effect mode from the camera *
 * return true - success /false - failure
 */
bool See3CAM_130::getEffectMode()
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
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = GET_SPECIALEFFECT_130; /* get special effect command */

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
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
                g_in_packet_buf[1]==GET_SPECIALEFFECT_130 &&
                g_in_packet_buf[6]==GET_SUCCESS) {
                    effectMode = g_in_packet_buf[2];
                    emit effectModeValue(effectMode);                    
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
 * @brief See3CAM_130::setSceneMode - Setting scene mode in the camera.
 * @ param - enum scene Mode
 * return true - success /false - failure
 */
bool See3CAM_130::setSceneMode(const See3CAM_130::sceneModes &sceneMode)
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
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = SET_SCENEMODE_130; /* set scene mode command  */
    g_out_packet_buf[3] = sceneMode; /* pass scene mode value */

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
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
                g_in_packet_buf[1]==SET_SCENEMODE_130 &&
                g_in_packet_buf[2]==sceneMode &&
                g_in_packet_buf[6]==SET_SUCCESS) {
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
 * @brief See3CAM_130::setEffectMode - Setting effect mode in the camera.
 * param - enum specialEffect
 * return true - success /false - failure
 */
bool See3CAM_130::setEffectMode(const See3CAM_130::specialEffects &specialEffect)
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
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = SET_SPECIALEFFECT_130; /* set special effect command */
    g_out_packet_buf[3] = specialEffect; /* pass special effect value */

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
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
                g_in_packet_buf[1]==SET_SPECIALEFFECT_130 &&
                g_in_packet_buf[2]==specialEffect &&
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
 * @brief See3CAM_130::setDenoiseValue - setting denoise value to the camera
 * @param deNoiseVal - denoise value
 * return true - success /false - failure
 */
bool See3CAM_130::setDenoiseValue(int deNoiseVal)
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;

    if(DENOISE_MIN <= deNoiseVal || DENOISE_MAX >= deNoiseVal)
    {
        //Initialize the buffer
        memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
        //Set the Report Number

        g_out_packet_buf[1] = CAMERA_CONTROL_130; /* set camera control code */
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
                perror("read");
            } else {
                if (g_in_packet_buf[6]==SET_FAIL) {
                    return false;
                } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
                    g_in_packet_buf[1]==SET_DENOISE_CONTROL &&
                    g_in_packet_buf[2]==deNoiseVal &&
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
    }
    else
    {
        return false;
    }    
    return true;
}


/**
 * @brief See3CAM_130::getDenoiseValue - get denoise value from camera
 * return true - success /false - failure
 */
bool See3CAM_130::getDenoiseValue()
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
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* set camera control code */
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
            perror("read");
        } else {
            if (g_in_packet_buf[6]==GET_FAIL) {
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
                g_in_packet_buf[1]==GET_DENOISE_CONTROL &&
                g_in_packet_buf[6]==GET_SUCCESS) {
                    denoiseValue = g_in_packet_buf[2];
                    emit denoiseValueReceived(denoiseValue);
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
 * @brief See3CAM_130::setAutoFocusMode - set auto focus mode in camera
 * @param afMode - enum Auto focus mode
 * return true - success /false - failure
 */
bool See3CAM_130::setAutoFocusMode(camAfMode afMode){

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
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = SET_AF_MODE_130; /* Set auto focus mode command */
    g_out_packet_buf[3] = afMode; /* pass auto focus mode value[continous, oneshot, disabled] */

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
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
                g_in_packet_buf[1]==SET_AF_MODE_130 &&
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
 * @brief See3CAM_130::getAutoFocusMode - get auto focus mode from camera
 * return true - success /false - failure
 */
bool See3CAM_130::getAutoFocusMode()
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
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = GET_AF_MODE_130; /* get auto focus mode command */

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
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
                g_in_packet_buf[1]==GET_AF_MODE_130 &&
                g_in_packet_buf[6]==GET_SUCCESS) {
                    autoFocusMode = g_in_packet_buf[2];
                    emit afModeValue(autoFocusMode);
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
 * @brief See3CAM_130::setiHDRMode - set iHDR mode in camera
 * @param iHDRMode - HDR mode
 * @param iHDRValue - value of iHDR for manual mode
 * return true - success /false - failure
 */
bool See3CAM_130::setiHDRMode(camiHDRMode iHDRMode, uint iHDRValue){

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
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /*  camera id */
    g_out_packet_buf[2] = SET_HDR_MODE_130; /* set ihdr command  */
    g_out_packet_buf[3] = iHDRMode; /* pass ihdr mode[HdrOff, HdrAuto, HdrManual] */
    if(iHDRMode == HdrManual){
        g_out_packet_buf[4] = iHDRValue; /* ihdr value */
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
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
                g_in_packet_buf[1]==SET_HDR_MODE_130 &&
                g_in_packet_buf[2]==iHDRMode &&
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
 * @brief See3CAM_130::getiHDRMode - get iHDR mode from camera
 * return true - success /false - failure
 */
bool See3CAM_130::getiHDRMode()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;

    uint hdrMode, hdrValue;
    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = GET_HDR_MODE_130; /* get iHDR command */

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
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
                g_in_packet_buf[1]==GET_HDR_MODE_130 &&
                g_in_packet_buf[6]==GET_SUCCESS) {
                    hdrMode = g_in_packet_buf[2];
                    hdrValue = g_in_packet_buf[3];
                    emit hDRModeValueReceived(hdrMode, hdrValue);
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
 * @brief See3CAM_130::setBurstLength - set burst length in camera
 * @param burstLength - burst length - no of images to be taken.
 * return true - success /false - failure
 */
bool See3CAM_130::setBurstLength(uint burstLength){

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
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = SET_BURST_LENGTH_130; /* set burst length command */
    g_out_packet_buf[3] = burstLength; /* burst length value to set */
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
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
                g_in_packet_buf[1]==SET_BURST_LENGTH_130 &&
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
 * @brief See3CAM_130::getBurstLength - get burst length from camera
 * return true - success /false - failure
 */
bool See3CAM_130::getBurstLength()
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
    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = GET_BURST_LENGTH_130; /* get burst length command */

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
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
                g_in_packet_buf[1]==GET_BURST_LENGTH_130 &&
                g_in_packet_buf[6]==GET_SUCCESS) {
                    burstLength = g_in_packet_buf[2];
                    emit burstLengthValue(burstLength);
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
 * @brief See3CAM_130::setQFactor - set QFactor in camera
 * @param qFactor.
 * return true - success /false - failure
 */
bool See3CAM_130::setQFactor(uint qFactor){

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
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = SET_Q_FACTOR_130; /* set qfactor command */
    g_out_packet_buf[3] = qFactor; /* qfactor value */
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
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
                g_in_packet_buf[1]==SET_Q_FACTOR_130 &&
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
 * @brief See3CAM_130::getQFactor - get QFactor from camera
 * return true - success /false - failure
 */
bool See3CAM_130::getQFactor()
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
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = GET_Q_FACTOR_130; /* get qFactor value */

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
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
                g_in_packet_buf[1]==GET_Q_FACTOR_130 &&
                g_in_packet_buf[6]==GET_SUCCESS) {
                    qFactor = g_in_packet_buf[2];                    
                    emit qFactorValue(qFactor);                    
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
 * @brief See3CAM_130::setROIAutoFocus - Set ROI auto focus to camera
 * @param camROIAfMode - ROI mode
 * @param vidResolnWidth - preview resolution width
 * @param vidResolnHeight - preview resolution height
 * @param xCord - mouse xCordinate from preview
 * @param yCord - mouse yCordinate from preview
 * @param winSize - ROI window size
 * return true - success /false - failure
 */
bool See3CAM_130::setROIAutoFoucs(camROIAfMode see3camAfROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize)
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
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = SET_AF_ROI_MODE_130; /* set auto focus ROI mode command */
    g_out_packet_buf[3] = see3camAfROIMode; /* auto focus ROI mode value */

    if(see3camAfROIMode == AFManual){
        g_out_packet_buf[4] = outputXCord; // x cord
        g_out_packet_buf[5] = outputYCord; // y cord
        g_out_packet_buf[6] = winSize.toUInt(); // window size
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
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
                g_in_packet_buf[1]== SET_AF_ROI_MODE_130 &&
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
 * @brief See3CAM_130::getAutoFocusROIModeAndWindowSize - get ROI auto focus mode and window size
 * return true - success /false - failure
 */
bool See3CAM_130::getAutoFocusROIModeAndWindowSize(){
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
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = GET_AF_ROI_MODE_130; /* get auto focus ROI mode */

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
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
                g_in_packet_buf[1]==GET_AF_ROI_MODE_130 &&
                g_in_packet_buf[6]==GET_SUCCESS) {                
                    roiMode = g_in_packet_buf[2];                                        
                    windowSize = g_in_packet_buf[5];
                    emit roiAfModeValue(roiMode, windowSize);
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
 * @brief See3CAM_130::setROIAutoExposure - Set ROI auto exposure to camera
 * @param camROIAutoExposureMode - ROI mode
 * @param vidResolnWidth - preview resolution width
 * @param vidResolnHeight - preview resolution height
 * @param xCord - mouse xCordinate from preview
 * @param yCord - mouse yCordinate from preview
 * @param winSize - ROI window size
 * return true - success /false - failure
 */
bool See3CAM_130::setROIAutoExposure(camROIAutoExpMode see3camAutoexpROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize)
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
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = SET_EXP_ROI_MODE_130; /* set exposure ROI command */
    g_out_packet_buf[3] = see3camAutoexpROIMode; /* exposure ROI mode to set */

    if(see3camAutoexpROIMode == AutoExpManual){
        g_out_packet_buf[4] = outputXCord; // x cord
        g_out_packet_buf[5] = outputYCord; // y cord
        g_out_packet_buf[6] = winSize.toUInt(); // window size
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
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
                g_in_packet_buf[1]== SET_EXP_ROI_MODE_130 &&
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
 * @brief See3CAM_130::getAutoExpROIModeAndWindowSize - get ROI auto exposure mode and window size
 * return true - success /false - failure
 */
bool See3CAM_130::getAutoExpROIModeAndWindowSize(){
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
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = GET_EXP_ROI_MODE_130; /* get exposure ROI mode  */

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
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
                g_in_packet_buf[1]==GET_EXP_ROI_MODE_130 &&
                g_in_packet_buf[6]==GET_SUCCESS) {

                    roiMode = g_in_packet_buf[2];                                        
                    windowSize = g_in_packet_buf[5];                     
                    emit roiAutoExpModeValue(roiMode, windowSize);
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
 * @brief See3CAM_130::enableDisableAFRectangle - disable RF rectangle before capture image and enable back after capturing image
 * @param enableRFRect - true / false
 * @return true/false
 */
bool See3CAM_130::enableDisableAFRectangle(bool enableRFRect){

    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;
    uint inputRFRectMode = ENABLE_AF_RECTANGLE_130;

    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = ENABLE_DISABLE_MODE_AF_RECTANGLE_130; /* enable/disabler AF Rect */
    if(enableRFRect)
        g_out_packet_buf[3] = ENABLE_AF_RECTANGLE_130; /* value to enable AF Rectangle */
    else
        g_out_packet_buf[3] = DISABLE_AF_RECTANGLE_130; /*  value to disable AF Rectangle*/

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
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
                g_in_packet_buf[1]==ENABLE_DISABLE_MODE_AF_RECTANGLE_130 &&
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
 * @brief See3CAM_130::getAFRectMode - get AF rectangle mode[ disable/enable ] from camera
 * return true - success /false - failure
 */
bool See3CAM_130::getAFRectMode()
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
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = GET_AF_RECT_MODE; /* get AF Rect mode command */

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
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
                g_in_packet_buf[1]==GET_AF_RECT_MODE &&
                g_in_packet_buf[6]==GET_SUCCESS) {
                    afRectMode = g_in_packet_buf[2];                    
                    emit afRectModeValue(afRectMode);
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
 * @brief See3CAM_130::setToDefault - set all the values to default in camera
 * @return true/false
 */
bool See3CAM_130::setToDefault(){    

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
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = SET_TO_DEFAULT; /* set to default */

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
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
                g_in_packet_buf[1]==SET_TO_DEFAULT &&
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
 * @brief See3CAM_130::setFlipHorzMode - Setting horizontal flip mode
 * @param horizModeSel - mode selected / unselected in UI
 * @return true/false
 */
bool See3CAM_130::setFlipHorzMode(bool horizModeSel){
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
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = SET_FLIP_MODE_130; /* set flip mode command */
    g_out_packet_buf[3] = SET_HORZ_FLIP_MODE_130; /* set horizontal flip mode */
    if(horizModeSel){
        g_out_packet_buf[4] = FlipEnable; /* value to enable flip */
    }else{
        g_out_packet_buf[4] = FlipDisable; /* value to disable flip */
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
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
                g_in_packet_buf[1]==SET_FLIP_MODE_130 &&
                g_in_packet_buf[2]==SET_HORZ_FLIP_MODE_130 &&
                g_in_packet_buf[6]==SET_SUCCESS) {
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
 * @brief See3CAM_130::setFlipVertMode - Setting vertical flip mode
 * @param vertiModeSel - mode selected / unselected in UI
 * @return true/false
 */
bool See3CAM_130::setFlipVertiMode(bool vertModeSel){
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
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = SET_FLIP_MODE_130; /* Set flip mode command */
    g_out_packet_buf[3] = SET_VERT_FLIP_MODE_130; /* Set vertical flip mode command */
    if(vertModeSel){
        g_out_packet_buf[4] = FlipEnable; /* value to enable flip */
    }else{
        g_out_packet_buf[4] = FlipDisable; /* value to disable flip */
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
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
                g_in_packet_buf[1]==SET_FLIP_MODE_130 &&
                g_in_packet_buf[2]==SET_VERT_FLIP_MODE_130 &&
                g_in_packet_buf[6]==SET_SUCCESS) {
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
 * @brief See3CAM_130::getFlipMode - getting flip mode from the camera
 * return true - success /false - failure
 */
bool See3CAM_130::getFlipMode()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;

    uint flipMode, flipEnableDisableMode;
    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = GET_FLIP_MODE_130; /* get flip mode command */

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
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
                g_in_packet_buf[1]==GET_FLIP_MODE_130 &&
                g_in_packet_buf[6]==GET_SUCCESS) {
                    flipMode = g_in_packet_buf[2];
                    flipEnableDisableMode = g_in_packet_buf[3];
                    emit flipModeValue(flipMode, flipEnableDisableMode);
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
 * @brief See3CAM_130::setStreamMode - Setting  Streaming mode
 * @param streamMode - mode selected in UI
 * @return true/false
 */
bool See3CAM_130::setStreamMode(camStreamMode streamMode){
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
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = SET_STREAM_MODE_130; /* set stream mode command */
    g_out_packet_buf[3] = streamMode; /* stream mode to set */

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
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
                g_in_packet_buf[1]==SET_STREAM_MODE_130 &&
                g_in_packet_buf[2]==streamMode &&
                g_in_packet_buf[6]==SET_SUCCESS) {
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
 * @brief See3CAM_130::getStreamMode - get Stream mode from camera
 * return true - success /false - failure
 */
bool See3CAM_130::getStreamMode()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;

    uint streamMode;
    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = GET_STREAM_MODE_130; /* get stream mode command */

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
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
                g_in_packet_buf[1]==GET_STREAM_MODE_130 &&
                g_in_packet_buf[6]==GET_SUCCESS) {
                    streamMode = g_in_packet_buf[2];
                    emit streamModeValue(streamMode);
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
 * @brief See3CAM_130::setFaceDetectionRect - setting face detection rectangle
 * @param enableFaceDetectRect - enable / disable face detect rectangle
 * @param embedData - Enable / Disable embed data
 * @param overlayRect - Enable / Disable overlay Rectangle
 * @return true/false
 */
bool See3CAM_130::setFaceDetectionRect(bool enableFaceDetectRect, bool embedData, bool overlayRect){
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
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = SET_FACE_DETECT_RECT; /* set face detect Rect command */

    if(enableFaceDetectRect)
        g_out_packet_buf[3] = ENABLE_FACE_RECT; /* enable face Rect */
    else
        g_out_packet_buf[3] = DISABLE_FACE_RECT; /* disable face Rect*/

    if(embedData)
        g_out_packet_buf[4] = ENABLE_EMBED_DATA; /* enable embed data */
    else
        g_out_packet_buf[4] = DISABLE_EMBED_DATA; /* disable embed data */

    if(overlayRect)
        g_out_packet_buf[5] = ENABLE_OVERLAY_RECT; /* enable overlay rect */
    else
        g_out_packet_buf[5] = DISABLE_OVERLAY_RECT; /* disable overlay rect */

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
            if (g_in_packet_buf[6]==FACE_RECT_FAIL) {
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
                g_in_packet_buf[1]==SET_FACE_DETECT_RECT &&
                g_in_packet_buf[6]==FACE_RECT_SUCCESS) {
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
 * @brief See3CAM_130::getFactDetectMode - get face detect mode[ disable/enable ] from camera
 * return true - success /false - failure
 */
bool See3CAM_130::getFaceDetectMode()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;

    uint faceDetectMode, faceDetectEmbedDataValue, faceDetectOverlayRect;
    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = GET_FACE_DETECT_RECT; /* get face detect mode command */

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
            if (g_in_packet_buf[6]==FACE_RECT_FAIL) {
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
                g_in_packet_buf[1]==GET_FACE_DETECT_RECT &&
                g_in_packet_buf[6]==FACE_RECT_SUCCESS) {
                    faceDetectMode = g_in_packet_buf[2];
                    faceDetectEmbedDataValue = g_in_packet_buf[3];
                    faceDetectOverlayRect = g_in_packet_buf[4];
                    emit faceDetectModeValue(faceDetectMode, faceDetectEmbedDataValue, faceDetectOverlayRect);
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
 * @brief See3CAM_130::setSmileDetection - setting smile detection rectangle
 * @param enableSmileDetect - enable / disable smile detect
 * @param embedData - Enable / Disable embed data
 * @param thresholdValue - smile threshold value
 * @return true/false
 */
bool See3CAM_130::setSmileDetection(bool enableSmileDetect, bool embedData, uint thresholdValue){    
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    if((SMILE_THRESHOLD_MIN > thresholdValue || SMILE_THRESHOLD_MAX < thresholdValue) && enableSmileDetect){
        emit indicateSmileThresholdRangeFailure("Failure", "Given smile detection threshold value is invalid.");
        return false;
    }

    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;

    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = SET_SMILE_DETECTION; /* set smile detect command */

    if(enableSmileDetect)
        g_out_packet_buf[3] = ENABLE_SMILE_DETECT; /* enable smile detect */
    else
        g_out_packet_buf[3] = DISABLE_SMILE_DETECT; /* disable smile detect */

    g_out_packet_buf[4] = thresholdValue; // Setting threshold value

    if(embedData)
        g_out_packet_buf[5] = ENABLE_EMBED_DATA; /* enable embed data */
    else
        g_out_packet_buf[5] = DISABLE_EMBED_DATA; /* disable embed data */   


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
            if (g_in_packet_buf[6]==SMILE_DETECT_FAIL) {                
                emit indicateCommandStatus("Failure", "Failed to set smile detection threshold");
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
                g_in_packet_buf[1]==SET_SMILE_DETECTION &&
                g_in_packet_buf[6]==SMILE_DETECT_SUCCESS) {
                emit indicateCommandStatus("Success", "Smile detection threshold is set successfully");
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
 * @brief See3CAM_130::getSmileDetectMode - get smile detect mode[ disable/enable ] from camera
 * return true - success /false - failure
 */
bool See3CAM_130::getSmileDetectMode()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;

    uint smileDetectMode, smileDetectThresholdValue, smileDetectEmbedDataValue;

    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* Camera control id */
    g_out_packet_buf[2] = GET_SMILE_DETECTION; /* Get smile detection */

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
            if (g_in_packet_buf[6]==SMILE_DETECT_FAIL) {
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
                g_in_packet_buf[1]==GET_SMILE_DETECTION &&
                g_in_packet_buf[6]==SMILE_DETECT_SUCCESS) {
                    smileDetectMode = g_in_packet_buf[2];
                    smileDetectThresholdValue = g_in_packet_buf[3];
                    smileDetectEmbedDataValue = g_in_packet_buf[4];
                    emit smileDetectModeValue(smileDetectMode, smileDetectThresholdValue, smileDetectEmbedDataValue);
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
 * @brief See3CAM_130::setExposureCompensation - setting exposure compensation
 * @param exposureCompValue - exposure compensation value
 * @return true/false
 */
bool See3CAM_130::setExposureCompensation(unsigned int exposureCompValue){
    if(EXPOSURECOMP_MIN > exposureCompValue || EXPOSURECOMP_MAX < exposureCompValue){
        emit indicateExposureValueRangeFailure("Failure", "Given exposure compensation value is invalid");
        return false;
    }
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
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = SET_EXPOSURE_COMPENSATION; /* set exposure compensation command */
    g_out_packet_buf[3] = (u_int8_t)((exposureCompValue & 0xFF000000) >> 24);
    g_out_packet_buf[4] = (u_int8_t)((exposureCompValue & 0x00FF0000) >> 16);
    g_out_packet_buf[5] = (u_int8_t)((exposureCompValue & 0x0000FF00) >> 8);
    g_out_packet_buf[6] = (u_int8_t)((exposureCompValue & 0x000000FF) >> 0);

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
            if (g_in_packet_buf[6]==EXPOSURE_COMP_CMD_FAIL) {
                emit indicateCommandStatus("Failure", "Failed to set exposure compensation value");
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
                g_in_packet_buf[1]==SET_EXPOSURE_COMPENSATION &&
                g_in_packet_buf[6]==EXPOSURE_COMP_CMD_SUCCESS) {
                emit indicateCommandStatus("Success", "Exposure compensation value is set successfully");
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
 * @brief See3CAM_130::getExposureCompensation - getting exposure compensation
 * @return true/false
 */
bool See3CAM_130::getExposureCompensation(){
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0, exposureCompValue;

    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = GET_EXPOSURE_COMPENSATION; /* get exposure compensation command */

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
            if (g_in_packet_buf[6]==EXPOSURE_COMP_CMD_FAIL) {
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
                g_in_packet_buf[1]==GET_EXPOSURE_COMPENSATION &&
                g_in_packet_buf[6]==EXPOSURE_COMP_CMD_SUCCESS) {
                exposureCompValue = (((u_int8_t)g_in_packet_buf[2]) << 24)
                                    | (((u_int8_t)g_in_packet_buf[3]) << 16)
                                    | (((u_int8_t)g_in_packet_buf[4]) << 8)
                                    | (((u_int8_t)g_in_packet_buf[5]) << 0);
                exposureCompValueReceived(exposureCompValue);
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
 * @brief See3CAM_130::setFrameRatectrlValue - setting frame rate control value
 * @param frameRate
 * @return true/false
 */
bool See3CAM_130::setFrameRateCtrlValue(uint frameRate)
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

    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* set camera control code */
    g_out_packet_buf[2] = SET_FRAME_RATE_130; /* set framerate control code */
    g_out_packet_buf[3] = frameRate; /* set framerate value */

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
            if (g_in_packet_buf[6]==FRAMERATE_CMD_FAIL) {                
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
                g_in_packet_buf[1]==SET_FRAME_RATE_130 &&
                g_in_packet_buf[2]==frameRate &&
                g_in_packet_buf[6]==FRAMERATE_CMD_SUCCESS) {                  
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
 * @brief See3CAM_130::getFrameRateCtrlValue - get frameRate control value from camera
 * return true - success /false - failure
 */
bool See3CAM_130::getFrameRateCtrlValue()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;

    uint frameRateCtrlValue;
    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* set camera control code */
    g_out_packet_buf[2] = GET_FRAME_RATE_130; /* get frame rate code */

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
            if (g_in_packet_buf[6]==FRAMERATE_CMD_FAIL) {                
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
                g_in_packet_buf[1]==GET_FRAME_RATE_130 &&
                g_in_packet_buf[6]==FRAMERATE_CMD_SUCCESS) {                
                    frameRateCtrlValue = g_in_packet_buf[2];
                    emit frameRateCtrlValueReceived(frameRateCtrlValue);                    
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
 * @brief See3CAM_130::enableDisableFaceRectangle - disable RF rectangle before capture image and enable back after capturing image
 * and disable before recording video and enable back.
 * @param enableRFRect - true / false
 * @return true/false
 */
bool See3CAM_130::enableDisableFaceRectangle(bool enableFaceRect){

    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;
    uint inputFaceRectMode = ENABLE_FACE_RECTANGLE_130;

    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = ENABLE_DISABLE_MODE_FACE_RECTANGLE_130; /* pass enable/disable command */
    if(enableFaceRect)
        g_out_packet_buf[3] = ENABLE_FACE_RECTANGLE_130; /* enable auto focus rect */
    else
        g_out_packet_buf[3] = DISABLE_FACE_RECTANGLE_130; /* disable auto focus rect */

    inputFaceRectMode = g_out_packet_buf[3];

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
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
                g_in_packet_buf[1]==ENABLE_DISABLE_MODE_FACE_RECTANGLE_130 &&
                g_in_packet_buf[2]==inputFaceRectMode &&
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
