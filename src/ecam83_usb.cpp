#include "ecam83_usb.h"

ecam83_USB::ecam83_USB(QObject *parent) : QObject(parent)
{
    gvobj = NULL;
}

ecam83_USB::~ecam83_USB()
{

}

void ecam83_USB::initUVCExtensionUnit(Videostreaming *vobj)
{
    if(!vobj)
        return;
    gvobj = vobj;
}

void ecam83_USB::deInitUVCExtensionUnit()
{
    gvobj = NULL;
}

bool ecam83_USB::getControlValue(__u8 controlId, __u8 queryType, uint numberOfValues, __u8 *outputValues)
{
    if(!gvobj)
      return false;

    if(!outputValues)
        return false;

    struct uvc_xu_control_query xquery;
    __u16 size = 0;

    memset(&xquery, 0, sizeof(xquery));

    // To allocate a sufficiently large buffer and set the buffer size to the correct value
    xquery.query = UVC_GET_LEN;
    xquery.size = 2; /* size seems to always be 2 for the LEN query, but
                       doesn't seem to be documented. Use result for size
                                  in all future queries of the same control number */
    xquery.selector = controlId;
    xquery.unit = EXTENSION_UNIT_ID;

    xquery.data = (__u8 *)&size;

    if(!gvobj->setUvcExtControlValue(xquery))
    {
        perror("setUvcExtControlValue 1: ");
        return false;
    }

    memset(&xquery, 0, sizeof(xquery));

    // get value from camera
    xquery.query = queryType;
    xquery.size = size ;
    xquery.selector = controlId;
    xquery.unit = EXTENSION_UNIT_ID;
    xquery.data = outputValues;

    if(!gvobj->setUvcExtControlValue(xquery))
    {
        perror("setUvcExtControlValue 2");
        return false;
    }

    return true;
}

bool ecam83_USB::readFirmwareVersion()
{
    _title = "Firmware Version";
    __u8 outputValues[8];

    if(!getControlValue(READFIRMWAREVERSION, UVC_GET_CUR, 8, outputValues))
    {
        return false;
    }

    _text.clear();
    _text.append(QString::number(outputValues[0]).append(".").append(QString::number(outputValues[1])));
    _text.append(".");
    _text.append(QString::number((outputValues[3]<<8)|outputValues[2]));
    _text.append(".");
    _text.append(QString::number(outputValues[5],16).append(QString::number(outputValues[4],16)).append(QString::number(outputValues[7], 16)).append(QString::number(outputValues[6], 16)));

    emit titleTextChanged(_title, _text);

    return true;
}

bool ecam83_USB::resetDevice()
{
    unsigned char data[]={RESET_DEVICE,0x00,0x00,0x00};
    if(!getControlValue(RESET_DEVICE_CONTROL_REGISTER, UVC_SET_CUR, 4, data))
    {
        return false;
    }
    return true;
}
