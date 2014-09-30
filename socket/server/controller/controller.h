/*
Author: Anshuman kumar
These is the basic pid controller for our AUV matsya
Its takes a validityparam ,check it and then run the controller
*/

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../struct/database.h"
#include "../struct/pressureData.h"
#include "../struct/pwmData.h"
#include "../struct/sensor_data.h"
#include "../struct/setPointData.h"
#include "hardwareModule.h"

class Controller
{
	struct ControlParam controlParam;
	struct SetPoint setPoint;
	struct SensorData sensorData;
	struct PwmData pwmData;
	struct PressureData pressureData;
	struct ControllerMetaData controllerMetaData;
	public:
	Controller();
	void controllerErrorReset();
	void yawControllerErrorReset();
	void depthControllerErrorReset();
	void pitchControllerErrorReset();
	void resetPwm();
	void updateParam(ControlParam);
	void updatePoint(SensorData,PressureData,SetPoint);
	void motionController();
		
};

#define KI_DIV 2
#define DEFAULT_MOTOR_PWM_HB 512 // heave back
#define DEFAULT_MOTOR_PWM_HF 512 // heave left
#define DEFAULT_MOTOR_PWM_SWB 512 // sway  back
#define DEFAULT_MOTOR_PWM_SWF 512 // sway front
#define DEFAULT_MOTOR_PWM_SUL 512 // surge left
#define DEFAULT_MOTOR_PWM_SUR 512 // surge right


#define VALID_YAW_CONTROL 0x01
#define VALID_DEPTH_CONTROL 0x02
#define VALID_SWAY_CONTROL 0x04
#define VALID_SURGE_CONTROL 0x08
#define VALID_PITCH_CONTROL 0x10



#endif
