/*
 * logger.cpp -- logs the Debug, warning and critical message to the log file
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

#include "logger.h"
#include "common.h"

#include <QTextStream>
#include <QSettings>
#include <QDir>

logger::logger()
{
}

logger::~logger(){

}

void logger::logFileCreation() {
    QString log = "logfile";
    if(!QDir(log).exists() )
    {
        QDir().mkdir(log);
    }
    QString curr_date = date.currentDateTime().toString("dd.MM.yyyy");
    curr_date = log+"/"+curr_date;
    if(!QDir(curr_date).exists() )
    {
        QDir().mkdir(curr_date);
    }

    QString curr_time = date.currentDateTime().toString("hh:mm:ss");
    curr_time.append(".log");
    curr_time.prepend("log_");
    QString currentPath = QDir::currentPath();
    QString filename = currentPath+"/"+curr_date+"/"+curr_time;

    infile.setFileName(filename);
    if(infile.exists()) {
        infile.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream ts(&infile);
        QString dt = "********************************************************************************\n";
        dt += "\n Log Created On  " ;
        dt += date.currentDateTime().toString();
        dt += "\n********************************************************************************";
        ts << dt << "\n" << flush;
    } else {
        infile.open(QIODevice::WriteOnly);
        QTextStream ts(&infile);
        QString dt = "********************************************************************************";
        dt += "\n Log Created On  ";
        dt += date.currentDateTime().toString();
        dt += "\n********************************************************************************";
        ts << dt << "\n" << flush;
    }
#if LAUNCHPAD
    QSettings *releaseSettings = new QSettings("/usr/share/qml/qtcam/about/release.ini",QSettings::IniFormat);
#else
    QSettings *releaseSettings = new QSettings("qml/qtcam/about/release.ini",QSettings::IniFormat);
#endif
    releaseSettings->beginGroup("release");
    DEBUG = releaseSettings->value("logDebugEnable").toBool();
    WARNING = releaseSettings->value("logWarningEnable").toBool();
    FATAL = releaseSettings->value("logFatalEnable").toBool();
    CRITICAL = releaseSettings->value("logCriticalEnable").toBool();
    releaseSettings->endGroup();
    delete releaseSettings;
}

void logger::logHandler(QtMsgType type,QString msg)
{
    QString txt;
    txt = msg;
    if(type != QtCriticalMsg) {
        txt.prepend("  ");
        txt.prepend(date.currentDateTime().toString("MMM dd hh:mm:ss AP"));
    }
    QTextStream ts(&infile);
    switch(type) {
    case QtDebugMsg:
        if(DEBUG)
            ts << txt << "\t" << "\n" << flush;
        break;
    case QtWarningMsg:
        if(WARNING) {
            txt.prepend("WARNING : ");
            ts << txt << "\n" << flush;
        }
        break;
    case QtFatalMsg:
        if(FATAL) {
            txt.prepend("FATAL  \t: ");
            ts << txt << "\n" << flush;
        }
        break;
    case QtCriticalMsg:
        if(CRITICAL) {
            txt.prepend("CRITICAL  \t: ");
            ts << txt << "\n" << flush;
        }
        break;
    default:
        ts << txt << "\n" << flush;
    }
}

void logger::close()
{
    infile.close();
}
