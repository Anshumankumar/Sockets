#include "socket_client.h"


int setSock()
{
	//Read from files;
	//Parse the text;
	//open the sockets;
	//initialize sockets;
	//tell other to enjoy your sockets with only one command;

	int s;
	char *ip="127.0.0.1";
	char *data="Anshuman";
	Socket mcb(NULL,ip,SOCK_STREAM);
	Socket communicate("4950",ip,SOCK_DGRAM);
	mcb.initialize();
	communicate.initialize();
	port new_port;
	new_port.portno=mcb.get_port();
	s=communicate.send_dgram(new_port);
	s=mcb.accept_client();
	printf("Enjoy");
	return 0;

}


int main()
{
	int s;
	if ((s=setSock())==0)
	{
		while(true)
		{
		}


	}
	else
	{
		//take some action
	}
	return 0;
}

