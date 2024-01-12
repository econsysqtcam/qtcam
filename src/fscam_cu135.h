/*
 * seecam_cu135.h -- Handling special feature of seecamcu135 camera
 * Copyright © 2024  e-con Systems India Pvt. Limited
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
#ifndef FSCAM_CU135_H
#define FSCAM_CU135_H

#include <QObject>
#include "uvccamera.h"
#include <QtConcurrentRun>


#define EXPOSURECOMP_MIN 8000
#define EXPOSURECOMP_MAX 1000000
#define EXPOSURECOMP_MIN 8000
#define EXPOSURECOMP_MAX 1000000

/* FSCAM_CU135 */
#define CAMERA_CONTROL_FSCAM_CU135       0x95

#define SET_SPECIAL_EFFECT_MODE_FSCAM_CU135    0x04
#define GET_SPECIAL_EFFECT_MODE_FSCAM_CU135    0x03

#define SET_SCENE_MODE_FSCAM_CU135             0x02
#define GET_SCENE_MODE_FSCAM_CU135             0x01

#define SET_DENOISE_CONTROL_FSCAM_CU135        0x06
#define GET_DENOISE_CONTROL_FSCAM_CU135        0x05

#define GET_Q_FACTOR_FSCAM_CU135               0x0B
#define SET_Q_FACTOR_FSCAM_CU135		 0x0C

#define SET_HDR_MODE_FSCAM_CU135		 0x0A
#define GET_HDR_MODE_FSCAM_CU135		 0x09

#define SET_STREAM_MODE_FSCAM_CU135            0x0E
#define GET_STREAM_MODE_FSCAM_CU135            0x0D

#define GET_LED_CONTROL_FSCAM_CU135            0x26
#define SET_LED_CONTROL_FSCAM_CU135            0x27

#define SET_ORIENTATION_FSCAM_CU135            0x11
#define GET_ORIENTATION_FSCAM_CU135            0x10

#define SET_EXPOSURE_COMPENSATION_FSCAM_CU135  0x17
#define GET_EXPOSURE_COMPENSATION_FSCAM_CU135  0x16

#define SET_EXP_ROI_MODE_FSCAM_CU135                     0x08
#define GET_EXP_ROI_MODE_FSCAM_CU135                     0x07

#define SET_FRAME_RATE_FSCAM_CU135                       0x19
#define GET_FRAME_RATE_FSCAM_CU135                       0x18

#define SET_FACE_DETECT_FSCAM_CU135                      0x13
#define GET_FACE_DETECT_FSCAM_CU135                      0x12
#define ENABLE_FACE_RECT_FSCAM_CU135                     0x01
#define DISABLE_FACE_RECT_FSCAM_CU135                    0x00
#define ENABLE_EMBED_DATA_FSCAM_CU135                    0x01
#define DISABLE_EMBED_DATA_FSCAM_CU135                   0x00

#define SET_SMILE_DETECTION_FSCAM_CU135                  0x15
#define GET_SMILE_DETECTION_FSCAM_CU135                  0x14
#define ENABLE_SMILE_DETECT_FSCAM_CU135                  0x01
#define DISABLE_SMILE_DETECT_FSCAM_CU135                 0x00

#define GRAB_PREVIEW_FRAME                        0x1A
#define GRAB_STILL_FRAME                          0x1E
#define STORE_FRAME                               0x1B
#define QUERY_NEXT_FRAME                          0x01
#define STORE_PREV_FRAME                          0x01
#define STORE_STILL_FRAME                         0x02

#define SET_STILL_RESOLUTION                      0x1D
#define GET_STILL_RESOLUTION                      0x1C

#define GET_NUM_OF_FRAMES_STORE                   0x1F

#define ENABLE_FACE_RECTANGLE_FSCAM_CU135        0x01
#define DISABLE_FACE_RECTANGLE_FSCAM_CU135       0x00

#define SET_FACE_RECT_STATE                        0x20
#define RESET_TIME_STAMP                            0x21

#define SET_TO_DEFAULT_FSCAM_CU135               0x0F

#define SET_FAIL		0x00
#define SET_SUCCESS		0x01

#define GET_FAIL		0x00
#define GET_SUCCESS		0x01

