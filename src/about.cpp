/*
 * about.cpp -- Read and Display the details of the copyright file.
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

#include "about.h"
#include "common.h"

AboutPage::AboutPage()
{
#if LAUNCHPAD
    version = new QSettings("/usr/share/qml/qtcam/about/release.ini",QSettings::IniFormat);
#else
    version = new QSettings("qml/qtcam/about/release.ini",QSettings::IniFormat);
#endif
    version->beginGroup("release");
    appName = version->value("applicationname").toString();
    appVerNumber = version->value("version").toString();
    appRevNumber = version->value("revision").toString();
    appReleaseDate = version->value("releasedate").toString();
    version->endGroup();
    delete version;
}

AboutPage::~AboutPage(){

}

QString AboutPage::getApplicationName() {
    return appName;
}

QString AboutPage::getApplicationVersionNumber() {
    return appVerNumber;
}

QByteArray AboutPage::getCopyRight() {
#if LAUNCHPAD
    copyRightFile = new QFile("/usr/share/qml/qtcam/about/copyright.txt");
#else
    copyRightFile = new QFile("qml/qtcam/about/copyright.txt");
#endif
    if(copyRightFile->open(QIODevice::ReadOnly)){
        copyRight = copyRightFile->readAll();
        copyRightFile->close();
        delete copyRightFile;
    } else {
        copyRight = tr("Not available/Unable to read").toLocal8Bit();
    }
    return copyRight;
}

QString AboutPage::titleText(){
    return QString();
}
