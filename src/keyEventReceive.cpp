/*
 * keyEventReceive.cpp -- Getting key event using /dev/input driver files
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
#include "keyEventReceive.h"
#include <linux/input.h>

// Added by Sankari : 12 Feb 2018
CamKeyEventReceive::CamKeyEventReceive()
{
    m_capKeyNotifier = NULL;

}

/**
 * @brief CamKeyEventReceive::initializeToGetKey - initialize a socket notifier to get key from camera
 */
void CamKeyEventReceive::initializeToGetKey(){
    m_capKeyNotifier = new QSocketNotifier(camPropObj.event_fd, QSocketNotifier::Read);
    m_capKeyNotifier->setEnabled(true);
    connect(m_capKeyNotifier, SIGNAL(activated(int)), this, SLOT(doKeyReceived()));
}

/**
 * @brief CamKeyEventReceive::doKeyReceived - when key event is coming from camera, then process the key
 */
void CamKeyEventReceive::doKeyReceived()
{
    struct input_event ev[64];    
    if(camPropObj.event_fd < 0){
        perror("\nInvalid event file descriptor");
        return void();
    }

    int rd = read(camPropObj.event_fd, ev, sizeof(struct input_event) * 64);
    if (rd < (int) sizeof(struct input_event)) {
        perror("\nerror reading from event file descriptor");
        return void();
    }

    for (unsigned int i = 0; i < rd / sizeof(struct input_event); i++){
        if(ev[i].code == KEY_CAMERA){
            qDebug()<<"KEY_CAMERA: Key received";
            emit cameraTriggerKeyReceived();
        }
    }
}

/**
 * @brief CamKeyEventReceive::stopGetKeyFromCamera - stop Getting key from camera.
 */
void CamKeyEventReceive::stopGetKeyFromCamera(){
    // Close already existing fd
    if(camPropObj.event_fd > 0){
        ::close(camPropObj.event_fd);
        camPropObj.event_fd = -1;
    }

    // Delete created socket notifier to get key from camera
    if(m_capKeyNotifier){
        delete m_capKeyNotifier;
        m_capKeyNotifier = NULL;
    }
}
