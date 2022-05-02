#ifndef BARCODE_CAMERA_CPP
#define BARCODE_CAMERA_CPP
#include "barcode_camera.h"
#include "uvccamera.h"

BarcodeCamera ::BarcodeCamera(QObject *parent): QObject(parent)
{

}

void BarcodeCamera::initializeBuffers(){

    //Initialize input and output buffers
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));
}

bool BarcodeCamera ::setUartSPI(uint mode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] =CAMERA_CONTROL_BARCODE; /* set camera control code */
    g_out_packet_buf[2] = SET_UART_BARCODE ; /* set stream mode code */
    g_out_packet_buf[3] = mode; /* actual stream mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[5]==SET_FAILURE) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
            g_in_packet_buf[1]==SET_UART_BARCODE  &&
            g_in_packet_buf[5]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool BarcodeCamera::getUartSPI()
{
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_BARCODE;  /* set camera control code */
   g_out_packet_buf[2] = GET_UART_BARCODE ; /* get stream mode code */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[5]==GET_FAILURE) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
           g_in_packet_buf[1]==GET_UART_BARCODE  &&
           g_in_packet_buf[5]==GET_SUCCESS) {
           emit uartSpiStatus(g_in_packet_buf[2]);
           return true;
       }
   }
   return false;
}

bool BarcodeCamera ::setRedLedStatus(uint redLedStatus)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] =CAMERA_CONTROL_BARCODE; /* set camera control code */
    g_out_packet_buf[2] = SET_RED_LED_STATUS_BARCODE ; /* set stream mode code */
    g_out_packet_buf[3] = redLedStatus; /* actual stream mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[5]==SET_FAILURE) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
            g_in_packet_buf[1]==SET_RED_LED_STATUS_BARCODE  &&
            g_in_packet_buf[5]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool BarcodeCamera::getRedLedStatus()
{
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_BARCODE;  /* set camera control code */
   g_out_packet_buf[2] = GET_RED_LED_STATUS_BARCODE ; /* get stream mode code */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[5]==GET_FAILURE) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
           g_in_packet_buf[1]==GET_RED_LED_STATUS_BARCODE  &&
           g_in_packet_buf[5]==GET_SUCCESS) {
           emit redLedStatus(g_in_packet_buf[2]);
           return true;
       }
   }
   return false;
}

bool BarcodeCamera ::setWhiteLedStatus(uint whiteLedStatus)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] =CAMERA_CONTROL_BARCODE; /* set camera control code */
    g_out_packet_buf[2] = SET_WHITE_LED_STATUS_BARCODE ; /* set stream mode code */
    g_out_packet_buf[3] = whiteLedStatus; /* actual stream mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[5]==SET_FAILURE) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
            g_in_packet_buf[1]==SET_WHITE_LED_STATUS_BARCODE  &&
            g_in_packet_buf[5]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool BarcodeCamera::getWhiteLedStatus()
{
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_BARCODE;  /* set camera control code */
   g_out_packet_buf[2] = GET_WHITE_LED_STATUS_BARCODE ; /* get stream mode code */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[5]==GET_FAILURE) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
           g_in_packet_buf[1]==GET_WHITE_LED_STATUS_BARCODE  &&
           g_in_packet_buf[5]==GET_SUCCESS) {
           emit whiteLedStatus(g_in_packet_buf[2]);
           return true;
       }
   }
   return false;
}

bool BarcodeCamera ::setQRcode(uint qrCode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] =CAMERA_CONTROL_BARCODE; /* set camera control code */
    g_out_packet_buf[2] = SET_QRCODE_BARCODE ; /* set stream mode code */
    g_out_packet_buf[3] = qrCode; /* actual stream mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[5]==SET_FAILURE) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
            g_in_packet_buf[1]==SET_QRCODE_BARCODE  &&
            g_in_packet_buf[5]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool BarcodeCamera::getQRcode()
{
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_BARCODE;  /* set camera control code */
   g_out_packet_buf[2] = GET_QRCODE_BARCODE ; /* get stream mode code */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[5]==GET_FAILURE) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
           g_in_packet_buf[1]==GET_QRCODE_BARCODE  &&
           g_in_packet_buf[5]==GET_SUCCESS) {
           emit qrCodeValue(g_in_packet_buf[2]);
           return true;
       }
   }
   return false;
}

