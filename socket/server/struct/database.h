#ifndef DATABASE_H
#define DATABASE_H
#include <stdint.h>

struct  ControlParam
{
	double kpYaw,kdYaw,kiYaw;
	double kpDepth,kdDepth,kiDepth;
	double kpPitch,kdPitch,kiPitch;
	double cSurge, cSway;
	unsigned short controlValidity;
};


struct ControllerMetaData
{
    double yawErrorCurr;
    double yawErrorPrev;
    double yawErrorDiff;
    double yawErrorSum;
    double depthErrorCurr;
    double depthErrorPrev;
    double depthErrorDiff;
    double depthErrorSum;
    double pitchErrorCurr;
    double pitchErrorPrev;
    double pitchErrorDiff;
    double pitchErrorSum;

};
#endif
