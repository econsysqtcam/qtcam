#include "imx900usbcam.h"

IMX900USBCAM::IMX900USBCAM(QObject *parent) : QObject(parent)
{

}


/**
 * @brief IMX900USBCAM::getCameraMode - get current camera mode from the camera
 * @return - true/false
 */
bool IMX900USBCAM::getCameraMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = GET_CAMERA_MODE_IMX900;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            emit indicateFailureStatus("Failure","Get Camera Mode Failed. Retry!");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
            g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
            g_in_packet_buf[2] == GET_CAMERA_MODE_IMX900 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit currentCameraMode(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}


/**
 * @brief IMX900USBCAM::setCameraMode - setting camera mode to the camera
 * @param cameraMode - mode to set [value of enum  - Master/Trigger-Exposure/Trigger-Acquisition/Trigger-Software control]
 * @return true/false
 */
bool IMX900USBCAM::setCameraMode(CAMERA_MODE cameraMode){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = SET_CAMERA_MODE_IMX900;
    g_out_packet_buf[4] = cameraMode;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            emit indicateFailureStatus("Failure","Set Camera Mode Failed. Retry!");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
            g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
            g_in_packet_buf[2] == SET_CAMERA_MODE_IMX900 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}



/*
 * @brief IMX900USBCAM::getOrientation - getting flip mode from the camera
 * return true - success /false - failure
*/
bool IMX900USBCAM::getOrientation()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900; /* camera id_1 */
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900; /* camera id_2 */
    g_out_packet_buf[3] = GET_ORIENTATION_MODE_IMX900; /* get orientation command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            emit indicateFailureStatus("Failure","Get Orientation Failed. Retry!");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
            g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
            g_in_packet_buf[2] == GET_ORIENTATION_MODE_IMX900 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit currentFlipMode(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}



/*
 * @brief IMX900USBCAM::setOrientation - Setting orientation - set Normal/horizontal/vertical/Rotate180
 * @param - horizontal flip selection
 * @param - vertical flip selection
 * return true/false
*/
bool IMX900USBCAM::setOrientation(bool horzModeSel, bool vertiModeSel)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900; /* camera id_1 */
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900; /* camera id_2 */
    g_out_packet_buf[3] = SET_ORIENTATION_MODE_IMX900; /* set orientation command  */

    if(horzModeSel && vertiModeSel){
        g_out_packet_buf[4] = ROTATE_180; /* both flip enable */
    }else if(horzModeSel && !vertiModeSel){
        g_out_packet_buf[4] = HORIZONTAL; /* horizontal flip only mode */
    }else if(!horzModeSel && vertiModeSel){
        g_out_packet_buf[4] = VERTICAL; /* vertical flip only mode */
    }else
        g_out_packet_buf[4] = NORMAL; /* both flip disable */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            emit indicateFailureStatus("Failure","Set Orientation Failed. Retry!");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
            g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
            g_in_packet_buf[2] == SET_ORIENTATION_MODE_IMX900 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}



/**
 * @brief IMX900USBCAM::getBlackLevelAdjustment - get black level adjustment from the camera
 * return true - success /false - failure
 */
bool IMX900USBCAM::getBlackLevelAdjustment()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    uint blackLevelValue;

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = GET_BLACK_LEVEL_IMX900;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
                  g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
                  g_in_packet_buf[2] == GET_BLACK_LEVEL_IMX900 &&
                  g_in_packet_buf[6] == GET_SUCCESS) {

            blackLevelValue = (g_in_packet_buf[3] << 8) | (g_in_packet_buf[4] << 0);
            emit currentBlackLevelValue(blackLevelValue);
            return true;
        }
    }
    return false;
}


/**
 * @brief IMX900USBCAM::setBlackLevelAdjustment - set black level value in camera
 * @param blacklevel - Black Level value to be set
 * return true - success /false - failure
 */
bool IMX900USBCAM::setBlackLevelAdjustment(uint blackLevelValue)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = SET_BLACK_LEVEL_IMX900;
    g_out_packet_buf[4] = ((blackLevelValue & 0xFF00) >> 8);
    g_out_packet_buf[5] = ((blackLevelValue & 0x00FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
                  g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
                  g_in_packet_buf[2] == SET_BLACK_LEVEL_IMX900 &&
                  g_in_packet_buf[6] == GET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief IMX900USBCAM::getStrobeMode - To get status of strobe mode from the camera
 * return true - success /false - failure
 */
bool IMX900USBCAM::getStrobeMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = GET_STROBE_MODE_IMX900;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
            g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
            g_in_packet_buf[2] == GET_STROBE_MODE_IMX900 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit currentStrobeMode(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}


/*
 * @brief IMX900USBCAM::setStrobeMode - Setting Strobe mode to the camera
 * @param - strobe - To switch between OFF and ON Modes
 * return true/false
*/
bool IMX900USBCAM::setStrobeMode(STROBE_MODE strobe)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = SET_STROBE_MODE_IMX900;
    g_out_packet_buf[4] = strobe;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
                  g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
                  g_in_packet_buf[2] == SET_STROBE_MODE_IMX900 &&
                  g_in_packet_buf[6] == GET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief IMX900USBCAM::getBurstLength - To get burst length from camera
 * return true - success /false - failure
 */
bool IMX900USBCAM::getBurstLength()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = GET_IMAGE_BURST_IMX900; /* get burst length mode  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
                  g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
                  g_in_packet_buf[2] == GET_IMAGE_BURST_IMX900 &&
                  g_in_packet_buf[6] == GET_SUCCESS) {
            emit currentBurstLength(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}


/**
 * @brief IMX900USBCAM::setBurstLength - set burst length in camera
 * @param burstLength - burst length - no of images to be taken.
 * return true - success /false - failure
 */
bool IMX900USBCAM::setBurstLength(uint burstLength){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = SET_IMAGE_BURST_IMX900; /* set burst length command */
    g_out_packet_buf[4] = burstLength; /* burst length value to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
                  g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
                  g_in_packet_buf[2] == SET_IMAGE_BURST_IMX900 &&
                  g_in_packet_buf[6] == GET_SUCCESS) {
            return true;
        }
    }
    return false;
}



/**
 * @brief IMX900USBCAM::getGainMode - Get gain mode(Auto / Manual)
 * @return true/false
 */
bool IMX900USBCAM::getGainMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    uint min, max, current;

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = GET_GAIN_IMX900;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[12] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
            g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
            g_in_packet_buf[2] == GET_GAIN_IMX900 &&
            g_in_packet_buf[12] == SET_SUCCESS) {

            min     = (g_in_packet_buf[7] << 8) | (g_in_packet_buf[8] << 0);
            max     = (g_in_packet_buf[9] << 8) | (g_in_packet_buf[10] << 0);
            current = (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);

            emit currentGainProperties(g_in_packet_buf[3], g_in_packet_buf[4], min, max, current, g_in_packet_buf[11]);

            return true;
        }
    }
    return false;
}



