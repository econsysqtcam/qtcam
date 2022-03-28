#include "see3cam_cu136m.h"

See3CAM_CU136M::See3CAM_CU136M()
{
}

See3CAM_CU136M::~See3CAM_CU136M()
{
}


void See3CAM_CU136M::initializeBuffers()
{
    //Initialize input and output buffers
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));
}

/**
 * @brief See3CAM_CU136M::setStreamMode - Setting  Streaming mode
 * @param streamMode - mode selected in UI
 * @return true/false
 */
bool See3CAM_CU136M::setStreamMode(streamModes streamMode){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU136M; /* camera id */
    g_out_packet_buf[2] = SET_STREAM_MODE_CU136M; /* set stream mode command */
    g_out_packet_buf[3] = streamMode; /* stream mode to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU136M &&
            g_in_packet_buf[1]==SET_STREAM_MODE_CU136M &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_CU136M::getStreamMode - get Stream mode from camera
 * return true - success /false - failure
 */
bool See3CAM_CU136M::getStreamMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU136M; /* camera id */
    g_out_packet_buf[2] = GET_STREAM_MODE_CU136M; /* get stream mode command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU136M &&
            g_in_packet_buf[1]==GET_STREAM_MODE_CU136M &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit streamModeValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

bool See3CAM_CU136M ::setFlashState(flashStateValues flashMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] =CAMERA_CONTROL_CU136M; /* set camera control code */
    g_out_packet_buf[2] = SET_FLASH_MODE_CU136M ; /* set flash mode code */
    g_out_packet_buf[3] = flashMode; /* actual flash mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAILURE) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU136M &&
            g_in_packet_buf[1]==SET_FLASH_MODE_CU136M  &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }

    return false;
}


bool See3CAM_CU136M::getFlashState()
{
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_CU136M;  /* set camera control code */
   g_out_packet_buf[2] = GET_FLASH_MODE_CU136M ; /* get flash mode code */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[6]==GET_FAILURE) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU136M &&
           g_in_packet_buf[1]==GET_FLASH_MODE_CU136M  &&
           g_in_packet_buf[6]==GET_SUCCESS) {
           emit flashModeValue(g_in_packet_buf[2]);
           return true;
       }
   }
   return false;
}


bool See3CAM_CU136M::setFlipCtrlValue(bool hFlip ,bool vFlip)
{
    u_int8_t flipMode;

    if(hFlip && vFlip){
        flipMode = FLIP_BOTH; /* both flip enable */
    }else if(hFlip && !vFlip){
        flipMode = FLIP_HORIZONTAL; /* horizontal flip only mode */
    }else if(!hFlip && vFlip){
        flipMode = FLIP_VERTICAL; /* vertical flip only mode */
    }else{
        flipMode = FLIP_NORMAL;
    }
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU136M; /* set camera control code */
    g_out_packet_buf[2] = SET_ORIENTATION_CU136M ; /* set flip control code */
    g_out_packet_buf[3] = flipMode; /* set flipControl value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU136M &&
            g_in_packet_buf[1]==SET_ORIENTATION_CU136M  &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_CU136M::getFlipCtrlValue - get flip control value from camera
 * return true - success /false - failure
 */
bool See3CAM_CU136M::getFlipCtrlValue()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU136M; /* set camera control code */
    g_out_packet_buf[2] = GET_ORIENTATION_CU136M ; /* get flip control code */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU136M &&
            g_in_packet_buf[1]==GET_ORIENTATION_CU136M  &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit flipCtrlValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU136M::setToDefault - set all the values to default in camera
 * @return true/false
 */
bool See3CAM_CU136M::setToDefault(){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU136M; /* camera id */
    g_out_packet_buf[2] = SET_TO_DEFAULT_CU136M; /* set to default command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU136M &&
            g_in_packet_buf[1]==SET_TO_DEFAULT_CU136M &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}