#define SET_FLICKER_DETECTION_FSCAM_CU135           0x29
#define GET_FLICKER_DETECTION_FSCAM_CU135           0x28

#define ENABLE_LED_CONTROL_FSCAM_CU135        0x01
#define DISABLE_LED_CONTROL_FSCAM_CU135       0x00

#define ENABLE_POWERON_CONTROL_FSCAM_CU135        0x01
#define DISABLE_POWERON_CONTROL_FSCAM_CU135       0x00

#define ENABLE_STREAMING_CONTROL_FSCAM_CU135        0x01
#define DISABLE_STREAMING_CONTROL_FSCAM_CU135       0x00

#define ENABLE_TRIGGERACK_CONTROL_FSCAM_CU135        0x01
#define DISABLE_TRIGGERACK_CONTROL_FSCAM_CU135       0x00

class FSCAM_CU135 : public QObject
{
    Q_OBJECT

private:
    uvccamera uvc;

    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];

    void initializeBuffers();

public:
    explicit FSCAM_CU135(QObject *parent = 0);
    QString _title;
    QString _text;

    enum sceneModes {
        SCENE_NORMAL = 0x01,
        SCENE_DOCUMENT = 0x0C
    };
    Q_ENUMS(sceneModes)

    enum specialEffects {
        EFFECT_NORMAL = 0x01,
        EFFECT_BLACK_WHITE = 0x04,
        EFFECT_GREYSCALE = 0x07,
        EFFECT_NEGATIVE = 0x08,
        EFFECT_SKETCH = 0x10
    };

     Q_ENUMS(specialEffects)

    enum camiHDRMode {
        HdrOff = 0x01,
        HdrAuto = 0x02,
        HdrManual = 0x03
    };
    Q_ENUMS(camiHDRMode)

    enum camStreamMode {
        STREAM_MASTER_CONTINUOUS = 0x00,
        STREAM_MASTER_ONDEMAND = 0x01,
        STREAM_SOFTWARE_TRIGGER = 0x02,
        STREAM_HARDWARE_TRIGGER = 0x03
    };
    Q_ENUMS(camStreamMode)

    enum camROIAutoExpMode {
        AutoExpFace = 0x00,
        AutoExpFull = 0x01,
        AutoExpManual = 0x02,
        AutoExpDisabled = 0x03
    };
    Q_ENUMS(camROIAutoExpMode)

    enum camFaceRectMode {
        FaceRectEnable = 0x01,
        FaceRectDisable = 0x00
    };
    Q_ENUMS(camFaceRectMode)

    enum camFaceDetectEmbedDataMode {
        FaceDetectEmbedDataEnable = 0x01,
        FaceDetectEmbedDataDisable = 0x00
    };
    Q_ENUMS(camFaceDetectEmbedDataMode)

    enum camFaceDetectOverlayRect {
        FaceDetectOverlayRectEnable = 0x01,
        FaceDetectOverlayRectDisable = 0x00
    };
    Q_ENUMS(camFaceDetectOverlayRect)

    enum camSmileDetectMode {
        SmileDetectEnable = 0x01,
        SmileDetectDisable = 0x00
    };
    Q_ENUMS(camSmileDetectMode)

    enum camSmileDetectEmbedDataMode {
        SmileDetectEmbedDataEnable = 0x01,
        SmileDetectEmbedDataDisable = 0x00
    };
    Q_ENUMS(camSmileDetectEmbedDataMode)

    enum flipMirrorControls{
        SetBothFlipDisable,
        SetHorzFlip,
        SetVertiFlip,
        SetBothFlipEnable
    };
    Q_ENUMS(flipMirrorControls)

    enum camFlickerMode {
         MODE_AUTO = 0x00,
         MODE_50Hz =0x01,
         MODE_60Hz =0x02,
         MODE_DISABLE =0x03
       };
       Q_ENUMS(camFlickerMode)

    enum camLedControl {
      LedControlEnable = 0x01,
      LedControlDisable = 0x00
    };
    Q_ENUMS(camLedControl)

    enum camLedPowerControl {
      PowerOnControlEnable = 0x01,
      PowerOnControlDisable = 0x00
    };
    Q_ENUMS(camLedPowerControl)

    enum camLedStreamControl {
      StreamingControlEnable = 0x01,
      StreamingControlDisable = 0x00
    };
    Q_ENUMS(camLedStreamControl)

    enum camLedTriggerControl {
      TriggerACKControlEnable = 0x01,
      TriggerACKControlDisable = 0x00
    };
    Q_ENUMS(camLedTriggerControl)

