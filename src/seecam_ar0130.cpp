/*
 * seecam_ar0130.cpp -- Handling special feature of seecamar0130 camera
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

#include "seecam_ar0130.h"

See3CAM_AR130::See3CAM_AR130(QObject *parent):
QThread(parent){
    m_trigger = true;

}

See3CAM_AR130::~See3CAM_AR130(){
    if(!this->wait(3000)) //Wait until it actually has terminated (max. 3 sec)
    {
        this->terminate(); //Thread didn't exit in time, probably deadlocked, terminate it!
        this->wait(); //We have to wait again here!
        delete(this);
    }

}

bool See3CAM_AR130::enableMasterMode()
{
    bool ret = modeControl.enableMasterMode();
    return ret;
}

bool  See3CAM_AR130::enableTriggerMode()
{
    bool ret = modeControl.enableTriggerMode();
    return ret;
}

void See3CAM_AR130::setCroppedVGAMode()
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

bool See3CAM_AR130::enableCroppedVGAMode(u_int8_t *VGAStatus)
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

bool See3CAM_AR130::enableBinnedVGAMode(u_int8_t *VGAStatus)
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

void See3CAM_AR130::setBinnedVGAMode()
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

void See3CAM_AR130::getFlashLevel() {
    quint8 flash_level = 0;
    see3cam_ctrl.getFlashState(&flash_level);
    QString tmpStr = QString::number(flash_level);
    emit updateFlashCheckBox(tmpStr);
}


void See3CAM_AR130::initTriggerShotCapture(){
    init();
}


/**
 * @brief See3CAM_AR130::run - override QThread run method - To avoid key queueing
 */
void See3CAM_AR130::run()
{    
    emit triggershotSignal();    
}

/**
 * @brief See3CAM_AR130::init - Init thread
 */
void See3CAM_AR130::init()
{
    if(!isRunning())
    {
        this->start();
    }

}
