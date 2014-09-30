#include "controller.h"
#include "hardwareModule.h"

double abs(double a)
{
	return (a>0)?a:-a;
}



double normailizeError(double sp, double reading)
{
	double error = sp - reading;
	if(abs(error) >= 180.0)
	{
		if (error <= 0.0)
		{
			error = 360.0 - abs(error);
		}
		else
		{
			error = abs(error) - 360.0;
		}
	}
	return error;
}
void cap(unsigned short &X)
{
	if((X>1023)&&(X<0x1FFF))
	{
		X = 1023;
	}
	if(X>0x1FFF)
	{
		X = 0;
	}
}

Controller::Controller()
{
	initPwm();
	initInputPins();
}



void  Controller::yawControllerErrorReset()
{
	controllerMetaData.yawErrorCurr = 0;
	controllerMetaData.yawErrorPrev = 0;
	controllerMetaData.yawErrorSum = 0;
	controllerMetaData.yawErrorDiff = 0;
}

void Controller::pitchControllerErrorReset()
{
	controllerMetaData.pitchErrorCurr = 0;
	controllerMetaData.pitchErrorPrev = 0;
	controllerMetaData.pitchErrorSum = 0;
	controllerMetaData.pitchErrorDiff = 0;
}
void Controller::depthControllerErrorReset()
{
	controllerMetaData.depthErrorCurr = 0;
	controllerMetaData.depthErrorPrev = 0;
	controllerMetaData.depthErrorSum = 0;
	controllerMetaData.depthErrorDiff = 0;
}

void Controller::resetPwm()
{	
	pwmData.heaveBack = DEFAULT_MOTOR_PWM_HB;
	pwmData.heaveFront = DEFAULT_MOTOR_PWM_HF;
	pwmData.swayBack = DEFAULT_MOTOR_PWM_SWB;
	pwmData.swayFront = DEFAULT_MOTOR_PWM_SWF;
	pwmData.surgeLeft = DEFAULT_MOTOR_PWM_SUL;
	pwmData.surgeRight = DEFAULT_MOTOR_PWM_SUR;
	setPoint.posZ=pressureData.height;
	setPoint.yaw=sensorData.imuData.yaw;
	setPoint.pitch=sensorData.imuData.pitch;
	setPoint.velX=0.0;
	setPoint.velY=0.0;
}

void Controller::updateParam(ControlParam data)
{
	controlParam.kpYaw=data.kpYaw;
	controlParam.kdYaw=data.kdYaw;
	controlParam.kiYaw=data.kiYaw;
	controlParam.kpDepth=data.kpDepth;
	controlParam.kdDepth=data.kdDepth;
	controlParam.kiDepth=data.kiDepth;
	controlParam.kpPitch=data.kpPitch;
	controlParam.kdPitch=data.kdPitch;
	controlParam.kiPitch=data.kiPitch;
	controlParam.cSway=data.cSway;
	controlParam.cSurge=data.cSurge;
}


