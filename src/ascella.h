/*
 * ascella.h -- Handling special feature of ascella camera
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
#ifndef ASCELLA_H
#define ASCELLA_H

#include "uvccamera.h"

/* Ascella camera */
#define ASCELLA_VID 0x04b4
#define ASCELLA_PID 0x00c3
#define ASCELLA_BUFLEN 64
#define ASCELLA_TIMEOUT 1000
#define ASCELLA_DEFAULT_MAXFRAMERATE 119

#define LEDSTATUS_MODE				0xF031
#define LED_BRIGHTNESS				0xF033
#define AUTOEXPOSURE				0xF022
#define AUTOFOCUSMODE				0xF041
#define SKIPFOCUSMODE				0xF040
#define COLORMODE					0xF051
#define CM_BLACKWHITE				0xF058
#define NOISE_REDUCTION				0xF059
#define SCENEMODE					0xF018
#define LIMITFRAMERATE				0xF02F

#define SCENEMODE_NORMAL			0x00
#define SCENEMODE_DOCUMENTSCANNER		0x20
#define FP_DISABLE					0x00
#define FP_APPLYMAX					0x03
#define CX3UVC_SET_QFACTOR 			0x14

class ASCELLA: public QObject {
    Q_OBJECT
    Q_ENUMS(camLedMode)
    Q_ENUMS(camAfMode)
    Q_ENUMS(camSceneMode)
    Q_ENUMS(camNoiseReduceMode)
    Q_ENUMS(camLimitMaxFRMode)
    Q_ENUMS(camColorMode)
    Q_ENUMS(camBinnResizeMode)
    Q_ENUMS(camQFactorMode)

private:
    unsigned char g_out_packet_buf[ASCELLA_BUFLEN];
    unsigned char g_in_packet_buf[ASCELLA_BUFLEN];
    uvccamera uvc;

public:
    QString _title;
    QString _text;

    enum camLedMode {
        LedOff = 1,
        LedAuto = 2,
        LedManual = 3
    };

    enum camAfMode {
        Continuous = 1,
        OneShot = 2
    };

    enum camSceneMode {
        SceneNormal = 1,
        SceneDocScan = 2
    };

    enum camNoiseReduceMode {
        NoiseReduceNormal = 1,
        NoiseReduceFix = 2
    };

    enum camLimitMaxFRMode{
        Disable = 1,
        ApplyMaxFrameRate = 2
    };

    enum camColorMode{
        ColorModeNormal = 1,
        ColorModeMono = 2,
        ColorModeNegative = 3,
        ColorModeBlackWhite = 4
    };

    enum camBinnResizeMode{
        Binned = 1,
        Resized = 2
    };

    enum camQFactorMode {
        QFactorAuto = 0x01,
        QFactorManual = 0x00
    };


signals:
    void logHandle(QtMsgType,QString);

    void titleTextChanged(QString _title,QString _text);

    void deviceStatus(QString title, QString message);

    /* Set Default values */
    void ledOffEnable(QString brightness);

    void autoExposureEnable(QString exposureValue);

    void afContinuousEnable();

    void noiseReductionAutoEnable();

    void normalSceneModeEnable();

    void limitMaxFRDisableMode(QString frameRateValue);

    void normalColorModeEnable();

    void bwColorModeAutoEnable(QString bwThresholdValue);

    void binnModeEnable();

    /* Set Current values */

    void setCurrentLedValue(camLedMode ledCurMode, QString ledCurBrightness);

    void setCurrentAfMode(QString afMode);

    void setCurAutoExposureEnable(QString exposureValue);

    void setCurrentColorMode(QString curColorMode);

    void setCurrentBwMode(QString curBWMode, QString CurBWthreshold);

    void setCurNoiseReductionMode(QString curNoiseValue, camNoiseReduceMode curNoiseMode);

    void setCurSceneMode(QString curSceneMode);

    void setCurFRMode(camLimitMaxFRMode curFRMode, QString curMaxFRLimit);

    void setCurrentAfAreaCustomMode(QString curPosition, QString afTextBoxNumber);


    /*  set current and default values */
    void setAfAreaCenterMode();