/**
 * @brief IMX900USBCAM::setGainMode - Setting gain attributes
 * @param gainType   - Auto / Manual mode
 * @param autoModes  - Continous / Single shot in Auto gain mode
 * @param manualGain - Manual values set in UI
 * return true/false
*/
bool IMX900USBCAM::setGainMode(GAIN_MODE gainType, AUTO_GAIN_FEATURES autoModes, uint manualGain)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = SET_GAIN_IMX900;
    g_out_packet_buf[4] = gainType;
    if(g_out_packet_buf[4] == 0x00)
    {
        g_out_packet_buf[5] = autoModes;
    }
    else if(g_out_packet_buf[4] == 0x01)
    {
        g_out_packet_buf[6] = ((manualGain & 0xFF00) >> 8);
        g_out_packet_buf[7] = ((manualGain & 0x00FF) >> 0);
    }

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[12] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
            g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
            g_in_packet_buf[2] == SET_GAIN_IMX900 &&
            g_in_packet_buf[12] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief IMX900USBCAM::getExposureMode - Get Exposure mode(Auto / Manual)
 * @return true/false
 */
bool IMX900USBCAM::getExposureMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    uint milliSeconds, microSeconds;

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = GET_EXPOSURE_IMX900;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[10] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
            g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
            g_in_packet_buf[2] == GET_EXPOSURE_IMX900 &&
            g_in_packet_buf[10] == GET_SUCCESS) {

            milliSeconds  = (g_in_packet_buf[6] << 8) | (g_in_packet_buf[7] << 0);
            microSeconds  = (g_in_packet_buf[8] << 8) | (g_in_packet_buf[9] << 0);

            emit currentExposureProperties(g_in_packet_buf[3], g_in_packet_buf[4], g_in_packet_buf[5], milliSeconds, microSeconds);

            return true;
        }
    }
    return false;
}


/**
 * @brief IMX900USBCAM::setExposureMode - Setting Exposure attributes
 * @param mode         - Auto / Manual mode
 * @param autoFeature  - Continous / Single shot / Single Shot Exposure Trigger in Auto gain mode
 * @param manualGain   - Manual values set in UI
 * return true/false
*/
bool IMX900USBCAM::setExposureMode(EXPOSURE_MODE mode, AUTO_EXPOSURE_FEATURES autoFeature, uint seconds, uint milliSeconds, uint microSeconds)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = SET_EXPOSURE_IMX900;
    g_out_packet_buf[4] = mode;
    if(g_out_packet_buf[4] == 0x00)
    {
        g_out_packet_buf[5] = autoFeature;
    }
    else if(g_out_packet_buf[4] == 0x01)
    {
        g_out_packet_buf[6] = seconds;

        g_out_packet_buf[7] = ((milliSeconds & 0xFF00) >> 8);
        g_out_packet_buf[8] = ((milliSeconds & 0x00FF) >> 0);

        g_out_packet_buf[9]  = ((microSeconds & 0xFF00) >> 8);
        g_out_packet_buf[10] = ((microSeconds & 0x00FF) >> 0);
    }

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[10] == SET_FAIL) {
            emit indicateCommandStatus("Failure", "Given exposure value is invalid");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
            g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
            g_in_packet_buf[2] == SET_EXPOSURE_IMX900 &&
            g_in_packet_buf[10] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief IMX900USBCAM::getTargetBrightness - get brightness from the camera
 * return true - success /false - failure
 */
bool IMX900USBCAM::getTargetBrightness()
{
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

    uint min, max, current;

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
   g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
   g_out_packet_buf[3] = GET_TARGET_BRIGHTNESS_IMX900;

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[10] == GET_FAIL) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
                 g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
                 g_in_packet_buf[2] == GET_TARGET_BRIGHTNESS_IMX900  &&
                 g_in_packet_buf[10] == GET_SUCCESS) {

           current   = (g_in_packet_buf[3] << 8) | (g_in_packet_buf[4] << 0);
           min       = (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);
           max       = (g_in_packet_buf[7] << 8) | (g_in_packet_buf[8] << 0);

           emit currentTargetBrightness(min, max, g_in_packet_buf[9], current);

           return true;
       }
   }
   return false;
}



/**
 * @brief IMX900USBCAM::setTargetBrightness - set brightness value in camera
 * @param targetBrightness - set brightness value to the camera
 * return true - success /false - failure
 */
bool IMX900USBCAM::setTargetBrightness(uint targetBrightness)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = SET_TARGET_BRIGHTNESS_IMX900;
    g_out_packet_buf[4] = ((targetBrightness & 0xFF00) >> 8);
    g_out_packet_buf[5] = ((targetBrightness & 0x00FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[10] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
                  g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
                  g_in_packet_buf[2] == SET_TARGET_BRIGHTNESS_IMX900  &&
                  g_in_packet_buf[10] == SET_SUCCESS) {
            return true;
        }
    }

    return false;
}



/**
 * @brief IMX900USBCAM::getGainLimit - To get gain upper and lower limit value in camera
 * return true - success /false - failure
 */
bool IMX900USBCAM::getGainLimit()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = GET_AUTO_GAIN_LIMIT_IMX900;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[12] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
                  g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
                  g_in_packet_buf[2] == GET_AUTO_GAIN_LIMIT_IMX900  &&
                  g_in_packet_buf[12] == GET_SUCCESS) {

            uint lowerLimit, upperLimit;
            uint min, max;

            min = (g_in_packet_buf[3] << 8) | (g_in_packet_buf[4] << 0);
            max = (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);

            lowerLimit = (g_in_packet_buf[7] << 8) | (g_in_packet_buf[8] << 0);
            upperLimit = (g_in_packet_buf[9] << 8) | (g_in_packet_buf[10] << 0);

            emit currentGainLimit(min, max, g_in_packet_buf[11], lowerLimit, upperLimit);

            return true;
        }
    }
    return false;
}

/**
 * @brief IMX900USBCAM::setGainLimit - set gain limit value in camera
 * @param lowerLimit - set lower limit for gain
 * @param upperLimit - set upper limit for gain
 * return true - success /false - failure
 */
