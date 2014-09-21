#ifndef SOCKET_CLIENT_H
#define SOCKET_CLIENT_H


#define MYIP "192.168.3.5"
#define BBIP "192.168.3.4"
#define PORTNO "4450"

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
	int identifier;
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
	int send_stream(char *,int);
	int receive_stream(char *,int);
	int get_port();
	int accept_client();
};


class childProcess
{
	Socket sensorData;
	port newPort;
	public:
	childProcess();
	void send (char *,int);
	void receive(char *,int);	
};

void getEnvVar();

#endif
