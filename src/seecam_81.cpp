/*
 * seecam_81.cpp -- Handling special feature of seecamcu80 camera
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


//Added By Sankari : 17 Dec 2016

#include "seecam_81.h"

See3CAM_81::See3CAM_81(){

}

See3CAM_81::~See3CAM_81(){


}

/**
 * @brief See3CAM_81::setEffectMode - Setting effect mode in the camera.
 * param - enum specialEffect
 * return true - success /false - failure
 */
bool See3CAM_81::setEffectMode(const See3CAM_81::specialEffects &specialEffect)
{
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
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_81; /* Report Number */
    g_out_packet_buf[2] = SETSPECIALEFFECT_81; /* Report Number */
    g_out_packet_buf[3] = specialEffect; /* Report Number */

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
            if (g_in_packet_buf[3]==SET_FAIL) {
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_81 &&
                g_in_packet_buf[1]==SETSPECIALEFFECT_81 &&
                g_in_packet_buf[2]==specialEffect &&
                g_in_packet_buf[3]==SET_SUCCESS) {
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
 * @brief See3CAM_81::getEffectMode - getting effect mode from the camera *
 * return true - success /false - failure
 */
bool See3CAM_81::getEffectMode()
{
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
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
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_81; /* camera id  */
    g_out_packet_buf[2] = GETSPECIALEFFECT_81; /* special effect */

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
            if (g_in_packet_buf[3]==GET_FAIL_CAM81) {
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_81 &&
                g_in_packet_buf[1]==GETSPECIALEFFECT_81 &&
                g_in_packet_buf[3]==GET_SUCCESS_CAM81) {
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
 * @brief See3CAM_81::setFlipMirrorMode - Setting Flip Mirror mode
 * @param flipMirrorMode - mode selected / unselected in UI
 * @return true/false
 */
bool See3CAM_81::setFlipMirrorMode(bool flipMode, bool mirrorMode){
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
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_81; /* Report Number */
    g_out_packet_buf[2] = SETFLIPMIRRORSTATUS_81; /* Report Number */

    if(flipMode && mirrorMode){
        g_out_packet_buf[3] = See3CAM_81::FLIP_ON_MIRROR_ON_81; /* Report Number */
    }else if(!flipMode && mirrorMode){
        g_out_packet_buf[3] = See3CAM_81::FLIP_OFF_MIRROR_ON_81; /* Report Number */
    }else if(flipMode && !mirrorMode){
        g_out_packet_buf[3] = See3CAM_81::FLIP_ON_MIRROR_OFF_81; /* Report Number */
    }else
        g_out_packet_buf[3] = See3CAM_81::FLIP_OFF_MIRROR_OFF_81; /* Report Number */

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
            if (g_in_packet_buf[3]==SET_FAIL) {
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_81 &&
                g_in_packet_buf[1]==SETFLIPMIRRORSTATUS_81 &&
                g_in_packet_buf[3]==SET_SUCCESS) {
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
 * @brief See3CAM_81::getFlipMirrorMode - getting effect mode from the camera *
 * return true - success /false - failure
 */
bool See3CAM_81::getFlipMirrorMode()
{
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];

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
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_81; /* Report Number */
    g_out_packet_buf[2] = GETFLIPMIRRORSTATUS_81; /* Report Number */

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
            if (g_in_packet_buf[3]==GET_FAIL_CAM81) {
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_81 &&
                g_in_packet_buf[1]==GETFLIPMIRRORSTATUS_81 &&
                g_in_packet_buf[3]==GET_SUCCESS_CAM81) {
                    flipMirrorMode = g_in_packet_buf[2];
                    emit flipMirrorModeValue(flipMirrorMode);
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
 * @brief See3CAM_81::setStrobeMode - Setting strobe mode
 * @param strobeMode - mode selected / unselected in UI
 * @return true/false
 */
bool See3CAM_81::setStrobeMode(const See3CAM_81::strobeValues &strobeMode){
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
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_81; /* Report Number */
    g_out_packet_buf[2] = SETSTROBE_STATUS; /* Report Number */
    g_out_packet_buf[3] = strobeMode; /* Report Number */    

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
            if (g_in_packet_buf[3]==SET_FAIL) {
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_81 &&
                g_in_packet_buf[1]==SETSTROBE_STATUS &&
                g_in_packet_buf[3]==SET_SUCCESS) {                
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
 * @brief See3CAM_81::getStrobeMode - getting strobe mode from the camera *
 * return true - success /false - failure
 */
bool See3CAM_81::getStrobeMode()
{
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];

    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;

    uint strobeMode;
    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_81; /* Report Number */
    g_out_packet_buf[2] = GETSTROBE_STATUS; /* Report Number */

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
            if (g_in_packet_buf[3]==GET_FAIL_CAM81) {
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_81 &&
                g_in_packet_buf[1]==GETSTROBE_STATUS &&
                g_in_packet_buf[3]==GET_SUCCESS_CAM81) {
                    strobeMode = g_in_packet_buf[2];
                    emit strobeModeValue(strobeMode);
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
 * @brief See3CAM_81::setFocusMode - call background function to set gpio level
 * @param gpioPin
 * @param gpioValue
 */
void See3CAM_81::setFocusMode(uint focusMode){
    QtConcurrent::run(setFocusModeBackgrndFn, this, focusMode);
}


/**
 * @brief See3CAM_81::setFocusModeBackgrndFn - Setting auto focus mode
 * @param see3cam81obj object of the class
 * @param focusMode
 * @return true/false
 */
bool See3CAM_81::setFocusModeBackgrndFn(See3CAM_81 *see3cam81obj, uint focusMode)
{
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];

    if(uvccamera::hid_fd < 0)
    {        
        emit see3cam81obj->returnFromSetFocusMode(false);
        return false;
    }

    bool timeout = true;
    int ret = 0;
    unsigned int start, end = 0;


    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_81; /* Report Number */
    g_out_packet_buf[2] = SET_FOCUS_MODE; /* Report Number */
    g_out_packet_buf[3] = focusMode; /* Report Number */

    ret = write(uvccamera::hid_fd, g_out_packet_buf, BUFFER_LENGTH);

    if (ret < 0) {
        perror("write");        
        emit see3cam81obj->returnFromSetFocusMode(false);
        return false;
    }

    /* Read the Status code from the device */
    start = see3cam81obj->uvc.getTickCount();
    while(timeout)
    {
        /* Get a report from the device */
        ret = read(uvccamera::hid_fd, g_in_packet_buf, BUFFER_LENGTH);
        if (ret < 0) {
            //perror("read");
        } else {
            if((g_in_packet_buf[0] == CAMERA_CONTROL_81)&&
                    (g_in_packet_buf[1]==SET_FOCUS_MODE)&&
                    (g_in_packet_buf[2]==focusMode) && (g_in_packet_buf[3])==0x01) { // For focus mode, always 3rd buffer will be 0x01
                timeout=false;                                
                emit see3cam81obj->returnFromSetFocusMode(true);
                return false;
            }
        }
        end = see3cam81obj->uvc.getTickCount();
        if(end - start > TIMEOUT)
        {            
            timeout = false;            
            emit see3cam81obj->returnFromSetFocusMode(false);
            return false;
        }
    }    
    emit see3cam81obj->returnFromSetFocusMode(false);
    return true;
}

/**
 * @brief See3CAM_81::getFocusStatus - getting focus status from the camera *
 * return true - success /false - failure
 */
bool See3CAM_81::getFocusStatus()
{    
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
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_81; /* Report Number */
    g_out_packet_buf[2] = GETFOCUS_STATUS; /* Report Number */

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
            if (g_in_packet_buf[3]==GET_FAIL_CAM81) {  		
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_81 &&
                g_in_packet_buf[1]==GETFOCUS_STATUS &&
                g_in_packet_buf[3]==GET_SUCCESS_CAM81) {		    
                    emit focusModeStatus((unsigned int)g_in_packet_buf[2]);
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
 * @brief See3CAM_81::setFocusPosition - set the focus position
 * @param focusPosition
 * @return true/false
 */
bool See3CAM_81::setFocusPosition(int focusPosition)
{
    unsigned char g_out_packet_buf[BUFFER_LENGTH];

    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    int ret =0;

    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_81; /* Report Number */
    g_out_packet_buf[2] = SET_FOCUS_POSITION; /* Report Number */
    g_out_packet_buf[3] = ((focusPosition&0xFF00)>>8); /* MSB of focus postion */
    g_out_packet_buf[4] = (focusPosition&0x00FF); /* LSB of focus postion */
    ret = write(uvccamera::hid_fd, g_out_packet_buf, BUFFER_LENGTH);

    if (ret < 0) {
        perror("write");
        return false;
    }
    return true;
}

/**
 * @brief See3CAM_81::getFocusMode - get focus mode from camera
 * @return true/false
 */
bool See3CAM_81::getFocusMode() {

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
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_81; /* Report Number */
    g_out_packet_buf[2] = GET_FOCUS_MODE; /* Report Number */

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
            if((g_in_packet_buf[0] == CAMERA_CONTROL_81)&&
                    (g_in_packet_buf[1]==GET_FOCUS_MODE)) {		
                emit focusModeValue((unsigned int)g_in_packet_buf[2]);
                timeout=false;
            }
        }
        end = uvc.getTickCount();
        if(end - start > TIMEOUT) {
            timeout = false;
            return false;
        }
    }
    return true;
}

/**
 * @brief See3CAM_81::getFocusPosition
 * @return true/false
 */
bool See3CAM_81::getFocusPosition() {    
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];

    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    quint16 focusPosition;
    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;
    unsigned short int MSB = 0, LSB = 0;
    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_81; /* Report Number */
    g_out_packet_buf[2] = GET_FOCUS_POSITION; /* Report Number */
    ret = write(uvccamera::hid_fd, g_out_packet_buf, BUFFER_LENGTH);

    if (ret < 0) {        
        perror("write");
        return false;
    }
    /* Read the position code from the device */
    start = uvc.getTickCount();
    while(timeout)
    {
        /* Get a report from the device */
        ret = read(uvccamera::hid_fd, g_in_packet_buf, BUFFER_LENGTH);        
        if (ret < 0) {
            //perror("read");
        } else {
            if((g_in_packet_buf[0] == CAMERA_CONTROL_81)&&
                    (g_in_packet_buf[1]==GET_FOCUS_POSITION)) {
                MSB = (g_in_packet_buf[2]);
                LSB = (g_in_packet_buf[3]);                
                focusPosition=LSB;
                focusPosition|=(MSB<<8);                
                emit focusPostionValue(focusPosition);
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
 * @brief See3CAM_81::setToDefault - set the default values in camera`
 * @return true/false
 */
bool See3CAM_81::setToDefault(){

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
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_81; /* Report Number */
    g_out_packet_buf[2] = SET_TO_DEFAULT_CAM81; /* Report Number */

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
            if (g_in_packet_buf[3]==SET_FAIL) {                
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_81 &&
                g_in_packet_buf[1]==SET_TO_DEFAULT_CAM81 &&
                g_in_packet_buf[3]==SET_SUCCESS) {
                timeout=false;
            }
        }
        end = uvc.getTickCount();
        if(end - start > CMD_TIMEOUT_81CAM)
        {            
            timeout = false;
            return false;
        }
    }
    return true;
}

/**
 * @brief See3CAM_81::getOutputGpioLevel - get Output GPIO level
 * @param gpioPinNumber
 */
void See3CAM_81::getOutputGpioLevel(gpioControlsCam81 gpioPinNumber)
{
    QtConcurrent::run(getOutputGpioLevelBackgrndFn, this, gpioPinNumber);
}


bool See3CAM_81::getOutputGpioLevelBackgrndFn(See3CAM_81 *see3cam81obj, gpioControlsCam81 gpioPinNumber){

    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];

    QMutex mutex;
    mutex.lock();
    if(uvccamera::hid_fd < 0)
    {
        mutex.unlock();
        return false;
    }
    bool timeout = true;
    int ret = 0;
    unsigned int start, end = 0;

    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = GPIO_OPERATION; 	/* Report Number */
    g_out_packet_buf[2] = GPIO_GET_LEVEL; 	/* Report Number */
    g_out_packet_buf[3] = gpioPinNumber; 		/* GPIO Pin Number */

    /* Send a Report to the Device */
    ret = write(uvccamera::hid_fd, g_out_packet_buf, BUFFER_LENGTH);
    if (ret < 0) {
        perror("write");
        mutex.unlock();
        return false;
    }
    /* Read the GPIO level and status of read from the device */
    start = see3cam81obj->uvc.getTickCount();
    while(timeout)
    {
        /* Get a report from the device */
        ret = read(uvccamera::hid_fd, g_in_packet_buf, BUFFER_LENGTH);

        if (ret < 0) {
            //perror("read");
        } else {
            if(g_in_packet_buf[0] == GPIO_OPERATION &&
                    g_in_packet_buf[1] == GPIO_GET_LEVEL &&
                    (g_in_packet_buf[2] == CAM81_OUT8_READVAL || g_in_packet_buf[2] == CAM81_OUT9_READVAL)) {                
                emit see3cam81obj->gpioLevel(g_in_packet_buf[3]);
                if(g_in_packet_buf[4] == GPIO_LEVEL_FAIL) {
                    mutex.unlock();
                    return false;
                } else if(g_in_packet_buf[4]==GPIO_LEVEL_SUCCESS) {
                    timeout = false;
                }
            }
        }
        end = see3cam81obj->uvc.getTickCount();
        if(end - start > TIMEOUT)
        {
            timeout = false;
            mutex.unlock();
            return false;
        }
    }
    mutex.unlock();
    return true;

}

/**
 * @brief See3CAM_81::getInputGpioLevel - Get input GPIO level
 * @param gpioPinNumber
 * return true/false
 */
bool See3CAM_81::getInputGpioLevel(gpioControlsCam81 gpioPinNumber)
{
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];

    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    bool timeout = true;
    int ret = 0;
    unsigned int start, end = 0;

    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = GPIO_OPERATION; 	/* Report Number */
    g_out_packet_buf[2] = GET_INPUT_GPIO_LEVEL; 	/* Report Number */
    g_out_packet_buf[3] = gpioPinNumber; 		/* GPIO Pin Number */

    /* Send a Report to the Device */
    ret = write(uvccamera::hid_fd, g_out_packet_buf, BUFFER_LENGTH);
    if (ret < 0) {
        perror("write");
        return false;
    }
    /* Read the GPIO level and status of read from the device */
    start = uvc.getTickCount();
    while(timeout)
    {
        /* Get a report from the device */
        ret = read(uvccamera::hid_fd, g_in_packet_buf, BUFFER_LENGTH);
        if (ret < 0) {
           // perror("read");
        } else {
            if(g_in_packet_buf[0] == GPIO_OPERATION &&
                    g_in_packet_buf[1] == GET_INPUT_GPIO_LEVEL &&
                    (g_in_packet_buf[2] == CAM81_IN3_READVAL || g_in_packet_buf[2] == CAM81_IN6_READVAL)) {                
                emit gpioLevel((unsigned int)g_in_packet_buf[3]);
                if(g_in_packet_buf[4] == GPIO_LEVEL_FAIL) {
                    return true;
                } else if(g_in_packet_buf[4]==GPIO_LEVEL_SUCCESS) {
                    timeout = false;
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
    return true;
}

/**
 * @brief See3CAM_81::setStillSkipCount - based on still resolution, skip count will vary
 * @param stillResoln
 */
void See3CAM_81::setStillSkipCount(QString stillResoln, QString videoResoln){
    uint frameToSkip;    

    if(stillResoln == videoResoln){
         frameToSkip = 5;
    }else{
        if(stillResoln == "640x480"){
            frameToSkip = 20;
        }else if(stillResoln == "1280x720"){
            frameToSkip = 20;
        }else if(stillResoln == "1920x1080"){
            frameToSkip = 25;
        }else if(stillResoln == "3264x2448"){
            frameToSkip = 30;
        }else{
            frameToSkip = 1;
        }
    }    
    // send frame to skip values to qtcam.qml
    emit updateFrameToSkipfromCam81(frameToSkip);
}

/**
 * @brief See3CAM_81::setStillSkipCount - based on fps change for still and preview, skip count will vary
 * @param stillResoln
 */
void See3CAM_81::setStillSkipCountWhenFPSChange(bool fpsChange){
    uint frameToSkip;

    if(fpsChange){
        frameToSkip = 25;
    }else{
        frameToSkip = 5;
    }

    // send frame to skip values to qtcam.qml
    emit updateFrameToSkipfromCam81(frameToSkip);
}

/**
 * @brief See3CAM_81::setGpioLevel - call background function to set gpio level
 * @param gpioPin
 * @param gpioValue
 */
void See3CAM_81::setGpioLevel(gpioControlsCam81 gpioPin,camGpioValue gpioValue){    
    QtConcurrent::run(setGpioLevelBackgrndFn, this, gpioPin, gpioValue);
}


/**
 * @brief See3CAM_81::setGpioLevelBackgrndFn - Setting GPIO level for output pins
 * @param see3cam81obj object of the class
 * @param gpioControlsCam81 gpioPin
 * @param camGpioValue gpioValue
 * @return true/false
 */
bool See3CAM_81::setGpioLevelBackgrndFn(See3CAM_81 *see3cam81obj, gpioControlsCam81 gpioPin, camGpioValue gpioValue)
{
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];

    QMutex mutex;
    mutex.lock();
    if(uvccamera::hid_fd < 0)
    {
        mutex.unlock();
        return false;
    }

    bool timeout = true;
    int ret = 0;
    unsigned int start, end = 0;

    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = GPIO_OPERATION; 	/* Report Number */
    g_out_packet_buf[2] = GPIO_SET_LEVEL; 	/* Report Number */
    g_out_packet_buf[3] = gpioPin; 		/* GPIO Pin Number */
    g_out_packet_buf[4] = gpioValue; 	/* GPIO Value */

    /* Send a Report to the Device */
    ret = write(uvccamera::hid_fd, g_out_packet_buf, BUFFER_LENGTH);
    if (ret < 0) {
        perror("write");
        mutex.unlock();
        return false;
    }
    /* Read the GPIO level and status of read from the device */
    start = see3cam81obj->uvc.getTickCount();
    while(timeout)
    {
        /* Get a report from the device */
        ret = read(uvccamera::hid_fd, g_in_packet_buf, BUFFER_LENGTH);
        if (ret < 0) {
            //perror("read");
        } else {
            if(g_in_packet_buf[0] == GPIO_OPERATION &&
                    g_in_packet_buf[1] == GPIO_SET_LEVEL &&
                    g_in_packet_buf[2] == gpioPin &&
                    g_in_packet_buf[3] == gpioValue) {
                if(g_in_packet_buf[4] == GPIO_LEVEL_FAIL) {
                    mutex.unlock();
                    return false;
                } else if(g_in_packet_buf[4]==GPIO_LEVEL_SUCCESS) {
                    timeout = false;
                }
            }
        }
        end = see3cam81obj->uvc.getTickCount();
        if(end - start > TIMEOUT)
        {            
            timeout = false;
            mutex.unlock();
            return false;
        }
    }
    mutex.unlock();
    return true;
}

/**
 * @brief See3CAM_81::setWbValues - set white balance gain values
 * @param rgbColor - red/blue/green enum value
 * @param rgbValue - value need to set
 * @return
 */
void See3CAM_81::setWbValues(camRGBcolor rgbColor, uint rgbValue){
    QtConcurrent::run(setWbValuesBackgrndFn, this, rgbColor, rgbValue);
}

/**
 * @brief See3CAM_81::setWbValuesBackgrndFn - to run in background[to avoid preview hang]
 * @param see3cam81obj - object of this class
 * @param rgbColor - red/blue/green enum value
 * @param rgbValue - value need to set
 * @return true/false
 */
bool See3CAM_81::setWbValuesBackgrndFn(See3CAM_81 *see3cam81obj, camRGBcolor rgbColor, uint rgbValue)
{
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

    // set command values
    g_out_packet_buf[1] = CAMERA_CONTROL_81; // camera control id
    g_out_packet_buf[2] = rgbColor; /* red / blue / green */    
    g_out_packet_buf[3] = (u_int8_t)((rgbValue & 0xFF00)>> 8);
    g_out_packet_buf[4] = (u_int8_t)((rgbValue & 0x00FF));

    ret = write(uvccamera::hid_fd, g_out_packet_buf, BUFFER_LENGTH);
    if (ret < 0) {
        perror("write");
        return false;
    }
    start = see3cam81obj->uvc.getTickCount();
    while(timeout)
    {
        /* Get a report from the device */
        ret = read(uvccamera::hid_fd, g_in_packet_buf, BUFFER_LENGTH);        
        if (ret < 0) {
            //perror("read");
        } else {
            if(g_in_packet_buf[0] == CAMERA_CONTROL_81  &&
                    g_in_packet_buf[1] == rgbColor){
                if(g_in_packet_buf[4] == GET_FAIL){                    
                    return false;
                }
                else if (g_in_packet_buf[4] == GET_SUCCESS) {                    
                    timeout = false;
                }
            }
        }
        end = see3cam81obj->uvc.getTickCount();
        if(end - start > TIMEOUT)
        {            
            timeout = false;
            return false;
        }
    }
    return true;
}

/**
 * @brief See3CAM_81::getWbValues - get whitebal gain values
 * @param rgbColor - color value - red/blue/green
 * @return
 */
bool See3CAM_81::getWbValues(camRGBcolor rgbColor)
{
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];    

    int whiteBalanceGain;

    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;

    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

    // set command values
    g_out_packet_buf[1] = CAMERA_CONTROL_81;// camera control id

    g_out_packet_buf[2] = rgbColor; /* red / blue / green */

    ret = write(uvccamera::hid_fd, g_out_packet_buf, BUFFER_LENGTH);
    if (ret < 0) {
        perror("write");
        return false;
    }    
    start = uvc.getTickCount();
    while(timeout)
    {
        /* Get a report from the device */
        ret = read(uvccamera::hid_fd, g_in_packet_buf, BUFFER_LENGTH);               
        if (ret < 0) {
            //perror("read");
        } else {
            if(g_in_packet_buf[0] == CAMERA_CONTROL_81  &&
                    g_in_packet_buf[1] == rgbColor){
                if(g_in_packet_buf[4] == GET_FAIL){
                    return false;
                }
                else if (g_in_packet_buf[4] == GET_SUCCESS) {
                    whiteBalanceGain =  (((u_int8_t)g_in_packet_buf[2]) << 8)
                     | (((u_int8_t)g_in_packet_buf[3]) << 0);
                    whiteBalanceGainValue(rgbColor, whiteBalanceGain);
                    timeout = false;
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
    return true;
}


/**
 * @brief See3CAM_81::setROIAutoFocus - Set ROI auto focus to camera
 * @param camROIAfMode - ROI mode
 * @param vidResolnWidth - preview resolution width
 * @param vidResolnHeight - preview resolution height
 * @param xCord - mouse xCordinate from preview
 * @param yCord - mouse yCordinate from preview
 * @param winSize - ROI window size
 * return true - success /false - failure
 */
bool See3CAM_81::setROIAutoFoucs(camROIAfMode see3camAfROIMode, QString winSize)
{
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

    g_out_packet_buf[1] = CAMERA_CONTROL_81; /* camera control id */
    g_out_packet_buf[2] = SET_AF_ROI_MODE_81; /* set roi focus mode command id */
    g_out_packet_buf[3] = see3camAfROIMode; /* af ROI mode */

    if(see3camAfROIMode == AFManual){
        g_out_packet_buf[4] = winSize.toUInt();
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
            if (g_in_packet_buf[4]==SET_FAIL) {
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_81 &&
                g_in_packet_buf[1]== SET_AF_ROI_MODE_81 &&
                g_in_packet_buf[2]==see3camAfROIMode &&
                g_in_packet_buf[4]==SET_SUCCESS) {
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
 * @brief See3CAM_81::setROIAutoFoucsCoordinates
 * @param vidResolnWidth
 * @param vidResolnHeight
 * @param xCord
 * @param yCord
 * @return true/false
 */
bool See3CAM_81::setROIAutoFoucsCoordinates(uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord){
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];

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
    g_out_packet_buf[1] = CAMERA_CONTROL_81; /* camera control id */
    g_out_packet_buf[2] = SET_AF_ROI_MODE_COORDINATES_81; /* set roi focus mode command id */
    g_out_packet_buf[3] = outputXCord; /* x coordinate */
    g_out_packet_buf[4] = outputYCord; /* y coordinate */

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
            if (g_in_packet_buf[4]==SET_FAIL) {
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_81 &&
                g_in_packet_buf[1]== SET_AF_ROI_MODE_COORDINATES_81 &&
                g_in_packet_buf[4]==SET_SUCCESS) {                
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
 * @brief See3CAM_81::getAutoFocusROIModeAndWindowSize - get ROI auto focus mode and window size
 * return true - success /false - failure
 */
bool See3CAM_81::getAutoFocusROIModeAndWindowSize(){
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];

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

    g_out_packet_buf[1] = CAMERA_CONTROL_81;  /* camera control id */
    g_out_packet_buf[2] = GET_AF_ROI_MODE_81; /* 81 cam get auto focus ROI mode */

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
            if (g_in_packet_buf[4]==GET_FAIL) {                
                emit roiAfModeFail(); // For manual focus mode, it will be fail
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_81 &&
                g_in_packet_buf[1]==GET_AF_ROI_MODE_81 &&
                g_in_packet_buf[4]==GET_SUCCESS) {                
                    roiMode = g_in_packet_buf[2];
                    if(roiMode == AFManual){
                        windowSize = g_in_packet_buf[3];
                    }
                    emit roiAfMode(roiMode, windowSize);
                    emit roiAfModeSuccess(); // For auto focus mode/single trigger mode, it will be success
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