bool IMX900USBCAM::setGainLimit(uint lowerLimit, uint upperLimit)
{
    if(lowerLimit == upperLimit)
    {

    }
    else if(lowerLimit > upperLimit)
    {
        emit indicateGainValueRangeFailure();
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
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = SET_AUTO_GAIN_LIMIT_IMX900;
    g_out_packet_buf[8] = ((lowerLimit & 0xFF00) >> 8);
    g_out_packet_buf[9] = ((lowerLimit & 0x00FF) >> 0);
    g_out_packet_buf[10] = ((upperLimit & 0xFF00) >> 8);
    g_out_packet_buf[11] = ((upperLimit & 0x00FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[12] == SET_FAIL) {
            emit indicateCommandStatus("Failure","Limit value is failed to set");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
                  g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
                  g_in_packet_buf[2] == SET_AUTO_GAIN_LIMIT_IMX900  &&
                  g_in_packet_buf[12] == GET_SUCCESS) {

            return true;
        }
    }
    return false;
}



/**
 * @brief IMX900USBCAM::getAutoExposureLowerLimit - To get current lower limit exposure from camera
 * @return - true/false
 */
bool IMX900USBCAM::getAutoExposureLowerLimit()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    uint milliSeconds, microSeconds;

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = GET_EXPOSURE_LOWER_LIMIT_IMX900;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[8] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
                  g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
                  g_in_packet_buf[2] == GET_EXPOSURE_LOWER_LIMIT_IMX900  &&
                  g_in_packet_buf[8] == GET_SUCCESS) {

            milliSeconds = (g_in_packet_buf[4] << 8) | (g_in_packet_buf[5] << 0);
            microSeconds = (g_in_packet_buf[6] << 8) | (g_in_packet_buf[7] << 0);

            emit currentExposureLowerLimit(g_in_packet_buf[3], milliSeconds, microSeconds);

            lowerLimitSec      = g_in_packet_buf[3];
            lowerLimitMilliSec = milliSeconds;
            lowerLimitMicroSec = microSeconds;

            return true;
        }
    }
    return false;
}

/**
 * @brief IMX900USBCAM::setAutoExposureLowerLimit - setting lower limit for exposure
 * @param lowerLimit - mode to set lower limit for exposure
 * @return true/false
 */
bool IMX900USBCAM::setAutoExposureLowerLimit(uint seconds, uint milliSeconds, uint microSeconds){

    lowerLimitSec      = seconds;
    lowerLimitMilliSec = milliSeconds;
    lowerLimitMicroSec = microSeconds;

    if(lowerLimitSec > upperLimitSec)
    {
        emit indicateExposureLimitsInvalidInput("Invalid Input", "Lower auto exposure limit <= Upper auto exposure limit");
        return false;
    }
    else if((lowerLimitSec == upperLimitSec) && (lowerLimitMilliSec > upperLimitMilliSec))
    {
        emit indicateExposureLimitsInvalidInput("Invalid Input", "Lower auto exposure limit <= Upper auto exposure limit");
        return false;
    }
    else if (((lowerLimitSec == upperLimitSec) && (lowerLimitMilliSec == upperLimitMilliSec)) && (lowerLimitMicroSec > upperLimitMicroSec))
    {
        emit indicateExposureLimitsInvalidInput("Invalid Input", "Lower auto exposure limit <= Upper auto exposure limit");
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
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = SET_EXPOSURE_LOWER_LIMIT_IMX900;
    g_out_packet_buf[4] = seconds;
    g_out_packet_buf[5] = ((milliSeconds & 0xFF00) >> 8);
    g_out_packet_buf[6] = ((milliSeconds & 0x00FF) >> 0);
    g_out_packet_buf[7] = ((microSeconds & 0xFF00) >> 8);
    g_out_packet_buf[8] = ((microSeconds & 0x00FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            emit indicateExposureValueRangeFailure("Failure","Set lower limit value failed");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
                  g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
                  g_in_packet_buf[2] == SET_EXPOSURE_LOWER_LIMIT_IMX900  &&
                  g_in_packet_buf[9] == GET_SUCCESS) {
            emit indicateCommandStatus("Success","Lower limit value set successfully");
            return true;
        }
    }
    return false;
}

/**
 * @brief IMX900USBCAM::getAutoExposureUpperLimit  - get current upper limit exposure from camera
 * @return - true/false
 */
bool IMX900USBCAM::getAutoExposureUpperLimit()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    uint milliSeconds, microSeconds;

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = GET_EXPOSURE_UPPER_LIMIT_IMX900; /* get AE_UPPER_LIMIT command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[8] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
                  g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
                  g_in_packet_buf[2] == GET_EXPOSURE_UPPER_LIMIT_IMX900  &&
                  g_in_packet_buf[8] == GET_SUCCESS) {

            milliSeconds = (g_in_packet_buf[4] << 8) | (g_in_packet_buf[5] << 0);
            microSeconds = (g_in_packet_buf[6] << 8) | (g_in_packet_buf[7] << 0);

            emit currentExposureUpperLimit(g_in_packet_buf[3], milliSeconds, microSeconds);

            upperLimitSec      = g_in_packet_buf[3];
            upperLimitMilliSec = milliSeconds;
            upperLimitMicroSec = microSeconds;

            return true;
        }
    }
    return false;
}

/**
 * @brief IMX900USBCAM::setAutoExposureUpperLimit - setting upper limit for exposure
 * @param upperLimit - mode to set upper limit for exposure
 * @return true/false
 */
bool IMX900USBCAM::setAutoExposureUpperLimit(uint seconds, uint milliSeconds, uint microSeconds){

    upperLimitSec      = seconds;
    upperLimitMilliSec = milliSeconds;
    upperLimitMicroSec = microSeconds;

    if(lowerLimitSec > upperLimitSec)
    {
        emit indicateExposureLimitsInvalidInput("Invalid Input", "Lower auto exposure limit <= Upper auto exposure limit");
        return false;
    }
    else if((lowerLimitSec == upperLimitSec) && (lowerLimitMilliSec > upperLimitMilliSec))
    {
        emit indicateExposureLimitsInvalidInput("Invalid Input", "Lower auto exposure limit <= Upper auto exposure limit");
        return false;
    }
    else if (((lowerLimitSec == upperLimitSec) && (lowerLimitMilliSec == upperLimitMilliSec)) && (lowerLimitMicroSec > upperLimitMicroSec))
    {
        emit indicateExposureLimitsInvalidInput("Invalid Input", "Lower auto exposure limit <= Upper auto exposure limit");
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
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = SET_EXPOSURE_UPPER_LIMIT_IMX900;
    g_out_packet_buf[4] = seconds;
    g_out_packet_buf[5] = ((milliSeconds & 0xFF00) >> 8);
    g_out_packet_buf[6] = ((milliSeconds & 0x00FF) >> 0);
    g_out_packet_buf[7] = ((microSeconds & 0xFF00) >> 8);
    g_out_packet_buf[8] = ((microSeconds & 0x00FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            emit indicateCommandStatus("Failure","Set upper limit value failed");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
                  g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
                  g_in_packet_buf[2] == SET_EXPOSURE_UPPER_LIMIT_IMX900  &&
                  g_in_packet_buf[9] == GET_SUCCESS) {
            emit indicateCommandStatus("Success","Upper limit value set successfully");
            return true;
        }
    }
    return false;
}



/**
 * @brief IMX900USBCAM::getAntiFlickerMode - get current anti flicker mode from camera
 * @return - true/false
 */
bool IMX900USBCAM::getAntiFlickerMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = GET_ANTI_FLICKER_DETECTION_IMX900;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
                  g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
                  g_in_packet_buf[2] == GET_ANTI_FLICKER_DETECTION_IMX900  &&
                  g_in_packet_buf[6] == GET_SUCCESS) {
            emit currentAntiFlickerMode(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}

/**
 * @brief IMX900USBCAM::setAntiFlickerMode - setting anti flicker mode
 * @param antiFlickerMode - mode to set [value of enum  - auto/50hz/60hz]
 * @return true/false
 */
bool IMX900USBCAM::setAntiFlickerMode(ANTI_FLICKER_DETECTION antiFlickerMode){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = SET_ANTI_FLICKER_DETECTION_IMX900;
    g_out_packet_buf[4] = antiFlickerMode;
    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
                  g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
                  g_in_packet_buf[2] == SET_ANTI_FLICKER_DETECTION_IMX900  &&
                  g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}



/**
 * @brief IMX900USBCAM::readStatistics - To retrieve the current exposure, gain values during its auto mode from the camera.
 * @return True if successful, false otherwise.
 */
bool IMX900USBCAM::readStatistics(){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();


    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = READ_STATISTICS_IMX900;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[10] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
            g_in_packet_buf[1]  == CAMERA_ID_2_IMX900 &&
            g_in_packet_buf[2]  == READ_STATISTICS_IMX900 &&
            g_in_packet_buf[10] == GET_SUCCESS) {

            uint milliSeconds, microSeconds, gain;

            milliSeconds  = (g_in_packet_buf[4] << 8) | (g_in_packet_buf[5] << 0);
            microSeconds  = (g_in_packet_buf[6] << 8) | (g_in_packet_buf[7] << 0);
            gain          = (g_in_packet_buf[8] << 8) | (g_in_packet_buf[9] << 0);

            emit currentStatistics(g_in_packet_buf[3], milliSeconds, microSeconds, gain);

            return true;
        }
    }
    return false;
}



