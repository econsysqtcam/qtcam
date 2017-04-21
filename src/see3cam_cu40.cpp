/*
 * seecam_cu40.cpp -- Handling special feature of seecamcu40 camera
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

#include "see3cam_cu40.h"

See3CAM_CU40::See3CAM_CU40(QObject *parent) :
    QObject(parent)
{
}

/**
 * @brief See3CAM_CU40::setFlashState - setting flash mode to the camera
 * return true - success /false - failure
 */
bool See3CAM_CU40::setFlashState(const flashStateValues &flashMode)
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;

    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

    g_out_packet_buf[1] = CAMERA_CONTROL_CU40; /* set camera control code */
    g_out_packet_buf[2] = SETFLASH_STATUS_CU40; /* set flash status command code */
    g_out_packet_buf[3] = flashMode; /* set flash state [off/torch/storbe] */

    ret = write(uvccamera::hid_fd, g_out_packet_buf, BUFFER_LENGTH);

    if (ret < 0) {
        perror("write");
        return false;
    }

    /* Read the Status code from the device */
    start = uvc.getTickCount();


    while(timeout)
    {
        /* Get a report from the device */
        ret = read(uvccamera::hid_fd, g_in_packet_buf, BUFFER_LENGTH);
        if (ret < 0) {
            perror("read");
        } else {
            if (g_in_packet_buf[6]==SET_FAIL) {
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU40 &&
                g_in_packet_buf[1]==SETFLASH_STATUS_CU40 &&
                g_in_packet_buf[2]==flashMode &&
                g_in_packet_buf[6]==SET_SUCCESS) {
                    timeout=false;
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

/**
 * @brief See3CAM_CU40::getFlashState - getting flash state from the camera
 * return true - success /false - failure
 */
bool See3CAM_CU40::getFlashState()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;

    uint flashMode;

    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_CU40; /* camera control id */
    g_out_packet_buf[2] = GETFLASH_STATUS_CU40; /* get strobe mode */

    ret = write(uvccamera::hid_fd, g_out_packet_buf, BUFFER_LENGTH);

    if (ret < 0) {
        perror("write");
        return false;
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
            if (g_in_packet_buf[3]==GET_FAIL_CU40) {
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU40 &&
                g_in_packet_buf[1]==GETFLASH_STATUS_CU40 &&
                g_in_packet_buf[3]==GET_SUCCESS_CU40) {
                    flashMode = g_in_packet_buf[2];
                    emit flashModeValue(flashMode);
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