signals:
     void sceneModeValue(uint sceneMode);
     void effectModeValue(uint effectMode);
     void denoiseValue(uint denoiseVal);
     void qFactorValue(uint qFactor);
     void hdrModeValue(uint hdrMode, uint hdrValue);
     void streamModeValue(uint streamMode);
     void flipMirrorModeChanged(uint flipMirrorMode);
     void burstLengthValue(uint burstLength);
     void roiAutoExpMode(uint roiMode, uint winSize);
     void exposureCompValue(uint exposureCompensation);
     void indicateCommandStatus(QString title, QString text);
     void frameRateCtrlValue(uint frameRateVal);
     void faceDetectModeValue(uint faceDetectMode, uint faceDetectEmbedDataValue, uint faceDetectOverlayRect);
     void smileDetectModeValue(uint smileDetectMode, uint smileDetectEmbedDataValue);
     void indicateSmileThresholdRangeFailure(QString title, QString text);
     void indicateExposureValueRangeFailure(QString title, QString text);
     void numberOfframesStoreCapacity(uint numberOfFrames);
     void stillsettingsReceived(uint stillformat, uint stillResolution);
     void grabStillFrameStatus(bool grabStatus);
     void storeStillFrameSucceess();
     void titleTextChanged(QString _title,QString _text);
     // Added by Sankari: Mar 21, 2019
     //To set number of frames to skip in preview - signal to qml
     void updatePreviewFrameToSkip(uint previewSkip);
     void ledControlStatus(uint ledstatus,uint powerctl,uint stream,uint trigger);
     void flickerDetectionMode(uint flickerMode);



public slots:

    // Added by Sankari: Mar 21, 2019
    void setPreviewSkipCount();

    bool setSceneMode(sceneModes sceneMode);
    bool getSceneMode();

    bool getEffectMode();
    bool setEffectMode(specialEffects specialEffect);

    bool setDenoiseValue(uint deNoiseVal);
    bool getDenoiseValue();

    bool setQFactor(uint qFactor);
    bool getQFactor();

    bool setiHDRMode(camiHDRMode iHDRMode, uint iHDRValue);
    bool getiHDRMode();

    bool setStreamMode(camStreamMode streamMode);
    bool getStreamMode();

    bool setOrientation(bool horzModeSel, bool vertiModeSel);
    bool getOrientation();

    bool setROIAutoExposure(camROIAutoExpMode see3camAutoexpROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize);
    bool getAutoExpROIModeAndWindowSize();

    bool setExposureCompensation(unsigned int exposureCompValue);
    bool getExposureCompensation();

    bool setLedControl(bool ledstatus,bool powerctl,bool stream,bool trigger);
    bool getLedControl();

    bool setFrameRateCtrlValue(uint frameRate);
    bool getFrameRateCtrlValue();

    bool setFaceDetectionRect(bool enableFaceDetectRect, bool embedData, bool overlayRect);
    bool getFaceDetectMode();

    bool setSmileDetection(bool enableSmileDetect, bool embedData);
    bool getSmileDetectMode();   

    bool setFlickerDetection(camFlickerMode flickerMode);
    bool getFlickerDetection();

    bool grabPreviewFrame();
    bool storePreviewFrame();
    bool storeStillFrame(uint stillformatId,uint stillresolutionId);

    bool getNumberOfFramesCanStore(uint stillformatId,uint stillresolutionId);
    bool grabStillFrame(uint frameIndex, uint stillformatId, uint stillresolutionId);

    bool setStillResolution(uint stillformatId, uint stillresolutionId);
    bool getStillResolution();


    bool setToDefault();
    bool initFSCAM_CU135(char *hidDeviceName);
    void deinitFSCAM_CU135();
    bool readFirmwareVersion(uint *pMajorVersion, uint *pMinorVersion1, uint *sdkVersion, uint *svnVersion);
    bool resetTimeStamp();

};

#endif // FSCAM_CU135_H