/**
 * @brief IMX900USBCAM::calculateTemperature() - To calculate the temperature in the camera
 * @return true/false
 */
bool IMX900USBCAM::calculateTemperature(){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    float temperature;

    //Initialize buffers
    initializeBuffers();


    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = READ_TEMPERATURE_IMX900;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
            g_in_packet_buf[1]  == CAMERA_ID_2_IMX900 &&
            g_in_packet_buf[2]  == READ_TEMPERATURE_IMX900 &&
            g_in_packet_buf[7]  == GET_SUCCESS) {

            uint32_t combinedValue;
            combinedValue = (g_in_packet_buf[3] << 24) | (g_in_packet_buf[4] << 16) | (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);

            temperature = *reinterpret_cast<float*>(&combinedValue);

            emit currentTemperature(temperature);

            return true;
        }
    }
    return false;
}



/**
 * @brief IMX900USBCAM::getHighDynamicRange - To get status of HDR mode from the camera
 * return true - success /false - failure
 */
bool IMX900USBCAM::getHighDynamicRange()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = GET_HDR_IMX900;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
            g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
            g_in_packet_buf[2] == GET_HDR_IMX900 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit currentHDRStatus(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}


/*
 * @brief IMX900USBCAM::setHighDynamicRange - To set HDR status to the camera
 * @param - status - To switch between Enable / Disable mode
 * return true/false
*/
bool IMX900USBCAM::setHighDynamicRange(HDR hdr)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = SET_HDR_IMX900;
    g_out_packet_buf[4] = hdr;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
                  g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
                  g_in_packet_buf[2] == SET_HDR_IMX900 &&
                  g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}




/**
 * @brief IMX900USBCAM::getQuadShutterControl - To get status of Quad Shutter Control from the camera
 * return true - success /false - failure
 */
bool IMX900USBCAM::getQuadShutterControl()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = GET_QUAD_SHUTTER_CONTROL_IMX900;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
            g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
            g_in_packet_buf[2] == GET_QUAD_SHUTTER_CONTROL_IMX900 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit currentQuadShutterControlStatus(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}


/*
 * @brief IMX900USBCAM::setQuadShutterControl - To set Quad Shutter Control status to the camera
 * @param - status - To switch between Enable / Disable mode
 * return true/false
*/
bool IMX900USBCAM::setQuadShutterControl(uint status)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = SET_QUAD_SHUTTER_CONTROL_IMX900;
    g_out_packet_buf[4] = status;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
                  g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
                  g_in_packet_buf[2] == SET_QUAD_SHUTTER_CONTROL_IMX900 &&
                  g_in_packet_buf[6] == GET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief IMX900USBCAM::getFastAutoExposure - To get status of strobe mode from the camera
 * return true - success /false - failure
 */
bool IMX900USBCAM::getFastAutoExposure()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = GET_FAST_AUTO_EXPOSURE_IMX900;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
            g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
            g_in_packet_buf[2] == GET_FAST_AUTO_EXPOSURE_IMX900 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit currentFastAutoExposureStatus(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}


/*
 * @brief IMX900USBCAM::setFastAutoExposure - To set Fast Auto Exposure status to the camera
 * @param - status - To switch between Enable / Disable mode
 * return true/false
*/
bool IMX900USBCAM::setFastAutoExposure(uint status)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = SET_FAST_AUTO_EXPOSURE_IMX900;
    g_out_packet_buf[4] = status;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
                  g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
                  g_in_packet_buf[2] == SET_FAST_AUTO_EXPOSURE_IMX900 &&
                  g_in_packet_buf[6] == GET_SUCCESS) {
            return true;
        }
    }
    return false;
}



/**
 * @brief IMX900USBCAM::getToneControl - To get status of Tone Control from the camera
 * return true - success /false - failure
 */
