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
class Socket
{	
	int sockfd;
	char* portno;
	int rv;
	const char* ipaddress;
	struct addrinfo hints,*servinfo,*p;
	public:
        Socket(char*, char*,int);
	int initialize();
	int send_dgram();
	int send();
	int receive();
};
