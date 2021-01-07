#ifndef ECAM51A_USB_H
#define ECAM51A_USB_H

#include "videostreaming.h"
#include <QObject>
#include <errno.h>

#define EXTENSION_UNIT_ID                   4

#define GPI6_STATE		(1<<0)			//AF_KEY
#define GPI7_STATE		(1<<1)			//LED_KEY
#define GPI3_STATE		(1<<2)			//FLIP_KEY
#define GPO8_STATE		(1<<3)			//GPO KEY

#define SENSOR_DEVICE_ADDRESS				0x3C  // 7bit address
#define SENSOR_DEVICE_ADDRESS_FOR_READ_OPERATION	(SENSOR_DEVICE_ADDRESS << 1)
#define SENSOR_DEVICE_ADDRESS_FOR_WRITE_OPERATION	((SENSOR_DEVICE_ADDRESS << 1) | 0x01)


#define SENSOR_MODE_SELECT_REG_ADDR             0x8102


#define SENSOR_ID1_REG_ADDR				0x300A
#define SENSOR_ID2_REG_ADDR				0x300B

//OV5640
#define SENSOR_CMD_MAIN_REG_ADDR_5M             0x3022
#define SENSOR_CMD_ACK_REG_ADDR_5M              0x3023
#define SENSOR_CMD_PARA3_REG_ADDR_5M			0x3027
#define SENSOR_MANUAL_FOCUS_REG_ADDR_5M			0x3028
#define SENSOR_FW_STATUS_REG_ADDR_5M			0x3029
#define SENSOR_SELECT_STROBE_REG_ADDR_5M		0x3B00
#define SENSOR_ENABLE_STROBE_REG_ADDR_5M		0x3016
#define DATA_FOR_ENABLE_STROBE_OUTPUT			0x02
#define DATA_FOR_INITIAL                        0x00

#define DATA_FOR_SETTING_CMD_ACK_5M                 0x01
#define DATA_FOR_SETTING_SINGLE_TRIGGER_MODE		0x03
#define DATA_FOR_SETTING_CNT_AUTO_FOCUS_MODE		0x04
#define DATA_FOR_RELEASING_FOCUS_MODE               0x08
#define DATA_FOR_SETTING_MANUAL_FOCUS_POSITION		0x1A
#define DATA_FOR_GETTING_MANUAL_FOCUS_POSITION		0x1B

//EEPROM CONTROL REG_ADDRESS
#define EEPROM_DEVICE_ADDRESS_FOR_READ_OPERATION	0xA0
#define EEPROM_DEVICE_ADDRESS_FOR_WRITE_OPERATION 	0xA1
#define EEPROM_FW_START_ADDRESS				0x0000
#define EEPROM_FW_END_ADDRESS				0x8000
#define EEPROM_FWV_REG_ADDRESS1				0x0096
#define EEPROM_FWV_REG_ADDRESS2				0x0097
#define EEPROM_FWV_REG_ADDRESS3				0x0098
#define EEPROM_FWV_REG_ADDRESS4				0x0099

#define UVC_RC_UNDEFINED                    0x00
#define UVC_SET_CUR                         0x01
#define UVC_GET_LEN                         0x85
#define UVC_GET_INFO                        0x86

#define UVC_XU_HW_CONTROL_REGISTER          0x04
#define UVC_XU_HW_CONTROL_I2C_DEVICE        0x05
#define UVC_XU_HW_CONTROL_SENSOR            0x10

#define V4L2_CID_XU_FW_VERSION              0x07  //Get firmware


#define REG_CONTROL_REG_TYPE_CHIP_REGISTER		(0x0 << 0)
#define REG_CONTROL_REG_TYPE_SENSOR_REGISTER		(0x1 << 0)
#define REG_CONTROL_REG_TYPE_MASK			(0xF << 0)
#define REG_CONTROL_REG_VALUE_TYPE_BYTE			(0x0 << 4)
#define REG_CONTROL_REG_VALUE_TYPE_WORD			(0x1 << 4)
#define REG_CONTROL_REG_VALUE_MASK			(0x1 << 4)
#define REG_CONTROL_READ_REGISTER			(0x0 << 5)
#define REG_CONTROL_WRITE_REGISTER			(0x1 << 5)
#define REG_CONTROL_READ_WRITE_MASK			(0x1 << 5)

#define EXT_BUTTONS_REG_ADDR				0x8107

class ecam51A_USB : public QObject , public v4l2
{
    Q_OBJECT
public:
    Videostreaming *gvobj;
    QString _title;
    QString _text;
    static uint AFStatus;
    QFuture <bool >threadMonitor;

    explicit ecam51A_USB(QObject *parent = NULL);
    ~ecam51A_USB();
    enum getValueQueryType{
        UVC_GET_CUR = 0x81,
        UVC_GET_DEF = 0x87,
        UVC_GET_MIN = 0x82,
        UVC_GET_MAX = 0x83,
        UVC_GET_RES = 0x84
    };
    Q_ENUMS(getValueQueryType)

    enum diagnostic_control_options
    {
        DIAGNOSTIC_CNT_READ_SENSOR_ID1=1,
        DIAGNOSTIC_CNT_READ_SENSOR_ID2
    };
    Q_ENUMS(diagnostic_control_options)

    enum focus_selection_options
    {
        CNT_AUTO_FOCUS_FREEZE_SELECT=1,
        CNT_AUTO_FOCUS_UNFREEZE_SELECT,
        MANUAL_FOCUS_SELECT,
        AUTO_FOCUS_SELECT,
        SINGLE_TRIGGER_AUTO_FOCUS_SELECT,
        GET_FOCUS_STATUS_
    };
    Q_ENUMS(focus_selection_options)

    enum focus_status_options
    {
        AF_CONT_FROZEN = 1,
        AF_CONT_FOCUSED,
        MANUAL_FOCUS_,
        AF_CONT_FOCUSING,
        AF_SINGLE_TRIGGER_FOCUSING,
        AF_SINGLE_TRIGGER_FOCUSED,
        AF_SINGLE_TRIGGER_FROZEN
    };
    Q_ENUMS(focus_status_options)

    enum do_pin_states
    {
        CLEAR = 0,
        SET
    };
    Q_ENUMS(do_pin_states)
private:
    char g_buff[256];

    bool sensor_mode_selection();

    unsigned char reg_read_eeprom(__u16 reg_address);
    int read_sensor_reg(unsigned int reg_address, unsigned char *reg_data);
    bool write_sensor_reg(unsigned int reg_address, unsigned char reg_data);
    bool write_gpo_register(unsigned int reg_addr, unsigned char reg_data);

    int read_key_status_register();
    int clear_key_status_register(unsigned char value);

    int diagnostic_control(int diagnostic_option);
    bool getControlValue(__u8 controlId, __u8 queryType, uint numberOfValues, __u8 *outputValues);
    int auto_focus_control(int focus_options);
signals:
    void  manualFocusPosition(uint FocusPosition);
    void autoFocusStatus(uint AFStatus);
    void titleTextChanged(QString _title, QString _text);
    void gpioStatus(uint value);
public slots:
    void initUVCExtensionUnit(Videostreaming  *vobj);
    void deInitUVCExtensionUnit();

    bool getFirmwareVersion();
    bool getSensorId();

    void rungetAFStatus();
    static bool getAFStatus(ecam51A_USB *device);
    bool selectFocusMode(uint mode);

    bool getFocusPosition();
    bool setManualFocusPosition(uint FocusPosition);

    bool getGPIStatus();
    bool controlGPO(uint GPIONumber, uint Value);
};

#endif // ECAM51A_USB_H
