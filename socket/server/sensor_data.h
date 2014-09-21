#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

#include <stdint.h>

struct ImuData
{
	double orientationX;
	double orientationY;
	double orientationZ;
	double orientationW;	
	double rollRate;
	double pitchRate;
	double yawRate;
};

struct DvlData
{
	double positionX;
	double positionY;
	double positionZ;
	double velocityX;
	double velocityY;
	double velocityZ;
};

struct sensorData
{
	struct ImuData imuData;
	struct DvlData dvlData;
};
#endif
