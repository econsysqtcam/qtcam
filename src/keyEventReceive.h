/*
 * keyEventReceive.h -- Getting key event using /dev/input driver files
 * Copyright © 2018  e-con Systems India Pvt. Limited
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
#ifndef KEYEVENTRECEIVE_H
#define KEYEVENTRECEIVE_H

#include <QObject>
#include "cameraproperty.h"

class CamKeyEventReceive : public QObject
{
    Q_OBJECT

private:
    bool m_quit;
    int fpsCount;

    Cameraproperty camPropObj;

public:
    explicit CamKeyEventReceive();
    QSocketNotifier *m_capKeyNotifier;

signals:
    void cameraTriggerKeyReceived();

public slots:
    // initialize a socket notifier to get key from camera
    void initializeToGetKey();

    // when key event is coming from camera, then process the key
    void doKeyReceived();

    //stopGetKeyFromCamera - stop Getting key from camera.
    void stopGetKeyFromCamera();
};

#endif // KEYEVENTRECEIVE_H
