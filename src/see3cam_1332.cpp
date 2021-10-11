#include "see3cam_1332.h"


See3CAM_1332::See3CAM_1332(QObject *parent):QObject(parent)
{
}

void See3CAM_1332::initializeBuffers()
{
    //Initialize input and output buffers
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));
}

/**
 * @brief See3CAM_1332::getSceneMode - getting scene mode from the camera *
 * return true - success /false - failure
 */
bool See3CAM_1332::getSceneMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* camera id */
    g_out_packet_buf[2] = GET_SCENEMODE_1332; /* get scene mode command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH))
    {
        if (g_in_packet_buf[6]==GET_FAIL)
        {
            return false;
        }
        else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&g_in_packet_buf[1]==GET_SCENEMODE_1332 &&
            g_in_packet_buf[6]==GET_SUCCESS)
        {
            emit sceneModeValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_1332::setSceneMode - Setting scene mode in the camera.
 * @ param - enum scene Mode
 * return true - success /false - failure
 */
bool See3CAM_1332::setSceneMode(const See3CAM_1332::sceneModes &sceneMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* Camera control id */
    g_out_packet_buf[2] = SET_SCENEMODE_1332; /* Set scene mode command */
    g_out_packet_buf[3] = sceneMode; /* Scene mode to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&
            g_in_packet_buf[1]==SET_SCENEMODE_1332 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_1332::getEffectMode - getting effect mode from the camera.
 * return true - success /false - failure
 */
bool See3CAM_1332::getEffectMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* set camera control code */
    g_out_packet_buf[2] = GET_SPECIALEFFECT_1332; /* get special effect code */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&
            g_in_packet_buf[1]==GET_SPECIALEFFECT_1332 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
                emit effectModeValue(g_in_packet_buf[2]);
                return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_1332::setEffectMode - Setting effect mode in the camera.
 * param - enum specialEffect
 * return true - success /false - failure
 */
bool See3CAM_1332::setEffectMode(const See3CAM_1332::specialEffects &specialEffect)
{

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* Camera control id */
    g_out_packet_buf[2] = SET_SPECIALEFFECT_1332; /* Set effect mode command */
    g_out_packet_buf[3] = specialEffect; /* Scene mode to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&
            g_in_packet_buf[1]==SET_SPECIALEFFECT_1332 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_1332::getDenoiseValue - get denoise value from camera
 * return true - success /false - failure
 */
bool See3CAM_1332::getDenoiseValue()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* set camera control code */
    g_out_packet_buf[2] = GET_DENOISE_1332; /* get denoise code */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&
            g_in_packet_buf[1]==GET_DENOISE_1332 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit denoiseValueReceived(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_1332::setDenoiseValue - setting denoise value to the camera
 * @param deNoiseVal - denoise value
 * return true - success /false - failure
 */
bool See3CAM_1332::setDenoiseValue(int deNoiseVal)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* set camera control code */
    g_out_packet_buf[2] = SET_DENOISE_1332; /* set denoise control code */
    g_out_packet_buf[3] = deNoiseVal; /* set denoise value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&
            g_in_packet_buf[1]==SET_DENOISE_1332 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_1332::getAutoFocusMode - get auto focus mode from camera
 * return true - success /false - failure
 */
bool See3CAM_1332::getAutoFocusMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* camera id */
    g_out_packet_buf[2] = GET_AF_MODE_1332; /* get auto focus mode command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&
            g_in_packet_buf[1]==GET_AF_MODE_1332 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit afModeValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_1332::setAutoFocusMode - set auto focus mode in camera
 * @param afMode - enum Auto focus mode
 * return true - success /false - failure
 */
bool See3CAM_1332::setAutoFocusMode(camAfMode afMode){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* camera id */
    g_out_packet_buf[2] = SET_AF_MODE_1332; /* set auto focus mode command  */
    g_out_packet_buf[3] = afMode; /* pass af mode value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&
            g_in_packet_buf[1]==SET_AF_MODE_1332 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_1332::getAutoFocusROIModeAndWindowSize - get ROI auto focus mode and window size
 * return true - success /false - failure
 */
bool See3CAM_1332::getAutoFocusROIModeAndWindowSize(){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* camera id */
    g_out_packet_buf[2] = GET_AF_ROI_MODE_1332; /* get AF ROI mode command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&
            g_in_packet_buf[1]==GET_AF_ROI_MODE_1332 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
                emit roiAfModeValue(g_in_packet_buf[2], g_in_packet_buf[5]);
                return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_1332::setROIAutoFocus - Set ROI auto focus to camera
 * @param camROIAfMode - ROI mode
 * @param vidResolnWidth - preview resolution width
 * @param vidResolnHeight - preview resolution height
 * @param xCord - mouse xCordinate from preview
 * @param yCord - mouse yCordinate from preview
 * @param winSize - ROI window size
 * return true - success /false - failure
 */
bool See3CAM_1332::setROIAutoFoucs(camROIAfMode see3camAfROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize)
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

    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* camera control id for camera See3CAM_1332 */
    g_out_packet_buf[2] = SET_AF_ROI_MODE_1332; /* set Auto focus ROI mode command  */
    g_out_packet_buf[3] = see3camAfROIMode; /*  ROI mode which is need to set */

    if(see3camAfROIMode == AFManual){
        g_out_packet_buf[4] = outputXCord;
        g_out_packet_buf[5] = outputYCord;
        g_out_packet_buf[6] = winSize.toUInt();
    }

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&
            g_in_packet_buf[1]==SET_AF_ROI_MODE_1332 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_1332::getAutoExpROIModeAndWindowSize - get ROI auto exposure mode and window size
 * return true - success /false - failure
 */
bool See3CAM_1332::getAutoExpROIModeAndWindowSize(){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* camera id */
    g_out_packet_buf[2] = GET_EXP_ROI_MODE_1332; /* get Auto exposure ROI mode command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&
            g_in_packet_buf[1]==GET_EXP_ROI_MODE_1332 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
                emit roiAutoExpModeValue(g_in_packet_buf[2], g_in_packet_buf[5]);
                return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_1332::setROIAutoExposure - Set ROI auto exposure to camera
 * @param camROIAutoExposureMode - ROI mode
 * @param vidResolnWidth - preview resolution width
 * @param vidResolnHeight - preview resolution height
 * @param xCord - mouse xCordinate from preview
 * @param yCord - mouse yCordinate from preview
 * @param winSize - ROI window size
 * return true - success /false - failure
 */
bool See3CAM_1332::setROIAutoExposure(camROIAutoExpMode see3camAutoexpROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize)
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

    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* camera control id for camera See3CAM_1332 */
    g_out_packet_buf[2] = SET_EXP_ROI_MODE_1332; /* set Auto exposure ROI mode command  */
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
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&
            g_in_packet_buf[1]==SET_EXP_ROI_MODE_1332 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_1332::getBurstLength - get burst length from camera
 * return true - success /false - failure
 */
bool See3CAM_1332::getBurstLength()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* camera id */
    g_out_packet_buf[2] = GET_BURST_LENGTH_1332; /* get burst length command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&
            g_in_packet_buf[1]==GET_BURST_LENGTH_1332 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit burstLengthValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_1332::setBurstLength - set burst length in camera
 * @param burstLength - burst length - no of images to be taken.
 * return true - success /false - failure
 */
bool See3CAM_1332::setBurstLength(uint burstLength){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* camera id */
    g_out_packet_buf[2] = SET_BURST_LENGTH_1332; /* set burst length command */
    g_out_packet_buf[3] = burstLength; /* burst length value to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&
            g_in_packet_buf[1]==SET_BURST_LENGTH_1332 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_1332::getiHDRMode - get iHDR mode from camera
 * return true - success /false - failure
 */
bool See3CAM_1332::getiHDRMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* camera id */
    g_out_packet_buf[2] = GET_HDR_MODE_1332; /* get hdr mode value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&
            g_in_packet_buf[1]==GET_HDR_MODE_1332 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit hDRModeValueReceived(g_in_packet_buf[2], g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_1332::setiHDRMode - set iHDR mode in camera
 * @param iHDRMode - HDR mode
 * @param iHDRValue - value of iHDR for manual mode
 * return true - success /false - failure
 */
bool See3CAM_1332::setiHDRMode(camiHDRMode iHDRMode, uint iHDRValue){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* set camera control code */
    g_out_packet_buf[2] = SET_HDR_MODE_1332; /* set ihdr mode command code */
    g_out_packet_buf[3] = iHDRMode; /* iHdr value */
    if(iHDRMode == HdrManual){
        g_out_packet_buf[4] = iHDRValue;
    }
    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&
            g_in_packet_buf[1]==SET_HDR_MODE_1332 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_1332::getQFactor - get QFactor from camera
 * return true - success /false - failure
 */
bool See3CAM_1332::getQFactor()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* camera id */
    g_out_packet_buf[2] = GET_Q_FACTOR_1332; /* get q factor command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&
            g_in_packet_buf[1]==GET_Q_FACTOR_1332 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            emit qFactorValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_1332::setQFactor - set QFactor in camera
 * @param qFactor.
 * return true - success /false - failure
 */
bool See3CAM_1332::setQFactor(uint qFactor){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* camera id */
    g_out_packet_buf[2] = SET_Q_FACTOR_1332; /* set q factor command */
    g_out_packet_buf[3] = qFactor; /* qfactor value to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&
            g_in_packet_buf[1]==SET_Q_FACTOR_1332 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_1332::getAFRectMode - get AF rectangle mode[ disable/enable ] from camera
 * return true - success /false - failure
 */
bool See3CAM_1332::getAFRectMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* camera id */
    g_out_packet_buf[2] = GET_AF_RECT_MODE_1332; /* get af rect mode command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH))
    {
        if (g_in_packet_buf[6]==GET_FAIL)
        {
            return false;
        }
        else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&
            g_in_packet_buf[1]==GET_AF_RECT_MODE_1332 &&
            g_in_packet_buf[6]==GET_SUCCESS)
        {
            emit afRectModeValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_1332::enableDisableAFRectangle - disable RF rectangle before capture image and enable back after capturing image
 * @param enableRFRect - true / false
 * @return true/false
 */
bool See3CAM_1332::enableDisableAFRectangle(bool enableRFRect){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* camera id */
    g_out_packet_buf[2] = ENABLE_DISABLE_MODE_AF_RECTANGLE_1332; /* enable/disabler AF Rect */
    if(enableRFRect)
        g_out_packet_buf[3] = ENABLE_AF_RECTANGLE_1332; /* value to enable AF Rectangle */
    else
        g_out_packet_buf[3] = DISABLE_AF_RECTANGLE_1332; /* value to disable AF Rectangle*/

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&
            g_in_packet_buf[1]==ENABLE_DISABLE_MODE_AF_RECTANGLE_1332 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_1332::getStreamMode - get Stream mode from camera
 * return true - success /false - failure
 */
bool See3CAM_1332::getStreamMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* camera id */
    g_out_packet_buf[2] = GET_STREAM_MODE_1332; /* get stream mode command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&
            g_in_packet_buf[1]==GET_STREAM_MODE_1332 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit streamModeValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_1332::setStreamMode - Setting  Streaming mode
 * @param streamMode - mode selected in UI
 * @return true/false
 */
bool See3CAM_1332::setStreamMode(camStreamMode streamMode){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* camera id */
    g_out_packet_buf[2] = SET_STREAM_MODE_1332; /* set stream mode command */
    g_out_packet_buf[3] = streamMode; /* stream mode to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&
            g_in_packet_buf[1]==SET_STREAM_MODE_1332 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_1332::setToDefault - set all the values to default in camera
 * @return true/false
 */
bool See3CAM_1332::setToDefault(){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* camera id */
    g_out_packet_buf[2] = SET_TO_DEFAULT_1332; /* set to default command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&
            g_in_packet_buf[1]==SET_TO_DEFAULT_1332 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_1332::getFlipMode - getting flip mode from the camera
 * return true - success /false - failure
 */
bool See3CAM_1332::getFlipMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* camera id */
    g_out_packet_buf[2] = GET_FLIP_MODE_1332; /* get flip mode command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&
            g_in_packet_buf[1]==GET_FLIP_MODE_1332 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
                emit flipMirrorModeChanged(g_in_packet_buf[2]);
                return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_1332::setFlipMode - Setting orientation
*/
bool See3CAM_1332::setFlipMode(bool horzModeSel, bool vertiModeSel)
{

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* camera control for see3cam_1332 camera */
    g_out_packet_buf[2] = SET_FLIP_MODE_1332; /* set flip mode  */
    if(horzModeSel && vertiModeSel){
        g_out_packet_buf[3] = SET_FLIP_BOTHFLIP_ENABLE_1332 ; /* both flip enable */
    }else if(horzModeSel && !vertiModeSel){
        g_out_packet_buf[3] = SET_FLIP_HORZFLIP_1332; /* horizontal flip only mode */
    }else if(!horzModeSel && vertiModeSel){
        g_out_packet_buf[3] = SET_FLIP_VERTFLIP_1332; /* vertical flip only mode */
    }else
        g_out_packet_buf[3] = SET_FLIP_BOTHFLIP_DISABLE_1332; /* both flip disable */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&
            g_in_packet_buf[1]==SET_FLIP_MODE_1332 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_1332::getFactDetectMode - get face detect mode[ disable/enable ] from camera
 * return true - success /false - failure
 */
bool See3CAM_1332::getFaceDetectMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* camera id */
    g_out_packet_buf[2] = GET_FACE_DETECT_RECT; /* get face detect mode command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&
            g_in_packet_buf[1]==GET_FACE_DETECT_RECT &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit faceDetectModeValue(g_in_packet_buf[2], g_in_packet_buf[3], g_in_packet_buf[4]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_1332::setFaceDetectionRect - setting face detection rectangle
 * @param enableFaceDetectRect - enable / disable face detect rectangle
 * @param embedData - Enable / Disable embed data
 * @param overlayRect - Enable / Disable overlay Rectangle
 * @return true/false
 */
bool See3CAM_1332::setFaceDetectionRect(bool enableFaceDetectRect, bool embedData, bool overlayRect){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* camera id */
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

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&
            g_in_packet_buf[1]==SET_FACE_DETECT_RECT &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_1332::getSmileDetectMode - get smile detect mode[ disable/enable ] from camera
 * return true - success /false - failure
 */
bool See3CAM_1332::getSmileDetectMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* Camera control id */
    g_out_packet_buf[2] = GET_SMILE_DETECTION; /* Get smile detection */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&
            g_in_packet_buf[1]==GET_SMILE_DETECTION &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit smileDetectModeValue(g_in_packet_buf[2], g_in_packet_buf[4]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_1332::setSmileDetection - setting smile detection rectangle
 * @param enableSmileDetect - enable / disable smile detect
 * @param embedData - Enable / Disable embed data
 * @return true/false
 */
bool See3CAM_1332::setSmileDetection(bool enableSmileDetect, bool embedData){

    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* camera id */
    g_out_packet_buf[2] = SET_SMILE_DETECTION; /* set smile detect command */

    if(enableSmileDetect)
        g_out_packet_buf[3] = ENABLE_SMILE_DETECT; /* enable smile detect */
    else
        g_out_packet_buf[3] = DISABLE_SMILE_DETECT; /* disable smile detect */

    if(embedData)
        g_out_packet_buf[5] = ENABLE_EMBED_DATA; /* enable embed data */
    else
        g_out_packet_buf[5] = DISABLE_EMBED_DATA; /* disable embed data */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&
            g_in_packet_buf[1]==SET_SMILE_DETECTION &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_1332::getExposureCompensation - getting exposure compensation
 * @return true/false
 */
bool See3CAM_1332::getExposureCompensation(){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* set camera control code */
    g_out_packet_buf[2] = GET_EXPOSURE_COMPENSATION; /* get exposure compensation code */

    unsigned int exposureCompValue;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&
            g_in_packet_buf[1]==GET_EXPOSURE_COMPENSATION &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            exposureCompValue = (((u_int8_t)g_in_packet_buf[2]) << 24)
                                | (((u_int8_t)g_in_packet_buf[3]) << 16)
                                | (((u_int8_t)g_in_packet_buf[4]) << 8)
                                | (((u_int8_t)g_in_packet_buf[5]) << 0);
            exposureCompValueReceived(exposureCompValue);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_1332::setExposureCompensation - setting exposure compensation
 * @param exposureCompValue - exposure compensation value
 * @return true/false
 */
bool See3CAM_1332::setExposureCompensation(unsigned int exposureCompValue){

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
    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* camera id */
    g_out_packet_buf[2] = SET_EXPOSURE_COMPENSATION; /* set exposure compensation command */
    g_out_packet_buf[3] = (u_int8_t)((exposureCompValue & 0xFF000000) >> 24);
    g_out_packet_buf[4] = (u_int8_t)((exposureCompValue & 0x00FF0000) >> 16);
    g_out_packet_buf[5] = (u_int8_t)((exposureCompValue & 0x0000FF00) >> 8);
    g_out_packet_buf[6] = (u_int8_t)((exposureCompValue & 0x000000FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            emit indicateCommandStatus("Failure", "Failed to set exposure compensation value");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&
            g_in_packet_buf[1]==SET_EXPOSURE_COMPENSATION &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            emit indicateCommandStatus("Success", "Exposure compensation value is set successfully");
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_1332::getFrameRateCtrlValue - get frameRate control value from camera
 * return true - success /false - failure
 */
bool See3CAM_1332::getFrameRateCtrlValue()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* set camera control code */
    g_out_packet_buf[2] = GET_FRAME_RATE_1332; /* set framerate control code */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&
            g_in_packet_buf[1]==GET_FRAME_RATE_1332 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit frameRateCtrlValueReceived(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_1332::setFrameRatectrlValue - setting frame rate control value
 * @param frameRate
 * @return true/false
 */
bool See3CAM_1332::setFrameRateCtrlValue(uint frameRate)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* set camera control code */
    g_out_packet_buf[2] = SET_FRAME_RATE_1332; /* set framerate control code */
    g_out_packet_buf[3] = frameRate; /* set framerate value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&
            g_in_packet_buf[1]==SET_FRAME_RATE_1332 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_1332::enableDisableFaceRectangle - disable RF rectangle before capture image and enable back after capturing image
 * and disable before recording video and enable back.
 * @param enableRFRect - true / false
 * @return true/false
 */
bool See3CAM_1332::enableDisableFaceRectangle(bool enableFaceRect){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* camera id */
    g_out_packet_buf[2] = ENABLE_DISABLE_MODE_AF_RECTANGLE_1332; /* pass enable/disable command */
    if(enableFaceRect)
        g_out_packet_buf[3] = ENABLE_AF_RECTANGLE_1332; /* enable auto focus rect */
    else
        g_out_packet_buf[3] = DISABLE_AF_RECTANGLE_1332; /* disable auto focus rect */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&
            g_in_packet_buf[1]==ENABLE_DISABLE_MODE_AF_RECTANGLE_1332 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool See3CAM_1332::getAntiFlickerMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* camera id */
    g_out_packet_buf[2] = GET_ANTIFLICKER_1332;/* get anti flicker command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&
            g_in_packet_buf[1] == GET_ANTIFLICKER_1332 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit antiFlickerModeChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;

}

bool See3CAM_1332::setAntiFlickerMode(See3CAM_1332::camAntiFlickerMode antiFlickerMode)
{

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* camera id */
    g_out_packet_buf[2] = SET_ANTIFLICKER_1332; /* set anti flicker command  */
    g_out_packet_buf[3] = antiFlickerMode; /* anti flicker mode to set */
    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&
            g_in_packet_buf[1] == SET_ANTIFLICKER_1332 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
}

bool See3CAM_1332::enable_disablerect(bool value)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_1332; /* camera id */
    g_out_packet_buf[2] = ENABLE_DISABLE_AF_RECT;
    g_out_packet_buf[3] = value;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_1332 &&
            g_in_packet_buf[1] == ENABLE_DISABLE_AF_RECT &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
}

