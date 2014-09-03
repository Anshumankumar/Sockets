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
	printf("OPENING THE PORT");
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
			
	printf("OPENED THE DGRAM PORT");
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

		if (p == NULL)  {
			fprintf(stderr, "server: failed to bind\n");
			return 2;
		}
		printf("OPENED THE STREAM PORT");
		
	}
	return 0;
}
 
