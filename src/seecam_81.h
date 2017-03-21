#ifndef SEECAM_81_H
#define SEECAM_81_H
// Added By Sankari : 17 Dec 2016

#include <QObject>
#include <QtConcurrentRun>
#include "uvccamera.h"

#define CMD_TIMEOUT_81CAM 3000

class See3CAM_81: public QObject {
    Q_OBJECT

public:    
    uvccamera uvc;        

    See3CAM_81();
    ~See3CAM_81();

    enum flipMirrorControls{
        FLIP_OFF_MIRROR_OFF_81 = 0x00,
        FLIP_ON_MIRROR_OFF_81  = 0x01,
        FLIP_OFF_MIRROR_ON_81  = 0x02,
        FLIP_ON_MIRROR_ON_81   = 0x03
    };
    Q_ENUMS(flipMirrorControls)

    enum specialEffects {
        EFFECT_NORMAL = 0x01,
        EFFECT_GRAYSCALE = 0x02
    };
    Q_ENUMS(specialEffects)

    enum strobeValues{
        STROBE_OFF_81 = 0x00,
        STROBE_FLASH_VS_81 = 0x01,        
        STROBE_TORCH_81 = 0x03
    };
    Q_ENUMS(strobeValues)

    enum focusModes{
        CONTINUOUS_FOCUS_81 = 0x01,
        MANUAL_FOCUS_81 = 0x02,
        SINGLETRIGGER_FOCUS_81 = 0x03
    };
    Q_ENUMS(focusModes)

    enum focusModeStatus{
        CAM_FOCUS_FAILED = 0x00,
        CAM_FOCUS_SUCCESS = 0x01,
        CAM_FOCUS_BUSY = 0x02
    };
    Q_ENUMS(focusModeStatus)

    enum gpioLevel{
        GPIO_LOW_81 = 0x00,
        GPIO_HIGH_81 = 0x01
    };
    Q_ENUMS(gpioLevel)

    enum gpoUIIndex{
        GPIO_OUT8_INDEX = 0x00,
        GPIO_OUT9_INDEX = 0x01
    };
    Q_ENUMS(gpoUIIndex)

    enum gpiUIIndex{
        GPIO_IN3_INDEX = 0x00,
        GPIO_IN6_INDEX = 0x01
    };
    Q_ENUMS(gpiUIIndex)

    enum gpioControlsCam81{
        CAM81_IN3 = 0x01,
        CAM81_IN6 = 0x02,
        CAM81_OUT8 = 0x01,
        CAM81_OUT9 = 0x02,
        CAM81_OUT8_READVAL = 25,
        CAM81_OUT9_READVAL = 24,
        CAM81_IN3_READVAL = 21,
        CAM81_IN6_READVAL = 23,
    };
    Q_ENUMS(gpioControlsCam81)

    /**
     * @brief The camGpioValue enum are used to set status (High/Low) for the Gpio pin.
     */
    enum camGpioValue{
    High = 1,
    Low = 0
    };
    Q_ENUMS(camGpioValue)

    enum camFocusModesComboIndex{
        AUTO_MODE_INDEX = 0,
        MANUAL_MODE_INDEX,
        SINGLE_TRIGGER_MODE_INDEX
    };
    Q_ENUMS(camFocusModesComboIndex)

    // This is used to set RGB gain value
    enum camRGBcolor{
        Red = 0x12,
        Green = 0x10,
        Blue = 0x0E
    };
    Q_ENUMS(camRGBcolor)

    // This is used to get RGB gain value
    enum camRGBcolorGet{
        GetRed = 0x11,
        GetGreen = 0x0F,
        GetBlue = 0x0D
    };

    Q_ENUMS(camRGBcolorGet)

    enum camROIAfMode {
        AFCentered = 0x01,
        AFManual = 0x02,
        AFDisabled = 0x03
    };
    Q_ENUMS(camROIAfMode)

    // APIs running in background to avoid preview hang
    static bool setGpioLevelBackgrndFn(See3CAM_81 *see3cam81obj, gpioControlsCam81 gpioPin,camGpioValue gpioValue);
    static bool setFocusModeBackgrndFn(See3CAM_81 *see3cam81obj, uint focusMode);
    static bool getOutputGpioLevelBackgrndFn(See3CAM_81 *see3cam81obj, gpioControlsCam81 gpioPinNumber);
    static bool setWbValuesBackgrndFn(See3CAM_81 *see3cam81obj, camRGBcolor rgbColor, uint rgbValue);

signals:
    void effectModeValue(uint effectMode);
    void flipMirrorModeValue(uint flipMirrorMode);
    void strobeModeValue(uint strobeMode);
    void focusModeStatus(unsigned int focusModeStatus);
    void focusModeValue(unsigned int focusModeValue);
    void focusPostionValue(quint16 focusPositionValue);
    void gpioLevel(unsigned int level);
    void updateFrameToSkipfromCam81(uint stillSkip);
    void returnFromSetFocusMode(bool status);
    void whiteBalanceGainValue(uint rgbColor, uint whiteBalanceGain);
    void roiAfMode(uint roiMode, uint winSize);
    void roiAfModeSuccess();
    void roiAfModeFail();

public slots:
    bool setEffectMode(const See3CAM_81::specialEffects &specialEffect);
    bool getEffectMode();

    bool setFlipMirrorMode(bool flipMode, bool mirrorMode);
    bool getFlipMirrorMode();

    bool setStrobeMode(const See3CAM_81::strobeValues &strobeMode);
    bool getStrobeMode();

    bool getFocusMode();

    bool setFocusPosition(int focusPosition);
    bool getFocusPosition();
    bool getFocusStatus();

    bool setToDefault();    

    bool getInputGpioLevel(gpioControlsCam81 gpioPinNumber);
    void getOutputGpioLevel(gpioControlsCam81 gpioPinNumber);

    void setGpioLevel(gpioControlsCam81 gpioPin,camGpioValue gpioValue);    
    void setFocusMode(uint focusMode);

    void setStillSkipCount(QString stillResoln, QString videoResoln);
    void setStillSkipCountWhenFPSChange(bool fpsChange);

    void setWbValues(camRGBcolor rgbColor, uint rgbValue);
    bool getWbValues(camRGBcolor rgbColor);

    bool setROIAutoFoucs(camROIAfMode see3camAfROIMode, QString winSize);
    bool setROIAutoFoucsCoordinates(uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord);
    bool getAutoFocusROIModeAndWindowSize();
};

#endif // SEECAM_81_H
