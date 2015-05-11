/*
 * seecam_10cug_bayer.h -- Handling special feature of seecam10cug bayer camera
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

#ifndef SEECAM_10CUG_BAYER_H
#define SEECAM_10CUG_BAYER_H
#include "uvccamera.h"

class See3CAM_10CUG_Bayer: public QObject {
    Q_OBJECT
private:
    See3CAM_ModeControls modeControl;

public slots:
    /**
     * @brief Enable Master Mode
     * Enable the camera in master mode
     * @return
     * true - Master mode is enabled successfully,
     * false - Master mode enabling failed
     */
    bool enableMasterMode();

    /**
     * @brief Enable Trigger Mode
     * @return
     * true - Trigger mode is enabled successfully,
     * false - Trigger mode enabling failed
     */
    bool enableTriggerMode();
};


#endif // SEECAM_10CUG_BAYER_H
