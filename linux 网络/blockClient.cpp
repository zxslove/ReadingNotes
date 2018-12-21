/*
	*验证阻塞模式下send函数的行为，client端
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <vector>

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 3000
#define SEND_DATA "helloworld"

int main(int argc, char* argv[])
{
	int clientfd = socket(AF_INET, SOCK_STREAM, 0);
	if(clientfd == -1)
	{
		std::cout << "create client socket error." << std::endl;
		close(clientfd);
		return -1;
	}
	
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
	serveraddr.sin_port = htons(SERVER_PORT);
	if(connect(clientfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1)
	{
		std::cout << "connect server error." << std::endl;
		return -1;	
	}
	
	
	
	
	
	return 0;
}