bool BarcodeCamera ::setDataMatrix(uint dataMatrix)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] =CAMERA_CONTROL_BARCODE; /* set camera control code */
    g_out_packet_buf[2] = SET_DATAMATRIX_BARCODE ; /* set stream mode code */
    g_out_packet_buf[3] = dataMatrix; /* actual stream mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[5]==SET_FAILURE) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
            g_in_packet_buf[1]==SET_DATAMATRIX_BARCODE  &&
            g_in_packet_buf[5]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool BarcodeCamera::getDataMatrix()
{
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_BARCODE;  /* set camera control code */
   g_out_packet_buf[2] = GET_DATAMATRIX_BARCODE ; /* get stream mode code */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[5]==GET_FAILURE) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
           g_in_packet_buf[1]==GET_DATAMATRIX_BARCODE  &&
           g_in_packet_buf[5]==GET_SUCCESS) {
           emit dataMatrixValue(g_in_packet_buf[2]);
           return true;
       }
   }
   return false;
}

bool BarcodeCamera ::setCode128(uint code128)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] =CAMERA_CONTROL_BARCODE; /* set camera control code */
    g_out_packet_buf[2] = SET_CODE128_BARCODE ; /* set stream mode code */
    g_out_packet_buf[3] = code128; /* actual stream mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[5]==SET_FAILURE) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
            g_in_packet_buf[1]==SET_CODE128_BARCODE  &&
            g_in_packet_buf[5]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool BarcodeCamera::getCode128()
{
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_BARCODE;  /* set camera control code */
   g_out_packet_buf[2] = GET_CODE128_BARCODE ; /* get stream mode code */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[5]==GET_FAILURE) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
           g_in_packet_buf[1]==GET_CODE128_BARCODE  &&
           g_in_packet_buf[5]==GET_SUCCESS) {
           emit code128Value(g_in_packet_buf[2]);
           return true;
       }
   }
   return false;
}

bool BarcodeCamera ::setCode39(uint code39)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] =CAMERA_CONTROL_BARCODE; /* set camera control code */
    g_out_packet_buf[2] = SET_CODE39_BARCODE ; /* set stream mode code */
    g_out_packet_buf[3] = code39; /* actual stream mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[5]==SET_FAILURE) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
            g_in_packet_buf[1]==SET_CODE39_BARCODE  &&
            g_in_packet_buf[5]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool BarcodeCamera::getCode39()
{
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_BARCODE;  /* set camera control code */
   g_out_packet_buf[2] = GET_CODE39_BARCODE ; /* get stream mode code */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[5]==GET_FAILURE) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
           g_in_packet_buf[1]==GET_CODE39_BARCODE  &&
           g_in_packet_buf[5]==GET_SUCCESS) {
           emit code39Value(g_in_packet_buf[2]);
           return true;
       }
   }
   return false;
}

bool BarcodeCamera ::setCode93(uint code93)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] =CAMERA_CONTROL_BARCODE; /* set camera control code */
    g_out_packet_buf[2] = SET_CODE93_BARCODE ; /* set stream mode code */
    g_out_packet_buf[3] = code93; /* actual stream mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[5]==SET_FAILURE) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
            g_in_packet_buf[1]==SET_CODE93_BARCODE  &&
            g_in_packet_buf[5]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool BarcodeCamera::getCode93()
{
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_BARCODE;  /* set camera control code */
   g_out_packet_buf[2] = GET_CODE93_BARCODE ; /* get stream mode code */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[5]==GET_FAILURE) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
           g_in_packet_buf[1]==GET_CODE93_BARCODE  &&
           g_in_packet_buf[5]==GET_SUCCESS) {
           emit code93Value(g_in_packet_buf[2]);
           return true;
       }
   }
   return false;
}

