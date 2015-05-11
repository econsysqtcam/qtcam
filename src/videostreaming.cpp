/*
 * videostreaming.cpp -- preview the camera buffer
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

#include "videostreaming.h"
#include <QtCore/QCoreApplication>
#include <QtGui/QGuiApplication>
#include <QtWidgets>
#include <QDesktopWidget>
#include <QLabel>
#include <QMessageBox>
#include <QRectF>
#include <fcntl.h>
#define CLEAR(x) memset(&(x), 0, sizeof(x))
QStringListModel Videostreaming::resolution;
QStringListModel Videostreaming::outputFormat;
QStringListModel Videostreaming::fpsList;
int Videostreaming::deviceNumber;
QString Videostreaming::camDeviceName;

Videostreaming::Videostreaming()
{
    openSuccess = false;
    updateOnce = true;
    m_snapShot = false;
    makeSnapShot = false;
    triggerShot = false;
    correctionDisplay = 0;
    m_VideoRecord = false;
}

Videostreaming::~Videostreaming()
{

}

void Videostreaming::setDevice(QString deviceName) {

    close();
    deviceName.append(QString::number(deviceNumber,10));
    if(open(deviceName,false)) {
        emit logDebugHandle("Device Opened - "+deviceName);
        m_convertData = v4lconvert_create(fd());
        m_buftype= V4L2_BUF_TYPE_VIDEO_CAPTURE;
        openSuccess = true;
    } else {
        emit logCriticalHandle("Device Opening Failed - "+deviceName);
    }
}

void Videostreaming::getFirstDevice(int deviceNode) {
    deviceNumber = deviceNode;
}

void Videostreaming::getCameraName(QString deviceName){
    camDeviceName=deviceName;
}

void Videostreaming::paint(QPainter *painter)
{
    if(pixmapMutex.tryLock()) {
        if(!m_snapShot) {
            painter->drawPixmap(0,0,qImage);
            qStaticImage = qImage;
        } else {
            painter->drawPixmap(0,0, qStaticImage);
        }
        pixmapMutex.unlock();
    }
}

void Videostreaming::updateFrame(QImage img) {

    if(!m_snapShot && !updateStop) {
        qImage=QPixmap::fromImage(img);
        update();
    }
}

void Videostreaming::capFrame()
{
    __u32 buftype = m_buftype;
    v4l2_plane planes[VIDEO_MAX_PLANES];
    v4l2_buffer buf;
    int err = 0;
    bool again;
    //temp.fmt.pix.pixelformat = V4L2_PIX_FMT_Y16;  // cu51
    unsigned char *displaybuf = NULL;
    memset(planes, 0, sizeof(planes));
    buf.length = VIDEO_MAX_PLANES;
    buf.m.planes = planes;
    if (!dqbuf_mmap(buf, buftype, again)) {
        closeDevice();
        unsigned char *m_data=NULL;
        QImage tempImage(m_data,320,240,QImage::Format_RGB888);
        qImage = QPixmap::fromImage(tempImage);
        update();
        emit deviceUnplugged("Disconnected","Device Not Found");
        emit logCriticalHandle("Device disconnected");
        return;
    }
    if (again) {
        return;
    }
    if (buf.flags & V4L2_BUF_FLAG_ERROR) {
        qbuf(buf);
        return;
    }

    err = v4lconvert_convert(m_convertData, &m_capSrcFormat, &m_capDestFormat,
                             (unsigned char *)m_buffers[buf.index].start[0], buf.bytesused,
            m_capImage->bits(), m_capDestFormat.fmt.pix.sizeimage);

    if (err != -1) {
        displaybuf = m_capImage->bits();
    }

    if (err == -1) {
        logCriticalHandle(v4lconvert_get_error_message(m_convertData));
        //return void();
    }

    unsigned char* asil=(unsigned char*)malloc(m_capDestFormat.fmt.pix.sizeimage+qstrlen(header));
    memmove(asil, m_capImage->bits(), m_capDestFormat.fmt.pix.sizeimage);
    memmove(asil+qstrlen(header), asil, m_capDestFormat.fmt.pix.sizeimage);
    memcpy(asil,header,qstrlen(header));
    QImage *qq = new QImage();
    if(qq->loadFromData(asil,m_capDestFormat.fmt.pix.sizeimage+qstrlen(header),"PPM")) {
        updateFrame(*qq);
        if(m_VideoRecord) {
            if(videoEncoder!=NULL) {
                videoEncoder->encodeImage(*qq);
            }
        }
    } else {
        logCriticalHandle("Unsupported Format...");
    }
    free(asil);
    delete qq;
    if(m_frame > 1 && m_snapShot) {
        bool tmpValue;
        if(formatType == "raw") {
            QFile file(filename);
            if(file.open(QIODevice::WriteOnly)) {
                int tmpRet = file.write((const char*)m_buffers[buf.index].start[0],buf.bytesused);
                if(tmpRet != -1) {
                    tmpValue = true;
                } else {
                    tmpValue = false;
                }
                file.close();
            } else {
                tmpValue = false;
            }
        }
        else {
            QImage *qImage2 = new QImage(displaybuf, width, height,QImage::Format_RGB888);
            bool tmpRet = qImage2->save(filename);
            tmpValue = tmpRet;
            delete qImage2;
        }
        if(triggerShot) {
            captureSaveTime("Capture time: " +(QString::number((double)captureTime.elapsed()/1000)) + "seconds");
            makeSnapShot = false;
            m_snapShot = false;
            if(tmpValue)
                formatSaveSuccess(true);
            else
                formatSaveSuccess(false);

        } else {
            captureSaveTime("Capture time: " +(QString::number((double)captureTime.elapsed()/1000)) + "seconds");
            stopCapture();
            makeSnapShot = false;
            vidCapFormatChanged(lastFormat);
            setResoultion(lastPreviewSize);
            startAgain();
            m_snapShot = false;
            if(tmpValue)
                formatSaveSuccess(true);
            else
                formatSaveSuccess(false);
            return void();

        }
    }
    getFrameRates();
    qbuf(buf);
}

void Videostreaming::getFrameRates() {
    struct timeval tv, res;
    if (m_frame == 0)
        gettimeofday(&m_tv, NULL);
    gettimeofday(&tv, NULL);
    timersub(&tv, &m_tv, &res);
    if (res.tv_sec) {
        m_fps = (100 * (m_frame - m_lastFrame)) / (res.tv_sec * 100 + res.tv_usec / 10000);
        m_lastFrame = m_frame;
        m_tv = tv;
    }
    emit framesPlayed(++m_frame);
    emit averageFPS(m_fps);
}

bool Videostreaming::startCapture()
{
    __u32 buftype = m_buftype;
    v4l2_requestbuffers req;
    unsigned int i;

    memset(&req, 0, sizeof(req));


    if (!reqbufs_mmap(req, buftype, 3)) {
        emit logCriticalHandle("Cannot capture");
        return false;
    }

    if (req.count < 2) {
        emit logCriticalHandle("Too few buffers");
        reqbufs_mmap(req, buftype);
        return false;
    }

    m_buffers = (buffer *)calloc(req.count, sizeof(*m_buffers));

    if (!m_buffers) {
        emit logCriticalHandle("Out of memory");
        reqbufs_mmap(req, buftype);
        return false;
    }

    for (m_nbuffers = 0; m_nbuffers < req.count; ++m_nbuffers) {
        v4l2_plane planes[VIDEO_MAX_PLANES];
        v4l2_buffer buf;

        memset(&buf, 0, sizeof(buf));

        buf.type        = buftype;
        buf.memory      = V4L2_MEMORY_MMAP;
        buf.index       = m_nbuffers;
        buf.length      = VIDEO_MAX_PLANES;
        buf.m.planes    = planes;

        if (-1 == ioctl(VIDIOC_QUERYBUF, &buf)) {
            perror("VIDIOC_QUERYBUF");
            return false;
        }

        m_buffers[m_nbuffers].planes = 1;
        m_buffers[m_nbuffers].length[0] = buf.length;
        m_buffers[m_nbuffers].start[0] = mmap(buf.length, buf.m.offset);
        if (MAP_FAILED == m_buffers[m_nbuffers].start[0]) {
            perror("mmap");
            return false;
        }
    }
    for (i = 0; i < m_nbuffers; ++i) {
        if (!qbuf_mmap(i, buftype)) {
            emit logCriticalHandle("Querying Buffer failed");
            perror("VIDIOC_QBUF");
            return false;
        }
    }
    if (!streamon(buftype)) {
        emit logCriticalHandle("Stream ON failed");
        perror("VIDIOC_STREAMON");
        return false;
    }
    return true;
}

int Videostreaming::findMax(QList<int> *list) {

    int array[list->count()];
    for(int j=0;j<list->count();j++)
        array[j] = list->at(j);
    int index_of_min = 0;
    for(int x=0; x<list->count(); x++)
    {
        index_of_min = x;
        for(int y=x; y<list->count(); y++)

        {
            if(array[index_of_min] > array[y])
            {
                index_of_min = y;
            }
        }
        int temp = array[x];
        array[x] = array[index_of_min];
        array[index_of_min] = temp;
    }
    return array[index_of_min];
}

void Videostreaming::makeShot(QString filePath,QString imgFormatType) {

    captureTime.start();
    m_snapShot = true;
    QDateTime dateTime = QDateTime::currentDateTime();
    QDir tmpDir;
    if(tmpDir.cd(filePath)) {
        QStringList filters,list;
        filters << "Qtcam-" + dateTime.toString("yy_MM_dd:hh_mm_ss")+"-*"+imgFormatType;
        tmpDir.setNameFilters(filters);
        list << tmpDir.entryList(filters,QDir::Files);
        if(!list.isEmpty()) {
            QList<int> tmpStr;
            for(int i=0;i<list.count();i++) {
                tmpStr.append(list.at(i).split(".").at(0).split("-").at(1).toInt());
            }
            int lastNumber = findMax(&tmpStr);
            lastNumber++;
            filename = filePath +"/Qtcam-" + dateTime.toString("yy_MM_dd:hh_mm_ss")+"-" +QString::number(lastNumber,10)+"."+ imgFormatType;
        } else {
            filename = filePath +"/Qtcam-" + dateTime.toString("yy_MM_dd:hh_mm_ss")+"-" +QString::number(1,10)+"."+ imgFormatType;
        }
    } else {
        QStringList filters,list;
        filters << "Qtcam-" + dateTime.toString("yy_MM_dd:hh_mm_ss")+"-*"+imgFormatType;
        tmpDir.cd(tmpDir.currentPath());
        tmpDir.setNameFilters(filters);
        list << tmpDir.entryList(filters,QDir::Files);
        if(!list.isEmpty()) {
            QList<int> tmpStr;
            for(int i=0;i<list.count();i++) {
                tmpStr.append(list.at(i).split(".").at(0).split("-").at(1).toInt());
            }
            int lastNumber = findMax(&tmpStr);
            lastNumber++;
            filename = "Qtcam-" + dateTime.toString("yy_MM_dd:hh_mm_ss")+"-" +QString::number(lastNumber,10)+"."+ imgFormatType;
        } else {
            filename = "Qtcam-" + dateTime.toString("yy_MM_dd:hh_mm_ss")+"-" +QString::number(1,10)+"."+ imgFormatType;
        }
    }
    makeSnapShot = true;
    triggerShot = false;
    stopCapture();
    vidCapFormatChanged(stillOutFormat);
    setResoultion(stillSize);
    formatType = imgFormatType;
    startAgain();
}

void Videostreaming::triggerModeShot(QString filePath,QString imgFormatType) {

    captureTime.restart();
    m_snapShot = true;
    QDateTime dateTime = QDateTime::currentDateTime();
    QDir tmpDir;
    if(tmpDir.cd(filePath)) {
        filename = filePath +"/Qtcam-" + dateTime.toString("yy_MM_dd:hh_mm_ss")+"."+ imgFormatType;
    } else {
        filename = "Qtcam" + dateTime.toString("yy_MM_dd:hh_mm_ss")+"." + imgFormatType;
    }
    makeSnapShot = true;
    triggerShot = true;
    formatType = imgFormatType;
    m_frame = 3;
}

void Videostreaming::formatSaveSuccess(bool success) {
    if(success) {
        _title = "Captured";
        _text = "Image saved in the location:" + filename;
        emit logDebugHandle("Still image saved successfully in " + filename);
        emit titleTextChanged(_title,_text);
    } else {
        _title = "Failure";
        _text = "Image not saved in the selected location";
        emit logCriticalHandle("Still image not saved successfully");
        emit titleTextChanged(_title,_text);
    }
}

bool Videostreaming::getInterval(struct v4l2_fract &interval)
{
    if (m_has_interval)
        interval = m_interval;
    return m_has_interval;
}

void Videostreaming::displayFrame() {
    emit logDebugHandle("Start Previewing");
    m_frame = m_lastFrame = m_fps = 0;
    emit framesPlayed(m_frame);
    emit averageFPS(m_fps);

    __u32 buftype = m_buftype;

    g_fmt_cap(buftype, m_capSrcFormat);
    if (try_fmt(m_capSrcFormat)) {
        if(!s_fmt(m_capSrcFormat)) {
            emit titleTextChanged("Error", "Device or Resource is busy");
            emit logCriticalHandle("Device or Resource is busy");
            if (fd() >= 0) {
                v4lconvert_destroy(m_convertData);
                close();
            }
            return void();
        }
    } else {
        return void();
    }

    if (getInterval(interval))
        set_interval(buftype, interval);

    emit logDebugHandle("Preview settings completed");

    m_capDestFormat = m_capSrcFormat;
    m_capDestFormat.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;
    //m_capDestFormat.fmt.pix.sizeimage = (m_capDestFormat.fmt.pix.width * m_capDestFormat.fmt.pix.height * 3);
    v4l2_format copy = m_capSrcFormat;
    v4lconvert_try_format(m_convertData, &m_capDestFormat, &m_capSrcFormat);
    // v4lconvert_try_format sometimes modifies the source format if it thinks
    // that there is a better format available. Restore our selected source
    // format since we do not want that happening.
    m_capSrcFormat = copy;
    width = m_capDestFormat.fmt.pix.width;
    height = m_capDestFormat.fmt.pix.height;
    pixfmt = m_capDestFormat.fmt.pix.pixelformat;

    m_capImage = new QImage(width, height, QImage::Format_RGB888);
    if(camDeviceName == "e-con's 1MP Bayer RGB \nCamera") {
        m_capSrcFormat.fmt.pix.pixelformat = V4L2_PIX_FMT_SGRBG8;
    }
    if (startCapture()) {
        sprintf(header,"P6\n%d %d 255\n",width,height);
        m_capNotifier = new QSocketNotifier(fd(), QSocketNotifier::Read);
        connect(m_capNotifier, SIGNAL(activated(int)), this, SLOT(capFrame()));
    }
}

void Videostreaming::stopCapture() {
    if (fd() >= 0) {
        emit logDebugHandle("Stop Previewing...");
        const char *m_data;
        m_data=NULL;
        QPixmap tempImage(m_data);
        qImage = tempImage;
        v4l2_requestbuffers reqbufs;
        if (m_buffers == NULL)
            return;
        if (!streamoff(V4L2_BUF_TYPE_VIDEO_CAPTURE)) {
            perror("VIDIOC_STREAMOFF");
            emit logCriticalHandle("Stream OFF failed");
        }
        for (uint i = 0; i < m_nbuffers; ++i)
            for (unsigned p = 0; p < m_buffers[i].planes; p++)
                if (-1 == munmap(m_buffers[i].start[p], m_buffers[i].length[p]))
                    perror("munmap");
        // Free all buffers.
        reqbufs_mmap(reqbufs, V4L2_BUF_TYPE_VIDEO_CAPTURE, 1);  // videobuf workaround
        reqbufs_mmap(reqbufs, V4L2_BUF_TYPE_VIDEO_CAPTURE, 0);
        emit logDebugHandle("Value of FD is:"+ QString::number(fd(),10));
        if (m_capNotifier) {
            delete m_capNotifier;
            delete m_capImage;
            m_capNotifier = NULL;
            m_capImage = NULL;
        }
    }
}

void Videostreaming::closeDevice() {
    emit logDebugHandle("Closing the current camera device");
    if (fd() >= 0) {
        if (m_capNotifier) {
            delete m_capNotifier;
            delete m_capImage;
            m_capNotifier = NULL;
            m_capImage = NULL;
        }
        v4lconvert_destroy(m_convertData);
        close();
    }
}

void Videostreaming::startAgain() {
    if(openSuccess) {
        displayFrame();
    }
}

void Videostreaming::lastPreviewResolution(QString resolution,QString format) {
    lastPreviewSize = resolution;
    lastFormat = format;
    emit logDebugHandle("Last Resolution displayed at::"+resolution);
}

void Videostreaming::setResoultion(QString resolution)
{

    emit logDebugHandle("Resolution set at::"+resolution);
    v4l2_format fmt;
    unsigned int width, height;
    QStringList tempResList = resolution.split('x');
    width = tempResList.value(0).toInt();
    height = tempResList.value(1).toInt();
    g_fmt_cap(V4L2_BUF_TYPE_VIDEO_CAPTURE, fmt);
    fmt.fmt.pix.width = width;
    fmt.fmt.pix.height = height;
    m_width = width;
    m_height = height;
    if (try_fmt(fmt)) {
        s_fmt(fmt);
    }
}

void Videostreaming::displayResolution() {
    g_fmt_cap(V4L2_BUF_TYPE_VIDEO_CAPTURE, fmt);

    QStringList dispRes;

    m_pixelformat = fmt.fmt.pix.pixelformat;
    m_width       = fmt.fmt.pix.width;
    m_height      = fmt.fmt.pix.height;

    unsigned int indexCount = -1;
    v4l2_frmsizeenum frmsize;
    bool ok = false;
    ok = enum_framesizes(frmsize, m_pixelformat);
    if (ok && frmsize.type == V4L2_FRMSIZE_TYPE_DISCRETE) {
        do {
            indexCount++;
            dispRes.append(QString("%1x%2").arg(frmsize.discrete.width).arg(frmsize.discrete.height));
            if (frmsize.discrete.width == m_width && frmsize.discrete.height == m_height) {
                emit defaultFrameSize(indexCount);
            }
        } while (enum_framesizes(frmsize));
    }
    outputFormat.setStringList(dispRes);
    emit logDebugHandle("Supported Resolution: " +dispRes.join(", "));
}

void Videostreaming::vidCapFormatChanged(QString idx)
{
    v4l2_fmtdesc desc;
    enum_fmt_cap(desc, m_buftype, true, idx.toInt());
    v4l2_format fmt;
    g_fmt_cap(m_buftype, fmt);
    fmt.fmt.pix.pixelformat = desc.pixelformat;
    if (try_fmt(fmt))
        s_fmt(fmt);
    else {
        return void();
    }
    if(!makeSnapShot)
        updateVidOutFormat();
}

void Videostreaming::updateVidOutFormat()
{
    v4l2_fmtdesc desc;
    v4l2_format fmt;
    g_fmt_cap(m_buftype, fmt);
    m_pixelformat = fmt.fmt.pix.pixelformat;
    m_width       = fmt.fmt.pix.width;
    m_height      = fmt.fmt.pix.height;
    if (enum_fmt_cap(desc, m_buftype, true)) {
        do {
            if (desc.pixelformat == fmt.fmt.pix.pixelformat)
                break;
        } while (enum_fmt_cap(desc, m_buftype));
    }
    if (desc.pixelformat != fmt.fmt.pix.pixelformat)
        return;
    emit defaultOutputFormat(desc.index);
    emit logDebugHandle("Color Space set to: "+pixfmt2s(m_pixelformat));
}

void Videostreaming::displayOutputFormat() {
    QStringList dispOutFormat;
    v4l2_fmtdesc fmt;
    pixFormat.clear();
    if (enum_fmt_cap(fmt, V4L2_BUF_TYPE_VIDEO_CAPTURE, true)) {
        do {
            QString s(pixfmt2s(fmt.pixelformat) + " (");
            pixFormat.insert(pixfmt2s(fmt.pixelformat), QString::number(fmt.pixelformat));
            if (fmt.flags & V4L2_FMT_FLAG_EMULATED)
                dispOutFormat.append(s + "Emulated)");
            else {
                dispOutFormat.append(s + (const char *)fmt.description + ")");
            }
        } while (enum_fmt_cap(fmt, V4L2_BUF_TYPE_VIDEO_CAPTURE));

    }
    emit logDebugHandle("Output format supported: " +dispOutFormat.join(", "));
    //dispOutFormat.sort();
    resolution.setStringList(dispOutFormat);
    updateVidOutFormat();
}

void Videostreaming::updateFrameInterval(QString pixelFormat, QString frameSize)
{
    v4l2_frmivalenum frmival;
    v4l2_fract curr;
    bool curr_ok, ok;
    QStringList tempResList = frameSize.split('x');
    width = tempResList.value(0).toInt();
    height = tempResList.value(1).toInt();
    QStringList tempPixFmt = pixelFormat.split(' ');
    QString pixFmtValue = tempPixFmt.value(0);
    ok = enum_frameintervals(frmival,pixFormat.value(pixFmtValue).toInt(), width, height);
    m_has_interval = ok && frmival.type == V4L2_FRMIVAL_TYPE_DISCRETE;
    QStringList availableFPS;
    availableFPS.clear();
    if (m_has_interval) {
        m_interval = frmival.discrete;
        curr_ok = v4l2::get_interval(m_buftype, curr);
        do {
            availableFPS.append(QString::number((double)frmival.discrete.denominator / frmival.discrete.numerator).append(" FPS"));
            if (curr_ok && frmival.discrete.numerator == curr.numerator && frmival.discrete.denominator == curr.denominator) {
                emit defaultFrameInterval(frmival.index);
                m_interval = frmival.discrete;
            }
        } while (enum_frameintervals(frmival));
    }
    emit logDebugHandle("Available FPS:"+ availableFPS.join(", "));
    fpsList.setStringList(availableFPS);
}

void Videostreaming::frameIntervalChanged(int idx)
{
    v4l2_frmivalenum frmival;
    emit logDebugHandle("Pixel Format:"+ QString::number(m_pixelformat));
    emit logDebugHandle("Width:"+ QString::number(m_width));
    emit logDebugHandle("Height:"+ QString::number(m_height));
    emit logDebugHandle("IDX Value:"+QString::number(idx));
    if (enum_frameintervals(frmival, m_pixelformat, m_width, m_height, idx)
            && frmival.type == V4L2_FRMIVAL_TYPE_DISCRETE) {
        if (set_interval(m_buftype, frmival.discrete)) {
            m_interval = frmival.discrete;
        }
    }
}

void Videostreaming::cameraFilterControls(bool actualValue) {
    v4l2_queryctrl qctrl;
    v4l2_querymenu qmenu;
    int indexValue;
    qctrl.id = V4L2_CTRL_FLAG_NEXT_CTRL;
    emit logDebugHandle("Available Controls:");
    while(queryctrl(qctrl)) {
        emit logDebugHandle((char*)qctrl.name);
        switch (qctrl.type) {
        case V4L2_CTRL_TYPE_BOOLEAN:
            ctrlName = (char*)qctrl.name;
            ctrlType = QString::number(qctrl.type,10);
            ctrlID = QString::number(qctrl.id,10);
            if(actualValue) {
                emit newControlAdded(ctrlName,ctrlType,ctrlID,QString::number(0,10),QString::number(1,10),getSettings(qctrl.id));
            } else {
                emit newControlAdded(ctrlName,ctrlType,ctrlID,QString::number(0,10),QString::number(1,10),QString::number(qctrl.default_value,10));
            }
            break;
        case V4L2_CTRL_TYPE_INTEGER:
            ctrlName = (char*)qctrl.name;
            ctrlType = QString::number(qctrl.type,10);
            ctrlID = QString::number(qctrl.id,10);
            ctrlMaxValue = QString::number(qctrl.maximum,10);
            ctrlMinValue = QString::number(qctrl.minimum,10);
            ctrlDefaultValue = QString::number(qctrl.default_value,10);
            if(actualValue) {
                emit newControlAdded(ctrlName,ctrlType,ctrlID,ctrlMinValue,ctrlMaxValue,getSettings(qctrl.id));
            } else {
                emit newControlAdded(ctrlName,ctrlType,ctrlID,ctrlMinValue,ctrlMaxValue,QString::number(qctrl.default_value,10));
            }
            break;
        case V4L2_CTRL_TYPE_MENU:
        case V4L2_CTRL_TYPE_INTEGER_MENU:
            ctrlName = (char*)qctrl.name;
            ctrlType = QString::number(qctrl.type,10);
            ctrlID = QString::number(qctrl.id,10);
            for (int i = qctrl.minimum; i <= qctrl.maximum; i++) {
                qmenu.id = qctrl.id;
                qmenu.index = i;
                if (!querymenu(qmenu))
                    continue;
                if (qctrl.type == V4L2_CTRL_TYPE_MENU) {
                    emit newControlAdded((char *)qmenu.name,ctrlType,ctrlID);
                }
                else {
                    //combo->addItem(QString("%1").arg(qmenu.value));
                    //menuItems.append(qmenu.value);
                }
            }
            indexValue =  getSettings(qctrl.id).toInt();
            if(actualValue) {
                emit newControlAdded(ctrlName,ctrlType,ctrlID,ctrlMinValue,ctrlMaxValue,QString::number(getMenuIndex(qctrl.id,indexValue),10));
            } else {
                emit newControlAdded(ctrlName,ctrlType,ctrlID,ctrlMinValue,ctrlMaxValue,QString::number(getMenuIndex(qctrl.id,qctrl.default_value),10));
            }
            break;
        case V4L2_CTRL_TYPE_BITMASK:
            break;
        case V4L2_CTRL_TYPE_BUTTON:
            break;
        case V4L2_CTRL_TYPE_INTEGER64:
            break;
        case V4L2_CTRL_TYPE_STRING:
            break;
        case V4L2_CTRL_TYPE_CTRL_CLASS:
            break;
        }
        qctrl.id |= V4L2_CTRL_FLAG_NEXT_CTRL;
    }
}

QString Videostreaming::getSettings(unsigned int id) {
    struct v4l2_control c;
    c.id = id;
    if (ioctl(VIDIOC_G_CTRL, &c)) {
        v4l2_queryctrl qctrl;
        qctrl.id = id;
        emit logCriticalHandle("Unable to get the Value, setting the Default value: "+ QString::number(qctrl.default_value,10));
        return QString::number(qctrl.default_value,10);
    }
    QString value = QString::number(c.value,10);
    return value;
}

void Videostreaming::changeSettings(unsigned int id, QString value) {
    struct v4l2_control c;
    c.id = id;
    c.value = value.toInt();
    if (ioctl(VIDIOC_S_CTRL, &c)) {
        emit logCriticalHandle("Error in setting the Value");
    }
}


void Videostreaming::selectMenuIndex(unsigned int id, int value) {
    v4l2_queryctrl qctrl;
    v4l2_querymenu qmenu;
    qctrl.id = id;
    queryctrl(qctrl);
    int i;
    for (i = qctrl.minimum; i <= qctrl.maximum; i++) {
        qmenu.id = qctrl.id;
        qmenu.index = i;
        if (!querymenu(qmenu))
            continue;
        if (value-- == 0)
            break;
    }
    changeSettings(id,QString::number(i,10));
}

int Videostreaming::getMenuIndex(unsigned int id,int value) {
    v4l2_queryctrl qctrl;
    v4l2_querymenu qmenu;
    qctrl.id = id;
    queryctrl(qctrl);
    int i, j = 0;
    for (i = qctrl.minimum; i <= qctrl.maximum; i++) {
        qmenu.id = qctrl.id;
        qmenu.index = i;
        if (!querymenu(qmenu)) {
            value--;
            continue;
        }
        if (value-- == 0)
            break;
        j++;
    }
    return j;
}

void Videostreaming::setStillVideoSize(QString stillValue, QString stillFormat) {
    stillSize = stillValue;
    stillOutFormat = stillFormat;
}

void Videostreaming::recordBegin(int videoEncoderType, QString videoFormatType, QString fileLocation) {
    m_VideoRecord = true;
    videoEncoder=new VideoEncoder();
    if(videoFormatType.isEmpty()) {
        videoFormatType = "avi";        //Application never enters in this condition
    }
    switch(videoEncoderType) {
    case 0:
        videoEncoderType = AV_CODEC_ID_RAWVIDEO;
        break;
    case 1:
        videoEncoderType = AV_CODEC_ID_MJPEG;
        break;
    case 2:
        videoEncoderType = AV_CODEC_ID_H264;
        break;
    case 3:
        videoEncoderType = AV_CODEC_ID_VP8;
        break;
    }

    fileName = fileLocation +"/Qtcam-" + QDateTime::currentDateTime().toString("yy_MM_dd:hh_mm_ss")+"."+ videoFormatType;
    v4l2_frmivalenum frmival;
    enum_frameintervals(frmival, m_pixelformat, m_width, m_height);
    bool tempRet = videoEncoder->createFile(fileName,(AVCodecID)videoEncoderType, m_capDestFormat.fmt.pix.width,m_capDestFormat.fmt.pix.height,frmival.discrete.denominator,frmival.discrete.numerator,10000000);
    if(!tempRet){
        emit rcdStop("Unable to record the video");
    }
}

void Videostreaming::recordStop() {

    emit videoRecord(fileName);
    m_VideoRecord = false;
    if(videoEncoder!=NULL){
        videoEncoder->closeFile();
        delete videoEncoder;
        videoEncoder=NULL;
    }
}

void Videostreaming::triggerModeEnabled() {
    updateStop = true;
}

void Videostreaming::masterModeEnabled() {
    updateStop = false;
}
