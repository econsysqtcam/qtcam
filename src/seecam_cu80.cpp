/*
 * seecam_cu80.cpp -- Handling special feature of seecamcu80 camera
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

#include "seecam_cu80.h"

void See3CAM_80::getFlashLevel() {
    quint8 flash_level = 0;
    see3cam_ctrl.getFlashState(&flash_level);
    QString tmpStr = QString::number(flash_level);
    emit updateFlashCheckBox(tmpStr);
}

void See3CAM_80::getTorchLevel() {
    quint8 torch_level = 0;
    see3cam_ctrl.getTorchState(&torch_level);
    QString tmpStr = QString::number(torch_level);
    emit updateTorchCheckBox(tmpStr);
}

void See3CAM_80::getFocusPosition() {

    if(uvccamera::hid_fd < 0)
    {
        return void();
    }

    quint16 focusPosition;
    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;
    unsigned short int MSB = 0, LSB = 0;
    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_80; /* Report Number */
    g_out_packet_buf[2] = GET_FOCUS_POSITION; /* Report Number */
    ret = write(uvccamera::hid_fd, g_out_packet_buf, BUFFER_LENGTH);

    if (ret < 0) {
        perror("write");
        return void();
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
            if((g_in_packet_buf[0] == CAMERA_CONTROL_80)&&
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
            return void();
        }
    }
}

void See3CAM_80::getFocusMode() {

    if(uvccamera::hid_fd < 0)
    {
        return void();
    }

    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;

    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_80; /* Report Number */
    g_out_packet_buf[2] = GET_FOCUS_MODE; /* Report Number */

    ret = write(uvccamera::hid_fd, g_out_packet_buf, BUFFER_LENGTH);

    if (ret < 0) {
        perror("write");
        return void();
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
            if((g_in_packet_buf[0] == CAMERA_CONTROL_80)&&
                    (g_in_packet_buf[1]==GET_FOCUS_MODE)) {
                emit focusModeValue(g_in_packet_buf[2]);
                timeout=false;
            }
        }
        end = uvc.getTickCount();
        if(end - start > TIMEOUT) {            
            timeout = false;
            return void();
        }
    }
}

void See3CAM_80::setFocusMode(camFocusmode focusMode)
{
    if(uvccamera::hid_fd < 0)
    {
        return void();
    }

    bool timeout = true;
    int ret = 0;
    unsigned int start, end = 0;

    if(focusMode == Auto || focusMode == Manual || focusMode == Singletrigger)
    {
    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_80; /* Report Number */
    g_out_packet_buf[2] = SET_FOCUS_MODE; /* Report Number */
    g_out_packet_buf[3] = focusMode; /* Report Number */

    ret = write(uvccamera::hid_fd, g_out_packet_buf, BUFFER_LENGTH);

    if (ret < 0) {
        perror("write");
        return void();
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
            if((g_in_packet_buf[0] == CAMERA_CONTROL_80)&&
                    (g_in_packet_buf[1]==SET_FOCUS_MODE)&&
                    (g_in_packet_buf[2]==focusMode)) {
                emit focusModeStatus(g_in_packet_buf[3]);
                timeout=false;
            }
        }
        end = uvc.getTickCount();
        if(end - start > TIMEOUT)
        {
            timeout = false;
            return void();
        }
    }
    }
    else
    {
        return void();
    }
}

void See3CAM_80::setFocusPosition(int focusPosition)
{
    if(uvccamera::hid_fd < 0)
    {
        return void();
    }

    int ret =0;
    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_80; /* Report Number */
    g_out_packet_buf[2] = SET_FOCUS_POSITION; /* Report Number */
    g_out_packet_buf[3] = ((focusPosition&0xFF00)>>8); /* MSB of focus postion */
    g_out_packet_buf[4] = (focusPosition&0x00FF); /* LSB of focus postion */
    ret = write(uvccamera::hid_fd, g_out_packet_buf, BUFFER_LENGTH);

    if (ret < 0) {
        perror("write");
        return void();
    }
}
