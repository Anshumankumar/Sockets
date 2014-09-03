#include "socket_server.h"
#define MAX_THREAD 10

char buf[10];
port new_port;
Socket child[MAX_THREAD];
int thread_counter;

void * socketHandler(void *)

{
	printf("%s",buf);
        child[thread_counter-1].setPort(buf,"127.0.0.1" );
        child[thread_counter-1].initialize();
	while (true)
	{
	}
}

//pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int main()
{
	int flag;
	
	thread_counter=0;
	pthread_t thread_id[MAX_THREAD];
	int iret[MAX_THREAD];
	Socket parent("4950","127.0.0.1",SOCK_DGRAM);
	flag=parent.initialize();
	flag=0;
	while(flag==0 && thread_counter<MAX_THREAD)
	{	
		if (parent.receive_dgram(new_port)==0)
		{	
			sprintf(buf,"%d",new_port.portno);	
			if( (iret[thread_counter]=pthread_create( &thread_id[thread_counter++], NULL, &socketHandler, NULL)) )        
			{
				printf("Thread creation failed: %d\n", iret[thread_counter-1]);
			} 
			


		}
	}
	pthread_exit(NULL);
	return 0;
}
