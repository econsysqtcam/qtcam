/*
 * videostreaming.h -- preview the camera buffer
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


#ifndef VIDEOSTREAMING_H
#define VIDEOSTREAMING_H

#include <QQuickPaintedItem>
#include <QImage>
#include <QPainter>
#include <QTimer>
#include <QDateTime>
#include <QSocketNotifier>
#include <QStringList>
#include <QStringListModel>
#include <QDir>
#include <sys/mman.h>
#include <libv4l2.h>
#include <QMutex>
#include <QList>
#include <QStandardPaths>
#include <turbojpeg.h>
#include "v4l2-api.h"
#include "videoencoder.h"
#include "h264decoder.h"

#if !LIBAVCODEC_VER_AT_LEAST(54,25)
    #define AV_CODEC_ID_NONE CODEC_ID_NONE
    #define AV_CODEC_ID_MJPEG CODEC_ID_MJPEG
    #define AV_CODEC_ID_RAWVIDEO CODEC_ID_RAWVIDEO
    #define AV_CODEC_ID_H264 CODEC_ID_H264
    #define AV_CODEC_ID_VP8 CODEC_ID_VP8
#endif


class Videostreaming : public QQuickPaintedItem, public v4l2
{
    Q_OBJECT
public:
    struct buffer {
        unsigned planes;
        void   *start[VIDEO_MAX_PLANES];
        size_t  length[VIDEO_MAX_PLANES];
    };
    Videostreaming();
    ~Videostreaming();
    void paint(QPainter *painter);

    QString fileName;

    static QStringListModel resolution;
    static QStringListModel stillOutputFormat;
    static QStringListModel videoOutputFormat;
    static QStringListModel fpsList;    

    void setFrame(unsigned char *data);
    void displayFrame();
    bool setBuffer(unsigned char *buf, unsigned size);

    /* cu130 camera - MPEG high frame rate */
    int jpegDecode(unsigned char **pic, unsigned char *buf, unsigned long bytesUsed);
    int decomp(unsigned char **jpegbuf,
                                unsigned long *jpegsize, unsigned char *dstbuf, int w, int h,
                                int jpegqual, int tilew, int tileh,unsigned char **pic);
    double getTimeInSecs(void);
    void freeBuffer(unsigned char *ptr);

    /* cu40 IR image capture */
    bool extractIRImage(unsigned short int *srcBuffer, unsigned char *irBuffer);

    bool findNativeFormat(__u32 format, QImage::Format &dstFmt);
    bool startCapture();

    v4l2_format fmt;
    QString _title;
    QString _text;
    QString lastPreviewSize;
    QString lastFormat;

    VideoEncoder  *videoEncoder;
    H264Decoder *h264Decode;

    /* Jpeg decode */
    int doyuv;
    int dotile;
    int pf;
    int yuvpad;
    int warmup;
    int flags;
    tjscalingfactor sf;

private:

    __u32 m_pixelformat;
    __u32 m_width, m_height;
    __u32 m_buftype;
    __u32 width, height, pixfmt;

    fd_set fds;
    struct timeval                  tv;
    int                             r;
    unsigned int                    n_buffers;

    QMutex mutex;
    QTime captureTime;

    unsigned m_frame;
    unsigned m_lastFrame;
    unsigned m_fps;
    struct timeval m_tv;

    char header[50];

    bool m_supportedFormat;
    bool openSuccess;
    bool updateOnce;
    bool m_snapShot;
    bool updateStop;
    bool makeSnapShot;
    bool triggerShot;
    bool m_has_interval;
    bool tempMsgBoxValue;
    bool m_VideoRecord;
    bool previewStop;

    QSocketNotifier *m_capNotifier;

    struct v4l2_fract m_interval;
    struct v4l2_fract interval;
    struct v4l2_format m_capSrcFormat;
    struct v4l2_format m_capDestFormat;

    struct v4lconvert_data *m_convertData;
    struct buffer *m_buffers;

    QImage snapShotImage;
    QPixmap qStaticImage;
    QImage *m_capImage;    
    QPixmap qImage;

    QString ctrlName, ctrlType, ctrlID, ctrlMaxValue, ctrlMinValue,ctrlDefaultValue;
    QString stillSize;
    QString stillOutFormat;
    QString formatType;
    QString filename;

    QMap<QString,QString> pixFormat;

    QMutex pixmapMutex;

    unsigned char *m_data;
    unsigned char *m_frameData;

    unsigned char *m_buf;
    unsigned m_size;

    uint m_nbuffers;
    int correctionDisplay;
    static int deviceNumber;
    static QString camDeviceName;

    QString getSettings(unsigned int);
    void getFrameRates();
    void updateVidOutFormat();
    void tmpMsgBox(QString);
    bool getInterval(struct v4l2_fract &interval);

    int findMax(QList<int> *llist);
    void freeBuffers(unsigned char *destBuffer,unsigned char *copyBuffer);

