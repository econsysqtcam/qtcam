/*
 * seecam_cu40.h -- Handling special feature of seecamcu40 camera
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
#ifndef SEE3CAM_CU40_H
#define SEE3CAM_CU40_H

#include <QObject>
#include "uvccamera.h"

/* SEE3CAM_CU40 */
#define CAMERA_CONTROL_CU40         0x77
#define SETFLASH_STATUS_CU40        0x02
#define GETFLASH_STATUS_CU40        0x01
#define GET_FAIL_CU40		0x01
#define GET_SUCCESS_CU40	0x00

class See3CAM_CU40 : public QObject
{
    Q_OBJECT

private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
    uvccamera uvc;

public:
    explicit See3CAM_CU40(QObject *parent = 0);

    enum flashStateValues{
        FLASHMODE_OFF = 0x00,
        FLASHMODE_STROBE = 0x01,
        FLASHMODE_TORCH = 0x02
    };
    Q_ENUMS(flashStateValues)

signals:
    void flashModeValue(uint flashMode);

public slots:
    bool setFlashState(const flashStateValues &flashMode);
    bool getFlashState();
};

#endif // SEE3CAM_CU40_H
