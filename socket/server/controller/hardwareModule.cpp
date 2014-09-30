#include "hardwareModule.h"

extern struct PwmData pwmData;
void initPwm()
{

}
void initInputPins()
{

}
void pwmSet(PwmData data)
{
	printf("HF:%d,HB:%d,SWB:%d,SWF:%d,SUL%d,SUR%d\n",data.heaveFront,data.heaveBack,data.swayBack,data.swayFront,data.surgeLeft,data.surgeRight);
	
	pwmData.heaveFront=data.heaveFront;
	pwmData.heaveBack=data.heaveBack;
	pwmData.swayFront=data.swayFront;
	pwmData.swayBack=data.swayBack;
	pwmData.surgeLeft=data.surgeLeft;
	pwmData.surgeRight=data.surgeRight;
}
