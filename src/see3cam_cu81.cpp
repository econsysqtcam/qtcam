#include "see3cam_cu81.h"

See3CAM_CU81::See3CAM_CU81()
{
}

See3CAM_CU81::~See3CAM_CU81()
{
}

bool See3CAM_CU81::getCameraMode()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU81; /* set camera control code */
    g_out_packet_buf[2] = GET_CAMERA_MODE_CU81; /* get special effect code */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU81 &&
            g_in_packet_buf[1]==GET_CAMERA_MODE_CU81 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit sendCameraModeValue(g_in_packet_buf[2]);
                return true;
        }
    }
    return false;
}

bool See3CAM_CU81::setCameraMode(const cameraModes &cameraMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU81; /* Camera control id */
    g_out_packet_buf[2] = SET_CAMERA_MODE_CU81; /* Set effect mode command */
    g_out_packet_buf[3] = cameraMode; /* Scene mode to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU81 &&
            g_in_packet_buf[1]==SET_CAMERA_MODE_CU81 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool See3CAM_CU81::getEffect()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU81; /* set camera control code */
    g_out_packet_buf[2] = GET_SPECIALEFFECT_CU81; /* get special effect code */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU81 &&
            g_in_packet_buf[1]==GET_SPECIALEFFECT_CU81 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit sentEffectMode(g_in_packet_buf[2]);
                return true;
        }
    }
    return false;
}

bool See3CAM_CU81::setEffect(const See3CAM_CU81::specialEffects &specialEffect)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU81; /* Camera control id */
    g_out_packet_buf[2] = SET_SPECIALEFFECT_CU81; /* Set effect mode command */
    g_out_packet_buf[3] = specialEffect; /* Scene mode to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU81 &&
            g_in_packet_buf[1]==SET_SPECIALEFFECT_CU81 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool See3CAM_CU81::getDenoise()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU81; /* set camera control code */
    g_out_packet_buf[2] = GET_DENOISE_CU81; /* get special effect code */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU81 &&
            g_in_packet_buf[1]==GET_DENOISE_CU81 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit sendDenoiseValue(g_in_packet_buf[2]);
                return true;
        }
    }
    return false;
}

bool See3CAM_CU81::setDenoise(int deNoiseValue)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU81; /* Camera control id */
    g_out_packet_buf[2] = SET_DENOISE_CU81; /* Set effect mode command */
    g_out_packet_buf[3] = deNoiseValue; /* Scene mode to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU81 &&
            g_in_packet_buf[1]==SET_DENOISE_CU81 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool See3CAM_CU81::getAutoExpROIModeAndWindowSize()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU81; /* camera id */
    g_out_packet_buf[2] = GET_ROIEXPMODE_CU81; /* get Auto exposure ROI mode command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU81 &&
            g_in_packet_buf[1]==GET_ROIEXPMODE_CU81 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
                emit roiAutoExpModeValue(g_in_packet_buf[2], g_in_packet_buf[5]);
                return true;
        }
    }
    return false;
}

bool See3CAM_CU81::setROIAutoExposure(See3CAM_CU81::camROIAutoExpMode see3camAutoexpROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize)
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

    g_out_packet_buf[1] = CAMERA_CONTROL_CU81; /* camera control id for camera See3CAM_130D */
    g_out_packet_buf[2] = SET_ROIEXPMODE_CU81; /* set Auto exposure ROI mode command  */
    g_out_packet_buf[3] = see3camAutoexpROIMode; /* ROI mode which is need to set */

    if(see3camAutoexpROIMode == AutoExpManual){
        g_out_packet_buf[4] = outputXCord;
        g_out_packet_buf[5] = outputYCord;
        g_out_packet_buf[6] = winSize.toUInt();
    }

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU81 &&
            g_in_packet_buf[1]==SET_ROIEXPMODE_CU81 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool See3CAM_CU81::getBurstLength()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU81; /* camera id */
    g_out_packet_buf[2] = GET_BURSTLENGTH_CU81; /* get Auto exposure ROI mode command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU81 &&
            g_in_packet_buf[1]==GET_BURSTLENGTH_CU81 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
                emit burstLengthValue(g_in_packet_buf[2]);
                return true;
        }
    }
    return false;
}

bool See3CAM_CU81::setBurstLength(uint burstLength)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU81; /* Camera control id */
    g_out_packet_buf[2] = SET_BURSTLENGTH_CU81; /* Set effect mode command */
    g_out_packet_buf[3] = burstLength; /* Scene mode to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU81 &&
            g_in_packet_buf[1]==SET_BURSTLENGTH_CU81 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool See3CAM_CU81::getQFactor()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU81; /* camera id */
    g_out_packet_buf[2] = GET_QFACTOR_CU81; /* get Auto exposure ROI mode command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU81 &&
            g_in_packet_buf[1]==GET_QFACTOR_CU81 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
                emit qFactorValue(g_in_packet_buf[2]);
                return true;
        }
    }
    return false;
}

bool See3CAM_CU81::setQFactor(uint qFactor)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU81; /* Camera control id */
    g_out_packet_buf[2] = SET_QFACTOR_CU81; /* Set effect mode command */
    g_out_packet_buf[3] = qFactor; /* Scene mode to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU81 &&
            g_in_packet_buf[1]==SET_QFACTOR_CU81 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool See3CAM_CU81::getFlipMode()
{
    // hid validation
        if(uvccamera::hid_fd < 0)
        {
            return false;
        }

        //Initialize buffers
        initializeBuffers();

        // fill buffer values
        g_out_packet_buf[1] = CAMERA_CONTROL_CU81; /* camera id */
        g_out_packet_buf[2] = GET_ORIENTATION_CU81; /* get Auto exposure ROI mode command  */

        // send request and get reply from camera
        if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
            if (g_in_packet_buf[6]==GET_FAIL) {
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU81 &&
                g_in_packet_buf[1]==GET_ORIENTATION_CU81 &&
                g_in_packet_buf[6]==GET_SUCCESS) {
                    emit flipMirrorModeChanged(g_in_packet_buf[2]);
                    return true;
            }
        }
        return false;
}