bool BarcodeCamera ::setUPCA(uint upca)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] =CAMERA_CONTROL_BARCODE; /* set camera control code */
    g_out_packet_buf[2] = SET_UPCA_BARCODE ; /* set stream mode code */
    g_out_packet_buf[3] = upca; /* actual stream mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[5]==SET_FAILURE) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
            g_in_packet_buf[1]==SET_UPCA_BARCODE  &&
            g_in_packet_buf[5]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool BarcodeCamera::getUPCA()
{
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_BARCODE;  /* set camera control code */
   g_out_packet_buf[2] = GET_UPCA_BARCODE ; /* get stream mode code */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[5]==GET_FAILURE) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
           g_in_packet_buf[1]==GET_UPCA_BARCODE  &&
           g_in_packet_buf[5]==GET_SUCCESS) {
           emit upcaValue(g_in_packet_buf[2]);
           return true;
       }
   }
   return false;
}

bool BarcodeCamera ::setUPCE(uint upce)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] =CAMERA_CONTROL_BARCODE; /* set camera control code */
    g_out_packet_buf[2] = SET_UPCE_BARCODE ; /* set stream mode code */
    g_out_packet_buf[3] = upce; /* actual stream mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[5]==SET_FAILURE) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
            g_in_packet_buf[1]==SET_UPCE_BARCODE  &&
            g_in_packet_buf[5]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool BarcodeCamera::getUPCE()
{
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_BARCODE;  /* set camera control code */
   g_out_packet_buf[2] = GET_UPCE_BARCODE ; /* get stream mode code */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[5]==GET_FAILURE) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
           g_in_packet_buf[1]==GET_UPCE_BARCODE  &&
           g_in_packet_buf[5]==GET_SUCCESS) {
           emit upceValue(g_in_packet_buf[2]);
           return true;
       }
   }
   return false;
}

bool BarcodeCamera ::setEAN13(uint ean13)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] =CAMERA_CONTROL_BARCODE; /* set camera control code */
    g_out_packet_buf[2] = SET_UPCE_BARCODE ; /* set stream mode code */
    g_out_packet_buf[3] = ean13; /* actual stream mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[5]==SET_FAILURE) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
            g_in_packet_buf[1]==SET_EAN13_BARCODE  &&
            g_in_packet_buf[5]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool BarcodeCamera::getEAN13()
{
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_BARCODE;  /* set camera control code */
   g_out_packet_buf[2] = GET_EAN13_BARCODE ; /* get stream mode code */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[5]==GET_FAILURE) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
           g_in_packet_buf[1]==GET_EAN13_BARCODE  &&
           g_in_packet_buf[5]==GET_SUCCESS) {
           emit ean13Value(g_in_packet_buf[2]);
           return true;
       }
   }
   return false;
}

bool BarcodeCamera ::setEAN8(uint ean8)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] =CAMERA_CONTROL_BARCODE; /* set camera control code */
    g_out_packet_buf[2] = SET_EAN8_BARCODE ; /* set stream mode code */
    g_out_packet_buf[3] = ean8; /* actual stream mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[5]==SET_FAILURE) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
            g_in_packet_buf[1]==SET_EAN8_BARCODE  &&
            g_in_packet_buf[5]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool BarcodeCamera::getEAN8()
{
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_BARCODE;  /* set camera control code */
   g_out_packet_buf[2] = GET_EAN8_BARCODE ; /* get stream mode code */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[5]==GET_FAILURE) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
           g_in_packet_buf[1]==GET_EAN8_BARCODE  &&
           g_in_packet_buf[5]==GET_SUCCESS) {
           emit ean8Value(g_in_packet_buf[2]);
           return true;
       }
   }
   return false;
}

