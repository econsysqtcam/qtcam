/*
 * seecam_cu30.h -- Handling special feature of seecamcu130 camera
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
#ifndef SEECAM_CU30_H
#define SEECAM_CU30_H

#include <QObject>
#include "uvccamera.h"
#define DENOISE_MIN 0
#define DENOISE_MAX 15

class See3CAM_CU30 : public QObject
{
    Q_OBJECT

private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
    uvccamera uvc;

public:
    explicit See3CAM_CU30(QObject *parent = 0);

    enum specialEffects {
        EFFECT_NORMAL = 0x01,
        EFFECT_BLACK_WHITE = 0x04,
        EFFECT_GREYSCALE = 0x07,
        EFFECT_SKETCH = 0x10,
        EFFECT_NEGATIVE = 0x08
    };

    Q_ENUMS(specialEffects)


signals:
    void sendEffectMode(uint effectMode);
    void sendDenoiseValue(uint denoiseValue);

public slots:
    bool setEffectMode(const specialEffects &specialEffect);
    bool setDenoiseValue(int deNoiseVal);
    bool getEffectMode();
    bool getDenoiseValue();
};
#endif // SEECAM_CU30_H