bool IMX900USBCAM::getToneControl()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = GET_TONE_CONTROL_IMX900;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
            g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
            g_in_packet_buf[2] == GET_TONE_CONTROL_IMX900 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit currentToneControlStatus(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}


/*
 * @brief IMX900USBCAM::setToneControl - To set Tone Control status to the camera
 * @param - status - To switch between Enable / Disable mode
 * return true/false
*/
bool IMX900USBCAM::setToneControl(uint status)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = SET_TONE_CONTROL_IMX900;
    g_out_packet_buf[4] = status;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
                  g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
                  g_in_packet_buf[2] == SET_TONE_CONTROL_IMX900 &&
                  g_in_packet_buf[6] == GET_SUCCESS) {
            return true;
        }
    }
    return false;
}




/**
 * @brief IMX900USBCAM::getShortExposureMode - To get Short Exposure Mode from camera
 * @return - true/false
 */
bool IMX900USBCAM::getShortExposureMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = GET_SHORT_EXPOUSRE_IMX900;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
                  g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
                  g_in_packet_buf[2] == GET_SHORT_EXPOUSRE_IMX900  &&
                  g_in_packet_buf[7] == GET_SUCCESS) {
            emit currentShortExposureStatus(g_in_packet_buf[3], g_in_packet_buf[5], g_in_packet_buf[6], g_in_packet_buf[4]);
            return true;
        }
    }
    return false;
}

/**
 * @brief IMX900USBCAM::setShortExpousreMode - setting setShortExpousreMode
 * @param - status - To switch between Enable / Disable mode
 * @param - value  - The manual Value to be set
 * @return true/false
 */
bool IMX900USBCAM::setShortExposureMode(uint status, uint value){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = SET_SHORT_EXPOUSRE_IMX900;
    g_out_packet_buf[4] = status;
    g_out_packet_buf[5] = value;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
                  g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
                  g_in_packet_buf[2] == SET_SHORT_EXPOUSRE_IMX900  &&
                  g_in_packet_buf[7] == GET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief IMX900USBCAM::getMultiFrameSet - To get Multi-Frame Set Control Properties
 * @return true/false
 */
bool IMX900USBCAM::getMultiFrameSet(){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    uint exposure1, exposure2, exposure3, exposure4;

    uint gain1, gain2, gain3, gain4;

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = GET_MULTI_FRAME_SET_IMX900;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[28] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0]  == CAMERA_ID_1_IMX900 &&
                  g_in_packet_buf[1]  == CAMERA_ID_2_IMX900 &&
                  g_in_packet_buf[2]  == GET_MULTI_FRAME_SET_IMX900  &&
                  g_in_packet_buf[28] == GET_SUCCESS) {

            gain1 = (g_in_packet_buf[4] << 8) | (g_in_packet_buf[5] << 0);
            gain2 = (g_in_packet_buf[6] << 8) | (g_in_packet_buf[7] << 0);
            gain3 = (g_in_packet_buf[8] << 8) | (g_in_packet_buf[9] << 0);
            gain4 = (g_in_packet_buf[10] << 8) | (g_in_packet_buf[11] << 0);

            exposure1 = (((u_int8_t)g_in_packet_buf[12]) << 24) | (((u_int8_t)g_in_packet_buf[13]) << 16) | (((u_int8_t)g_in_packet_buf[14]) << 8) | (((u_int8_t)g_in_packet_buf[15]) << 0);
            exposure2 = (((u_int8_t)g_in_packet_buf[16]) << 24) | (((u_int8_t)g_in_packet_buf[17]) << 16) | (((u_int8_t)g_in_packet_buf[18]) << 8) | (((u_int8_t)g_in_packet_buf[19]) << 0);
            exposure3 = (((u_int8_t)g_in_packet_buf[20]) << 24) | (((u_int8_t)g_in_packet_buf[21]) << 16) | (((u_int8_t)g_in_packet_buf[22]) << 8) | (((u_int8_t)g_in_packet_buf[23]) << 0);
            exposure4 = (((u_int8_t)g_in_packet_buf[24]) << 24) | (((u_int8_t)g_in_packet_buf[25]) << 16) | (((u_int8_t)g_in_packet_buf[26]) << 8) | (((u_int8_t)g_in_packet_buf[27]) << 0);

            emit multiFrameSetModeReceived(g_in_packet_buf[3]);

            emit frameSet1Received(gain1, exposure1);
            emit frameSet2Received(gain2, exposure2);
            emit frameSet3Received(gain3, exposure3);
            emit frameSet4Received(gain4, exposure4);

            return true;
        }
    }
    return false;
}


/**
 * @brief IMX900USBCAM::setMultiFrameSet - To Set Multi-Frame Set Mode & values
 * @return true/false
 */
bool IMX900USBCAM::setMultiFrameSet(MULTI_FRAME_SET mode, uint gain1, unsigned int exposure1, uint gain2, uint exposure2, uint gain3, uint exposure3, uint gain4, uint exposure4)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1]  = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2]  = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3]  = SET_MULTI_FRAME_SET_IMX900;
    g_out_packet_buf[4]  = mode;

    g_out_packet_buf[5]  = ((gain1 & 0xFF00) >> 8);
    g_out_packet_buf[6]  = ((gain1 & 0x00FF) >> 0);

    g_out_packet_buf[7]  = ((gain2 & 0xFF00) >> 8);
    g_out_packet_buf[8]  = ((gain2 & 0x00FF) >> 0);

    g_out_packet_buf[9]  = ((gain3 & 0xFF00) >> 8);
    g_out_packet_buf[10] = ((gain3 & 0x00FF) >> 0);

    g_out_packet_buf[11] = ((gain4 & 0xFF00) >> 8);
    g_out_packet_buf[12] = ((gain4 & 0x00FF) >> 0);

    g_out_packet_buf[13] = (u_int8_t)((exposure1 & 0xFF000000) >> 24);
    g_out_packet_buf[14] = (u_int8_t)((exposure1 & 0x00FF0000) >> 16);
    g_out_packet_buf[15] = (u_int8_t)((exposure1 & 0x0000FF00) >> 8);
    g_out_packet_buf[16] = (u_int8_t)((exposure1 & 0x000000FF) >> 0);

    g_out_packet_buf[17] = (u_int8_t)((exposure2 & 0xFF000000) >> 24);
    g_out_packet_buf[18] = (u_int8_t)((exposure2 & 0x00FF0000) >> 16);
    g_out_packet_buf[19] = (u_int8_t)((exposure2 & 0x0000FF00) >> 8);
    g_out_packet_buf[20] = (u_int8_t)((exposure2 & 0x000000FF) >> 0);

    g_out_packet_buf[21] = (u_int8_t)((exposure3 & 0xFF000000) >> 24);
    g_out_packet_buf[22] = (u_int8_t)((exposure3 & 0x00FF0000) >> 16);
    g_out_packet_buf[23] = (u_int8_t)((exposure3 & 0x0000FF00) >> 8);
    g_out_packet_buf[24] = (u_int8_t)((exposure3 & 0x000000FF) >> 0);

    g_out_packet_buf[25] = (u_int8_t)((exposure4 & 0xFF000000) >> 24);
    g_out_packet_buf[26] = (u_int8_t)((exposure4 & 0x00FF0000) >> 16);
    g_out_packet_buf[27] = (u_int8_t)((exposure4 & 0x0000FF00) >> 8);
    g_out_packet_buf[28] = (u_int8_t)((exposure4 & 0x000000FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[28] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
                  g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
                  g_in_packet_buf[2] == SET_MULTI_FRAME_SET_IMX900  &&
                  g_in_packet_buf[28] == GET_SUCCESS) {

            return true;
        }
    }
    return false;
}



