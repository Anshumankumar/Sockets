#ifndef SOCKET_CLIENT_H
#define SOCKET_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

using namespace std;

struct port 
{
	int portno;
};

class Socket
{	
	int sockfd;
	char* portno;
	int rv;
	const char* ipaddress;
	struct addrinfo hints,*servinfo,*p;
	struct sockaddr_in *sin;
	socklen_t len;
	struct port new_port;
	struct sockaddr_storage their_addr;
	public:
	Socket(char*, char*,int);
	int initialize();
	int send_dgram(port);
	int send();
	int receive();
	int get_port();
	int accept_client();
};

int setSock();

#endif
