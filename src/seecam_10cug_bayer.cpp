/*
 * seecam_10cug_bayer.cpp -- Handling special feature of seecam10cug bayer camera
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

#include "seecam_10cug_bayer.h"

bool See3CAM_10CUG_Bayer::enableMasterMode()
{
    bool ret = modeControl.enableMasterMode();
    return ret;
}

bool  See3CAM_10CUG_Bayer::enableTriggerMode()
{
    bool ret = modeControl.enableTriggerMode();
    return ret;
}
