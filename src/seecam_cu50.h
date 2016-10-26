/*
 * seecam_cu50.h -- Handling special feature of seecamcu50 camera
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

#ifndef SEECAM_CU50_H
#define SEECAM_CU50_H
#include "uvccamera.h"

class See3CAM_CU50: public QObject{
    Q_OBJECT
private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
public:
    See3CAM_Control see3cam_ctrl;
signals:
    void updateFlashCheckBox(QVariant flash_Check_state);
    void updateTorchCheckBox(QVariant torch_Check_state);

public slots:
    /**
     * @brief Torch Status(on/off)
     *  - Get the torch on/off value from the see3cam_control class
     */
    void getTorchLevel();

    /**
     * @brief Flash Status(on/off)
     *  - Get the flash on/off value from the see3cam_control class
     */
    void getFlashLevel();
};

#endif // SEECAM_CU50_H
