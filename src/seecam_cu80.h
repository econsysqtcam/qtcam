/*
 * seecam_cu80.h -- Handling special feature of seecamcu80 camera
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

#ifndef SEECAM_CU80_H
#define SEECAM_CU80_H

#include <QQmlComponent>
#include "uvccamera.h"

class See3CAM_80: public QObject {
    Q_OBJECT
private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
public:
    See3CAM_Control see3cam_ctrl;
    uvccamera uvc;

    enum camFocusmode {
        Auto = 1,
        Manual = 2,
        Singletrigger = 3
    };

    Q_ENUMS(camFocusmode)


signals:
    void updateFlashCheckBox(QVariant flash_Check_state);
    void updateTorchCheckBox(QVariant torch_Check_state);
    void deviceStatus(QString title, QString message);
    void focusPostionValue(quint16 focusPostion);
    void focusModeValue(unsigned char focusModeValue);
    void focusModeStatus(unsigned char focusModeStatus);
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

    /**
     * @brief Get the position of the lens of the seecam80 camera
     *  - Reads the position of the lens and emits the resultant value in the signal focusPostionValue()
     */
    void getFocusPosition();

    /**
     * @brief Get the camera focus mode(Auto/Manual/Single trigger)
     *  - Reads the camera focus mode and emits the resultant value in the signal focusModeValue()
     */
    void getFocusMode();
    /**
     * @brief The camRGBmode enum are used to set value (Auto/Manual) to white balance mode of the camera.
     */
    /**
     * @brief   Sets the camera focus mode.
     * @param focusMode  set the focus mode of the camera (Auto/Manual/SingleTrigger)
     */
    void setFocusMode(camFocusmode focusMode);

    /**
     * @brief Sets the camera lens to a certain position
     *  - Position can be set only if the focusMode is in "Manual Mode"
     *  - Focus will be adjusted as per the focusPosition parameter
     * @param focusPosition
     * Minimum value of the focusposition - 0,
     * Maximum value of the focustposition - 1022
     */
    void setFocusPosition(int focusPosition);
};


#endif // SEECAM_CU80_H
