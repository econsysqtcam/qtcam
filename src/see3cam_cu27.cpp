#include "see3cam_cu27.h"

//See3CAM_CU27::See3CAM_CU27(QObject *parent):QObject(parent)
//{
//    qDebug() << Q_FUNC_INFO;
//}

See3CAM_CU27::See3CAM_CU27()
{
}

See3CAM_CU27::~See3CAM_CU27()
{

}

void See3CAM_CU27::initializeBuffers()
{
    //Initialize input and output buffers
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));
}

/**
 * @brief See3CAM_CU27::getQFactor - get QFactor from camera
 * return true - success /false - failure
 */
bool See3CAM_CU27::getQFactor()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU27; /* camera id */
    g_out_packet_buf[2] = GET_Q_FACTOR_CU27; /* get q factor command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU27 &&
            g_in_packet_buf[1]==GET_Q_FACTOR_CU27 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit qFactorValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_CU27::setQFactor - set QFactor in camera
 * @param qFactor.
 * return true - success /false - failure
 */
bool See3CAM_CU27::setQFactor(uint qFactor){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU27; /* camera id */
    g_out_packet_buf[2] = SET_Q_FACTOR_CU27; /* set q factor command */
    g_out_packet_buf[3] = qFactor; /* qfactor value to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU27 &&
            g_in_packet_buf[1]==SET_Q_FACTOR_CU27 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU27::getFlickerMode - getting flicker mode
 * @return true/false
 */

bool See3CAM_CU27::getFlickerMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU27; /* camera id */
    g_out_packet_buf[2] = GET_FLICKER_DETECTION_CU27; /* get flicker detection command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU27 &&
            g_in_packet_buf[1]==GET_FLICKER_DETECTION_CU27 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit flickerModeChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;

}

/**
 * @brief See3CAM_CU27::setFlickerMode - setting flicker mode
 * @param flickerMode - Mode we are going to set
 * @return true/false
 */

bool See3CAM_CU27::setFlickerMode(camFlickerMode flickerMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU27; /* camera id */
    g_out_packet_buf[2] = SET_FLICKER_DETECTION_CU27; /* set flicker detection command */
    g_out_packet_buf[3] = flickerMode; /* flicker detection mode to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU27 &&
            g_in_packet_buf[1]==SET_FLICKER_DETECTION_CU27 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;

}

/**
 * @brief See3CAM_CU27::getAElockstatus - Getting current Auto exposure lock value
 * @return true/false
 */
bool See3CAM_CU27::getAElockstatus()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU27; /* camera id */
    g_out_packet_buf[2] = GET_AE_LOCK_STATUS_CU27; /* get ae lock status command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU27 &&
            g_in_packet_buf[1]==GET_AE_LOCK_STATUS_CU27 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit aeLockStatusChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU27::setAElockstatus - Setting Auto exposure lock - If it is true, we cant change auto exposure effect
 * @param lockeStatus - true/false
 * @return true/false
 */
bool See3CAM_CU27::setAElockstatus(aeLockStatus lockStatus)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU27; /* camera id */
    g_out_packet_buf[2] = SET_AE_LOCK_STATUS_CU27; /* set ae lock status command */
    g_out_packet_buf[3] = lockStatus; /* ae lock status mode to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU27 &&
            g_in_packet_buf[1]==SET_AE_LOCK_STATUS_CU27 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU27::getAEMeterMode - Getting AE metering mode value from camera
 * return true/false
 */
bool See3CAM_CU27::getAEMeterMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU27; /* camera id */
    g_out_packet_buf[2] = GET_AE_METERING_MODE_CU27; /* get ae metering mode command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU27 &&
            g_in_packet_buf[1]==GET_AE_METERING_MODE_CU27 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit aeMeterModeChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU27::setAEMeterMode - Setting Auto Exposure meter mode control
 * @param meterMode - auto exposure meter mode
 * @return true/false
 */
bool See3CAM_CU27::setAEMeterMode(aeMeteringMode meterMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU27; /* camera id */
    g_out_packet_buf[2] = SET_AE_METERING_MODE_CU27; /* set ae metering mode command */
    g_out_packet_buf[3] = meterMode; /* ae meter mode to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU27 &&
            g_in_packet_buf[1]==SET_AE_METERING_MODE_CU27 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU27::getAWBlockstatus - Getting current Auto whitebalance lock value
 * @return true/false
 */
bool See3CAM_CU27::getAWBlockstatus()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU27; /* camera id */
    g_out_packet_buf[2] = GET_AWB_LOCKSTATUS_CU27; /* get awb lock status command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU27 &&
            g_in_packet_buf[1]==GET_AWB_LOCKSTATUS_CU27 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit awbLockStatusChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU27::setAWBlockstatus - Setting Auto whitebalance lock - If it is true, we cant change auto white balance effect
 * @param lockStatus - awb lockstatus from UI(on/off)
 * @return true/false
 */
bool See3CAM_CU27::setAWBlockstatus(awbLockStatus lockStatus)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU27; /* camera id */
    g_out_packet_buf[2] = SET_AWB_LOCKSTATUS_CU27; /* set awb lock status command */
    g_out_packet_buf[3] = lockStatus; /* awb lock status mode to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU27 &&
            g_in_packet_buf[1]==SET_AWB_LOCKSTATUS_CU27 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU27::getAWBPresetMode - Getting AWB preset mode value
 * @return true/false
 */
bool See3CAM_CU27::getAWBpresetMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU27; /* camera id */
    g_out_packet_buf[2] = GET_AWB_MODE_CU27; /* get awb preset mode command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU27 &&
            g_in_packet_buf[1]==GET_AWB_MODE_CU27 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit awbPresetModeChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU27::setAWBPresetControl - Setting Auto whitebalance preset control
 * @param awbMode - auto white balance mode
 * @return true/false
 */
bool See3CAM_CU27::setAWBpresetMode(awbPresetMode awbMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU27; /* camera id */
    g_out_packet_buf[2] = SET_AWB_MODE_CU27; /* set awb preset mode command */
    g_out_packet_buf[3] = awbMode; /* awb mode to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU27 &&
            g_in_packet_buf[1]==SET_AWB_MODE_CU27 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU27::getFirmwareVersion - getting ISP firmware version
 * @return true/false
 */
bool See3CAM_CU27::getFirmwareVersion()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU27; /* camera id */
    g_out_packet_buf[2] = READ_FIRMWARE_VERSION_CU27; /* read firmware version command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU27 &&
            g_in_packet_buf[1] == READ_FIRMWARE_VERSION_CU27 &&
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
 * @brief See3CAM_CU27::setToDefault - set all the values to default in camera
 * @return true/false
 */
bool See3CAM_CU27::setToDefault(){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU27; /* camera id */
    g_out_packet_buf[2] = SET_TO_DEFAULT_CU27; /* set to default command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU27 &&
            g_in_packet_buf[1]==SET_TO_DEFAULT_CU27 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU27::setBurstLength - set burst length in camera
 * @param burstLength - burst length - no of images to be taken.
 * return true - success /false - failure
 */
bool See3CAM_CU27::setBurstLength(uint burstLength){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU27; /* camera id */
    g_out_packet_buf[2] = SET_BURST_LENGTH_CU27; /* set burst length command */
    g_out_packet_buf[3] = burstLength; /* burst length value to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU27 &&
            g_in_packet_buf[1]==SET_BURST_LENGTH_CU27 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU27::getBurstLength - get burst length from camera
 * return true - success /false - failure
 */
bool See3CAM_CU27::getBurstLength()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU27; /* camera id */
    g_out_packet_buf[2] = GET_BURST_LENGTH_CU27; /* get burst length command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU27 &&
            g_in_packet_buf[1]==GET_BURST_LENGTH_CU27 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit burstLengthValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}
