/* Author: "Anshuman kumar"
This file contain declaration of Sensor data and controller for controls of AUV
Note: If you are changing something please update the comments 
*/
#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <stdint.h>

struct Orientation //Data for IMU
{
	int16_t yaw;
	int16_t pitch;
	int16_t roll;
	int16_t yawRate;
	int16_t pitchRate;
	int16_t rollRate;
}

struct Position //Data for DVL
{
	int16_t posX;
	int16_t posY;
	int16_t posZ;
	int16_t velX;
	int16_t velY;
	int16_t velZ;
	int16_t accX;
	int16_t accY;
	int16_t accZ;
}

struct heightData
{
	uint16_t depth;
}

struct PWM
{
	//Thrusters as seen from front ,see from your hand
	uint16_t surgeLeft;
	uint16_t surgeRight;
	uint16_t heaveFront;
	uint16_t heaveBack;
	uint16_t surgeFront;
	uint16_t surgeBack;
}

class SensorValue
{
	struct Orientation imuData;
	struct Position dvlData;
	struct heightData pressure;
	public:
	void getSBCdata();//get the data from SBC through sockets
	void getHwData();
	
}

class Controller
{
	friend class SensorValue;
	int16_t yawErrorCurr;
	int16_t yawErrorPrev;
	int16_t yawErrorDiff;
	int32_t yawErrorSum;
	int16_t depthErrorCurr;
	int16_t depthErrorPrev;
	int16_t depthErrorDiff;
	int32_t depthErrorSum;
	int16_t pitchErrorCurr;
	int16_t pitchErrorPrev;
	int16_t pitchErrorDiff;
	int32_t pitchErrorSum;
	struct PWM pwm;
	public:
	controller();
	void reset();
	void motioncontrol(SensorValue currentReading);
	void yawReset();
	void depthReset();
	void pitchReset();
	void setPWM();
}


class Database
{


}  
#define DEFAULT_MOTOR_PWM_0 512 // sway front
#define DEFAULT_MOTOR_PWM_1 512 // surge left
#define DEFAULT_MOTOR_PWM_2 512 // heave back
#define DEFAULT_MOTOR_PWM_3 512 // heave front
#define DEFAULT_MOTOR_PWM_4 512 // surge right
#define DEFAULT_MOTOR_PWM_5 512 // sway back

#endif
