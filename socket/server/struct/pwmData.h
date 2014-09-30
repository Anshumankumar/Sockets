#ifndef PWM_DATA_H
#define PWM_DATA_H
#include <stdint.h>

/*
 *The left and Right are defined with respect to driver holding vechile from back
 */
struct PwmData
{
	uint16_t heaveBack;
	uint16_t heaveFront;
	uint16_t swayFront;
	uint16_t swayBack;
	uint16_t surgeLeft;
	uint16_t surgeRight;
};
#endif
