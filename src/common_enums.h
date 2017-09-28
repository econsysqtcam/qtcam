#ifndef COMMON_ENUMS_H
#define COMMON_ENUMS_H

#include <QObject>

/**
 * @brief The CommonEnums class - This class contains the enum values used by this Application.
 */

//Added by Dhurka - 12th Oct 2016
/*
 * Added this class for Camera enum values.
 * If any new camera, will add the ENUM value of the camera here
 */

class CommonEnums : public QObject
{
    Q_OBJECT
public:

    typedef enum
    {
        NONE = 0,
        SEE3CAM_11CUG,
        SEE3CAM_12CUNIR,
        SEE3CAM_CU30,
        SEE3CAM_CU40,
        SEE3CAM_CU50,
        SEE3CAM_CU51,
        SEE3CAM_CU130,        
        CX3_UVC_CAM,
        HD_PRO_WEBCAM,
        ECON_1MP_BAYER_RGB,
        ECON_1MP_MONOCHROME,
        ECON_CX3_RDX_T9P031,
        ECON_CX3_RDX_V5680,
        ECON_8MP_CAMERA,
        SEE3CAM_130,
		// Added by Sankari - 7 Feb 2017
		SEE3CAM_30,
		// Added by Sankari - 17 Dec 2016
        SEE3CAM_81,
        // Added by Sankari - 22 Feb 2017
        SEE3CAM_CU135,
		// Added by Sankari - 28 Jul 2017
        SEE3CAM_CU20,
        // Added by Sankari - 13 June 2017
        CX3_SNI_CAM

    }ECameraNames;
    //Enum for capture shot type
    typedef enum
    {
        SNAP_SHOT,
        TRIGGER_SHOT,
        BURST_SHOT,
        CHANGE_FPS_SHOT
    }ECaptureShotType;

    Q_ENUMS(ECameraNames)
	Q_ENUMS(ECaptureShotType)
};
#endif // COMMON_ENUMS_H