bool BarcodeCamera ::setRotate90(uint rotate90)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] =CAMERA_CONTROL_BARCODE; /* set camera control code */
    g_out_packet_buf[2] = SET_ROTATE90_BARCODE ; /* set stream mode code */
    g_out_packet_buf[3] = rotate90; /* actual stream mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[5]==SET_FAILURE) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
            g_in_packet_buf[1]==SET_ROTATE90_BARCODE  &&
            g_in_packet_buf[5]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool BarcodeCamera::getRotate90()
{
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_BARCODE;  /* set camera control code */
   g_out_packet_buf[2] = GET_ROTATE90_BARCODE ; /* get stream mode code */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[5]==GET_FAILURE) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
           g_in_packet_buf[1]==GET_ROTATE90_BARCODE  &&
           g_in_packet_buf[5]==GET_SUCCESS) {
           emit rotate90Value(g_in_packet_buf[2]);
           return true;
       }
   }
   return false;
}

bool BarcodeCamera ::setRotate180(uint rotate180)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] =CAMERA_CONTROL_BARCODE; /* set camera control code */
    g_out_packet_buf[2] = SET_ROTATE180_BARCODE ; /* set stream mode code */
    g_out_packet_buf[3] = rotate180; /* actual stream mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[5]==SET_FAILURE) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
            g_in_packet_buf[1]==SET_ROTATE180_BARCODE  &&
            g_in_packet_buf[5]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool BarcodeCamera::getRotate180()
{
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_BARCODE;  /* set camera control code */
   g_out_packet_buf[2] = GET_ROTATE180_BARCODE ; /* get stream mode code */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[5]==GET_FAILURE) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
           g_in_packet_buf[1]==GET_ROTATE180_BARCODE  &&
           g_in_packet_buf[5]==GET_SUCCESS) {
           emit rotate180Value(g_in_packet_buf[2]);
           return true;
       }
   }
   return false;
}

bool BarcodeCamera ::setRotate270(uint rotate270)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] =CAMERA_CONTROL_BARCODE; /* set camera control code */
    g_out_packet_buf[2] = SET_ROTATE270_BARCODE ; /* set stream mode code */
    g_out_packet_buf[3] = rotate270; /* actual stream mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[5]==SET_FAILURE) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
            g_in_packet_buf[1]==SET_ROTATE270_BARCODE  &&
            g_in_packet_buf[5]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool BarcodeCamera::getRotate270()
{
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_BARCODE;  /* set camera control code */
   g_out_packet_buf[2] = GET_ROTATE270_BARCODE ; /* get stream mode code */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[5]==GET_FAILURE) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
           g_in_packet_buf[1]==GET_ROTATE270_BARCODE  &&
           g_in_packet_buf[5]==GET_SUCCESS) {
           emit rotate270Value(g_in_packet_buf[2]);
           return true;
       }
   }
   return false;
}

bool BarcodeCamera ::setGain(uint gain)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] =CAMERA_CONTROL_BARCODE; /* set camera control code */
    g_out_packet_buf[2] = SET_GAIN_BARCODE ; /* set stream mode code */
    g_out_packet_buf[3] = gain; /* actual stream mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[5]==SET_FAILURE) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
            g_in_packet_buf[1]==SET_GAIN_BARCODE  &&
            g_in_packet_buf[5]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool BarcodeCamera::getGain()
{
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_BARCODE;  /* set camera control code */
   g_out_packet_buf[2] = GET_GAIN_BARCODE ; /* get stream mode code */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[5]==GET_FAILURE) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
           g_in_packet_buf[1]==GET_GAIN_BARCODE  &&
           g_in_packet_buf[5]==GET_SUCCESS) {
           emit gainValue(g_in_packet_buf[2]);
           return true;
       }
   }
   return false;
}

