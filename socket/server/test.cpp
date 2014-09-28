#include <iostream>
#include "socket_server.h"
#include "childProcess.h"
//#include "struct/sensor_data.h"
//#include "struct/setPointData.h"
//#include "struct/pressureData.h"
#include "struct/debugData.h"
//#include "struct/pwmData.h"
//#include "struct/database.h"
#include "controller/controller.h"

#define MAX_THREAD 10

Controller controller;
port new_port;

int  port_identifier[MAX_THREAD];
processIdentifier identifier[MAX_THREAD];
Socket child[MAX_THREAD];

struct SensorData sensorData;
struct PressureData pressureData;
struct SetPoint setPoint;

void storeSensorData (Socket &imuSocket)
{

	while (true)
	{		
		if	(imuSocket.receive((void *)&sensorData,sizeof(sensorData))!=0) 
		{
		//	printf("Roll %lf, Roll Rate %lf, Yaw %lf Position %lf\n",test.imuData.orientationX,test.imuData.rollRate,test.imuData.yawRate,test.dvlData.positionX);
		}
		else break;
	}

}

void handleSetPoint(Socket &setPointSocket)
{
	printf("Inside Set Point Handler\n");
	while (true)
	{
		if (setPointSocket.receive((void *)&setPoint,sizeof(setPoint))!=0 )
		{
			printf("Yaw SetPoint %lf\n" ,setPoint.yaw);
			controller.updatePoint(sensorData,pressureData,setPoint);
			controller.motionController();
		}
		else break;
	}
}


void sendPwmData(Socket &pwmSocket)
{
	PwmData test;
	int flag;
	while (true)
	{
		flag=pwmSocket.send((void *)&test,sizeof(test)) ;
		if (sizeof(test) == flag)
		{	
			//printf("SendingPwmDatai\n");
			usleep(100000);
		}
		else if (flag == 0)break;
	}
}

void sendPressureData(Socket &pressureSocket)
{
	pressureData.height=100;
}


void sendDebugData(Socket &debugSocket)
{
}
void storeParam(Socket &paramSocket)
{
	ControlParam controlParam;
	while (true)
    {
		int flag;
        flag=paramSocket.receive((void *)&controlParam,sizeof(controlParam)) ;
        if (sizeof(controlParam) == flag)
        {
			printf("got Param\n");
			controller.updateParam(controlParam);
        }
        else if (flag == 0)break;
    }

	
}
void * socketHandler(void * i)
{
	printf("Started Socket Handler\n");
	port currentPort;
	char buf[10];
	int portNo;
	currentPort=*(port *)i;
	portNo=currentPort.portno;
		sprintf(buf,"%d",portNo);
	int currentThread=currentPort.identifier;
	printf("Started the thread with identifier %d \n", currentThread);	
	child[currentThread].setPort(buf,"127.0.0.1" );
	child[currentThread].initialize();
	if (currentThread == SENSOR_DATA)
	{
		storeSensorData(child[currentThread]);
	}
	else if (currentThread == SET_POINT)
	{
		handleSetPoint(child[currentThread]);
	}
		
	else if (currentThread == PWM_DATA)
	{
		sendPwmData(child[currentThread]);
	}
	else if (currentThread == PARAM_SEND)
	{
		storeParam(child[currentThread]);
	}
	else if (currentThread == DEBUG_RECEIVE)
	{
		sendPwmData(child[currentThread]);
	}
	else if (currentThread == PRESSURE_DATA)
	{
		sendPressureData(child[currentThread]);
	}
	else cout <<"No handler defined for this identifier\n"; 
}


int main()
{

	printf("Starting the Socket\n");
	port curPort[MAX_THREAD];
	int flag;
	int threadCount=0;
	pthread_t thread_id[MAX_THREAD];
	int iret[MAX_THREAD];
	Socket parent("4950","127.0.0.1",SOCK_DGRAM);

	if ((flag=parent.initialize()) == 0)
	{
		printf("Parent is ready\n");

		while(true)
		{
			threadCount++;
			parent.receive_dgram(new_port);
			curPort[threadCount].identifier=new_port.identifier;
			curPort[threadCount].portno=new_port.portno;
			printf("The Data Identifier is %d\n",new_port.identifier);
			printf("The port no is %d\n",new_port.portno);
			if( (iret[curPort[threadCount].identifier]=pthread_create( &thread_id[curPort[threadCount].identifier], NULL, &socketHandler, (void *)&curPort[threadCount])) ) 
			{
				printf("Thread creation failed: %d\n", iret[new_port.identifier]);
			} 
		}
	}
	else printf("FATAL_ERROR parent not intialize");
	pthread_exit(NULL);
	return 0;
}
