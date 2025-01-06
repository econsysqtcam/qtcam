#include "see3cam_160.h"


See3CAM_160::See3CAM_160(QObject *parent):QObject(parent)
{
}

void See3CAM_160::initializeBuffers()
{
    //Initialize input and output buffers
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));
}

/**
 * @brief See3CAM_160::getAFstatus - set auto focus status(manual/auto)
 * return true - success /false - failure
 */
bool See3CAM_160::getAFstatus()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_160; /* camera id */
    g_out_packet_buf[2] = GET_AF_STATUS_160; /* get auto focus status command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_160 &&
            g_in_packet_buf[1]==GET_AF_STATUS_160 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit afStatusChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_160::setAFstatus - set auto status(manual/auto)
 * @param afMode - enum Auto focus status
 * return true - success /false - failure
 */
bool See3CAM_160::setAFstatus(afStatus afstatus)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_160; /* camera id */
    g_out_packet_buf[2] = SET_AF_STATUS_160; /* set auto focus status command  */
    g_out_packet_buf[3] = afstatus; /* pass af status value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_160 &&
            g_in_packet_buf[1]==SET_AF_STATUS_160 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_160::getManualLensPositionMode - Getting manual lens position mode value from camera
 * return true/false
 */
bool See3CAM_160::getManualLensPositionMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_160; /* camera id */
    g_out_packet_buf[2] = GET_LENS_POSITION_160; /* get manual lens position mode command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_160 &&
            g_in_packet_buf[1]==GET_LENS_POSITION_160 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit manualLensPositionModeChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_160::setManualLensPositionMode - Setting manual lens position mode
 * @param lensPosition - lens position mode
 * @return true/false
 */
bool See3CAM_160::setManualLensPositionMode(manualLensPositionMode lensPosition)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_160; /* camera id */
    g_out_packet_buf[2] = SET_LENS_POSITION_160; /* set manual lens position mode command  */
    g_out_packet_buf[3] = lensPosition; /* pass manual lens position mode value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_160 &&
            g_in_packet_buf[1]==SET_LENS_POSITION_160 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_160::getCAFScanRangeMode - Getting Scan Range mode value from camera
 * @return true/false
 */
bool See3CAM_160::getCAFScanRangeMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_160; /* camera id */
    g_out_packet_buf[2] = GET_CAF_RANGE_160; /* get caf range mode command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_160 &&
            g_in_packet_buf[1]==GET_CAF_RANGE_160 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit cafScanRangeModeChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_160::setCAFScanRangeMode - Setting scan Range mode control
 * @param cafMode - caf scan Range mode
 * @return true/false
 */
