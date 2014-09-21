/*
 * Author :Anshuman Kumar
 * This is the main file for creating different threads for communication with
 * begelboard. This takes the data from various nodes create socket for sending
 * data. It also receive data like pressure sensor and pwm and send to varios 
 * topic
 */
#include <pthread.h>
#include "socket_client.h"
#include "sensor_data.h"
#include "childProcess.h"
#include "imu_data.h"
#include "dvl_data.h"
#include "pressure_data.h"
#include "pwm.h"

#define THREADNUMBER 5
void *childSensorData(void *);
void *childPressureData(void *);
void *childPwmData(void *);
void *childDebugDataSend(void *);
void *childDebugDataReceive(void *);

int main ()
{
	pthread_t threads[THREADNUMBER];
	int threadFlag[THREADNUMBER];	
	getEnvVar();
	threadFlag[SENSOR_DATA] = pthread_create(&threads[SENSOR_DATA],NULL,childSensorData,NULL);
	threadFlag[PRESSURE_DATA] = pthread_create(&threads[PRESSURE_DATA],NULL,childSensorData,NULL);
	threadFlag[PWM_DATA] = pthread_create(&threads[PWM_DATA],NULL,childSensorData,NULL);
	threadFlag[DEBUG_SEND] = pthread_create(&threads[DEBUG_SEND],NULL,childSensorData,NULL);
	threadFlag[DEBUG_RECEIVE] = pthread_create(&threads[DEBUG_RECEIVE],NULL,childSensorData,NULL);
	//initialize threads
	//thread should start communication with the beagleboard

}

void *childSensorData(void *)
{
	printf("Starting the Sensor data child\n");
	
}

void *childPressureData(void *)
{
	printf("Starting the Pressure child\n");
	
}
void *childPwmData(void *)
{
	printf("Starting the Pwm child\n");
	
}
void *DebugDataSend(void *)
{
	printf("Starting the Debug data send child\n");
	
}
void *childDataReceive(void *)
{
	printf("Starting the Debug data receive child\n");
	
}