void Controller::updatePoint(SensorData data1,PressureData data2,SetPoint data3)
{
	pressureData.height=data2.height;
	sensorData.imuData.roll=data1.imuData.roll;
	sensorData.imuData.yaw=data1.imuData.yaw;
	sensorData.imuData.pitch=data1.imuData.pitch;
	setPoint.velX=data3.velX;
	setPoint.velY=data3.velY;
	setPoint.posZ=data3.posZ;
	setPoint.yaw=data3.yaw;
	setPoint.pitch=data3.pitch;

}
void Controller::motionController()
{
	controlParam.controlValidity=0x01;
	controllerMetaData.yawErrorPrev = controllerMetaData.yawErrorCurr;
	controllerMetaData.yawErrorCurr = normailizeError(setPoint.yaw, sensorData.imuData.yaw);
	controllerMetaData.yawErrorDiff = controllerMetaData.yawErrorCurr - controllerMetaData.yawErrorPrev;
	controllerMetaData.yawErrorSum += controllerMetaData.yawErrorCurr;

	controllerMetaData.pitchErrorPrev = controllerMetaData.pitchErrorCurr;
	controllerMetaData.pitchErrorCurr = normailizeError(setPoint.pitch,sensorData.imuData.pitch);
	controllerMetaData.pitchErrorDiff = controllerMetaData.pitchErrorCurr - controllerMetaData.pitchErrorPrev;
	controllerMetaData.pitchErrorSum += controllerMetaData.pitchErrorCurr;

	controllerMetaData.depthErrorPrev = controllerMetaData.depthErrorCurr;
	controllerMetaData.depthErrorCurr = setPoint.posZ - pressureData.height;
	controllerMetaData.depthErrorDiff = controllerMetaData.depthErrorCurr - controllerMetaData.depthErrorPrev;
	controllerMetaData.depthErrorSum += controllerMetaData.depthErrorCurr;
	
	if(controllerMetaData.yawErrorSum > 50)
    {
        controllerMetaData.yawErrorSum = 50;
    }

    if(controllerMetaData.yawErrorSum < -50)
    {
        controllerMetaData.yawErrorSum = -50;
    }

    if(controllerMetaData.pitchErrorSum > 50)
    {
        controllerMetaData.pitchErrorSum = 50;
    }

    if(controllerMetaData.pitchErrorSum < -50)
    {
        controllerMetaData.pitchErrorSum = -50;
    }
	
	pwmData.swayBack = DEFAULT_MOTOR_PWM_SWB;
    pwmData.swayFront = DEFAULT_MOTOR_PWM_SWF;
    pwmData.surgeLeft = DEFAULT_MOTOR_PWM_SUL;
    pwmData.surgeRight = DEFAULT_MOTOR_PWM_SUR;
    pwmData.heaveBack = DEFAULT_MOTOR_PWM_HB;
    pwmData.heaveFront = DEFAULT_MOTOR_PWM_HF;
	
	if (setPoint.velY>50.0) setPoint.velY=50.0;
	else if (setPoint.velY<-50.0) setPoint.velY=-50.0;
	if (setPoint.velX>50.0) setPoint.velX=50.0;
	else if (setPoint.velX<-50.0) setPoint.velX=-50.0;
	
	if ( controlParam.controlValidity & VALID_SWAY_CONTROL)
	{
		pwmData.swayBack += (int)controlParam.cSway*setPoint.velY;
		pwmData.swayFront += (int)controlParam.cSway*setPoint.velY;
	}
	
	if ( controlParam.controlValidity & VALID_SURGE_CONTROL)
    {
        pwmData.swayBack += (int)controlParam.cSurge*setPoint.velX;
        pwmData.swayFront += (int)controlParam.cSurge*setPoint.velX;
    }
	
	if ( controlParam.controlValidity & VALID_YAW_CONTROL)
	{
		if (setPoint.velY < 10.0)
		{
			pwmData.swayBack -=(int) controlParam.kpYaw*controllerMetaData.yawErrorCurr;
			pwmData.swayBack -=(int) controlParam.kdYaw*controllerMetaData.yawErrorDiff;
			pwmData.swayBack -=(int) controlParam.kiYaw*controllerMetaData.yawErrorSum;
			
			pwmData.swayFront +=(int) controlParam.kiYaw*controllerMetaData.yawErrorSum;
			pwmData.swayFront +=(int) controlParam.kdYaw*controllerMetaData.yawErrorDiff;
			pwmData.swayFront +=(int) controlParam.kpYaw*controllerMetaData.yawErrorCurr;
		}	
		else 
		{
			int factor=1;
			pwmData.surgeLeft +=(int) controlParam.kpYaw*controllerMetaData.yawErrorCurr;
            pwmData.surgeLeft +=factor*(int) controlParam.kdYaw*controllerMetaData.yawErrorDiff;
            pwmData.surgeLeft +=(int) controlParam.kiYaw*controllerMetaData.yawErrorSum;

            pwmData.surgeRight -=(int) controlParam.kiYaw*controllerMetaData.yawErrorSum;
            pwmData.surgeRight -=factor*(int) controlParam.kdYaw*controllerMetaData.yawErrorDiff;
            pwmData.surgeRight -=(int) controlParam.kpYaw*controllerMetaData.yawErrorCurr;

		}
	}


	if ( controlParam.controlValidity & VALID_DEPTH_CONTROL)
	{
			pwmData.heaveBack +=(int) controlParam.kpDepth*controllerMetaData.depthErrorCurr;
            pwmData.heaveBack +=(int) controlParam.kdDepth*controllerMetaData.depthErrorDiff;
            pwmData.heaveBack +=(int) controlParam.kiDepth*controllerMetaData.depthErrorSum;

            pwmData.heaveFront +=(int) controlParam.kiDepth*controllerMetaData.depthErrorSum;
            pwmData.heaveFront +=(int) controlParam.kdDepth*controllerMetaData.depthErrorDiff;
            pwmData.heaveFront +=(int) controlParam.kpDepth*controllerMetaData.depthErrorCurr;

	}

	if ( controlParam.controlValidity & VALID_DEPTH_CONTROL)
    {
            pwmData.heaveBack +=(int) controlParam.kpDepth*controllerMetaData.depthErrorCurr;
            pwmData.heaveBack +=(int) controlParam.kdDepth*controllerMetaData.depthErrorDiff;
            pwmData.heaveBack +=(int) controlParam.kiDepth*controllerMetaData.depthErrorSum;

            pwmData.heaveFront +=(int) controlParam.kiDepth*controllerMetaData.depthErrorSum;
            pwmData.heaveFront +=(int) controlParam.kdDepth*controllerMetaData.depthErrorDiff;
            pwmData.heaveFront +=(int) controlParam.kpDepth*controllerMetaData.depthErrorCurr;

    }

	
	if (controlParam.controlValidity & VALID_PITCH_CONTROL)
	{
		pwmData.heaveBack -=(int) controlParam.kpPitch*controllerMetaData.pitchErrorCurr;
		pwmData.heaveBack -=(int) controlParam.kdPitch*controllerMetaData.pitchErrorDiff;
		pwmData.heaveBack -=(int) controlParam.kiPitch*controllerMetaData.pitchErrorSum;
    
		pwmData.heaveFront +=(int) controlParam.kiPitch*controllerMetaData.pitchErrorSum;
		pwmData.heaveFront +=(int) controlParam.kdPitch*controllerMetaData.pitchErrorDiff;
		pwmData.heaveFront +=(int) controlParam.kpPitch*controllerMetaData.pitchErrorCurr;

	}

	cap(pwmData.swayBack);
	cap(pwmData.swayFront);
	cap(pwmData.surgeLeft);
	cap(pwmData.surgeRight);
	cap(pwmData.heaveBack);
	cap(pwmData.heaveFront);
	pwmSet(pwmData);
	
}



