/*
 * about.h -- Read and Display the details of the copyright file.
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

#ifndef ABOUT_H
#define ABOUT_H

#include <QSettings>
#include <QImage>
#include <QDebug>
#include <QFile>

class AboutPage:public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString titleText READ titleText)
public:
    AboutPage();
    ~AboutPage();
    QString titleText();

private:
    QSettings *version;
    QFile *copyRightFile;
    QString appName,appVerNumber,appRevNumber,appReleaseDate;
    QByteArray copyRight;


public slots:
    /**
     * @brief getApplicationName
     *  - The user will get the application name when this function get called
     * @return
     * Application Name
     */
    QString getApplicationName();

    /**
     * @brief getApplicationVersionNumber
     *  - The user will get current application version number
     * @return
     * Current application version
     */
    QString getApplicationVersionNumber();

    /**
     * @brief getCopyRight
     *  - Read the copyright file from the device
     * @return
     * Copyright file data
     */
    QByteArray getCopyRight();

};
#endif // ABOUT_H
