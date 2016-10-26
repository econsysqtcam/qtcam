/*
 * seecam_cu51.cpp -- Handling special feature of seecamcu51 camera
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

#include "seecam_cu51.h"


bool See3CAM_CU51::enableMasterMode()
{
    bool ret = modeControl.enableMasterMode();
    return ret;
}

bool  See3CAM_CU51::enableTriggerMode()
{
    bool ret = modeControl.enableTriggerMode();
    return ret;
}

void See3CAM_CU51::getTorchLevel() {
    quint8 torch_level = 0;
    see3cam_ctrl.getTorchState(&torch_level);
    QString tmpStr = QString::number(torch_level);
    emit updateFlashCheckBox(tmpStr);
}

void See3CAM_CU51::getExposure()
{
    if(uvccamera::hid_fd < 0)
       {
           return void();
       }
           bool timeout = true;
           int ret =0;
           unsigned int start, end = 0;

           uint exposureValue;

        //Initialize the buffer
        memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

        //Set the Report Number
        g_out_packet_buf[1] = CAMERA_CONTROL_51; 	/* Report Number */
        g_out_packet_buf[2] = GET_EXPOSURE_VALUE; 	/* Report Number */

        /* Send a Report to the Device */
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
                if(g_in_packet_buf[0] == CAMERA_CONTROL_51 &&
                    g_in_packet_buf[1] == GET_EXPOSURE_VALUE ) {
                        if(g_in_packet_buf[4] == EXP_SUCCESS) {
                            exposureValue = (g_in_packet_buf[3]<<8)+g_in_packet_buf[2];                            
                            emit sendExposure(exposureValue);

                            timeout = false;
                        } else if(g_in_packet_buf[4] == EXP_FAIL) {
                            return void();
                        }
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

void See3CAM_CU51::setExposure(const uint &exposureValue)
{
    if(uvccamera::hid_fd < 0)
       {
           return void();
       }    
    if(exposureValue >= 1 && exposureValue <= 30000)
    {
           bool timeout = true;
           int ret =0;
           unsigned int start, end = 0;

        //Initialize the buffer
        memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

        //Set the Report Number
        g_out_packet_buf[1] = CAMERA_CONTROL_51; 	/* Report Number */
        g_out_packet_buf[2] = SET_EXPOSURE_VALUE; 	/* Report Number */
        g_out_packet_buf[3] = exposureValue & 0xFF;	/* LSB of Exposure value */
        g_out_packet_buf[4] = exposureValue>>8; 	/* MSB of exposure value */

        /* Send a Report to the Device */
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
                if(g_in_packet_buf[0] == CAMERA_CONTROL_51 &&
                    g_in_packet_buf[1] == SET_EXPOSURE_VALUE &&
                    g_in_packet_buf[2] == g_out_packet_buf[3] &&
                    g_in_packet_buf[3] == g_out_packet_buf[4] ) {
                        if(g_in_packet_buf[4] == EXP_SUCCESS) {
                            timeout = false;
                        } else if(g_in_packet_buf[4] == EXP_FAIL) {
                            return void();
                        }
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
}
