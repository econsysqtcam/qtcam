#include "ecam51_usb.h"

uint ecam51_USB::AFStatus;

ecam51_USB::ecam51_USB(QObject *parent) : QObject(parent)
{
    gvobj = NULL;
}

ecam51_USB::~ecam51_USB()
{
        threadMonitor.waitForFinished();
}

bool ecam51_USB::getControlValue(__u8 controlId, __u8 queryType, uint numberOfValues, __u8 *outputValues)
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

/*  Name	:	auto_focus_control
 *  Parameter1	:	int
 *  Returns	:	  int
 *  Description	: Control function for autofocus mode control
 */
int ecam51_USB::auto_focus_control(int focus_option)
{
    unsigned char focus_status, reg_value;
    //unsigned int sensor_addr = 0;
    int timeout = 10;

    switch(focus_option)
    {
    case CNT_AUTO_FOCUS_FREEZE_SELECT:
        break;
    case CNT_AUTO_FOCUS_UNFREEZE_SELECT:
        break;
    case MANUAL_FOCUS_SELECT:
         {
            if(write_sensor_reg(SENSOR_CMD_ACK_REG_ADDR_5M, DATA_FOR_SETTING_CMD_ACK_5M))
            {
                if(write_sensor_reg(SENSOR_CMD_MAIN_REG_ADDR_5M, DATA_FOR_RELEASING_FOCUS_MODE))
                {
                        read_sensor_reg(SENSOR_CMD_ACK_REG_ADDR_5M, &reg_value);
                        while((reg_value != 0) & timeout-- )
                        {
                            usleep(10000);
                            read_sensor_reg(SENSOR_CMD_ACK_REG_ADDR_5M, &reg_value);
                        }
                        if(reg_value == 0)
                             return SUCCESS;
                        else
                             return FAILURE;
                }
                else
                {
                    sprintf(g_buff,"Error in writing the sensor register:%x\n",SENSOR_CMD_ACK_REG_ADDR_5M);
                    perror(g_buff);
                    return FAILURE;
                }
            }
         }
        break;
    case AUTO_FOCUS_SELECT:
         {
                if(write_sensor_reg(SENSOR_CMD_ACK_REG_ADDR_5M, DATA_FOR_SETTING_CMD_ACK_5M))
                {
                    if(write_sensor_reg(SENSOR_CMD_MAIN_REG_ADDR_5M, DATA_FOR_SETTING_CNT_AUTO_FOCUS_MODE))
                    {
                        read_sensor_reg(SENSOR_CMD_ACK_REG_ADDR_5M, &reg_value);
                        while((reg_value != 0) & timeout-- )
                        {
                            usleep(10000);
                            read_sensor_reg(SENSOR_CMD_ACK_REG_ADDR_5M, &reg_value);
                        }
                        if(reg_value == 0)
                            return SUCCESS;
                        else
                            return FAILURE;
                    }
                }
                else
                {
                     sprintf(g_buff,"Error in writing the sensor register:%x\n",SENSOR_CMD_MAIN_REG_ADDR_5M);
                     perror(g_buff);
                     return FAILURE;
                }

         }
        break;
    case SINGLE_TRIGGER_AUTO_FOCUS_SELECT:
         {
                 if(write_sensor_reg(SENSOR_CMD_ACK_REG_ADDR_5M, DATA_FOR_SETTING_CMD_ACK_5M))
                 {
                     if(write_sensor_reg(SENSOR_CMD_MAIN_REG_ADDR_5M, DATA_FOR_SETTING_SINGLE_TRIGGER_MODE))
                     {
                         read_sensor_reg(SENSOR_CMD_ACK_REG_ADDR_5M, &reg_value);
                         while((reg_value != 0) & timeout-- )
                         {
                             usleep(10000);
                             read_sensor_reg(SENSOR_CMD_ACK_REG_ADDR_5M, &reg_value);
                         }
                         return SUCCESS;
                     }
                     return FAILURE;
                 }
                 else
                     return FAILURE;
         }
        break;
    case GET_FOCUS_STATUS_:
         {
                if(read_sensor_reg(SENSOR_FW_STATUS_REG_ADDR_5M, &focus_status))
                      return (int)focus_status;
                return FAILURE;
         }
        break;
    default:
            {
                return FAILURE;
            }
            break;
    }//END of switch
    return true;
}

