#include "see3cam_24cug.h"

void See3CAM_24CUG::initializeBuffers()
{
    //Initialize input and output buffers
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));
}

See3CAM_24CUG::See3CAM_24CUG(QObject *parent)
{

}

bool See3CAM_24CUG::getSceneMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_24CUG; /* camera id */
    g_out_packet_buf[2] = GET_SCENEMODE_24CUG; /* get scene mode command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH))
    {
        if (g_in_packet_buf[6]==GET_FAIL)
        {
            return false;
        }
        else if(g_in_packet_buf[0] == CAMERA_CONTROL_24CUG &&g_in_packet_buf[1]==GET_SCENEMODE_24CUG &&
            g_in_packet_buf[6]==GET_SUCCESS)
        {
            emit sceneModeValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

bool See3CAM_24CUG::setSceneMode(const See3CAM_24CUG::sceneModes &sceneMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_24CUG; /* Camera control id */
    g_out_packet_buf[2] = SET_SCENEMODE_24CUG; /* Set scene mode command */
    g_out_packet_buf[3] = sceneMode; /* Scene mode to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_24CUG &&
            g_in_packet_buf[1]==SET_SCENEMODE_24CUG &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool See3CAM_24CUG::getEffectMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_24CUG; /* set camera control code */
    g_out_packet_buf[2] = GET_SPECIALEFFECT_24CUG; /* get special effect code */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_24CUG &&
            g_in_packet_buf[1]==GET_SPECIALEFFECT_24CUG &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit sendEffectMode(g_in_packet_buf[2]);
                return true;
        }
    }
    return false;

}

bool See3CAM_24CUG::setEffectMode(const See3CAM_24CUG::specialEffects &specialEffect)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_24CUG; /* Camera control id */
    g_out_packet_buf[2] = SET_SPECIALEFFECT_24CUG; /* Set effect mode command */
    g_out_packet_buf[3] = specialEffect; /* Scene mode to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_24CUG &&
            g_in_packet_buf[1]==SET_SPECIALEFFECT_24CUG &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;

}

bool See3CAM_24CUG::getDenoiseValue()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_24CUG; /* set camera control code */
    g_out_packet_buf[2] = GET_DENOISE_24CUG; /* get denoise code */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_24CUG &&
            g_in_packet_buf[1]==GET_DENOISE_24CUG &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit sendDenoiseValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

bool See3CAM_24CUG::setDenoiseValue(int deNoiseVal)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_24CUG; /* set camera control code */
    g_out_packet_buf[2] = SET_DENOISE_24CUG; /* set denoise control code */
    g_out_packet_buf[3] = deNoiseVal; /* set denoise value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_24CUG &&
            g_in_packet_buf[1]==SET_DENOISE_24CUG &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

bool See3CAM_24CUG::getAutoExpROIModeAndWindowSize()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_24CUG; /* camera id */
    g_out_packet_buf[2] = GET_EXP_ROI_MODE_24CUG; /* get exposure ROI mode  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_24CUG &&
            g_in_packet_buf[1]==GET_EXP_ROI_MODE_24CUG &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit roiAutoExpModeValue(g_in_packet_buf[2], g_in_packet_buf[5]);
            return true;
        }
    }
    return false;

}

bool See3CAM_24CUG::setROIAutoExposure(See3CAM_24CUG::camROIAutoExpMode see3camAutoexpROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize)
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
    g_out_packet_buf[1] = CAMERA_CONTROL_24CUG; /* camera id */
    g_out_packet_buf[2] = SET_EXP_ROI_MODE_24CUG; /* set exposure ROI command */
    g_out_packet_buf[3] = see3camAutoexpROIMode; /* exposure ROI mode to set */

    if(see3camAutoexpROIMode == AutoExpManual){
        g_out_packet_buf[4] = outputXCord; // x cord
        g_out_packet_buf[5] = outputYCord; // y cord
        g_out_packet_buf[6] = winSize.toUInt(); // window size
    }

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_24CUG &&
            g_in_packet_buf[1]==SET_EXP_ROI_MODE_24CUG &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;

}

