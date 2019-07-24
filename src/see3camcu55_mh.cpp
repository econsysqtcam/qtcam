#include "see3camcu55_mh.h"
#include "uvccamera.h"

See3CAM_CU55_MH ::See3CAM_CU55_MH(QObject *parent): QObject(parent)
{

}

void See3CAM_CU55_MH::initializeBuffers(){

    //Initialize input and output buffers
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));
}

bool See3CAM_CU55_MH ::setStreamMode(streamModes streamMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] =CAMERA_CONTROL_SEE3CAM_CU55_MH; /* set camera control code */
    g_out_packet_buf[2] = SET_STREAM_MODE; /* set stream mode code */
    g_out_packet_buf[3] = streamMode; /* actual stream mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[6]==SET_FAILURE) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_CU55_MH &&
            g_in_packet_buf[1]==SET_STREAM_MODE &&
            g_in_packet_buf[6]==SET_SUCCESS) {

            return true;
        }
    }

    return false;
}

bool See3CAM_CU55_MH::getStreamMode()
{
   uint streamMode;

   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_CU55_MH;  /* set camera control code */
   g_out_packet_buf[2] = GET_STREAM_MODE; /* get stream mode code */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[6]==GET_FAILURE) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_CU55_MH &&
           g_in_packet_buf[1]==GET_STREAM_MODE &&
           g_in_packet_buf[6]==GET_SUCCESS) {
           streamMode = g_in_packet_buf[2];

           emit streamModeValue(streamMode);
           return true;
       }
   }
   return false;

}
bool See3CAM_CU55_MH ::setFlashMode(flashModes flashMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] =CAMERA_CONTROL_SEE3CAM_CU55_MH; /* set camera control code */
    g_out_packet_buf[2] = SET_FLASH_MODE; /* set flash mode code */
    g_out_packet_buf[3] = flashMode; /* actual flash mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAILURE) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_CU55_MH &&
            g_in_packet_buf[1]==SET_FLASH_MODE &&
            g_in_packet_buf[6]==SET_SUCCESS) {

            return true;
        }
    }

    return false;
}
bool See3CAM_CU55_MH::getFlashMode()
{
   uint flashMode;

   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_CU55_MH;  /* set camera control code */
   g_out_packet_buf[2] = GET_FLASH_MODE; /* get flash mode code */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[6]==GET_FAILURE) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_CU55_MH &&
           g_in_packet_buf[1]==GET_FLASH_MODE &&
           g_in_packet_buf[6]==GET_SUCCESS) {
           flashMode = g_in_packet_buf[2];
           emit flashModeValue(flashMode);
           return true;
       }
   }
   return false;
}

bool See3CAM_CU55_MH::setRollCtrlValue(uint rollValue)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_CU55_MH; /* set camera control code */
    g_out_packet_buf[2] = SET_ROLL_CONTROL; /* set roll control code */
    g_out_packet_buf[3] = rollValue; /* set rollControl value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_CU55_MH &&
            g_in_packet_buf[1]==SET_ROLL_CONTROL &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_CU55_MH::getRollCtrlValue - get roll control value from camera
 * return true - success /false - failure
 */
bool See3CAM_CU55_MH::getRollCtrlValue()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_CU55_MH; /* set camera control code */
    g_out_packet_buf[2] = GET_ROLL_CONTROL; /* get roll control code */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_CU55_MH &&
            g_in_packet_buf[1]==GET_ROLL_CONTROL &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit rollCtrlValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

bool See3CAM_CU55_MH::setToDefault()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_CU55_MH; /* camera id */
    g_out_packet_buf[2] = SET_DEFAULT; /* set to default command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAILURE) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_CU55_MH &&
            g_in_packet_buf[1]==SET_DEFAULT &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            emit titleTextChanged(_title,_text);
            return true;
        }
    }
    return false;
}