/**
 * @brief IMX900USBCAM::getDualTrigger - To Get Dual Trigger Properties from the camera
 * @return - true/false
 */
bool IMX900USBCAM::getDualTrigger()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = GET_DUAL_TRIGGER_IMX900;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[19] == GET_FAIL) {
            emit indicateFailureStatus("Failure","Get Dual Trigger Properties Failed. Retry!");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
            g_in_packet_buf[1]  == CAMERA_ID_2_IMX900 &&
            g_in_packet_buf[2]  == GET_DUAL_TRIGGER_IMX900 &&
            g_in_packet_buf[19] == GET_SUCCESS) {

            uint min, max, gain, min1, max1, gain1;

            min  = (g_in_packet_buf[7] << 8) | (g_in_packet_buf[8] << 0);
            max  = (g_in_packet_buf[9] << 8) | (g_in_packet_buf[10] << 0);
            gain = (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);

            min1  = (g_in_packet_buf[14] << 8) | (g_in_packet_buf[15] << 0);
            max1  = (g_in_packet_buf[16] << 8) | (g_in_packet_buf[17] << 0);
            gain1 = (g_in_packet_buf[12] << 8) | (g_in_packet_buf[13] << 0);

            emit currentDualModeProperties(g_in_packet_buf[3], g_in_packet_buf[4], min, max, g_in_packet_buf[11], min1, max1, g_in_packet_buf[18], gain, gain1);
            return true;
        }
    }
    return false;
}


/**
 * @brief IMX900USBCAM::setDualTrigger - setting camera mode to the camera
 * @return true/false
 */
bool IMX900USBCAM::setDualTrigger(uint mode, uint division, uint gain, uint gain1){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = SET_DUAL_TRIGGER_IMX900;
    g_out_packet_buf[4] = mode;
    g_out_packet_buf[5] = division;

    g_out_packet_buf[6]  = ((gain & 0xFF00) >> 8);
    g_out_packet_buf[7]  = ((gain & 0x00FF) >> 0);

    g_out_packet_buf[13]  = ((gain1 & 0xFF00) >> 8);
    g_out_packet_buf[14]  = ((gain1 & 0x00FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[19] == SET_FAIL) {
            emit indicateFailureStatus("Failure","Set Dual Trigger Properties Failed. Retry!");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
            g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
            g_in_packet_buf[2] == SET_DUAL_TRIGGER_IMX900 &&
            g_in_packet_buf[19] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}



/**
 * @brief IMX900USBCAM::getSelfTrigger - To Get Self Trigger Properties from the camera
 * @return - true/false
 */
bool IMX900USBCAM::getSelfTrigger()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = GET_SELF_TRIGGER_IMX900;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[58] == GET_FAIL) {
            emit indicateFailureStatus("Failure","Get Self Trigger Properties Failed. Retry!");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
            g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
            g_in_packet_buf[2] == GET_SELF_TRIGGER_IMX900 &&
            g_in_packet_buf[58] == GET_SUCCESS) {

            uint32_t exposure = (g_in_packet_buf[22] << 24) | (g_in_packet_buf[23] << 16) | (g_in_packet_buf[24] << 8) | (g_in_packet_buf[25] << 0);

            emit currentSelfTriggerMode(g_in_packet_buf[3]);

            uint hCropMin, hCropMax, hCropSize, vCropMin, vCropMax, vCropSize;

            hCropSize = (g_in_packet_buf[8] << 8) | (g_in_packet_buf[9] << 0);
            hCropMax  = (g_in_packet_buf[12] << 8) | (g_in_packet_buf[13] << 0);
            hCropMin  = (g_in_packet_buf[10] << 8) | (g_in_packet_buf[11] << 0);

            vCropSize = (g_in_packet_buf[15] << 8) | (g_in_packet_buf[16] << 0);
            vCropMin  = (g_in_packet_buf[17] << 8) | (g_in_packet_buf[18] << 0);
            vCropMax  = (g_in_packet_buf[19] << 8) | (g_in_packet_buf[20] << 0);

            emit currentHCropSize(hCropMin, hCropMax, g_in_packet_buf[14], hCropSize);
            emit currentVCropSize(vCropMin, vCropMax, g_in_packet_buf[21], vCropSize);
            emit currentSensingFrameExposure(exposure);

            uint hLevel, lLevel;
            hLevel = (g_in_packet_buf[26] << 8) | (g_in_packet_buf[27] << 0);
            lLevel = (g_in_packet_buf[28] << 8) | (g_in_packet_buf[29] << 0);

            uint hCount, lCount;
            hCount = (((u_int8_t)g_in_packet_buf[30]) << 16) | (((u_int8_t)g_in_packet_buf[31]) << 8) | (((u_int8_t)g_in_packet_buf[32]) << 0);
            lCount = (((u_int8_t)g_in_packet_buf[33]) << 16) | (((u_int8_t)g_in_packet_buf[34]) << 8) | (((u_int8_t)g_in_packet_buf[35]) << 0);

            int step1 = g_in_packet_buf[50];
            int step2 = g_in_packet_buf[57];

            uint32_t hDetPixels, lDetPixels;
            hDetPixels = (((u_int8_t)g_in_packet_buf[38]) << 16) | (((u_int8_t)g_in_packet_buf[39]) << 8) | (((u_int8_t)g_in_packet_buf[40]) << 0);
            lDetPixels = (((u_int8_t)g_in_packet_buf[41]) << 16) | (((u_int8_t)g_in_packet_buf[42]) << 8) | (((u_int8_t)g_in_packet_buf[43]) << 0);

            uint min, max, gain, min1, max1, gain1;
            gain = (g_in_packet_buf[44] << 8) | (g_in_packet_buf[45] << 0);
            min  = (g_in_packet_buf[46] << 8) | (g_in_packet_buf[47] << 0);
            max  = (g_in_packet_buf[48] << 8) | (g_in_packet_buf[49] << 0);

            gain1 = (g_in_packet_buf[51] << 8) | (g_in_packet_buf[52] << 0);
            min1  = (g_in_packet_buf[53] << 8) | (g_in_packet_buf[54] << 0);
            max1  = (g_in_packet_buf[55] << 8) | (g_in_packet_buf[56] << 0);

            emit currentHighThreshold(hLevel, lLevel);
            emit currentCountThreshold(hCount, lCount);
            emit currentSensingFrameMode(g_in_packet_buf[36]);
            emit currentSelfTriggerForce(g_in_packet_buf[37]);
            emit currentDetectedPixels(hDetPixels, lDetPixels);
            emit currentGainForSensingFrame(min, max, step1, gain);
            emit currentGainForCapturingFrame(min1, max1, step2, gain1);

            return true;
        }
    }
    return false;
}



