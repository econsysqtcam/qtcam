/*
 * seecam_cu130.cpp -- Handling special feature of seecamcu130 camera
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

#include "see3cam_cu130.h"

See3CAM_CU130::See3CAM_CU130(QObject *parent) :
    QObject(parent)
{
}

void See3CAM_CU130::getSceneMode()
{
    if(uvccamera::hid_fd < 0)
    {
        return void();
    }
        bool timeout = true;
        int ret =0;
        unsigned int start, end = 0;

        uint sceneMode;
        //Initialize the buffer
        memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
        //Set the Report Number
        g_out_packet_buf[1] = CAMERA_CONTROL_CU130; /* Report Number */
        g_out_packet_buf[2] = GET_SCENE_MODE; /* Report Number */

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
                if (g_in_packet_buf[6] == GET_FAIL) {
                    return void();
                } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU130 &&
                    g_in_packet_buf[1]==GET_SCENE_MODE &&
                    g_in_packet_buf[6]==GET_SUCCESS) {
                        sceneMode = g_in_packet_buf[2];
                        emit sendSceneMode(sceneMode);
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

void See3CAM_CU130::getEffectMode()
{
    if(uvccamera::hid_fd < 0)
    {
        return void();
    }
        bool timeout = true;
        int ret =0;
        unsigned int start, end = 0;

        uint effectMode;
        //Initialize the buffer
        memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
        //Set the Report Number
        g_out_packet_buf[1] = CAMERA_CONTROL_CU130; /* Report Number */
        g_out_packet_buf[2] = GET_SPECIAL_EFFECT; /* Report Number */

        ret = write(uvccamera::hid_fd , g_out_packet_buf, BUFFER_LENGTH);

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
                if (g_in_packet_buf[6]==GET_FAIL) {
                    return void();
                } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU130 &&
                    g_in_packet_buf[1]==GET_SPECIAL_EFFECT &&
                    g_in_packet_buf[6]==GET_SUCCESS) {
                        effectMode = g_in_packet_buf[2];
                        emit sendEffectMode(effectMode);
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

void See3CAM_CU130::setSceneMode(const See3CAM_CU130::sceneModes &sceneMode)
{
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
        g_out_packet_buf[1] = CAMERA_CONTROL_CU130; /* Report Number */
        g_out_packet_buf[2] = SET_SCENE_MODE; /* Report Number */
        g_out_packet_buf[3] = sceneMode; /* Report Number */

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
                if (g_in_packet_buf[6]==SET_FAIL) {
                    return void();
                } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU130 &&
                    g_in_packet_buf[1]==SET_SCENE_MODE &&
                    g_in_packet_buf[2]==sceneMode &&
                    g_in_packet_buf[6]==SET_SUCCESS) {
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

void See3CAM_CU130::setEffectMode(const See3CAM_CU130::specialEffects &specialEffect)
{
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
        g_out_packet_buf[1] = CAMERA_CONTROL_CU130; /* Report Number */
        g_out_packet_buf[2] = SET_SPECIAL_EFFECT; /* Report Number */
        g_out_packet_buf[3] = specialEffect; /* Report Number */

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
                if (g_in_packet_buf[6]==SET_FAIL) {
                    return void();
                } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU130 &&
                    g_in_packet_buf[1]==SET_SPECIAL_EFFECT &&
                    g_in_packet_buf[2]==specialEffect &&
                    g_in_packet_buf[6]==SET_SUCCESS) {
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
