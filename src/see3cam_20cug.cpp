#include "see3cam_20cug.h"
#include "uvccamera.h"

See3CAM_20CUG ::See3CAM_20CUG(QObject *parent): QObject(parent)
{

}

void See3CAM_20CUG::initializeBuffers(){

    //Initialize input and output buffers
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));
}

bool See3CAM_20CUG ::setStreamMode(streamModes streamMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] =CAMERA_CONTROL_SEE3CAM_20CUG; /* set camera control code */
    g_out_packet_buf[2] = SET_STREAM_MODE_20CUG ; /* set stream mode code */
    g_out_packet_buf[3] = streamMode; /* actual stream mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[6]==SET_FAILURE) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_20CUG &&
            g_in_packet_buf[1]==SET_STREAM_MODE_20CUG  &&
            g_in_packet_buf[6]==SET_SUCCESS) {

            return true;
        }
    }

    return false;
}

bool See3CAM_20CUG::getStreamMode()
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
   g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_20CUG;  /* set camera control code */
   g_out_packet_buf[2] = GET_STREAM_MODE_20CUG ; /* get stream mode code */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[6]==GET_FAILURE) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_20CUG &&
           g_in_packet_buf[1]==GET_STREAM_MODE_20CUG  &&
           g_in_packet_buf[6]==GET_SUCCESS) {
           streamMode = g_in_packet_buf[2];
           emit streamModeValue(streamMode);
           return true;
       }
   }
   return false;

}
bool See3CAM_20CUG ::setFlashMode(flashModes flashMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] =CAMERA_CONTROL_SEE3CAM_20CUG; /* set camera control code */
    g_out_packet_buf[2] = SET_FLASH_MODE_20CUG ; /* set flash mode code */
    g_out_packet_buf[3] = flashMode; /* actual flash mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAILURE) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_20CUG &&
            g_in_packet_buf[1]==SET_FLASH_MODE_20CUG  &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }

    return false;
}
bool See3CAM_20CUG::getFlashMode()
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
   g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_20CUG;  /* set camera control code */
   g_out_packet_buf[2] = GET_FLASH_MODE_20CUG ; /* get flash mode code */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[6]==GET_FAILURE) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_20CUG &&
           g_in_packet_buf[1]==GET_FLASH_MODE_20CUG  &&
           g_in_packet_buf[6]==GET_SUCCESS) {
           flashMode = g_in_packet_buf[2];
           emit flashModeValue(flashMode);
           return true;
       }
   }
   return false;
}

bool See3CAM_20CUG::setFlipCtrlValue(bool hFlip ,bool vFlip)
{
    u_int8_t flipMode;

    if(hFlip && vFlip){
        flipMode = FLIP_BOTH; /* both flip enable */
    }else if(hFlip && !vFlip){
        flipMode = FLIP_HORIZONTAL; /* horizontal flip only mode */
    }else if(!hFlip && vFlip){
        flipMode = FLIP_VERTICAL; /* vertical flip only mode */
    }else
        flipMode = FLIP_NORMAL; /* both flip disable */
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_20CUG; /* set camera control code */
    g_out_packet_buf[2] = SET_FLIP_CONTROLS_20CUG ; /* set flip control code */
    g_out_packet_buf[3] = flipMode; /* set flipControl value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_20CUG &&
            g_in_packet_buf[1]==SET_FLIP_CONTROLS_20CUG  &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_20CUG::getFlipCtrlValue - get flip control value from camera
 * return true - success /false - failure
 */
bool See3CAM_20CUG::getFlipCtrlValue()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_20CUG; /* set camera control code */
    g_out_packet_buf[2] = GET_FLIP_CONTROLS_20CUG ; /* get flip control code */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_20CUG &&
            g_in_packet_buf[1]==GET_FLIP_CONTROLS_20CUG  &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit flipCtrlValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

bool See3CAM_20CUG::setToDefault()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_20CUG; /* camera id */
    g_out_packet_buf[2] = SET_DEFAULT_20CUG ; /* set to default command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAILURE) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_20CUG &&
            g_in_packet_buf[1]==SET_DEFAULT_20CUG  &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            emit titleTextChanged(_title,_text);
            return true;
        }
    }
    return false;
}


