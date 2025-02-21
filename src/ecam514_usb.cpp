/*
 * ecam514_usb.cpp -- Handling special feature of e-cam_514USB camera
 * Copyright © 2024  e-con Systems India Pvt. Limited
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
#include "ecam514_usb.h"

ECAM514_USB::ECAM514_USB()
{

}

/**
 * @brief ECAM514_USB::resetDevice - To Reset the device
 * @return True if successful, false otherwise.
 */
bool ECAM514_USB::resetDevice()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = RESET_COMMAND_1;
    g_out_packet_buf[2] = RESET_COMMAND_2;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if(g_in_packet_buf[0] == RESET_COMMAND_1 &&
            g_in_packet_buf[1] == RESET_COMMAND_2) {
            emit indicateCommandStatus("Success", "Reset Device Success!");
            return true;
        }
    }
    return false;
}

/**
 * @brief ECAM514_USB::get64BitSerialNumber - To get 64-bit Serial number
 * @return true/false
 */
bool ECAM514_USB::readFirmwareVersion(){

    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    int pMajorVersion = 0;
    int pMinorVersion1 = 0;
    int sdkVersion = 0;
    int svnVersion = 0;

    initializeBuffers();

    //Set the Report Number
    g_out_packet_buf[1] = READ_FIRMWARE_VERSION_ECAM514USB; 	/* Report Number */

    unsigned short int sdk_ver=0, svn_ver=0;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if(g_in_packet_buf[0] == READ_FIRMWARE_VERSION_ECAM514USB){
            sdk_ver = (g_in_packet_buf[3]<<8) | g_in_packet_buf[4];
            svn_ver = (g_in_packet_buf[5]<<8) | g_in_packet_buf[6];

            pMajorVersion = g_in_packet_buf[1];
            pMinorVersion1 = g_in_packet_buf[2];
            sdkVersion = sdk_ver;
            svnVersion = svn_ver;

            _title = "Firmware Version";
            _text.clear();
            _text.append("Version: ");
            _text.append(QString::number(g_in_packet_buf[1]).append("."));  //Firmware version_BYTE_1
            _text.append(QString::number(g_in_packet_buf[2]).append("."));  //Firmware version_BYTE_2
            _text.append(QString::number(sdk_ver).append("."));  //sdk version
            _text.append(QString::number(svn_ver));              //svn version

            emit indicateCommandStatus(_title, _text);
            return true;
        }
    }
    return false;
}

/**
 * @brief ECAM514_USB::get64BitSerialNumber - To get 64-bit Serial number
 * @return true/false
 */
bool ECAM514_USB::get64BitSerialNumber()
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
    _text.append("Serial Number: ");
    _text.append(lsb+msb);
    _title.clear();
    _title = tr("Serial Number");
    emit indicateCommandStatus(_title,_text);
    return true;
}

/**
 * @brief ECAM514_USB::saveConfiguration
 * @return true/false
 */
bool ECAM514_USB::saveConfiguration()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = SAVE_CONFIGURATION_ECAM514USB;
    g_out_packet_buf[2] = SAVE_ECAM514USB;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH))
    {
        qDebug() << "HID command success: " << g_in_packet_buf[6];
        if (g_in_packet_buf[6] == SET_FAIL)
        {
            emit indicateCommandStatus("Failure", "Saving Configurations Failed");
            return false;
        }
        else if(g_in_packet_buf[0] == SAVE_CONFIGURATION_ECAM514USB  &&
            g_in_packet_buf[1] == SAVE_ECAM514USB &&
            g_in_packet_buf[6] == SET_SUCCESS){
            emit indicateCommandStatus("Success", "Configurations saved successfully");
            return true;
        }
    }
    return false;
}

/**
 * @brief ECAM514_USB::setRawStream for MTF focus
 * @return true/false
 */
bool ECAM514_USB::setRawStream(RAW_STREAM rawStream)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = SET_RAW_STREAM_ECAM514USB;
    g_out_packet_buf[2] = rawStream;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH))
    {
        if (g_in_packet_buf[6] == SET_FAIL)
        {
            emit indicateCommandStatus("Failure", "Set Raw Stream Failed");
            return false;
        }
        else if(g_in_packet_buf[0] == SET_RAW_STREAM_ECAM514USB  &&
            g_in_packet_buf[6] == SET_SUCCESS){
            emit indicateCommandStatus("Raw Stream", "Set Raw Stream success");
            return true;
        }
    }
    return false;
}

/**
 * @brief ECAM514_USB::HID image capture
 * @return true/false
 */
bool ECAM514_USB::captureImage()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = HID_IMG_CAPTURE_ECAM514USB;
    g_out_packet_buf[2] = CAPTURE_ECAM514USB;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH))
    {
        if (g_in_packet_buf[6] == SET_FAIL)
        {
            emit indicateCommandStatus("Failure", "Image captured Failed");
            return false;
        }
        else if(g_in_packet_buf[0] == HID_IMG_CAPTURE_ECAM514USB  &&
            g_in_packet_buf[1] == CAPTURE_ECAM514USB &&
            g_in_packet_buf[6] == SET_SUCCESS){
            emit indicateCommandStatus("Success", "Image captured successfully");
            return true;
        }
    }
    return false;
}

