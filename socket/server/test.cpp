#include "socket_server.h"
#include <iostream>
#define MAX_THREAD 10
#include "sensor_data.h"
enum
{
	SENSOR_DATA=1,
	BEAGLE_SBC=2

};


char buf[10];
port new_port;
int identifier[MAX_THREAD];
Socket child[MAX_THREAD];
int thread_counter;

void printImuData (Socket &imuSocket)
{
	ImuData test;
	while (true)
	{		
		if	(imuSocket.receive((void *)&test,sizeof(test))!=0) 
		{
			printf("Roll %d, Roll Rate %d, Yaw %d\n",test.roll,test.rollRate,test.yaw);
		}
	}

}

void * socketHandler(void *)

{
	int currentThread=thread_counter-1;	
	printf("%s",buf);
	child[currentThread].setPort(buf,"127.0.0.1" );
	child[currentThread].initialize();
	if (identifier[currentThread]==1)
	{
		printImuData(child[currentThread]);
	}

}


//pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int main()
{
	printf("Starting the Socket\n");

	int flag;

	thread_counter=0;
	pthread_t thread_id[MAX_THREAD];
	int iret[MAX_THREAD];
	Socket parent("4950","127.0.0.1",SOCK_DGRAM);

	flag=parent.initialize();
	cout <<flag;
	flag=0;	cout << flag;
	printf("Parent is ready\n");

	while(flag==0 && thread_counter<MAX_THREAD)
	{
		parent.receive_dgram(new_port);
		printf("The Data Identifier is %d",new_port.identifier);

		sprintf(buf,"%d",new_port.portno);
		identifier[thread_counter]=new_port.identifier;
		if( (iret[thread_counter]=pthread_create( &thread_id[thread_counter++], NULL, &socketHandler, NULL)) )        
		{
			printf("Thread creation failed: %d\n", iret[thread_counter-1]);
		} 




	}
	pthread_exit(NULL);

	return 0;
}