public slots:
    /**
     * @brief Query buffer and Deque buffer
     *  -
     */
    void capFrame();

    /**
     * @brief Still Capture the image preview [In Default/Master Mode]
     * @param filePath - Captured still will be saved in this location
     * @param imgFormatType - Image formats is of four typede
     * 1. JPG
     * 2. BMP
     * 3. RAW
     * 4. PNG
     */
    void makeShot(QString filePath,QString imgFormatType);

    /**
     * @brief Still Capture the image preview [In Trigger Mode]
     * @param filePath - Captured still will be saved in this location
     * @param imgFormatType - Image formats is of four typede
     * 1. JPG
     * 2. BMP
     * 3. RAW
     * 4. PNG
     */
    void triggerModeShot(QString filePath,QString imgFormatType);

    /**
     * @brief To stop the camera preview
     */
    void stopCapture();

    /**
     * @brief To start the camera preview
     */
    void startAgain();

    /**
     * @brief To set the camera resolution.
     * @param resolution - Actual resolution set to the camera.  For example, 640x480.
     */
    void setResoultion(QString resolution);

    /**
     * @brief To open the camera device(video node).
     * @param deviceName - Its the video node name (/dev/video)
     */
    void setDevice(QString deviceName);

    /**
     * @brief Close the last instance opened camera device.  User should call stopCapture() before calling this function.
     */
    void closeDevice();

    /**
     * @brief Emulate the available colorspace supported by the camera.
     */
    void displayOutputFormat();

    /**
     * @brief Emulate the available resolution for still capturing supported by the camera.
     */
    void displayStillResolution();


    /**
     * @brief Emulate the available resolution for video streaming supported by the camera.
     */
    void displayVideoResolution();

    /**
     * @brief To set the camera device node number to setDevice() function
     * @param deviceNode - Device node number.  Must be 0 or greater than 0.
     */
    void getFirstDevice(int deviceNode);

    /**
     * @brief To set the camera name
     * @param deviceName - Camera name
     */
    void getCameraName(QString deviceName);

    /**
     * @brief To get the available camera controls of the current device(Brightness, contrast...)
     * @param actualValue
     * 1. true - To get the last saved values of the controls.  On First plug last saved values will be default values.
     * 2. false - To get the default values of the controls.
     */
    void cameraFilterControls(bool actualValue = true);

    /**
     * @brief To change the values of the camera controls
     * @param id - Id of any one of the camera control
     * @param value - Value to be set the for the camera control based on the ID.  Value should be with in the maximum
     * and minimum value.
     */
    void changeSettings(unsigned int id,QString value);

    /**
     * @brief selectMenuIndex
     * @param id
     * @param value
     */
    void selectMenuIndex(unsigned int id, int value);


    void vidCapFormatChanged(QString idx);
    void setStillVideoSize(QString stillValue,QString stillFormat);
    void lastPreviewResolution(QString resolution,QString format);
    void formatSaveSuccess(bool success);
    void updateFrameInterval(QString pixelFormat, QString frameSize);

    /**
     * @brief frameIntervalChanged
     * @param idx
     */
    void frameIntervalChanged(int idx);

    /**
     * @brief This function should be called to update the preview
     * @param img - image displayed in the preview
     */
    void updateFrame(QImage img);

    /**
     * @brief To begin video recording this function should be called
     * @param videoEncoderType - Encoder types are video codecs, Currently four codecs are used as follows
     * 1. RAW VIDEO
     * 2. MJPEG
     * 3. H264
     * 4. VP8
     * @param videoFormatType - Format type is the container used to record the video.  Currently two containers are used as follows
     * 1. avi
     * 2. mkv
     * @param fileLocation - Location where the recorded file is saved
     */
    void recordBegin(int videoEncoderType, QString videoFormatType, QString fileLocation);

    /**
     * @brief This function should be called to stop the video recording
     */
    void recordStop();

    /**
     * @brief To stop updating the preview in the trigger Mode
     */
    void triggerModeEnabled();

    /**
     * @brief Preview will be continously updating in this mode
     */
    void masterModeEnabled();

    int getMenuIndex(unsigned int,int);

signals:
    void logDebugHandle(QString _text);
    void logCriticalHandle(QString _text);
    void titleTextChanged(QString _title,QString _text);
    void enableCaptureAndRecord();
    void newControlAdded(QString ctrlName,QString ctrlType,QString ctrlID,QString ctrlMinValue= "0", QString ctrlMaxValue = "0",QString ctrlDefaultValue="0", QString ctrlHardwareDefault="0");
    void deviceUnplugged(QString _title,QString _text);    
    void averageFPS(unsigned fps);
    void defaultStillFrameSize(unsigned int outputIndexValue);
    void defaultFrameSize(unsigned int outputIndexValue, unsigned int  defaultWidth, unsigned int defaultHeight);
    void defaultOutputFormat(unsigned int formatIndexValue);
    void defaultFrameInterval(unsigned int frameInterval);
    void captureSaveTime(QString saveTime);
    void refreshDevice();
    void addControls();
    void rcdStop(QString recordFail);
    void videoRecord(QString fileName);
};

#endif // VIDEOSTREAMING_H
