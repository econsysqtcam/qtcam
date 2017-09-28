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
#include "common_enums.h"
#include <linux/uvcvideo.h>

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
    static QStringListModel encoderList;

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

    // get current resoution set in v4l2
    QString getResoultion();

    // Things need to do after change FPS and take shot
    void doAfterChangeFPSAndShot();

    v4l2_format fmt;
    QString _title;
    QString _text;
    QString lastPreviewSize;
    QString lastFormat;
    // Added by Sankari - To maintain last set framerate
    QString lastFPSValue;

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

    uint frameToSkip;

private:

    __u32 m_pixelformat;
    __u32 m_width, m_height;
    __u32 m_buftype;
    __u32 width, height, pixfmt;

    QString m_filePath;
    QString m_imgFormatType;

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
    bool changeFpsAndShot; // To change fps and take shot
    bool fpsChangedForStill;
    bool m_burstShot;
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

    QString ctrlName, ctrlType, ctrlID, ctrlStepSize, ctrlMaxValue, ctrlMinValue,ctrlDefaultValue;
    QString stillSize;
    QString stillOutFormat;
    QString formatType;
    QString filename;
    QString ubuntuVersion;
    QStringList availableFPS; // Moved from funtion updateFrameInterval() in videostreaming.c to here

    QMap<QString,QString> pixFormat;

    QMutex pixmapMutex;

    unsigned char *m_data;
    unsigned char *m_frameData;

    unsigned char *m_buf;
    unsigned m_size;

    uint m_nbuffers;    
    static int deviceNumber;
    static QString camDeviceName;
 /**
     * @brief currentlySelectedCameraEnum - This contains currently selected camera enum value
     */
    static CommonEnums::ECameraNames currentlySelectedCameraEnum;

    uint m_burstLength;
    uint m_burstNumber;

    // Added by Sankari  - 10 Nov 2016 - To decide whether display pop up dialog will appear in while capturing image
    bool m_displayCaptureDialog;

    // Added by Sankari  - 10 Nov 2016 - To decide whether to save image or not
    bool m_saveImage;

    QString getSettings(unsigned int);
    void getFrameRates();
    void updateVidOutFormat();
    void tmpMsgBox(QString);
    bool getInterval(struct v4l2_fract &interval);

    int findMax(QList<int> *llist);
    void freeBuffers(unsigned char *destBuffer,unsigned char *copyBuffer);

    void getFileName(QString filePath,QString imgFormatType);
    void setFilePath(QString filePath);
    QString getFilePath();
    void setImageFormatType(QString imgFormatType);
    QString getImageFormatType();


public slots:
    // Added by Sankari : 10 Dec 2016
    // To Disable image capture dialog when taking trigger shot in trigger mode for 12cunir camera
    void disableImageCaptureDialog();

    // Added by Sankari : 10 Dec 2016
    // Disable saving image when focus is changed from trigger mode to master mode
    // or changing to any other camera if it is m_saveImage flag set as true to avoid displaying unnecessary pop up dialog.
    // It is used in 12cunir camera. It can be used for other cameras also.
    void disableSavingImage();

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
     * @brief Make burst shot preview
     * @param filePath - Captured still will be saved in this location
     * @param imgFormatType - Image formats is of four typede
     * @param burstLength - Number of images to take in a single click
     * 1. JPG
     * 2. BMP
     * 3. RAW
     * 4. PNG
     */
    void makeBurstShot(QString filePath,QString imgFormatType, uint burstLength);

    /**
     * @brief changeFPSandTakeShot - change fps and take still
     * @param filePath
     * @param imgFormatType
     */
    void  changeFPSandTakeShot(QString filePath,QString imgFormatType, uint fpsIndex);

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
    // Added by Sankari - To maintain last set framerate
    void lastFPS(QString fps);
    void formatSaveSuccess(uint imgSaveSuccessCount, bool burstFlag);
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

    void displayEncoderList();
 /**
     * @brief selectedCameraEnum - This slot contains selected camera enum value
     * @param selectedDeviceEnum - Camera enum value
     */
    void selectedCameraEnum(CommonEnums::ECameraNames selectedDeviceEnum);

    void updateFrameToSkip(uint stillSkip);

    void enumerateFPSList();

    // Set the uvc extension control value
    bool setUvcExtControlValue(struct uvc_xu_control_query xquery);

signals:
    void logDebugHandle(QString _text);
    void logCriticalHandle(QString _text);
    void titleTextChanged(QString _title,QString _text);
    void enableCaptureAndRecord();
    void newControlAdded(QString ctrlName,QString ctrlType,QString ctrlID,QString ctrlStepSize = "0",QString ctrlMinValue= "0", QString ctrlMaxValue = "0",QString ctrlDefaultValue="0", QString ctrlHardwareDefault="0");
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
    void enableRfRectBackInPreview();
    void enableFactRectInPreview();

    // Added by Sankari: 02 Dec 2017
    void stillSkipCount(QString stillResoln, QString videoResoln);

    void stillSkipCountWhenFPSChange(bool fpsChange);

    void requestToChangeFPSandTakeShot();

    // To get FPS list
    void sendFPSlist(QString fpsList);   
};

#endif // VIDEOSTREAMING_H