bool See3CAM_24CUG::getQFactor()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_24CUG; /* camera id */
    g_out_packet_buf[2] = GET_Q_FACTOR_24CUG; /* get q factor command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_24CUG &&
            g_in_packet_buf[1]==GET_Q_FACTOR_24CUG &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            emit qFactorValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;

}

bool See3CAM_24CUG::setQFactor(uint qFactor)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_24CUG; /* camera id */
    g_out_packet_buf[2] = SET_Q_FACTOR_24CUG; /* set q factor command */
    g_out_packet_buf[3] = qFactor; /* qfactor value to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_24CUG &&
            g_in_packet_buf[1]==SET_Q_FACTOR_24CUG &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool See3CAM_24CUG::getOrientation()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_24CUG; /* camera id */
    g_out_packet_buf[2] = GET_FLIP_MODE_24CUG; /* get flip mode command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_24CUG &&
            g_in_packet_buf[1]==GET_FLIP_MODE_24CUG &&
            g_in_packet_buf[6]==GET_SUCCESS) {
                emit flipMirrorModeChanged(g_in_packet_buf[2]);
                return true;
        }
    }
    return false;
}

bool See3CAM_24CUG::setOrientation(bool horzModeSel, bool vertiModeSel)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_24CUG; /* camera control for see3cam_24CUG camera */
    g_out_packet_buf[2] = SET_FLIP_MODE_24CUG; /* set flip mode  */
    if(horzModeSel && vertiModeSel){
        g_out_packet_buf[3] = SET_FLIP_BOTHFLIP_ENABLE_24CUG ; /* both flip enable */
    }else if(horzModeSel && !vertiModeSel){
        g_out_packet_buf[3] = SET_FLIP_HORZFLIP_24CUG; /* horizontal flip only mode */
    }else if(!horzModeSel && vertiModeSel){
        g_out_packet_buf[3] = SET_FLIP_VERTFLIP_24CUG; /* vertical flip only mode */
    }else
        g_out_packet_buf[3] = SET_FLIP_BOTHFLIP_DISABLE_24CUG; /* both flip disable */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_24CUG &&
            g_in_packet_buf[1]==SET_FLIP_MODE_24CUG &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

bool See3CAM_24CUG::getFaceDetectMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_24CUG; /* camera id */
    g_out_packet_buf[2] = GET_FACE_DETECT_RECT_24CUG; /* get face detect mode command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_24CUG &&
            g_in_packet_buf[1]==GET_FACE_DETECT_RECT_24CUG &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit faceDetectModeValue(g_in_packet_buf[2], g_in_packet_buf[3], g_in_packet_buf[4]);
            return true;
        }
    }
    return false;

}

bool See3CAM_24CUG::setFaceDetectionRect(bool enableFaceDetectRect, bool embedData, bool overlayRect)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_24CUG; /* camera id */
    g_out_packet_buf[2] = SET_FACE_DETECT_RECT_24CUG; /* set face detect Rect command */

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

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_24CUG &&
            g_in_packet_buf[1]==SET_FACE_DETECT_RECT_24CUG &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;

}

bool See3CAM_24CUG::getFrameRateCtrlValue()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_24CUG; /* set camera control code */
    g_out_packet_buf[2] = GET_FRAME_RATE_24CUG; /* set framerate control code */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_24CUG &&
            g_in_packet_buf[1]==GET_FRAME_RATE_24CUG &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit frameRateCtrlValueReceived(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

bool See3CAM_24CUG::setFrameRateCtrlValue(uint frameRate)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_24CUG; /* set camera control code */
    g_out_packet_buf[2] = SET_FRAME_RATE_24CUG; /* set framerate control code */
    g_out_packet_buf[3] = frameRate; /* set framerate value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_24CUG &&
            g_in_packet_buf[1]==SET_FRAME_RATE_24CUG &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;

}

bool See3CAM_24CUG::getExposureCompensation()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_24CUG; /* set camera control code */
    g_out_packet_buf[2] = GET_EXPOSURE_COMPENSATION_24CUG; /* get exposure compensation code */

    unsigned int exposureCompValue;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_24CUG &&
            g_in_packet_buf[1]==GET_EXPOSURE_COMPENSATION_24CUG &&
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

bool See3CAM_24CUG::setExposureCompensation(unsigned int exposureCompValue)
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
    g_out_packet_buf[1] = CAMERA_CONTROL_24CUG; /* camera id */
    g_out_packet_buf[2] = SET_EXPOSURE_COMPENSATION_24CUG; /* set exposure compensation command */
    g_out_packet_buf[3] = (u_int8_t)((exposureCompValue & 0xFF000000) >> 24);
    g_out_packet_buf[4] = (u_int8_t)((exposureCompValue & 0x00FF0000) >> 16);
    g_out_packet_buf[5] = (u_int8_t)((exposureCompValue & 0x0000FF00) >> 8);
    g_out_packet_buf[6] = (u_int8_t)((exposureCompValue & 0x000000FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            emit indicateCommandStatus("Failure", "Failed to set exposure compensation value");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_24CUG &&
            g_in_packet_buf[1]==SET_EXPOSURE_COMPENSATION_24CUG &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            emit indicateCommandStatus("Success", "Exposure compensation value is set successfully");
            return true;
        }
    }
    return false;

}