bool See3CAM_CU81::setFlipMode(bool horzModeSel, bool vertiModeSel)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU81; /* camera control for see3cam_130D camera */
    g_out_packet_buf[2] = SET_ORIENTATION_CU81; /* set flip mode  */
    if(horzModeSel && vertiModeSel){
        g_out_packet_buf[3] = FLIP_BOTHFLIP_ENABLE ; /* both flip enable */
    }else if(horzModeSel && !vertiModeSel){
        g_out_packet_buf[3] = FLIP_HORZFLIP; /* horizontal flip only mode */
    }else if(!horzModeSel && vertiModeSel){
        g_out_packet_buf[3] = FLIP_VERTFLIP; /* vertical flip only mode */
    }else
        g_out_packet_buf[3] = FLIP_BOTHFLIP_DISABLE; /* both flip disable */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU81 &&
            g_in_packet_buf[1]==SET_ORIENTATION_CU81 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

bool See3CAM_CU81::getExposureCompensation()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU81; /* set camera control code */
    g_out_packet_buf[2] = GET_EXPCOMPENSATION_CU81; /* get exposure compensation code */

    unsigned int exposureCompValue;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU81 &&
            g_in_packet_buf[1]==GET_EXPCOMPENSATION_CU81 &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            exposureCompValue = (((u_int8_t)g_in_packet_buf[2]) << 24)
                                | (((u_int8_t)g_in_packet_buf[3]) << 16)
                                | (((u_int8_t)g_in_packet_buf[4]) << 8)
                                | (((u_int8_t)g_in_packet_buf[5]) << 0);
            emit exposureCompValueReceived(exposureCompValue);
            return true;
        }
    }
    return false;

}

bool See3CAM_CU81::setExposureCompensation(unsigned int exposureCompValue)
{
    if(EXPOSURECOMP_MIN > exposureCompValue || EXPOSURECOMP_MAX < exposureCompValue){
        emit indicateExposureValueRangeFailure("Failure", "Given exposure compensation value is invalid");
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
    g_out_packet_buf[1] = CAMERA_CONTROL_CU81; /* camera id */
    g_out_packet_buf[2] = SET_EXPCOMPENSATION_CU81; /* set exposure compensation command */
    g_out_packet_buf[3] = (u_int8_t)((exposureCompValue & 0xFF000000) >> 24);
    g_out_packet_buf[4] = (u_int8_t)((exposureCompValue & 0x00FF0000) >> 16);
    g_out_packet_buf[5] = (u_int8_t)((exposureCompValue & 0x0000FF00) >> 8);
    g_out_packet_buf[6] = (u_int8_t)((exposureCompValue & 0x000000FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            emit indicateCommandStatus("Failure", "Failed to set exposure compensation value");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU81 &&
            g_in_packet_buf[1]==SET_EXPCOMPENSATION_CU81 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            emit indicateCommandStatus("Success", "Exposure compensation value is set successfully");
            return true;
        }
    }
    return false;

}

bool See3CAM_CU81::getFrameRateCtrlValue()
{
    // hid validation
        if(uvccamera::hid_fd < 0)
        {
            return false;
        }

        //Initialize buffers
        initializeBuffers();

        // fill buffer values
        g_out_packet_buf[1] = CAMERA_CONTROL_CU81; /* camera id */
        g_out_packet_buf[2] = GET_FRAMERATE_CU81; /* get Auto exposure ROI mode command  */

        // send request and get reply from camera
        if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
            if (g_in_packet_buf[6]==GET_FAIL) {
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU81 &&
                g_in_packet_buf[1]==GET_FRAMERATE_CU81 &&
                g_in_packet_buf[6]==GET_SUCCESS) {
                    emit frameRateCtrlValueReceived(g_in_packet_buf[2]);
                    return true;
            }
        }
        return false;
}

bool See3CAM_CU81::setFrameRateCtrlValue(uint frameRate)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU81; /* set camera control code */
    g_out_packet_buf[2] = SET_FRAMERATE_CU81; /* set framerate control code */
    g_out_packet_buf[3] = frameRate; /* set framerate value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU81 &&
            g_in_packet_buf[1]==SET_FRAMERATE_CU81 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool See3CAM_CU81::getAntiFlickerMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU81; /* camera id */
    g_out_packet_buf[2] = GET_ANTIFLICKER_CU81;/* get anti flicker command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU81 &&
            g_in_packet_buf[1] == GET_ANTIFLICKER_CU81 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit antiFlickerModeChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

bool See3CAM_CU81::setAntiFlickerMode(See3CAM_CU81::camAntiFlickerMode antiFlickerMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU81; /* camera id */
    g_out_packet_buf[2] = SET_ANTIFLICKER_CU81; /* set flicker detection command */
    g_out_packet_buf[3] = antiFlickerMode; /* flicker detection mode to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU81 &&
            g_in_packet_buf[1]==SET_ANTIFLICKER_CU81 &&
            g_in_packet_buf[6]==SET_SUCCESS) {

            return true;
        }
    }
    return false;
}

bool See3CAM_CU81::setToDefault()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU81; /* camera id */
    g_out_packet_buf[2] = SET_DEFAULTS_CU81; /* set to default command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU81 &&
            g_in_packet_buf[1]==SET_DEFAULTS_CU81 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;

}

void See3CAM_CU81::initializeBuffers(){

    //Initialize input and output buffers
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));
}