/**
 * @brief IMX900USBCAM::setSelfTrigger - To Set Self Trigger Mode to the camera
 * @return true/false
 */
bool IMX900USBCAM::setSelfTrigger(uint mode, uint vidResolnWidth, uint vidResolnHeight, uint hCropPos, uint vCropPos, uint hCropSize, uint vCropSize, uint exposure, uint hLevel, uint lLevel, uint hCount, uint lCount, uint sensingMode, uint forceMode, uint hDetPixels, uint lDetPixels, uint sensingGain, uint capturingGain)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    //((Input - InputLow) / (InputHigh - InputLow)) * (OutputHigh - OutputLow) + OutputLow // map resolution width and height -  0 to 255

    double outputLow = 0;
    double outputHigh = 255;
    double inputXLow = 0;
    double inputXHigh = vidResolnWidth-1;
    double inputXCord = hCropPos;
    int outputXCord = ((inputXCord - inputXLow) / (inputXHigh - inputXLow)) * (outputHigh - outputLow) + outputLow;

    double inputYLow = 0;
    double inputYHigh = vidResolnHeight-1;
    double inputYCord = vCropPos;
    int outputYCord = ((inputYCord - inputYLow) / (inputYHigh - inputYLow)) * (outputHigh - outputLow) + outputLow;

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = SET_SELF_TRIGGER_IMX900;
    g_out_packet_buf[4] = mode;

    g_out_packet_buf[5]  = ((outputXCord & 0xFF00) >> 8);
    g_out_packet_buf[6]  = ((outputXCord & 0x00FF) >> 0);

    g_out_packet_buf[7]  = ((outputYCord & 0xFF00) >> 8);
    g_out_packet_buf[8]  = ((outputYCord & 0x00FF) >> 0);

    g_out_packet_buf[9]  = ((hCropSize & 0xFF00) >> 8);
    g_out_packet_buf[10] = ((hCropSize & 0x00FF) >> 0);

    g_out_packet_buf[16] = ((vCropSize & 0xFF00) >> 8);
    g_out_packet_buf[17] = ((vCropSize & 0x00FF) >> 0);

    g_out_packet_buf[23] = (u_int8_t)((exposure & 0xFF000000) >> 24);
    g_out_packet_buf[24] = (u_int8_t)((exposure & 0x00FF0000) >> 16);
    g_out_packet_buf[25] = (u_int8_t)((exposure & 0x0000FF00) >> 8);
    g_out_packet_buf[26] = (u_int8_t)((exposure & 0x000000FF) >> 0);

    g_out_packet_buf[27] = ((hLevel & 0xFF00) >> 8);
    g_out_packet_buf[28] = ((hLevel & 0x00FF) >> 0);

    g_out_packet_buf[29] = ((lLevel & 0xFF00) >> 8);
    g_out_packet_buf[30] = ((lLevel & 0x00FF) >> 0);

    g_out_packet_buf[31] = (u_int8_t)((hCount & 0x00FF0000) >> 16);
    g_out_packet_buf[32] = (u_int8_t)((hCount & 0x0000FF00) >> 8);
    g_out_packet_buf[33] = (u_int8_t)((hCount & 0x000000FF) >> 0);

    g_out_packet_buf[34] = (u_int8_t)((lCount & 0x00FF0000) >> 16);
    g_out_packet_buf[35] = (u_int8_t)((lCount & 0x0000FF00) >> 8);
    g_out_packet_buf[36] = (u_int8_t)((lCount & 0x000000FF) >> 0);

    g_out_packet_buf[37] = sensingMode;
    g_out_packet_buf[38] = forceMode;

    g_out_packet_buf[39] = (u_int8_t)((hDetPixels & 0x00FF0000) >> 16);
    g_out_packet_buf[40] = (u_int8_t)((hDetPixels & 0x0000FF00) >> 8);
    g_out_packet_buf[41] = (u_int8_t)((hDetPixels & 0x000000FF) >> 0);

    g_out_packet_buf[42] = (u_int8_t)((lDetPixels & 0x00FF0000) >> 16);
    g_out_packet_buf[43] = (u_int8_t)((lDetPixels & 0x0000FF00) >> 8);
    g_out_packet_buf[44] = (u_int8_t)((lDetPixels & 0x000000FF) >> 0);

    g_out_packet_buf[45] = ((sensingGain & 0xFF00) >> 8);
    g_out_packet_buf[46] = ((sensingGain & 0x00FF) >> 0);

    g_out_packet_buf[52] = ((capturingGain & 0xFF00) >> 8);
    g_out_packet_buf[53] = ((capturingGain & 0x00FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[58] == SET_FAIL) {
            emit indicateFailureStatus("Failure","Set Self Trigger Properties Failed. Retry!");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
            g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
            g_in_packet_buf[2] == SET_SELF_TRIGGER_IMX900 &&
            g_in_packet_buf[58] == SET_SUCCESS) {

            return true;
        }
    }
    return false;
}



/**
 * @brief IMX900USBCAM::setToDefaultValues - To set all the values to default in camera
 * @return true/false
 */
bool IMX900USBCAM::setToDefaultValues()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900;
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900;
    g_out_packet_buf[3] = SET_TO_DEFAULT_IMX900;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH))
    {
        if (g_in_packet_buf[6] == SET_FAIL){
            emit indicateFailureStatus("Failure","Set Default Failed. Retry!");
            return false;
        }else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&
            g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
            g_in_packet_buf[2] == SET_TO_DEFAULT_IMX900 &&
            g_in_packet_buf[6] == SET_SUCCESS){
            return true;
        }
    }
    return false;
}

