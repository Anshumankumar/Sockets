/*
Author:Anshuman kumar
These are the functions for to run on main computer to contact the server runing on Beagleboard.There is DGRAM connection for sending data to parent server in beagleboard 
This source of code is only tested for ip4 version not for ip6 version
 */

#ifndef SOCKET_CLIENT_CPP
#define SOCKET_CLIENT_CPP
#include "socket_client.h"
#include "sensor_data.h"

 char *myIpAddress;
  char *beagleIpAddress;
  char *beaglePortNo;
extern ImuData test;
/*--------------------------------------------------------------------------------
  CONSTRUCTOR
  --------------------------------------------------------------------------------*/
Socket::Socket(char * port,char* address, int socktype)
{		
	memset(&hints,0,sizeof(hints));
	hints.ai_family=AF_INET;
	hints.ai_socktype=socktype;
	portno=port;
	ipaddress=address;
}

/*-------------------------------------------------------------------------------
  MAIN INTIALIZATION
  --------------------------------------------------------------------------------*/

//This function  intialize all the socket variables
int Socket::initialize()
{
	int yes=1;
	if ((rv=getaddrinfo(ipaddress,portno, &hints, &servinfo)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	if (hints.ai_socktype==SOCK_DGRAM)
	{
		for(p = servinfo; p != NULL; p = p->ai_next) 
		{
			if ((sockfd = socket(p->ai_family, p->ai_socktype,
							p->ai_protocol)) == -1) 
			{
				perror("talker: socket");
				continue;
			}
			break;
		}
		if (p == NULL)
		{
			fprintf(stderr, "talker: failed to open socket\n");
			return 2;
		}
		//freeaddrinfo(servinfo);

	}
	else if (hints.ai_socktype==SOCK_STREAM)
	{
		for(p = servinfo; p != NULL; p = p->ai_next) {
			if ((sockfd = socket(p->ai_family, p->ai_socktype,
							p->ai_protocol)) == -1) {
				perror("server: socket");
				continue;
			}

			if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
						sizeof(int)) == -1) {
				perror("setsockopt");
				exit(1);
			}

			if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
				close(sockfd);
				perror("server: bind");
				continue;
			}

			break;
		}

		if (p == NULL)  
		{
			fprintf(stderr, "server: failed to bind\n");
			return 2;
		}
		if (listen(sockfd,1) == -1)
		{
			perror("listen");
			exit(1);
		}

		socklen_t len=sizeof(*(p->ai_addr));

		if (getsockname(sockfd, (struct sockaddr *)p->ai_addr, &len) == -1)
		{
			perror("getsockname");
		}

		else	
			sin=(sockaddr_in*)(p->ai_addr);
		printf("port number %d\n", ntohs(sin->sin_port));

	}
	return 0;
}

/*-----------------------------Intialization ends-------------------------------*/


/*
   Always Call the below function after Intialization
 */

/*-------------------------------------------------------------------------------
  ACCEPT SEND AND RECEIVE
  --------------------------------------------------------------------------------*/                               
int Socket::accept_client()
{
	printf("Listening for connection");
	if((sockfd = accept(sockfd, (struct sockaddr *)&their_addr, &len))!=-1)
	{
		printf("Accepted connection from ipaddress");
	}

	return 0;
}

/*
   This function get the port no from current Socket server to connect parent in
   beagleboard
 */
int Socket::get_port()
{
	return ntohs(sin->sin_port);
}

int Socket::send_stream(char* data,int len )
{
	int flag;
	flag=write(sockfd,data,len);
	return flag;
}

int Socket::receive_stream(char* data,int len)
{
	int flag;
	flag=read(sockfd,data,len);
}

/*
   This function is only for DGRAM connectiom
 */
int Socket::send_dgram(port portToSend) 
{
	int a;
	printf( "anshuman");
	if (hints.ai_socktype==SOCK_DGRAM)
	{
		if ((a= sendto(sockfd,(char *)&portToSend, sizeof(portToSend), 0,

						p->ai_addr, p->ai_addrlen)) == -1)
		{
			return 1;
		}
	}
	else 
	{
		return 2;
	}
	close (sockfd);
	return 0;

}

/*----------------------END of send receive and accept--------------------------*/

/*-------------------------------------------------------------------------------
  CHILD PROCESS
  -------------------------------------------------------------------------------*/
childProcess::childProcess() :sensorData(NULL,myIpAddress,SOCK_STREAM)
{


	Socket connector(beaglePortNo,beagleIpAddress,SOCK_DGRAM);	

	if (0!=connector.initialize())
	{
		printf("Error in Initialize of socket Communicate");
		return;
	}
	if (0 != sensorData.initialize())
	{
		printf("Error in Initialize of socket sensorData");
		return;
	}
	
	newPort.portno=sensorData.get_port();
	newPort.identifier=1;
	connector.send_dgram(newPort);
	sensorData.accept_client();

}

void childProcess::send(char * data,int len)
{
	sensorData.send_stream(data,len);

}

void childProcess::receive(char *data, int len)
{

sensorData.receive_stream(data,len);

}


/*--------------------------------------------------------------------------------
  Getting the environment variable
  --------------------------------------------------------------------------------*/
void  getEnvVar()
{
	beaglePortNo = getenv ("AUV_PORT");
	beagleIpAddress=getenv("AUV_IPADDRESS");
	myIpAddress=getenv("MY_IPADDRESS");
	if (beagleIpAddress==NULL || myIpAddress==NULL |beaglePortNo==NULL)
	{ 
		printf ("Environment variable are not set,Using the #define Parameter");
		beaglePortNo=PORTNO;
		beagleIpAddress=BBIP;
		myIpAddress=MYIP;
	}
}
/*----------------------End get enviroment variable----------------------------*/
#endif 
