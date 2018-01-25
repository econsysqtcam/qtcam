/*
 * seecam_cu135.h -- Handling special feature of seecamcu135 camera
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
#ifndef SEE3CAM_CU135_H
#define SEE3CAM_CU135_H

#include <QObject>
#include "uvccamera.h"
#include <QtConcurrentRun>

#define EXPOSURECOMP_MIN 8000
#define EXPOSURECOMP_MAX 1000000
#define SMILE_THRESHOLD_MIN 40
#define SMILE_THRESHOLD_MAX 75
#define DEFAULT_SMILE_THRESHOLD 40

/* SEE3CAM_CU135 */
#define CAMERA_CONTROL_CU135             0x81

#define SET_SPECIAL_EFFECT_MODE_CU135    0x04
#define GET_SPECIAL_EFFECT_MODE_CU135    0x03

#define SET_SCENE_MODE_CU135             0x02
#define GET_SCENE_MODE_CU135             0x01

#define SET_DENOISE_CONTROL_CU135        0x06
#define GET_DENOISE_CONTROL_CU135        0x05

#define GET_Q_FACTOR_CU135               0x0D
#define SET_Q_FACTOR_CU135				 0x0E

#define SET_HDR_MODE_CU135				 0x0C
#define GET_HDR_MODE_CU135				 0x0B

#define SET_STREAM_MODE_CU135            0x11
#define GET_STREAM_MODE_CU135            0x0F

#define SET_BURST_LENGTH_CU135           0x0A
#define GET_BURST_LENGTH_CU135           0x09

#define SET_ORIENTATION_CU135            0x14
#define GET_ORIENTATION_CU135            0x13

#define SET_EXPOSURE_COMPENSATION_CU135  0x1A
#define GET_EXPOSURE_COMPENSATION_CU135  0x19

#define SET_EXP_ROI_MODE_CU135                     0x08
#define GET_EXP_ROI_MODE_CU135                     0x07

#define SET_FRAME_RATE_CU135                       0x1C
#define GET_FRAME_RATE_CU135                       0x1B

#define SET_FACE_DETECT_CU135                      0x16
#define GET_FACE_DETECT_CU135                      0x15
#define ENABLE_FACE_RECT_CU135                     0x01
#define DISABLE_FACE_RECT_CU135                    0x00
#define ENABLE_EMBED_DATA_CU135                    0x01
#define DISABLE_EMBED_DATA_CU135                   0x00
#define ENABLE_OVERLAY_RECT_CU135                  0x01
#define DISABLE_OVERLAY_RECT_CU135                 0x00

#define SET_SMILE_DETECTION_CU135                  0x18
#define GET_SMILE_DETECTION_CU135                  0x17
#define ENABLE_SMILE_DETECT_CU135                  0x01
#define DISABLE_SMILE_DETECT_CU135                 0x00

#define ENABLE_FACE_RECTANGLE_CU135               0x01
#define DISABLE_FACE_RECTANGLE_CU135              0x00
#define ENABLE_DISABLE_MODE_FACE_RECTANGLE_CU135       0x10

#define SET_TO_DEFAULT_CU135                       0x12


class See3CAM_CU135 : public QObject
{
    Q_OBJECT

private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
    uvccamera uvc;

    void initializeBuffers();

public:
    explicit See3CAM_CU135(QObject *parent = 0);

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
        STREAM_MASTER = 0x00,
        STREAM_TRIGGER = 0x01
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
        smileDetectEmbedDataEnable = 0x01,
        smileDetectEmbedDataDisable = 0x00
    };
    Q_ENUMS(camSmileDetectEmbedDataMode)

    enum flipMirrorControls{
        SetBothFlipDisable,
        SetHorzFlip,
        SetVertiFlip,
        SetBothFlipEnable
    };
    Q_ENUMS(flipMirrorControls)



    // To run function in background
    static bool setOrientationBackgrndFn(See3CAM_CU135 *see3cam30obj, bool horzModeSel, bool vertiModeSel);

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
     void smileDetectModeValue(uint smileDetectMode, uint smileDetectThresholdValue, uint smileDetectEmbedDataValue);
     void indicateSmileThresholdRangeFailure(QString title, QString text);
     void indicateExposureValueRangeFailure(QString title, QString text);

public slots:
    bool setSceneMode(const sceneModes& sceneMode);
    bool getSceneMode();

    bool getEffectMode();
    bool setEffectMode(const specialEffects& specialEffect);

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

    bool setBurstLength(uint burstLength);
    bool getBurstLength();

    bool setROIAutoExposure(camROIAutoExpMode see3camAutoexpROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize);
    bool getAutoExpROIModeAndWindowSize();

    bool setExposureCompensation(unsigned int exposureCompValue);
    bool getExposureCompensation();

    bool setFrameRateCtrlValue(uint frameRate);
    bool getFrameRateCtrlValue();

    bool setFaceDetectionRect(bool enableFaceDetectRect, bool embedData, bool overlayRect);
    bool getFaceDetectMode();

    bool setSmileDetection(bool enableSmileDetect, bool embedData, uint thresholdValue);
    bool getSmileDetectMode();

    bool enableDisableFaceRectangle(bool enableRFRect);

    bool setToDefault();
};

#endif // SEE3CAM_CU135_H