bool See3CAM_24CUG::setToDefault()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_24CUG; /* camera id */
    g_out_packet_buf[2] = SET_TO_DEFAULT_24CUG; /* set to default command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_24CUG &&
            g_in_packet_buf[1]==SET_TO_DEFAULT_24CUG &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;

}

bool See3CAM_24CUG::getBurstLength()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_24CUG; /* camera id */
    g_out_packet_buf[2] = GET_BURST_LENGTH_24CUG; /* get burst length mode  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_24CUG &&
            g_in_packet_buf[1]==GET_BURST_LENGTH_24CUG &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            emit burstLengthValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;

}

bool See3CAM_24CUG::setBurstLength(uint burstLength)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_24CUG; /* camera id */
    g_out_packet_buf[2] = SET_BURST_LENGTH_24CUG; /* get burst length mode  */
    g_out_packet_buf[3] = burstLength;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_24CUG &&
            g_in_packet_buf[1]==SET_BURST_LENGTH_24CUG &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            emit burstLengthValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

bool See3CAM_24CUG::getSmileDetectMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_24CUG; /* Camera control id */
    g_out_packet_buf[2] = GET_SMILE_DETECTION; /* Get smile detection */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_24CUG &&
            g_in_packet_buf[1]==GET_SMILE_DETECTION &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            emit smileDetectModeValue(g_in_packet_buf[2], g_in_packet_buf[4]);
            return true;
        }
    }
    return false;

}

bool See3CAM_24CUG::setSmileDetection(bool enableSmileDetect, bool embedData)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_24CUG; /* camera id */
    g_out_packet_buf[2] = SET_SMILE_DETECTION; /* set face detect Rect command */

    if(enableSmileDetect)
        g_out_packet_buf[3] = ENABLE_SMILE_DETECT_24CUG; /* enable smile detect */
    else
        g_out_packet_buf[3] = DISABLE_SMILE_DETECT_24CUG; /* disable smile detect */

    if(embedData)
        g_out_packet_buf[5] = ENABLE_EMBED_DATA; /* enable embed data */
    else
        g_out_packet_buf[5] = DISABLE_EMBED_DATA; /* disable embed data */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_24CUG &&
            g_in_packet_buf[1]==SET_SMILE_DETECTION &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;

}

bool See3CAM_24CUG::getFlickerDetection()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_24CUG; /* camera id */
    g_out_packet_buf[2] = GET_FLICKER_CONRTOL_24CUG; /* get flicker detection command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_24CUG &&
            g_in_packet_buf[1]==GET_FLICKER_CONRTOL_24CUG &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit flickerDetectionMode(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;

}

bool See3CAM_24CUG::setFlickerDetection(See3CAM_24CUG::camFlickerMode flickerMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_24CUG; /* camera id */
    g_out_packet_buf[2] = SET_FLICKER_CONTROL_24CUG; /* set flicker detection command */
    g_out_packet_buf[3] = flickerMode; /* flicker detection mode to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_24CUG &&
            g_in_packet_buf[1]==SET_FLICKER_CONTROL_24CUG &&
            g_in_packet_buf[6]==SET_SUCCESS) {

            return true;
        }
    }
    return false;

}

bool See3CAM_24CUG::getFlashState()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_24CUG; /* camera control id */
    g_out_packet_buf[2] = GET_STROBE_CONTROL_24CUG; /* get strobe mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_24CUG &&
            g_in_packet_buf[1]==GET_STROBE_CONTROL_24CUG &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit flashModeValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;

}

bool See3CAM_24CUG::setFlashState(See3CAM_24CUG::flashStateValues flashMode)
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

    g_out_packet_buf[1] = CAMERA_CONTROL_24CUG; /* set camera control code */
    g_out_packet_buf[2] = SET_STROBE_CONTROL_24CUG; /* set flash status command code */
    g_out_packet_buf[3] = flashMode; /* set flash state [off/storbe] */

    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_24CUG &&
            g_in_packet_buf[1]==SET_STROBE_CONTROL_24CUG &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;

}

bool See3CAM_24CUG::getStreamMode()
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
    g_out_packet_buf[1] = CAMERA_CONTROL_24CUG;  /* set camera control code */
    g_out_packet_buf[2] = GET_STREAM_MADE_24CUG ; /* get stream mode code */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_24CUG &&
            g_in_packet_buf[1]==GET_STREAM_MADE_24CUG  &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            streamMode = g_in_packet_buf[2];
            emit streamModeValue(streamMode);
            return true;
        }
    }
    return false;

}

bool See3CAM_24CUG::setStreamMode(See3CAM_24CUG::streamModes streamMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] =CAMERA_CONTROL_24CUG; /* set camera control code */
    g_out_packet_buf[2] = GET_STREAM_MADE_24CUG ; /* set stream mode code */
    g_out_packet_buf[3] = streamMode; /* actual stream mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_24CUG &&
            g_in_packet_buf[1]==GET_STREAM_MADE_24CUG  &&
            g_in_packet_buf[6]==SET_SUCCESS) {

            return true;
        }
    }

    return false;

}
