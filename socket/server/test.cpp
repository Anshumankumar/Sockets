#include "socket_server.h"
#include <iostream>
#include "sensor_data.h"
#include "childProcess.h"
#include "setPointData.h"

#define MAX_THREAD 10

port new_port;

int  port_identifier[MAX_THREAD];
processIdentifier identifier[MAX_THREAD];
Socket child[MAX_THREAD];

void printImuData (Socket &imuSocket)
{
	sensorData test;
	while (true)
	{		
		if	(imuSocket.receive((void *)&test,sizeof(test))!=0) 
		{
			printf("Roll %lf, Roll Rate %lf, Yaw %lf Position %lf\n",test.imuData.orientationX,test.imuData.rollRate,test.imuData.yawRate,test.dvlData.positionX);
		}
	}

}

void handleSetPoint(Socket &setPointSocket)
{
	printf("Inside Set Point Handler\n");
	SetPoint test;
	while (true)
	{
		if (setPointSocket.receive((void *)&test,sizeof(test))!=0 )
		{
			printf("Yaw SetPoint %lf\n" ,test.yaw);
		}
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
		printImuData(child[currentThread]);
	}
	else if (currentThread == SET_POINT)
	{
		handleSetPoint(child[currentThread]);
	}

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
