/*
 * seecam_11cug.cpp -- Handling special feature of seecam11cug camera
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

#include "seecam_11cug.h"

bool See3CAM_11CUG::enableMasterMode()
{
    bool ret = modeControl.enableMasterMode();
    return ret;
}

bool  See3CAM_11CUG::enableTriggerMode()
{
    bool ret = modeControl.enableTriggerMode();
    return ret;
}

void See3CAM_11CUG::setCroppedVGAMode()
{
    bool ret = false;
    u_int8_t cropped_vga_status = 0;
    ret = enableCroppedVGAMode(&cropped_vga_status);
    if(!ret)
    {
        emit deviceStatus("Failure", "Unable to switch to cropped VGA Mode");
        return void();
    }
    else
    {
        switch(cropped_vga_status)
        {
        case 1 :            
            emit deviceStatus("Success","Cropped VGA mode set successfully");
            break;
        case 2 :            
            emit deviceStatus("Failure","The current resolution is not 640x480, please switch to 640x480 before using the Cropping and Binning modes");
            break;
        case 3 :            
            emit deviceStatus("Failure","Device is already in Cropped VGA mode");
            break;
        case 4 :            
            emit deviceStatus("Failure","Failed to set Cropped VGA mode");
            break;
        default :            
            emit deviceStatus("Failure","Unknown error");

        }
    }
}

bool See3CAM_11CUG::enableCroppedVGAMode(u_int8_t *VGAStatus)
{
    *VGAStatus = 0;
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
    g_out_packet_buf[1] = ENABLE_CROPPED_VGA_MODE; /* Report Number */

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
            if(g_in_packet_buf[0] == ENABLE_CROPPED_VGA_MODE) {
                *VGAStatus = g_in_packet_buf[1];
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

bool See3CAM_11CUG::enableBinnedVGAMode(u_int8_t *VGAStatus)
{
    *VGAStatus = 0;

    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    bool timeout = true;
    int ret = 0;
    unsigned int start, end = 0;

    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = ENABLE_BINNED_VGA_MODE; /* Report Number */

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
            if(g_in_packet_buf[0] == ENABLE_BINNED_VGA_MODE) {
                *VGAStatus = g_in_packet_buf[1];
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

void See3CAM_11CUG::setBinnedVGAMode()
{
    bool ret = false;
    u_int8_t binned_vga_status = 0;
    ret = enableBinnedVGAMode(&binned_vga_status);
    if(ret == false)
    {
        emit deviceStatus("Failure","Unable to switch to binned VGA Mode");
        return void();
    }
    else
    {
        switch(binned_vga_status)
        {
        case 1 :
            emit deviceStatus("Success","Binned VGA mode set successfully");
            break;
        case 2 :
            emit deviceStatus("Failure","The current resolution is not 640x480, please switch to 640x480 before using the Cropping and Binning modes");
            break;
        case 3 :
            emit deviceStatus("Failure","Device is already in Binned VGA mode");
            break;
        case 4 :
            emit deviceStatus("Failure","Failed to set Binned VGA mode");
            break;
        default :            
            emit deviceStatus("Failure","Unknown error");
        }
    }
}

bool See3CAM_11CUG::setWbValues(camRGBcolor rgbColor, uint rgbValue)
{

    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;

    if((rgbColor == Red || rgbColor == Green || rgbColor == Blue))
    {
        //Modified by Nithyesh
        /*
         * Changed if condition from if(rgbValue >= 0 && rgbValue <= 255) to if(rgbValue <= 255)
         * as uint values will always be greater than or equal to 0.
         */
        if(rgbValue <= 255)
        {
            //Initialize the buffer
            memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

            //Set the Report Number
            g_out_packet_buf[1] = WHITE_BAL_CONTROL; /* Report Number */
            g_out_packet_buf[2] = SET_WB_GAIN; /* Report Number */
            g_out_packet_buf[3] = rgbColor; /* Report Number */ //0x01 - WB_RED, 0x02 - WB_GREEN, 0x03- WB_BLUE
            g_out_packet_buf[4] = rgbValue; /* Report Number */

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
                    if(g_in_packet_buf[0] == WHITE_BAL_CONTROL  &&
                            g_in_packet_buf[1] == SET_WB_GAIN &&
                            g_in_packet_buf[2] == rgbColor &&
                            g_in_packet_buf[3] == rgbValue ) {
                        if(g_in_packet_buf[4] == WB_FAIL)
                            return false;
                        else if (g_in_packet_buf[4] == WB_SUCCESS) {
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
        }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
        return true;
    }

    uint See3CAM_11CUG::getWbValues(camRGBcolor rgbColor)
    {
        if(uvccamera::hid_fd < 0)
        {
            return 0;
        }

        bool timeout = true;
        int ret =0;
        unsigned int start, end = 0;
        uint rgbValue = 0;

        if((rgbColor == Red || rgbColor == Green || rgbColor == Blue))
        {
            //Initialize the buffer
            memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

            //Set the Report Number
            g_out_packet_buf[1] = WHITE_BAL_CONTROL; /* Report Number */
            g_out_packet_buf[2] = GET_WB_GAIN; /* Report Number */
            g_out_packet_buf[3] = rgbColor; /* Report Number */

            ret = write(uvccamera::hid_fd, g_out_packet_buf, BUFFER_LENGTH);
            if (ret < 0) {
                perror("write");
                emit deviceStatus("Failure",tr("Unable to get whitebalance values"));
            }
            start = uvc.getTickCount();
            while(timeout)
            {
                /* Get a report from the device */
                ret = read(uvccamera::hid_fd, g_in_packet_buf, BUFFER_LENGTH);
                if (ret < 0) {
                    //perror("read");
                } else {                    
                    if(g_in_packet_buf[0] == WHITE_BAL_CONTROL  &&
                            g_in_packet_buf[1] == GET_WB_GAIN &&
                            g_in_packet_buf[2] == rgbColor ) {
                        if(g_in_packet_buf[4] == WB_FAIL)
                            emit deviceStatus("Failure",tr("Unable to get whitebalance values"));
                        else if (g_in_packet_buf[4] == WB_SUCCESS) {
                            rgbValue = g_in_packet_buf[3];
                            timeout = false;
                        }
                    }
                }
                end = uvc.getTickCount();
                if(end - start > TIMEOUT)
                {         
                    timeout = false;
                    emit deviceStatus("Failure",tr("Unable to get whitebalance values"));
                }
            }
        }
        else
        {
            return 0;
        }
        return rgbValue;
    }

    bool See3CAM_11CUG::defaultValues()
    {
        if(uvccamera::hid_fd < 0)
        {
            return false;
        }

        uint WhiteBalanceMode, WhiteBalanceRedGain, WhiteBalanceGreenGain, WhiteBalanceBlueGain;
        bool timeout = true;
        int ret =0;
        unsigned int start, end = 0;

        //Initialize the buffer
        memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

        //Set the Report Number
        g_out_packet_buf[1] = WHITE_BAL_CONTROL; /* Report Number */
        g_out_packet_buf[2] = SET_WB_DEFAULTS; /* Report Number */

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
                if(g_in_packet_buf[0] == WHITE_BAL_CONTROL  &&
                        g_in_packet_buf[1] == SET_WB_DEFAULTS ) {
                    if(g_in_packet_buf[6] == WB_FAIL)
                        return false;
                    else if (g_in_packet_buf[6] == WB_SUCCESS) {
                        WhiteBalanceMode = g_in_packet_buf[2];
                        WhiteBalanceRedGain = g_in_packet_buf[3];
                        WhiteBalanceGreenGain = g_in_packet_buf[4];
                        WhiteBalanceBlueGain = g_in_packet_buf[5];
                        emit defaultValue(WhiteBalanceMode,WhiteBalanceRedGain,WhiteBalanceGreenGain,WhiteBalanceBlueGain);
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

    bool See3CAM_11CUG::setWbMode(camRGBmode rgbMode)
    {
        if(uvccamera::hid_fd < 0)
        {
            return false;
        }

        bool timeout = true;
        int ret =0;
        unsigned int start, end = 0;

        if(rgbMode == Auto || rgbMode == Manual)
        {
        //Initialize the buffer
        memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

        //Set the Report Number
        g_out_packet_buf[1] = WHITE_BAL_CONTROL; /* Report Number */
        g_out_packet_buf[2] = SET_WB_MODE; /* Report Number */
        g_out_packet_buf[3] = rgbMode; /* Report Number */

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
                if(g_in_packet_buf[0] == WHITE_BAL_CONTROL  &&
                        g_in_packet_buf[1] == SET_WB_MODE &&
                        g_in_packet_buf[2] == rgbMode ) {
                    if(g_in_packet_buf[3] == WB_FAIL)
                        return false;
                    else if (g_in_packet_buf[3] == WB_SUCCESS) {
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
        }
        else
        {
            return false;
        }
        return true;
    }

    bool See3CAM_11CUG::getWbMode()
    {
        if(uvccamera::hid_fd < 0)
        {
            return false;
        }

        bool timeout = true;
        int ret =0;
        unsigned int start, end = 0;
        uint WhiteBalanceMode;
        //Initialize the buffer
        memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

        //Set the Report Number
        g_out_packet_buf[1] = WHITE_BAL_CONTROL; /* Report Number */
        g_out_packet_buf[2] = GET_WB_MODE; /* Report Number */

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
                if(g_in_packet_buf[0] == WHITE_BAL_CONTROL  &&
                        g_in_packet_buf[1] == GET_WB_MODE ) {
                    if(g_in_packet_buf[3] == WB_FAIL)
                        return false;
                    else if (g_in_packet_buf[3] == WB_SUCCESS) {
                        WhiteBalanceMode = g_in_packet_buf[2];
                        emit auto_manualMode(WhiteBalanceMode);
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
