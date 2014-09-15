#include "controller.h"

int abs(int a)
{
   return (a>0)?a:-a;
}

int normailizeError(int sp, int reading)
{
    int error = sp - reading;
    if(abs(error) >= 180)
    {
        if (error <= 0)
        {
            error = 360 - abs(error);
        }
        else
        {
            error = abs(error) - 360;
        }
    }
    return error;
}


void Controller::motioncontrol(SensorValue currentReading)
{
	yawErrorPrev=yawErrorCurr;
	yawErrorCurr=normalizeError(


}
