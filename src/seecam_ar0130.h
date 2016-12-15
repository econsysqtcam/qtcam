/*
 * seecam_ar0130.h -- Handling special feature of seecamar0130 camera
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

#ifndef SEECAM_AR0130_H
#define SEECAM_AR0130_H
#include <QThread>
#include "uvccamera.h"

class See3CAM_AR130: public QThread {
    Q_OBJECT
private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];    
    uvccamera uvc;
    See3CAM_ModeControls modeControl;

public:
    See3CAM_Control see3cam_ctrl;

    explicit See3CAM_AR130(QObject *parent = 0);
    ~See3CAM_AR130();

    void init();
    void run();
    bool m_trigger;

    /**
     * @brief Enable camera in cropped mode and its output value is passed/set to the parameter
     * @param VGAStatus
     *  1 - Cropped Mode set successfully
     *  2 - The current resolution is not 640x480, please switch to 640x480 before using the Cropping mode
     *  3 - Device is already in cropped VGA mode
     *  4 - Failed to set cropped VGA mode
     *  X - Unknown error(X may be any value other than 1,2,3 & 4)
     * @return
     * true - If value obtained in VGAStatus parameter
     * false - On failure(timout happens)
     */
    bool enableCroppedVGAMode(u_int8_t *VGAStatus);

    /**
     * @brief Enable camera in binned mode and its output value is passed/set to the parameter
     * @param VGAStatus
     *  1 - Cropped Mode set successfully
     *  2 - The current resolution is not 640x480, please switch to 640x480 before using the binned mode
     *  3 - Device is already in binned VGA mode
     *  4 - Failed to set binned VGA mode
     *  X - Unknown error(X may be any value other than 1,2,3 & 4)
     * @return
     * true - If value obtained in VGAStatus parameter
     * false - On failure(timout happens)
    */
    bool enableBinnedVGAMode(u_int8_t *VGAStatus);


signals:
    void updateFlashCheckBox(QVariant flash_Check_state);
    void deviceStatus(QString title, QString message);

    void triggershotSignal();

public slots:


    void initTriggerShotCapture();
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
     * @brief Sets the camera preview in cropped mode
     *  - Mode success/failure is updated to the qml via deviceStatus() signal.
     *  - Possible values emitted from the signals are,
     *  1. Cropped VGA mode set successfully
     *  2. The current resolution is not 640x480, please switch to 640x480 before using the Cropping and Binning modes
     *  3. Device is already in Cropped VGA mode
     *  4. Failed to set Cropped VGA mode
     *  5. Unknown error
     */
    void setCroppedVGAMode();

    /**
     * @brief Sets the camera preview in Binned mode
     *  - Mode success/failure is updated to the qml via deviceStatus() signal.
     *  - Possible values emitted from the signals are,
     *  1. Cropped VGA mode set successfully
     *  2. The current resolution is not 640x480, please switch to 640x480 before using the Cropping and Binning modes
     *  3. Device is already in Binned VGA mode
     *  4. Failed to set Binned VGA mode
     *  5. Unknown error
     */
    void setBinnedVGAMode();

    /**
     * @brief Flash Status(on/off)
     *  - Get the flash on/off value from the see3cam_control class
     */
    void getFlashLevel();
};

#endif // SEECAM_AR0130_H
