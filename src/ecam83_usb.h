#ifndef ECAM83_USB_H
#define ECAM83_USB_H

#include "videostreaming.h"
#include <QObject>
#include <errno.h>

#define EXTENSION_UNIT_ID                   3

#define UVC_RC_UNDEFINED                    0x00
#define UVC_SET_CUR                         0x01
#define UVC_GET_CUR                         0x81
#define UVC_GET_LEN                         0x85
#define UVC_GET_INFO                        0x86

#define READFIRMWAREVERSION                 0x13
#define RESET_DEVICE_CONTROL_REGISTER       0x03
#define UVC_XU_HW_CONTROL_REGISTER          0x04
#define UVC_XU_HW_CONTROL_I2C_DEVICE        0x05
#define UVC_XU_HW_CONTROL_SENSOR            0x10

#define RESET_DEVICE                        0x01

class ecam83_USB : public QObject , public v4l2
{
    Q_OBJECT
public:
    Videostreaming *gvobj;
    QString _title;
    QString _text;

    explicit ecam83_USB(QObject *parent = NULL);
    ~ecam83_USB();

private:
    bool getControlValue(__u8 controlId, __u8 queryType, uint numberOfValues, __u8 *outputValues);

signals:
    void titleTextChanged(QString _title, QString _text);

public slots:

    void initUVCExtensionUnit(Videostreaming  *vobj);
    void deInitUVCExtensionUnit();

    bool readFirmwareVersion();
    bool resetDevice();
};

#endif // ECAM83_USB_H
