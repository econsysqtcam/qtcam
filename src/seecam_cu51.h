 /* seecam_cu51.h -- Handling special feature of seecamcu51 camera
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

#ifndef SEECAM_CU51_H
#define SEECAM_CU51_H
#include "uvccamera.h"

class See3CAM_CU51:public QObject {
    Q_OBJECT
private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
    uvccamera uvc;
    See3CAM_ModeControls modeControl;


public:
    See3CAM_Control see3cam_ctrl;

signals:
    void updateFlashCheckBox(QVariant flash_Check_state);
    void deviceStatus(QString title, QString message);
    void sendExposure(uint xposureValue);
    void triggerModeCapture();

public slots:
    /**
     * @brief Enable Master Mode
     *  - Enable the camera in master mode
     * @return
     * true - Master mode is enabled successfully,
     * false - Master mode enabling failed
     */
    bool enableMasterMode();

    /**
     * @brief Enable Trigger Mode
     *  - Enable the camera in Trigger mode
     * @return
     * true - Trigger mode is enabled successfully,
     * false - Trigger mode enabling failed
     */
    bool enableTriggerMode();

    /**
     * @brief Torch Status(on/off)
     *  - Get the torch on/off value from the see3cam_control class
     */
    void getTorchLevel();
    void getExposure();
    void setExposure(const uint& exposureValue);

};

#endif // SEECAM_CU51_H