bool BarcodeCamera ::setExposure(uint exposure)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] =CAMERA_CONTROL_BARCODE; /* set camera control code */
    g_out_packet_buf[2] = SET_EXPOSURE_BARCODE ; /* set stream mode code */
    g_out_packet_buf[3] = exposure; /* actual stream mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[5]==SET_FAILURE) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
            g_in_packet_buf[1]==SET_EXPOSURE_BARCODE  &&
            g_in_packet_buf[5]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool BarcodeCamera::getExposure()
{
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_BARCODE;  /* set camera control code */
   g_out_packet_buf[2] = GET_EXPOSURE_BARCODE ; /* get stream mode code */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[5]==GET_FAILURE) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
           g_in_packet_buf[1]==GET_EXPOSURE_BARCODE  &&
           g_in_packet_buf[5]==GET_SUCCESS) {
           emit exposureType(g_in_packet_buf[2]);
           return true;
       }
   }
   return false;
}

bool BarcodeCamera ::setExposureValue(uint exposure)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] =CAMERA_CONTROL_BARCODE; /* set camera control code */
    g_out_packet_buf[2] = SET_EXPOSURE_VALUE_BARCODE ; /* set stream mode code */
    g_out_packet_buf[3] = (exposure>>8) & 0x00FF; /* actual stream mode */
    g_out_packet_buf[4] = exposure & 0x00FF;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[5]==SET_FAILURE) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
            g_in_packet_buf[1]==SET_EXPOSURE_VALUE_BARCODE  &&
            g_in_packet_buf[5]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool BarcodeCamera::getExposureValue()
{
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_BARCODE;  /* set camera control code */
   g_out_packet_buf[2] = GET_EXPOSURE_VALUE_BARCODE ; /* get stream mode code */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[5]==GET_FAILURE) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
           g_in_packet_buf[1]==GET_EXPOSURE_VALUE_BARCODE  &&
           g_in_packet_buf[5]==GET_SUCCESS) {
           unsigned short val = (g_in_packet_buf[2]<<8) | (unsigned short)g_in_packet_buf[3];
           emit exposureValue(val);
           return true;
       }
   }
   return false;
}

bool BarcodeCamera ::saveSettings()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] =CAMERA_CONTROL_BARCODE; /* set camera control code */
    g_out_packet_buf[2] = SAVE_SETTINGS_BARCODE ; /* set stream mode code */
    g_out_packet_buf[3] = 1; /* actual stream mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[5]==SET_FAILURE) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
            g_in_packet_buf[1]==SAVE_SETTINGS_BARCODE  &&
            g_in_packet_buf[5]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool BarcodeCamera::setToDefault()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_BARCODE; /* camera id */
    g_out_packet_buf[2] = SET_DEFAULT_BARCODE ; /* set to default command */
    g_out_packet_buf[3] = 1;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[5]==SET_FAILURE) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_BARCODE &&
            g_in_packet_buf[1]==SET_DEFAULT_BARCODE  &&
            g_in_packet_buf[5]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool BarcodeCamera::readFirmwareVersion()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();
    uint32_t sdk_ver=0, svn_ver=0;

    // fill buffer values
    g_out_packet_buf[1] = READFIRMWAREVERSION; /* camera id */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        sdk_ver = (g_in_packet_buf[3]<<8)+g_in_packet_buf[2];
        svn_ver = (g_in_packet_buf[4]<<24)+(g_in_packet_buf[5]<<16)+(g_in_packet_buf[6]<<8)+g_in_packet_buf[7];
        _title = "Firmware Version";
        _text.clear();
        _text.append(QString::number(g_in_packet_buf[0]).append("."));  //Firmware version_BYTE_1
        _text.append(QString::number(g_in_packet_buf[1]).append("."));  //Firmware version_BYTE_2
        _text.append(QString::number(sdk_ver).append("."));  //sdk version
        _text.append(QString::number(svn_ver,16));              //svn version

        emit titleTextChanged(_title, _text);
        return true;
    }
    return false;
}

#endif // BARCODE_CAMERA_CPP