unsigned char ecam51_USB::reg_read_eeprom(__u16 reg_address)
{
    unsigned char data[]={0x00, 0x00, 0x00, 0x00};

    data[0] = EEPROM_DEVICE_ADDRESS_FOR_READ_OPERATION;
    data[1] = (reg_address & 0xFF);	// LSB of 16 bit EEPROM Register Address
    data[2] = (reg_address >> 8);	// MSB of 16 bit EEPROM Register Address
    data[3] = 0x00;

    if( !getControlValue(UVC_XU_HW_CONTROL_I2C_DEVICE, UVC_SET_CUR, 4, data))
    {
        perror("UVC_XU_HW_CONTROL_I2C_DEVICE: SET_CUR error");
        return false;
    }
    data[0] = EEPROM_DEVICE_ADDRESS_FOR_READ_OPERATION;
    data[1] = (reg_address & 0xFF);	// LSB of 16 bit EEPROM Register Address
    data[2] = (reg_address >> 8);	// MSB of 16 bit EEPROM Register Address
    data[3] = 0x00;

    if(!getControlValue(UVC_XU_HW_CONTROL_I2C_DEVICE, UVC_GET_CUR, 4, data))
    {
        perror("UVC_XU_HW_CONTROL_I2C_DEVICE: GET_CUR error");
        return false;
    }
    return (unsigned char)data[3];

}