/**
 * @brief ECAM514_USB::setChromaDentMode - Setting chroma dent mode in the camera
 * @param chromaDentMode - What chroma dent mode need to set
 * @return true/false
 */
bool ECAM514_USB::setChromaDentMode(CHROMA_DENT_MODE  chromaDentMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_ECAM514USB; /* camera id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_ECAM514USB; /* camera id_2 */
    g_out_packet_buf[3] = SET_CHROMA_DENT_MODE_ECAM514USB; /* set special effect command */
    g_out_packet_buf[4] = chromaDentMode; /* Passing special mode value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_ECAM514USB &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_ECAM514USB &&
            g_in_packet_buf[2] == SET_CHROMA_DENT_MODE_ECAM514USB &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}



/**
 * @brief ECAM514_USB::getChromaDentMode
 * @return true/false
 */
bool ECAM514_USB::getChromaDentMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_ECAM514USB; /* camera id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_ECAM514USB; /* camera id_2 */
    g_out_packet_buf[3] = GET_CHROMA_DENT_MODE_ECAM514USB; /* get special mode command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL){
            return false;
        }
        else if((g_in_packet_buf[0] == CAMERA_CONTROL_ID1_ECAM514USB) &&
                (g_in_packet_buf[1] == CAMERA_CONTROL_ID2_ECAM514USB) &&
                (g_in_packet_buf[2] == GET_CHROMA_DENT_MODE_ECAM514USB) &&
                (g_in_packet_buf[6] == GET_SUCCESS)){
            emit chromaDentModeReceived(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}

/**
 * @brief ECAM514_USB::setDenoiseValue - setting denoise value to the camera
 * @param denoiseVal - denoise value
 * return true - success /false - failure
 */
bool ECAM514_USB::setLedStatus(LED_STATUS ledStatus)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_ECAM514USB; /* camera id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_ECAM514USB; /* camera id_2 */
    g_out_packet_buf[3] = SET_LED_STATUS_ECAM514USB; /* set denoise value command */
    g_out_packet_buf[4] = ledStatus; /* set denoise value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL){
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_ECAM514USB &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_ECAM514USB &&
            g_in_packet_buf[2] == SET_LED_STATUS_ECAM514USB &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}



/**
 * @brief ECAM514_USB::getLedStatus - get led status value from camera
 * return true - success /false - failure
 */
bool ECAM514_USB::getLedStatus()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_ECAM514USB; /* camera id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_ECAM514USB; /* camera id_2 */
    g_out_packet_buf[3] = GET_LED_STATUS_ECAM514USB; /* get led status command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_ECAM514USB &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_ECAM514USB &&
            g_in_packet_buf[2] == GET_LED_STATUS_ECAM514USB &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit ledStatusReceived(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}

/**
 * @brief ECAM_514USB::setBurstLength - set burst length in camera
 * @param burstLength - burst length - no of images to be taken.
 * return true - success /false - failure
 */
bool ECAM514_USB::setBurstLength(uint burstLength){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_ECAM514USB; /* camera id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_ECAM514USB; /* camera id_2 */
    g_out_packet_buf[3] = SET_BURST_LENGTH_ECAM514USB; /* set burst length command */
    g_out_packet_buf[4] = burstLength; /* burst length value to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_ECAM514USB &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_ECAM514USB &&
            g_in_packet_buf[2] == SET_BURST_LENGTH_ECAM514USB &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}



/**
 * @brief ECAM_512USB::getBurstLength - get burst length from camera
 * return true - success /false - failure
 */
bool ECAM514_USB::getBurstLength()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_ECAM514USB; /* camera id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_ECAM514USB; /* camera id_2 */
    g_out_packet_buf[3] = GET_BURST_LENGTH_ECAM514USB; /* get burst length command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_ECAM514USB &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_ECAM514USB &&
            g_in_packet_buf[2] == GET_BURST_LENGTH_ECAM514USB &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit burstLengthValueRecieved(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}

/**
 * @brief ECAM514_USB::setToDefaultValues - set all the values to default in camera
 * @return true/false
 */
bool ECAM514_USB::setToDefaultValues()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_ECAM514USB; /* set camera control id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_ECAM514USB; /* set camera control id_2 */
    g_out_packet_buf[3] = SET_TO_DEFAULT_ECAM514USB; /* Restore default */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH))
    {
        if (g_in_packet_buf[6] == SET_FAIL)
        {
            emit indicateCommandStatus("Failure","Failed to restore default values");
            return false;
        }
        else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_ECAM514USB &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_ECAM514USB &&
            g_in_packet_buf[2] == SET_TO_DEFAULT_ECAM514USB &&
            g_in_packet_buf[6] == SET_SUCCESS){
            emit indicateCommandStatus("Success","Default value restored");
            return true;
        }
    }
    return false;
}

/*
 *
 * @brief ECAM514_USB::initializeBuffers - Initialize input and output buffers
 */
void ECAM514_USB::initializeBuffers(){
    //Initialize input and output buffers
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));
}
