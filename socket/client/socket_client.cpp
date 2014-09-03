/*This source of code is only tested for ip4 version not for ip6 version
 */
#ifndef SOCKET_CLIENT_CPP
#define SOCKET_CLIENT_CPP

#include "socket_client.h"

Socket::Socket(char * port,char* address, int socktype)
{	
	memset(&hints,0,sizeof(hints));
	hints.ai_family=AF_INET;
	hints.ai_socktype=socktype;
	portno=port;
	ipaddress=address;

}

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
int Socket::accept_client()
{
	printf("Listening for connection");
	if((sockfd= accept(sockfd, (struct sockaddr *)&their_addr, &len))==-1)
	{
		printf("accept");
	}

	return 0;
}

int Socket::get_port() //NEVER CALL THIS FUNCTION FOR DGRAM and withot initialize;
{
	return ntohs(sin->sin_port);
}
int Socket::send_dgram(port portToSend) 
{
	int a;
	if (hints.ai_socktype==SOCK_DGRAM)
	{
		if ((a= sendto(sockfd,(char *)&portToSend, sizeof(portToSend), 0,
						p->ai_addr, p->ai_addrlen)) == -1)
		{
		//	perror("sendto");
			return 1;
		}
	}
	else 
	{
		//close(sockfd);
		return 2;
	}
	close (sockfd);
	return 0;

}
#endif 
