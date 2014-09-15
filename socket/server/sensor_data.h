#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

#include <stdint.h>

struct ImuData
{
	int16_t roll;
	int16_t pitch;
	int16_t yaw;	
	int16_t rollRate;
	int16_t pitchRate;
	int16_t yawRate;
};

#endif
