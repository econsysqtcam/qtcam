#ifndef SEE3CAM50CUG_H
#define SEE3CAM50CUG_H

#include <QObject>
#include <QString>
#include "uvccamera.h"

class SEE3CAM_50CUG : public QObject
{
    Q_OBJECT

private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
    uvccamera uvc;

    void initializeBuffers();

public:
    SEE3CAM_50CUG();
};

#endif // SEE3CAM50CUG_H