    void setCurrentAfAreaCenterMode();

    void setBinnResizeSelect(QString binResizeSelect);
    void setCurbinnResizeSelect(QString binResizeSelect);
    void setCurrentQfactorMode(QString curqFactorValue, camQFactorMode curqFactorMode);

    void setBinnResizeEnableDisable(QString mode);
    void setCurrbinnResizeEnableDisable(QString mode);
    void qfactorAutoEnable();

public slots:
    /**
     * @brief   Sets the led status mode.
     * @param ledMode  set the led mode of the camera (off/Auto/Manual)
     * @param brightnessVal - set the brightness of led in the camera if auto or manual selected
     */
    void setLEDStatusMode(camLedMode ledMode, QString brightnessVal);

    /**
     * @brief   Sets the auto focus mode.
     * @param afMode  set the auto focus mode of the camera (continous/one shot)
     */
    void setAutoFocusMode(camAfMode afMode);

    /**
     * @brief   Sets the offset required for auto exposure.
     * @param exposureVal  set the exposure value in auto exposure in the camera.
     */
    void setExposureCompensation(QString exposureVal);

    /**
     * @brief   Sets the scene mode in the camera.
     * @param sceneMode  set the scene mode in the camera(normal/Document scanner).
     */
    void setSceneMode(camSceneMode sceneMode);

    /**
      * @brief set the noise reduction mode
      * @param noiseReduceMode - set the noise reduction mode(Auto/Fix)
      * @param noiseReduceFixVal - set the amount of noise is to be reduced when Fix mode is selected
      */
    void setNoiseReduceMode(camNoiseReduceMode noiseReduceMode, QString noiseReduceFixVal);

    /**
      * @brief set the limit maximum frame rate reduction mode
      * @param LimitMaxFRMode - set the maximium frame rate limit mode(Disable/Apply Maximum Frame Rate)
      * @param maxFrameRateVal - set the max frame rate choosen when Apply Maximum Frame Rate mode is selected
      */
    void setLimitMaxFrameRateMode(camLimitMaxFRMode LimitMaxFRMode, QString maxFrameRateVal);

    /**
      * @brief set the color mode
      * @param colorMode - set the color mode(Normal/Mono/Negative/Black and White)
      * @param blackwhiteThreshold - set the black and white threshold value
      */
    void setColorMode(camColorMode colorMode, QString blackwhiteThreshold);

    /**
      * @brief set the auto focus area as center weighted
      */
    void setCenterWeightedAutoFocus();

    /**
      * @brief set the auto focus area as custom weighted
      * @param afHoriStart - Auto focus area horizontal start position
      * @param afHoriEnd - Auto focus area horizontal end position
      * @param afVertiStart - Auto focus area vertical start position
      * @param afVertiEnd - Auto focus area vertical end position
      */
    bool setCustomAreaAutoFocus(QString afHoriStart, QString afHoriEnd, QString afVertiStart, QString afVertiEnd);

    /**
     * @brief setBinnedResizedMode
     * @param mode - Set binned or resized mode
     */
    void setBinnedResizedMode(camBinnResizeMode mode);
 #if 0
    /**
     * @brief setRollValue
     * @param rollValue - change the orientation of image
     */
    void setRollValue(QString rollValue);
#endif

    /**
     * @brief setDefaultValues when Default button pressed in extension settings
     */
    void setDefaultValues();

    /**
     * @brief getDefaultValues
     */
    void getDefaultValues(u_int8_t *pDefaultValue);

    /**
     * @brief getCurrentValues
     * @param pCurrentValue - pointer to getting current value
     */
    void getCurrentValues(u_int8_t *pCurrentValue);

    /**
     * @brief setCurrentValues
     */
    void setCurrentValues(QString vidResolution);

    /**
     * @brief setLedValueWithExternalHwButton
     */
   // void setLedValueWithExternalHwButton();

    void setQFactor(camQFactorMode qFactorMode, QString qFactorValue);

    /**
     * @brief getFirmwareVersion
     */
    void getFirmwareVersion();

};

#endif // ASCELLA_H