/**
 * @brief IMX900USBCAM::readISPFirmwareVersion - To read the firmware version of ISP
 * @return true/false
 */
bool IMX900USBCAM::readISPFirmwareVersion(){
    _title = tr("ISP Version");

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    g_out_packet_buf[1] = CAMERA_ID_1_IMX900; 	/* control_id_1 */
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900; 	/* control_id_2 */
    g_out_packet_buf[3] = ISP_FIRMWARE_VERSION_IMX900USBCAM;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == GET_SUCCESS) {
            _text.clear();
            _text.append("Version: ");
            _text.append(QString::number(g_in_packet_buf[3]).append(".").append(QString::number(g_in_packet_buf[4])).append(".").append(QString::number(g_in_packet_buf[5])).append(".").append(QString::number(g_in_packet_buf[6])));
            emit titleTextChanged(_title,_text);
        } else if(g_in_packet_buf[7] == GET_FAIL) {
            return false;
        }
    }
    return false;
}


/**
 * @brief IMX900USBCAM::saveConfiguration
 * @return true/false
 */
bool IMX900USBCAM::saveConfiguration()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = SAVE_CONFIGURATION_IMX900USBCAM;
    g_out_packet_buf[2] = SAVE_IMX900USBCAM;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH))
    {
        if (g_in_packet_buf[6] == SET_FAIL)
        {
            emit indicateCommandStatus("Failure", "Saving Configurations Failed");
            return false;
        }
        else if(g_in_packet_buf[0] == SAVE_CONFIGURATION_IMX900USBCAM  &&
            g_in_packet_buf[1] == SAVE_IMX900USBCAM &&
            g_in_packet_buf[6] == SET_SUCCESS){
            emit indicateCommandStatus("Success", "Configurations saved successfully");
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_130::setExposureCompensation - setting exposure compensation
 * @param exposureCompValue - exposure compensation value
 * @return true/false
 */
bool IMX900USBCAM::setExposureCompensation(uint seconds, uint milliSeconds, uint microSeconds){

   // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900; 	/* control_id_1 */
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900; 	/* control_id_2 */
    g_out_packet_buf[3] = SET_EXPOSURE_COMPENSATION_IMX900;

    g_out_packet_buf[4] = seconds;
    g_out_packet_buf[5] = ((milliSeconds & 0xFF00) >> 8);
    g_out_packet_buf[6] = ((milliSeconds & 0x00FF) >> 0);
    g_out_packet_buf[7] = ((microSeconds & 0xFF00) >> 8);
    g_out_packet_buf[8] = ((microSeconds & 0x00FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[8]==SET_FAIL) {
            emit indicateCommandStatus("Failure", "Given exposure compensation value is invalid");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 && g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
                  g_in_packet_buf[2]==SET_EXPOSURE_COMPENSATION_IMX900 && g_in_packet_buf[8]==SET_SUCCESS) {
            emit indicateCommandStatus("Success", "Exposure compensation value is set successfully");
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_130::getExposureCompensation - getting exposure compensation
 * @return true/false
 */
bool IMX900USBCAM::getExposureCompensation(){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900; 	/* control_id_1 */
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900; 	/* control_id_2 */
    g_out_packet_buf[3] = GET_EXPOSURE_COMPENSATION_IMX900;

    uint milliSeconds, microSeconds;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[8]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 && g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
            g_in_packet_buf[2]==GET_EXPOSURE_COMPENSATION_IMX900 &&   g_in_packet_buf[8]==GET_SUCCESS) {

            milliSeconds = (g_in_packet_buf[4] << 8) | (g_in_packet_buf[5] << 0);
            microSeconds = (g_in_packet_buf[6] << 8) | (g_in_packet_buf[7] << 0);

            emit currentExposureCompensation(g_in_packet_buf[3], milliSeconds, microSeconds);
            return true;
        }
    }
    return false;
}

bool IMX900USBCAM::setROIAutoExposure(AUTO_EXPOSURE_ROI roiMode, uint vidResolnWidth, uint vidResolnHeight, uint xCoord, uint yCoord, QString winSize){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    double outputLow = 0;
    double outputHigh = 255;
    double inputXLow = 0;
    double inputXHigh = vidResolnWidth-1;
    double inputXCord = xCoord;
    int outputXCord = ((inputXCord - inputXLow) / (inputXHigh - inputXLow)) * (outputHigh - outputLow) + outputLow;

    double inputYLow = 0;
    double inputYHigh = vidResolnHeight-1;
    double inputYCord = yCoord;
    int outputYCord = ((inputYCord - inputYLow) / (inputYHigh - inputYLow)) * (outputHigh - outputLow) + outputLow;

    //Initialize buffers
    initializeBuffers();

    g_out_packet_buf[1] = CAMERA_ID_1_IMX900; 	/* control_id_1 */
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900; 	/* control_id_2 */
    g_out_packet_buf[3] = SET_AUTO_EXPOSURE_ROI; /* ROI mode which is need to set */
    g_out_packet_buf[4] = roiMode;

    if(roiMode == AE_MANUAL_ROI){
        g_out_packet_buf[5] = outputXCord;
        g_out_packet_buf[6] = outputYCord;
        g_out_packet_buf[7] = winSize.toUInt();
    }

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&  g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
                  g_in_packet_buf[2] == SET_AUTO_EXPOSURE_ROI && g_in_packet_buf[7] == SET_SUCCESS){
           return true;
        }
    }
    return false;
}

bool IMX900USBCAM::getAutoExpROIModeAndWindowSize(){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_ID_1_IMX900; 	/* control_id_1 */
    g_out_packet_buf[2] = CAMERA_ID_2_IMX900; 	/* control_id_2 */
    g_out_packet_buf[3] = GET_AUTO_EXPOSURE_ROI;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_ID_1_IMX900 &&  g_in_packet_buf[1] == CAMERA_ID_2_IMX900 &&
            g_in_packet_buf[2] == GET_AUTO_EXPOSURE_ROI && g_in_packet_buf[7] == GET_SUCCESS) {
                emit roiAutoExpModeValueReceived(g_in_packet_buf[3], g_in_packet_buf[6]);
                return true;
        }
    }
    return false;
}


/*
 * @brief IMX900USBCAM::initializeBuffers - Initialize input and output buffers
 */
void IMX900USBCAM::initializeBuffers(){
    //Initialize input and output buffers
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));
}
