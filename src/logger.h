/*
 * logger.h -- logs the Debug, warning and critical message to the log file
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

#ifndef LOGGER_H
#define LOGGER_H
#include <QDateTime>
#include <QFile>

class logger
{
public:
    logger();
    ~logger();
    /**
     * @brief A log file will be created for the current day
     *  - Application will create a log file only if there is no log file exists for the current day
     */
    void logFileCreation();

    /**
     * @brief To close the log file
     *  - Application will close the log file which is already open
     */
    void close();

    QDateTime date;
    QFile infile;
private:
    bool DEBUG;
    bool WARNING;
    bool CRITICAL;
    bool FATAL;
public slots:
    /**
     * @brief To write the log data into a file
     *  - Application log data will be written to a file.
     * @param Type
     * QtDebugMsg,
     * QtWarningMsg,
     * QtFatalMsg,
     * QtCriticalMsg
     * @param msg
     * Data to be written in the file
     */
    void logHandler(QtMsgType Type,QString msg);
};

#endif // LOGGER_H
