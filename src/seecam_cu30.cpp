/*
 * seecam_cu30.cpp -- Handling special feature of seecamcu30 camera
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

#include "seecam_cu30.h"

See3CAM_CU30::See3CAM_CU30(QObject *parent) :
    QObject(parent)
{
}

/**
 * @brief See3CAM_CU30::setEffectMode - setting special effects to the camera
 * return true - success /false - failure
 */
bool See3CAM_CU30::setEffectMode(const specialEffects &specialEffect)
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

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_CU30; /* set camera control code */
    g_out_packet_buf[2] = SET_SPECIAL_EFFECT; /* set special effect code */
    g_out_packet_buf[3] = specialEffect; /* set special effect */

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
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU30 &&
                g_in_packet_buf[1]==SET_SPECIAL_EFFECT &&
                g_in_packet_buf[2]==specialEffect &&
                g_in_packet_buf[6]==SET_SUCCESS) {
                    timeout=false;
            }
        }
        end = uvc.getTickCount();
        if(end - start > SET_COMMAND_TIMEOUT)
        {
            timeout = false;
            return false;
        }
    }
    return true;
}

/**
 * @brief See3CAM_CU30::setDenoiseValue - setting denoise value to the camera
 * @param deNoiseVal - denoise value
 * return true - success /false - failure
 */
bool See3CAM_CU30::setDenoiseValue(int deNoiseVal)
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;

    if(DENOISE_MIN <= deNoiseVal || DENOISE_MAX >= deNoiseVal)
    {
        //Initialize the buffer
        memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
        //Set the Report Number

        g_out_packet_buf[1] = CAMERA_CONTROL_CU30; /* set camera control code */
        g_out_packet_buf[2] = SET_DENOISE_CONTROL; /* set denoise control code */
        g_out_packet_buf[3] = deNoiseVal; /* set denoise value */

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
                } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU30 &&
                    g_in_packet_buf[1]==SET_DENOISE_CONTROL &&
                    g_in_packet_buf[2]==deNoiseVal &&
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
    }
    else
    {
        return false;
    }
    return true;
}

/**
 * @brief See3CAM_CU30::getEffectMode - get effect mode set in camera
 * return true - success /false - failure
 */
bool See3CAM_CU30::getEffectMode()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
        bool timeout = true;
        int ret =0;
        unsigned int start, end = 0;

        uint effectMode;
        //Initialize the buffer
        memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
        //Set the Report Number
        g_out_packet_buf[1] = CAMERA_CONTROL_CU30; /* set camera control code */
        g_out_packet_buf[2] = GET_SPECIAL_EFFECT; /* get special effect code */

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
                if (g_in_packet_buf[6]==GET_FAIL) {
                    return false;
                } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU30 &&
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
                return false;
            }
        }
        return true;
}

/**
 * @brief See3CAM_CU30::getDenoiseValue - get denoise value from camera
 * return true - success /false - failure
 */
bool See3CAM_CU30::getDenoiseValue()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    bool timeout = true;
    int ret =0;
    unsigned int start, end = 0;

    uint denoiseValue;
    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_CU30; /* set camera control code */
    g_out_packet_buf[2] = GET_DENOISE_CONTROL; /* get denoise code */

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
            if (g_in_packet_buf[6]==GET_FAIL) {
                return false;
            } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU30 &&
                g_in_packet_buf[1]==GET_DENOISE_CONTROL &&
                g_in_packet_buf[6]==GET_SUCCESS) {
                    denoiseValue = g_in_packet_buf[2];
                    emit sendDenoiseValue(denoiseValue);
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
