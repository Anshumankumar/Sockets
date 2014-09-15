#include "socket_client.h"
#include "sensor_data.h"

/*extern port newPort;
  char *myIpAddress;
  char *beagleIpAddress;
  char *beaglePortNo;*/

ImuData test;
void init()
{
	test.roll=1;
	test.rollRate=2;
	test.pitchRate=3;
	test.pitch=4;
	test.yaw=5;
	test.yawRate=6;
}
int main()
{
	
	init();
	getEnvVar();
	childProcess ImuData;
	
	while (true)
	{
		printf("Sending data\n");
		ImuData.send((char*)&test,sizeof(test));
		usleep(10000);
	}

	return 0;
}

