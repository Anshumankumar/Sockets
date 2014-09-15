#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

#include <stdio.h>
#include <stdlib.h>
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
#include <pthread.h>

using namespace std;

enum datatype
{
	IMU_DATA=1,
	PRESSURE_SENSOR=2,
	PNEUMATICS_DATA=3,
	
};
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
	datatype receivedData;
        socklen_t len;
        struct port new_port;
        struct sockaddr_storage their_addr;
        public:
	Socket();
        Socket(char*, char*,int);
	void setPort(char*,char*);
        int initialize();
        int receive_dgram(port&);
        int send();
        int receive(void *,int);
        int get_port();
        int accept_client();
};

#endif

