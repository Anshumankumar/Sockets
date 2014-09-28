#ifndef SOCKET_SERVER_CPP
#define SOCKET_SERVER_CPP
#include "socket_server.h"
#include <stdio.h>
Socket::Socket (char* port,char* address,int socktype)
{
	memset(&hints,0,sizeof(hints));
	hints.ai_family=AF_INET;
	hints.ai_socktype=socktype;
	portno=port;
	ipaddress=address;

}

Socket::Socket()
{

	memset(&hints,0,sizeof(hints));
	hints.ai_family=AF_INET;
	hints.ai_socktype=SOCK_STREAM;
}

Socket::~Socket()
{
	printf("Closing the Socket\n");
	close(sockfd);
}
void Socket::setPort (char* port, char* address)
{
	ipaddress=address;
	portno=port;

}

int Socket::initialize()
{
	if ((rv = getaddrinfo(ipaddress, portno, &hints, &servinfo)) != 0)
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
				perror("listener: socket");
				continue;
			}

			if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
			{
				close(sockfd);
				perror("listener: bind");
				continue;
			}

			break;
		}

		if (p == NULL) 
		{
			fprintf(stderr, "listener: failed to bind socket\n");
			return 2;
		}

		freeaddrinfo(servinfo);
		return 0;
	}


	else if(hints.ai_socktype==SOCK_STREAM)
	{
		for(p = servinfo; p != NULL; p = p->ai_next) {
			if ((sockfd = socket(p->ai_family, p->ai_socktype,
							p->ai_protocol)) == -1) {
				perror("client: socket");
				continue;
			}

			if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
				close(sockfd);
				perror("client: connect");
				continue;
			}

			break;
		}

		if (p == NULL) {
			fprintf(stderr, "client: failed to connect\n");
			return 2;
		}

		printf("client: connecting to %s \n",portno);



	}

}
int Socket::receive_dgram(port &new_port )
{
	printf("Wating for clients \n");

	int numbytes;
	len = sizeof (their_addr);
	if ((numbytes = recvfrom(sockfd, &new_port, sizeof(new_port) , 0,
					(struct sockaddr *)&their_addr, &len)) == -1) {
		perror("recvfrom");
		exit(1);
	}

}

int Socket::receive(void * data,int len)
{
	int flag;
	flag=read(sockfd,(char *)data,len);
	return flag;
}

int Socket::send(void * data,int len)
{
	int flag;
	flag=write(sockfd,(char *)data,len);
	return flag;
}

#endif