bool See3CAM_160::setCAFScanRangeMode(cafScanRangeMode cafMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_160; /* camera id */
    g_out_packet_buf[2] = SET_CAF_RANGE_160; /* set caf scan range mode command  */
    g_out_packet_buf[3] = cafMode; /* pass caf scan range mode value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_160 &&
            g_in_packet_buf[1]==SET_CAF_RANGE_160 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_160::getAFLockStatus - Get Auto Focus lock mode
 * @return true/false
 */
bool See3CAM_160::getAFlockstatus()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_160; /* camera id */
    g_out_packet_buf[2] = GET_AF_LOCKSTATUS_160; /* get auto focus lock mode command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_160 &&
            g_in_packet_buf[1]==GET_AF_LOCKSTATUS_160 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit afLockStatusChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_160::setAFLockstatus - set Auto focus lock status mode
 * @param lockstatus - select/unselect in UI
 * @return true/false
 */
bool See3CAM_160::setAFlockstatus(afLockStatus lockStatus)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_160; /* camera id */
    g_out_packet_buf[2] = SET_AF_LOCKSTATUS_160; /* set auto focus lock mode command  */
    g_out_packet_buf[3] = lockStatus; /* pass af lock mode value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_160 &&
            g_in_packet_buf[1]==SET_AF_LOCKSTATUS_160 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_160::getAutoFocusMode - get auto focus mode from camera
 * return true - success /false - failure
 */
bool See3CAM_160::getAutoFocusMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_160; /* camera id */
    g_out_packet_buf[2] = GET_AF_MODE_160; /* get auto focus mode command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_160 &&
            g_in_packet_buf[1]==GET_AF_MODE_160 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit afModeChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_160::setAutoFocusMode - set auto focus mode in camera
 * @param afMode - enum Auto focus mode
 * return true - success /false - failure
 */
bool See3CAM_160::setAutoFocusMode(autoFocusModes afMode){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_160; /* camera id */
    g_out_packet_buf[2] = SET_AF_MODE_160; /* set auto focus mode command  */
    g_out_packet_buf[3] = afMode; /* pass af mode value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_160 &&
            g_in_packet_buf[1]==SET_AF_MODE_160 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_160::getAutoFocusROIMode - get Auto Focus ROI mode
 * @return true/false
 */
bool See3CAM_160::getAutoFocusROIMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_160; /* camera id */
    g_out_packet_buf[2] = GET_AF_ROI_MODE_160; /* get auto focus roi command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_160 &&
            g_in_packet_buf[1]==GET_AF_ROI_MODE_160 &&
            ((g_in_packet_buf[2]==CENTER_WEIGHTED_MODE_160 && g_in_packet_buf[7]==GET_SUCCESS) ||
             (g_in_packet_buf[2]==CUSTOM_AREA_MODE_160 && g_in_packet_buf[7]==GET_SUCCESS))) {
            emit autoFocusRoiModeChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_160::setCenteredAutoFocusMode - Setting centered auto focus mode
 * @return true/false
 */
bool See3CAM_160::setCenteredAutoFocusMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_160; /* camera id */
    g_out_packet_buf[2] = SET_AF_ROI_MODE_160; /* set ROI command */
    g_out_packet_buf[3] = CENTER_WEIGHTED_MODE_160; /* value to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_160 &&
            g_in_packet_buf[1]==SET_AF_ROI_MODE_160 &&
            g_in_packet_buf[2]==CENTER_WEIGHTED_MODE_160 &&
            g_in_packet_buf[7]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_160::setCustomAreaAutoFocus - Setting custom area auto focus area
 * @param vidResolnWidth - preview resolution width
 * @param vidResolnHeight - preview resolution height
 * @param xCord  - X co-ordinate
 * @param yCord  - Y co-ordinate
 * @param winSize - window size
 * @return true/false
 */
bool See3CAM_160::setCustomAreaAutoFocus(int vidResolnWidth, int vidResolnHeight, uint xCord, uint yCord, QString winSize)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    unsigned int calculatedWinWidth =  (vidResolnWidth * winSize.toUInt()) / MAX_WINDOW_SIZE;
    unsigned int calculatedWinHeight =  (vidResolnHeight * winSize.toUInt()) / MAX_WINDOW_SIZE;

    /* calculate focus window coordinates based on mouseclick point and window size */
    int focusWinXmin = xCord - calculatedWinWidth/2;
    int focusWinYmin = yCord - calculatedWinHeight/2;
    int focusWinXmax = xCord + calculatedWinWidth/2;
    int focusWinYmax = yCord + calculatedWinHeight/2;

    //  if resultant calculated focus window co-ordinates values lie less than zero , set to zero
    if(0 > focusWinXmin){ focusWinXmin = 0; }
    if(0 > focusWinYmin){ focusWinYmin = 0; }

    if(0 > focusWinXmax){ focusWinXmax = 0; }
    if(0 > focusWinYmax){ focusWinYmax = 0; }

    //  if resultant calculated focus window co-ordinates x,y values lie  greater than preview width, height respectively ,
    //  set the x,y corrdinate values to width, height respectively
    if(vidResolnWidth < focusWinXmax)   {  focusWinXmax = vidResolnWidth; }
    if(vidResolnHeight < focusWinYmax)  {  focusWinYmax = vidResolnHeight; }

    if(vidResolnWidth < focusWinXmin)   {  focusWinXmin = vidResolnWidth; }
    if(vidResolnHeight < focusWinYmin)  {  focusWinYmin = vidResolnHeight; }

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_160; /* camera id */
    g_out_packet_buf[2] = SET_AF_ROI_MODE_160; /* get auto focus roi command */
    g_out_packet_buf[3] = CUSTOM_AREA_MODE_160; /* value to set */

    g_out_packet_buf[4] = focusWinXmax;
    g_out_packet_buf[5] = focusWinXmax >> 8;
    g_out_packet_buf[6] = focusWinYmin;
    g_out_packet_buf[7] = focusWinYmin >> 8;
    g_out_packet_buf[8] = focusWinXmin;
    g_out_packet_buf[9] = focusWinXmin >> 8;
    g_out_packet_buf[10] = focusWinYmax;
    g_out_packet_buf[11] = focusWinYmax >> 8;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_160 &&
            g_in_packet_buf[1]==SET_AF_ROI_MODE_160 &&
            g_in_packet_buf[2]==CUSTOM_AREA_MODE_160 &&
            g_in_packet_buf[7]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_160::getQFactor - get QFactor from camera
 * return true - success /false - failure
 */
bool See3CAM_160::getQFactor()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_160; /* camera id */
    g_out_packet_buf[2] = GET_Q_FACTOR_160; /* get q factor command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_160 &&
            g_in_packet_buf[1]==GET_Q_FACTOR_160 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit qFactorValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_160::setQFactor - set QFactor in camera
 * @param qFactor.
 * return true - success /false - failure
 */
bool See3CAM_160::setQFactor(uint qFactor){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_160; /* camera id */
    g_out_packet_buf[2] = SET_Q_FACTOR_160; /* set q factor command */
    g_out_packet_buf[3] = qFactor; /* qfactor value to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_160 &&
            g_in_packet_buf[1]==SET_Q_FACTOR_160 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_160::getFlickerMode - getting flicker mode
 * @return true/false
 */

bool See3CAM_160::getFlickerMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_160; /* camera id */
    g_out_packet_buf[2] = GET_FLICKER_DETECTION_160; /* get flicker detection command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_160 &&
            g_in_packet_buf[1]==GET_FLICKER_DETECTION_160 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit flickerModeChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;

}

/**
 * @brief See3CAM_160::setFlickerMode - setting flicker mode
 * @param flickerMode - Mode we are going to set
 * @return true/false
 */

bool See3CAM_160::setFlickerMode(camFlickerMode flickerMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_160; /* camera id */
    g_out_packet_buf[2] = SET_FLICKER_DETECTION_160; /* set flicker detection command */
    g_out_packet_buf[3] = flickerMode; /* flicker detection mode to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_160 &&
            g_in_packet_buf[1]==SET_FLICKER_DETECTION_160 &&
            g_in_packet_buf[6]==SET_SUCCESS) {

            return true;
        }
    }
    return false;

}

/**
 * @brief See3CAM_160::getAElockstatus - Getting current Auto exposure lock value
 * @return true/false
 */
bool See3CAM_160::getAElockstatus()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_160; /* camera id */
    g_out_packet_buf[2] = GET_AE_LOCK_STATUS_160; /* get ae lock status command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_160 &&
            g_in_packet_buf[1]==GET_AE_LOCK_STATUS_160 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit aeLockStatusChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_160::setAElockstatus - Setting Auto exposure lock - If it is true, we cant change auto exposure effect
 * @param lockeStatus - true/false
 * @return true/false
 */
bool See3CAM_160::setAElockstatus(aeLockStatus lockStatus)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_160; /* camera id */
    g_out_packet_buf[2] = SET_AE_LOCK_STATUS_160; /* set ae lock status command */
    g_out_packet_buf[3] = lockStatus; /* ae lock status mode to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_160 &&
            g_in_packet_buf[1]==SET_AE_LOCK_STATUS_160 &&
            g_in_packet_buf[6]==SET_SUCCESS) {

            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_160::getAEMeterMode - Getting AE metering mode value from camera
 * return true/false
 */
bool See3CAM_160::getAEMeterMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_160; /* camera id */
    g_out_packet_buf[2] = GET_AE_METERING_MODE_160; /* get ae metering mode command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_160 &&
            g_in_packet_buf[1]==GET_AE_METERING_MODE_160 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit aeMeterModeChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_160::setAEMeterMode - Setting Auto Exposure meter mode control
 * @param meterMode - auto exposure meter mode
 * @return true/false
 */
bool See3CAM_160::setAEMeterMode(aeMeteringMode meterMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_160; /* camera id */
    g_out_packet_buf[2] = SET_AE_METERING_MODE_160; /* set ae metering mode command */
    g_out_packet_buf[3] = meterMode; /* ae meter mode to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_160 &&
            g_in_packet_buf[1]==SET_AE_METERING_MODE_160 &&
            g_in_packet_buf[6]==SET_SUCCESS) {

            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_160::getAWBlockstatus - Getting current Auto whitebalance lock value
 * @return true/false
 */
bool See3CAM_160::getAWBlockstatus()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_160; /* camera id */
    g_out_packet_buf[2] = GET_AWB_LOCKSTATUS_160; /* get awb lock status command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_160 &&
            g_in_packet_buf[1]==GET_AWB_LOCKSTATUS_160 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit awbLockStatusChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_160::setAWBlockstatus - Setting Auto whitebalance lock - If it is true, we cant change auto white balance effect
 * @param lockStatus - awb lockstatus from UI(on/off)
 * @return true/false
 */
bool See3CAM_160::setAWBlockstatus(awbLockStatus lockStatus)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_160; /* camera id */
    g_out_packet_buf[2] = SET_AWB_LOCKSTATUS_160; /* set awb lock status command */
    g_out_packet_buf[3] = lockStatus; /* awb lock status mode to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_160 &&
            g_in_packet_buf[1]==SET_AWB_LOCKSTATUS_160 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_160::getAWBPresetMode - Getting AWB preset mode value
 * @return true/false
 */
bool See3CAM_160::getAWBpresetMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_160; /* camera id */
    g_out_packet_buf[2] = GET_AWB_MODE_160; /* get awb preset mode command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_160 &&
            g_in_packet_buf[1]==GET_AWB_MODE_160 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit awbPresetModeChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_160::setAWBPresetControl - Setting Auto whitebalance preset control
 * @param awbMode - auto white balance mode
 * @return true/false
 */
bool See3CAM_160::setAWBpresetMode(awbPresetMode awbMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_160; /* camera id */
    g_out_packet_buf[2] = SET_AWB_MODE_160; /* set awb preset mode command */
    g_out_packet_buf[3] = awbMode; /* awb mode to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_160 &&
            g_in_packet_buf[1]==SET_AWB_MODE_160 &&
            g_in_packet_buf[6]==SET_SUCCESS) {

            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_160::getFirmwareVersion - getting ISP firmware version
 * @return true/false
 */
bool See3CAM_160::getFirmwareVersion()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_160; /* camera id */
    g_out_packet_buf[2] = READ_FIRMWARE_VERSION_160; /* read firmware version command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_160 &&
            g_in_packet_buf[1] == READ_FIRMWARE_VERSION_160 &&
            g_in_packet_buf[6] == GET_SUCCESS) {

            _title = "Firmware Version";
            _text.clear();
            _text.append("ISP Firmware Version: ");
            _text.append(QString::number(g_in_packet_buf[2]).append("."));  //ISP firmware version_BYTE_1
            _text.append(QString::number(g_in_packet_buf[3]).append("."));  //ISP firmware version_BYTE_2
            _text.append(QString::number(g_in_packet_buf[4]).append("."));  //ISP firmware version_BYTE_3
            _text.append(QString::number(g_in_packet_buf[2]));              //ISP firmware version_BYTE_4

            emit titleTextChanged(_title, _text);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_160::setToDefault - set all the values to default in camera
 * @return true/false
 */
bool See3CAM_160::setToDefault(){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_160; /* camera id */
    g_out_packet_buf[2] = SET_TO_DEFAULT_160; /* set to default command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_160 &&
            g_in_packet_buf[1]==SET_TO_DEFAULT_160 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_160::setBurstLength - set burst length in camera
 * @param burstLength - burst length - no of images to be taken.
 * return true - success /false - failure
 */
bool See3CAM_160::setBurstLength(uint burstLength){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_160; /* camera id */
    g_out_packet_buf[2] = SET_BURST_LENGTH_160; /* set burst length command */
    g_out_packet_buf[3] = burstLength; /* burst length value to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_160 &&
            g_in_packet_buf[1]==SET_BURST_LENGTH_160 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_160::getBurstLength - get burst length from camera
 * return true - success /false - failure
 */
bool See3CAM_160::getBurstLength()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_160; /* camera id */
    g_out_packet_buf[2] = GET_BURST_LENGTH_160; /* get burst length command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_160 &&
            g_in_packet_buf[1]==GET_BURST_LENGTH_160 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit burstLengthValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_130D::get64BitSerialNumber - To get 64-bit Serial number
 * @return true/false
 */
bool See3CAM_160::get64BitSerialNumber()
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
    g_out_packet_buf[1] = GET_64BIT_UNIQUE_ID_1; 	/* Report Number */

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
            if(g_in_packet_buf[0] == GET_64BIT_UNIQUE_ID_1)
            {
                lsb.sprintf("%02x%02x%02x%02x",g_in_packet_buf[1],g_in_packet_buf[2],g_in_packet_buf[3],g_in_packet_buf[4]);

                //Initialize the buffer
                memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

                //Set the Report Number
                g_out_packet_buf[1] = GET_64BIT_UNIQUE_ID_1;
                g_out_packet_buf[2] = GET_64BIT_UNIQUE_ID_2;

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
                        if((g_in_packet_buf[0] == GET_64BIT_UNIQUE_ID_1))
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
    _title.clear();
    _title = tr("Serial Number");
    emit titleTextChanged(_title,_text);
    return true;
}
