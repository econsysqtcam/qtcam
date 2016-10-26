/*
 * seecam_cu50.cpp -- Handling special feature of seecamcu50 camera
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

#include "seecam_cu50.h"

void See3CAM_CU50::getFlashLevel() {
    quint8 flash_level = 0;
    see3cam_ctrl.getFlashState(&flash_level);
    QString tmpStr = QString::number(flash_level);
    emit updateFlashCheckBox(tmpStr);
}

void See3CAM_CU50::getTorchLevel() {
    quint8 torch_level = 0;
    see3cam_ctrl.getTorchState(&torch_level);
    QString tmpStr = QString::number(torch_level);
    emit updateTorchCheckBox(tmpStr);
}
