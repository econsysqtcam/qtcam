/*
 * seecam_cu130.h -- Handling special feature of seecamcu130 camera
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
#ifndef SEE3CAM_CU130_H
#define SEE3CAM_CU130_H

#include <QObject>
#include "uvccamera.h"

#define EXPOSURECOMP_MIN 8000
#define EXPOSURECOMP_MAX 1000000

#define SMILE_THRESHOLD_MIN 40
#define SMILE_THRESHOLD_MAX 75
#define DEFAULT_SMILE_THRESHOLD 40

#define SET_FACE_DETECT_CU130                      0x12
#define GET_FACE_DETECT_CU130                      0x11
#define ENABLE_FACE_RECT_CU130                     0x01
#define DISABLE_FACE_RECT_CU130                    0x00
#define ENABLE_EMBED_DATA_CU130                    0x01
#define DISABLE_EMBED_DATA_CU130                  0x00
#define ENABLE_OVERLAY_RECT_CU130                  0x01
#define DISABLE_OVERLAY_RECT_CU130                 0x00

#define SET_SMILE_DETECTION_CU130                  0x14
#define GET_SMILE_DETECTION_CU130                  0x13
#define ENABLE_SMILE_DETECT_CU130                 0x01
#define DISABLE_SMILE_DETECT_CU130                 0x00

#define ENABLE_FACE_RECTANGLE_CU130               0x01
#define DISABLE_FACE_RECTANGLE_CU130              0x00
#define ENABLE_DISABLE_MODE_FACE_RECTANGLE_CU130       0x0D

#define CAMERA_CONTROL_CU130	0x69

#define GET_SCENE_MODE		0x01
#define SET_SCENE_MODE		0x02

#define GET_SPECIAL_EFFECT	0x03
#define SET_SPECIAL_EFFECT	0x04

#define SET_ORIENTATION_CU130              0x10
#define GET_ORIENTATION_CU130              0x0F
#define SET_ORIENTATION_HORZFLIP_CU130     0x01
#define SET_ORIENTATION_VERTFLIP_CU130     0x02
#define SET_ORIENTATION_BOTHFLIP_ENABLE_CU130      0x03
#define SET_ORIENTATION_BOTHFLIP_DISABLE_CU130     0x00

#define SET_EXPOSURE_COMPENSATION_CU130       0x16
#define GET_EXPOSURE_COMPENSATION_CU130       0x15

#define GET_DENOISE_CONTROL_CU130             0x05
#define SET_DENOISE_CONTROL_CU130             0x06

#define SET_BURST_LENGTH_CU130                0x0A
#define GET_BURST_LENGTH_CU130                0x09

#define SET_Q_FACTOR_CU130                    0x0C
#define GET_Q_FACTOR_CU130                    0x0B

#define SET_EXP_ROI_MODE_CU130                0x08
#define GET_EXP_ROI_MODE_CU130                0x07

#define SET_FRAME_RATE_CU130                  0x18
#define GET_FRAME_RATE_CU130                  0x17

#define SET_TO_DEFAULT_CU130                  0x0E

#define GET_FAIL		0x00
#define GET_SUCCESS		0x01


class See3CAM_CU130 : public QObject
{
    Q_OBJECT

private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
    uvccamera uvc;

    void initializeBuffers();

public:
    explicit See3CAM_CU130(QObject *parent = 0);

    enum sceneModes {
        SCENE_NORMAL = 0x01,
        SCENE_PORTRAIT,
        SCENE_LANDSCAPE,
        SCENE_SPORT,
        SCENE_CLOSE_UP,
        SCENE_NIGHT,
        SCENE_TWILIGHT,
        SCENE_BACKLIGHT,
        SCENE_HIGH_SENSITIVE,
        SCENE_NIGHT_PORTRAIT,
        SCENE_BEACH,
        SCENE_DOCUMENT,
        SCENE_PARTY,
        SCENE_FIREWORKS,
        SCENE_SUNSET,
        SCENE_AUTO_DETECT,
    };
    Q_ENUMS(sceneModes)

    enum specialEffects {
        EFFECT_NORMAL = 0x01,
        EFFECT_ALIEN,
        EFFECT_ANTIQUE,
        EFFECT_BLACK_WHITE,
        EFFECT_EMBOSS,
        EFFECT_EMBOSS_COLORED,
        EFFECT_GRAY,
        EFFECT_NEGATIVE,
        EFFECT_BLUE,
        EFFECT_GREEN,
        EFFECT_RED,
        EFFECT_POSTERIZE_1,
        EFFECT_POSTERIZE_2,
        EFFECT_SEPIA_1,
        EFFECT_SEPIA_2,
        EFFECT_SKETCH,
        EFFECT_SOLARIZE,
        EFFECT_FOGGY,
    };

     Q_ENUMS(specialEffects)

    enum flipMirrorControls{
        FLIP_OFF_MIRROR_OFF = 0x00,
        FLIP_ON_MIRROR_OFF  = 0x01,
        FLIP_OFF_MIRROR_ON  = 0x02,
        FLIP_ON_MIRROR_ON   = 0x03
    };
    Q_ENUMS(flipMirrorControls)

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

signals:
     void sceneModeValue(uint sceneMode);
     void effectModeValue(uint effectMode);
     void flipMirrorModeChanged(uint flipMirrorMode);
     void exposureCompValueReceived(uint exposureCompensation);
     void indicateCommandStatus(QString title, QString text);
     void indicateExposureValueRangeFailure(QString title, QString text);
     void denoiseValueReceived(uint denoiseValue);
     void burstLengthValue(uint burstLength);
     void qFactorValue(uint qFactor);
     void roiAutoExpModeValue(uint roiMode, uint winSize);
     void frameRateCtrlValueReceived(uint frameRateCtrlValue);     
     void faceDetectModeValue(uint faceDetectMode, uint faceDetectEmbedDataValue, uint faceDetectOverlayRect);
     void smileDetectModeValue(uint smileDetectMode, uint smileDetectThresholdValue, uint smileDetectEmbedDataValue);
     void indicateSmileThresholdRangeFailure(QString title, QString text);

public slots:
    bool setSceneMode(const sceneModes& sceneMode);
    bool getSceneMode();

    bool setEffectMode(const specialEffects& specialEffect);
    bool getEffectMode();

    bool setOrientation(bool horzModeSel, bool vertiModeSel);
    bool getOrientation();

    bool setExposureCompensation(unsigned int exposureCompValue);
    bool getExposureCompensation();

    bool setDenoiseValue(int deNoiseVal);
    bool getDenoiseValue();

    bool setBurstLength(uint burstLength);
    bool getBurstLength();

    bool setQFactor(uint qFactor);
    bool getQFactor();

    bool setROIAutoExposure(camROIAutoExpMode see3camAutoexpROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize);
    bool getAutoExpROIModeAndWindowSize();

    bool setToDefault();

    bool setFrameRateCtrlValue(uint frameRate);
    bool getFrameRateCtrlValue();

    bool setFaceDetectionRect(bool enableFaceDetectRect, bool embedData, bool overlayRect);
    bool getFaceDetectMode();

    bool setSmileDetection(bool enableSmileDetect, bool embedData, uint thresholdValue);
    bool getSmileDetectMode();

    bool enableDisableFaceRectangle(bool enableFaceRect);
};

#endif // SEE3CAM_CU130_H
