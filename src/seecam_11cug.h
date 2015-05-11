/*
 * seecam_11cug.cpp -- Handling special feature of seecam11cug camera
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

#ifndef SEECAM_11CUG_H
#define SEECAM_11CUG_H
#include "uvccamera.h"

class See3CAM_11CUG:public QObject {
    Q_OBJECT
private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
    uvccamera uvc;
    See3CAM_ModeControls modeControl;

signals:
    void deviceStatus(QString title, QString message);
    void defaultValue(uint autoMode, uint redValue, uint greenValue, uint blueValue);
    void auto_manualMode(uint cbValue);

public:

    /**
     * @brief The camRGBcolor enum are used to set camRGBcolor value ( Red, Green ,Blue)for rgbColor of the camera which need to be read.
     */
    enum camRGBcolor{
        Red = 1,
        Green = 2,
        Blue = 3
    };

    Q_ENUMS(camRGBcolor)
    /**
     * @brief The camRGBmode enum are used to set value (Auto/Manual) to white balance mode of the camera.
     */
    enum camRGBmode{
        Auto = 1,
        Manual = 2
    };

    Q_ENUMS(camRGBmode)
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
     * @brief Get the white balance values(0-255) for the extension settings of 11_CUG camera
     * @param rgbColor  camRGBcolor enum value ( Red, Green ,Blue)for rgbColor of the camera which need to be read.
     * @return
     * rgbValue - Current value of the rgbColor
     */
    uint getWbValues(camRGBcolor rgbColor);

    /**
     * @brief Set the white balance values(0-255) for the extension settings of 11_CUG camera
     * @param rgbColor camRGBcolor enum value (Red, Green ,Blue)for rgbColor of the camera which need to be set.
     * @param rgbValue  Set value for the rgbColor.
     * @return
     * true - Value set successfully
     * false - Value is not set successfully
     */
    bool setWbValues(camRGBcolor rgbColor, uint rgbValue);

    /**
     * @brief White balance values set to hardware defaults for the extension settings of 11_CUG camera
     * @return
     * true - Default value is set successfully
     * fale - Default value no set successfully
     */
    bool defaultValues();

    /**
     * @brief   Sets the camera white balance Mode.
     * @param rgbMode  camRGBmode enum value (Auto/Manual) to white balance mode of the camera.
     * @return
     * true -  successfull to set the  white balance mode of the camera.
     * @return
     * false - failed to set the white balance mode of the Device.
     */
    bool setWbMode(camRGBmode rgbMode);

    /**
     * @brief Get whether the white balance is in auto/manual mode
     * @return
     * true - Able to return the current mode successfully
     * false - Unable to return the current mode
     */
    bool getWbMode();
};

#endif // SEECAM_11CUG_H