int ecam51_USB::read_sensor_reg(unsigned int reg_address, unsigned char *reg_data)
{
    unsigned char data[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

    // Control Type
    data[0] = SENSOR_DEVICE_ADDRESS_FOR_READ_OPERATION;
    // Sensor Sub Address Byte Number
    data[1] = 0x02;	// Sensor Sub Address (or Register Address within the Sensor) is 16bit
    // Sensor value by number
    data[2] = 0x01;	// Sensor Data is 8bit
    // Sensor Address
    data[3] = (reg_address & 0xFF); // LSB of 16 bit Sensor Sub Address
    data[4] = (reg_address >> 8); // MSB of 16 bit Sensor Sub Address
    data[5] = 0x00; // Always 0x00 for 16bit Sensor Sub Address
    // Sensor Data
    data[6] = 0x00;
    data[7] = 0x0; // Always 0x00 for 8bit Sensor Data
    data[8] = 0x0; // Always 0x00 for 8bit Sensor Data

    if(!getControlValue(UVC_XU_HW_CONTROL_SENSOR, UVC_SET_CUR, 4, data))
    {
        return false;
    }
    // Control Type
    data[0] = SENSOR_DEVICE_ADDRESS_FOR_READ_OPERATION;
    // Sensor Sub Address Byte Number
    data[1] = 0x02;	// Sensor Sub Address (or Register Address within the Sensor) is 16bit
    // Sensor value by number
    data[2] = 0x01;	// Sensor Data is 8bit
    // Sensor Address
    data[3] = (reg_address & 0xFF); // LSB of 16 bit Sensor Sub Address
    data[4] = (reg_address >> 8); // MSB of 16 bit Sensor Sub Address
    data[5] = 0x00; // Always 0x00 for 16bit Sensor Sub Address
    // Sensor Data
    data[6] = 0x00;
    data[7] = 0x0; // Always 0x00 for 8bit Sensor Data
    data[8] = 0x0; // Always 0x00 for 8bit Sensor Data

    if(!getControlValue(UVC_XU_HW_CONTROL_SENSOR, UVC_GET_CUR, 4, data))
    {
        return false;
    }

    *reg_data = data[6];
    return true;
}

bool ecam51_USB::write_sensor_reg(unsigned int reg_address, unsigned char reg_data)
{
    unsigned char data[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

    // Control Type
    data[0] = SENSOR_DEVICE_ADDRESS_FOR_WRITE_OPERATION;
    // Sensor Sub Address Byte Number
    data[1] = 0x02;	// Sensor Sub Address (or Register Address within the Sensor) is 16bit
    // Sensor value by number
    data[2] = 0x01;	// Sensor Data is 8bit
    // Sensor Address
    data[3] = (reg_address & 0xFF); // LSB of 16 bit Sensor Sub Address
    data[4] = (reg_address >> 8); // MSB of 16 bit Sensor Sub Address
    data[5] = 0x00; // Always 0x00 for 16bit Sensor Sub Address
    // Sensor Data
    data[6] = reg_data; // Data for flip bit
    data[7] = 0x0; // Always 0x00 for 8bit Sensor Data
    data[8] = 0x0; // Always 0x00 for 8bit Sensor Data

    if(!getControlValue(UVC_XU_HW_CONTROL_SENSOR, UVC_SET_CUR, 4, data))
    {
        return false;
    }
    return true;
}

bool ecam51_USB::write_gpo_register(unsigned int reg_addr, unsigned char reg_data)
{
    unsigned char data[]={0x00,0x00,0x00,0x00};
    data[0] = REG_CONTROL_WRITE_REGISTER;
    data[1] = (reg_addr & 0xFF);
    data[2] = (reg_addr >> 8),
    data[3] = reg_data;

    if(!getControlValue(UVC_XU_HW_CONTROL_REGISTER, UVC_SET_CUR, 4, data))
        return false;
    return true;
}

int ecam51_USB::read_key_status_register()
{
    int value=0;
    unsigned char data[]={0x00,(EXT_BUTTONS_REG_ADDR & 0xFF), (EXT_BUTTONS_REG_ADDR>>8), 0x00};
    /* Need to send a dummy write */
    data[0]=0x00;
    data[3]=0x00;

    if(!getControlValue(UVC_XU_HW_CONTROL_REGISTER, UVC_SET_CUR, 4, data))
    {
        return false;
    }

    data[0]=0x00;
    data[3]=0x00;

    if(!getControlValue(UVC_XU_HW_CONTROL_REGISTER, UVC_GET_CUR, 4, data))
    {
        return false;
    }
    value = (int)data[3];

    return value;
}

int ecam51_USB::clear_key_status_register(unsigned char value)
{
   unsigned char data[]={0x00,(EXT_BUTTONS_REG_ADDR & 0xFF), (EXT_BUTTONS_REG_ADDR>>8), 0x00};

   data[0] |= REG_CONTROL_WRITE_REGISTER;
   data[3] = value;

   if(!getControlValue(UVC_XU_HW_CONTROL_REGISTER, UVC_SET_CUR, 4, data))
   {
       return false;
   }

   /* Need to send a dummy write */
   data[0]=0x00;
   data[3]=0x00;

   if(!getControlValue(UVC_XU_HW_CONTROL_REGISTER, UVC_SET_CUR, 4, data))
   {
       return false;
   }

   data[0]=0x00;
   data[3]=0x00;

   if(!getControlValue(UVC_XU_HW_CONTROL_REGISTER, UVC_GET_CUR, 4, data))
   {
       return false;
   }
   return true;
}

int ecam51_USB::diagnostic_control(int diagnostic_option)
{
    unsigned char sensor_id1_value, sensor_id2_value;
    switch(diagnostic_option)
    {
        case DIAGNOSTIC_CNT_READ_SENSOR_ID1:
             {
                if((read_sensor_reg(SENSOR_ID1_REG_ADDR, &sensor_id1_value)) != FAILURE)
                {
                    return (int)sensor_id1_value;
                }
                 perror(g_buff);
            }
            break;

        case DIAGNOSTIC_CNT_READ_SENSOR_ID2:
            {
                if((read_sensor_reg(SENSOR_ID2_REG_ADDR, &sensor_id2_value)) != FAILURE)
                {
                    return (int)sensor_id2_value;
                }
                 perror(g_buff);
            }
            break;
        default:
            {
                sprintf(g_buff, "FUNC %s(): %d is invalid option \n", __FUNCTION__, diagnostic_option);
                perror(g_buff);
            }
            break;
    }
    return FAILURE;
}

void ecam51_USB::initUVCExtensionUnit(Videostreaming *vobj)
{
    if(!vobj)
        return;
    gvobj = vobj;
    sensor_mode_selection();
}

void ecam51_USB::deInitUVCExtensionUnit()
{
    gvobj = NULL;
}

bool ecam51_USB::sensor_mode_selection()
{
    unsigned char data[]={REG_CONTROL_WRITE_REGISTER,(SENSOR_MODE_SELECT_REG_ADDR & 0xFF), (SENSOR_MODE_SELECT_REG_ADDR>>8), 0x09};

    if(!getControlValue(UVC_XU_HW_CONTROL_REGISTER, UVC_SET_CUR, 4, data))
    {
        return false;
    }
    data[0] = 0x00;
    data[3] = 0x00;
    if(!getControlValue(UVC_XU_HW_CONTROL_SENSOR, UVC_SET_CUR, 4, data))
    {
        return false;
    }
    return true;
}

bool ecam51_USB::getSensorId()
{
    int sensor_id1,sensor_id2,SensorId=0;

    sensor_id1 = diagnostic_control(DIAGNOSTIC_CNT_READ_SENSOR_ID1);
    sensor_id2 = diagnostic_control(DIAGNOSTIC_CNT_READ_SENSOR_ID2);

    if((sensor_id1 != FAILURE) && (sensor_id2 != FAILURE))
    {
            SensorId = (sensor_id1 << 8) | sensor_id2;
            _title = "SensorID";
            _text.clear();
            _text.append("Sensor ID: ");
            _text.append(QString::number(SensorId));

            emit  titleTextChanged( _title,  _text);
            return true;
    }
    return true;
}

void ecam51_USB::rungetAFStatus()
{
        threadMonitor=QtConcurrent::run(getAFStatus,this);
}

bool ecam51_USB::getAFStatus(ecam51_USB *device)
{
    int status;
    do
    {
        status = device->auto_focus_control(GET_FOCUS_STATUS_);

        if((status & 0x70) == 0x70)
        {
            //MANUAL FOCUS MODE
            device->AFStatus = MANUAL_FOCUS_;
        }
        else if((status & 0x10) == 0x10)
        {
            //AUTO FOCUS MODE
            device->AFStatus = AF_SINGLE_TRIGGER_FOCUSED;//AF_CONT_FOCUSED;
        }
        else if(status == 0x00)
        {
            //AUTO FOCUS MODE
            device->AFStatus = AF_SINGLE_TRIGGER_FOCUSING; //AF_CONT_FOCUSING;
        }
        else
        {
            device->AFStatus = FAILURE;
            usleep(100);
        }
        emit device->autoFocusStatus(device->AFStatus);

    }while(device->AFStatus == AF_SINGLE_TRIGGER_FOCUSING);

    return true;
}

bool ecam51_USB::selectFocusMode(uint mode)
{
    if(auto_focus_control(mode)==FAILURE)
    {
        return false;
    }
    return true;
}

/*  Name	:	GetManualFocusPosition
 *  Returns	:	true or false
 *  Description	:  to get the Manual Focus Position
 */
bool ecam51_USB::getFocusPosition()
{
    __u16 FocusPosition;
    unsigned char reg_value =0;
    unsigned int timeout = 10;

    write_sensor_reg(SENSOR_CMD_ACK_REG_ADDR_5M, DATA_FOR_SETTING_CMD_ACK_5M);//0x3023
    write_sensor_reg(SENSOR_CMD_MAIN_REG_ADDR_5M, DATA_FOR_GETTING_MANUAL_FOCUS_POSITION); //0x3022
    read_sensor_reg(SENSOR_CMD_ACK_REG_ADDR_5M, &reg_value);

    while((reg_value != 0) & timeout-- )
    {
         usleep(10000);
         read_sensor_reg(SENSOR_CMD_ACK_REG_ADDR_5M, &reg_value);
    }
    if(reg_value == 0)
    {
         if(read_sensor_reg(SENSOR_MANUAL_FOCUS_REG_ADDR_5M, &reg_value) == SUCCESS)
         {
               FocusPosition = (unsigned short int)reg_value;
               emit manualFocusPosition(FocusPosition);
               return true;
         }
         else
               return false;
    }
    else
        return false;
}

bool ecam51_USB::setManualFocusPosition(uint FocusPosition)
{
    unsigned char reg_value;
    unsigned int timeout = 10;

    write_sensor_reg(SENSOR_CMD_PARA3_REG_ADDR_5M, DATA_FOR_INITIAL);//0x3027
    write_sensor_reg(SENSOR_MANUAL_FOCUS_REG_ADDR_5M, FocusPosition); //0x3028
    write_sensor_reg(SENSOR_CMD_ACK_REG_ADDR_5M, DATA_FOR_SETTING_CMD_ACK_5M);//0x3023
    write_sensor_reg(SENSOR_CMD_MAIN_REG_ADDR_5M, DATA_FOR_SETTING_MANUAL_FOCUS_POSITION); //0x3022
    read_sensor_reg(SENSOR_CMD_ACK_REG_ADDR_5M, &reg_value);

    while((reg_value != 0) & timeout-- )
    {
        usleep(10000);
        read_sensor_reg(SENSOR_CMD_ACK_REG_ADDR_5M, &reg_value);
    }
    if(reg_value == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool ecam51_USB::getGPIStatus()
{
    int gpi_status =0,GPIStatus=0;

    // Read KeyStatus Register 0x8107
    gpi_status = read_key_status_register();

    if(gpi_status != FAILURE)
    {
        //Check for DIO
        if(gpi_status & GPI7_STATE)
        {
             GPIStatus |= 0x00000001;
        }
        //Check for Trigger GPI
        if(gpi_status & GPI6_STATE)
        {
            GPIStatus |= 0x00000002;
        }
        //Check for F/M
        if(gpi_status & GPI3_STATE)
        {
            GPIStatus |= 0x00000004;
        }
        emit gpioStatus(GPIStatus);
        //manually clear the GPI6 status in the GPI status register
        clear_key_status_register(gpi_status & ~(GPI6_STATE));

        return true;
    }
    return false;
}

bool ecam51_USB::controlGPO(uint GPIONumber, uint Value)
{
    unsigned int reg_addr;
    unsigned char reg_data;
    if(GPIONumber == GPO8_STATE)
    {
        reg_addr = 0xB03C;
        if(Value == SET)
            reg_data = 0x22;
         else
            reg_data = 0x20;
         if(!write_gpo_register(reg_addr, reg_data))
         {
             perror("write_gpo_register Failed: ");
             return false;
         }
         return true;
    }
    return false;
}

bool ecam51_USB::readFirmwareVersion51bUSB()
{
    _title = "Firmware Version";
    int outputValues[6];
    unsigned short int sdk_ver=0, svn_ver=0;

    if((outputValues[0] = reg_read_eeprom(EEPROM_51B_FWV_REG_ADDRESS1)) == FAILURE)
     {
            sprintf(g_buff, "Error in reading EEPROM Firmware Version register1: %x", EEPROM_51B_FWV_REG_ADDRESS1);
            perror(g_buff);
            return false;
     }
    if((outputValues[1] = reg_read_eeprom(EEPROM_51B_FWV_REG_ADDRESS2)) == FAILURE)
     {
            sprintf(g_buff, "Error in reading EEPROM Firmware Version register2: %x", EEPROM_51B_FWV_REG_ADDRESS2);
            perror(g_buff);
            return false;
     }
    if((outputValues[2] = reg_read_eeprom(EEPROM_FWV_REG_ADDRESS1)) == FAILURE)
     {
            sprintf(g_buff, "Error in reading EEPROM Firmware Version register3: %x", EEPROM_FWV_REG_ADDRESS1);
            perror(g_buff);
            return false;
     }
    if((outputValues[3] = reg_read_eeprom(EEPROM_FWV_REG_ADDRESS2)) == FAILURE)
     {
            sprintf(g_buff, "Error in reading EEPROM Firmware Version register4: %x", EEPROM_FWV_REG_ADDRESS2);
            perror(g_buff);
            return false;
     }
    if((outputValues[4] = reg_read_eeprom(EEPROM_FWV_REG_ADDRESS3)) == FAILURE)
     {
            sprintf(g_buff, "Error in reading EEPROM Firmware Version register5: %x", EEPROM_FWV_REG_ADDRESS3);
            perror(g_buff);
            return false;
     }
    if((outputValues[5] = reg_read_eeprom(EEPROM_FWV_REG_ADDRESS4)) == FAILURE)
     {
            sprintf(g_buff, "Error in reading EEPROM Firmware Version register6: %x", EEPROM_FWV_REG_ADDRESS4);
            perror(g_buff);
            return false;
     }
    _text.clear();
    _text.append(QString::number(outputValues[0]).append(".").append(QString::number(outputValues[1])));
    _text.append(".");

    sdk_ver = (outputValues[2]<<8)+outputValues[3];
    svn_ver = (outputValues[4]<<8)+outputValues[5];

    _text.append(QString::number(sdk_ver).append(".").append(QString::number(svn_ver)));
    emit titleTextChanged(_title, _text);
    return true;
}

bool ecam51_USB::readFirmwareVersion51aUSB()
{
    _title = "Firmware Version";
    int outputValues[4];

    if((outputValues[0] = reg_read_eeprom(EEPROM_FWV_REG_ADDRESS1)) == FAILURE)
     {
            sprintf(g_buff, "Error in reading EEPROM Firmware Version register1: %x", EEPROM_FWV_REG_ADDRESS1);
            perror(g_buff);
            return false;
     }
    if((outputValues[1] = reg_read_eeprom(EEPROM_FWV_REG_ADDRESS2)) == FAILURE)
     {
            sprintf(g_buff, "Error in reading EEPROM Firmware Version register2: %x", EEPROM_FWV_REG_ADDRESS2);
            perror(g_buff);
            return false;
     }
    if((outputValues[2] = reg_read_eeprom(EEPROM_FWV_REG_ADDRESS3)) == FAILURE)
     {
            sprintf(g_buff, "Error in reading EEPROM Firmware Version register3: %x", EEPROM_FWV_REG_ADDRESS3);
            perror(g_buff);
            return false;
     }
    if((outputValues[3] = reg_read_eeprom(EEPROM_FWV_REG_ADDRESS4)) == FAILURE)
     {
            sprintf(g_buff, "Error in reading EEPROM Firmware Version register4: %x", EEPROM_FWV_REG_ADDRESS4);
            perror(g_buff);
            return false;
     }
    _text.clear();
    _text.append("Firmware Version: 20");

    _text.append(QString::number(outputValues[0]).append(".").append(QString::number(outputValues[1])));
    _text.append(".");

    _text.append(QString::number(outputValues[2]).append(".").append(QString::number(outputValues[3])));
    emit titleTextChanged(_title, _text);
    return true;
}
