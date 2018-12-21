/*
	*验证scoket阻塞的TCP服务器
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

int main(int argc, char* argv[])
{
	//1.创建一个侦听的socket
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd == -1)
	{
		std::cout << "create listen socket error." << std::endl;
		return -1;
	}
	
	//2.绑定服务器的IP地址
	struct sockaddr_in bindaddr;
	binaddr.sin_family = AF_INET;
	bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bindaddr.sin_port = htons(3000);
	if(bind(listenfd, (struct sockaddr*)&bindaddr, sizeof(bindaddr)) == -1)
	{
		std::cout << bind socket error. << std::endl;
		return -1;
	}
	
	//3.启动侦听
	if(listen(listenfd, SOMAXCONN) == -1)
	{
		std::cout << "listen socket error." << std::endl;
		return -1;
	}
	
	while(true)
	{
		struct sockaddr_in clientaddr;
		socklen_t len = sizeof(clientaddr);
		//4.接受客户端的连接
		int clientfd = accept(listenfd, (struct sockaddr*)&clientaddr, &len);
		if(clientfd != -1)
		{
			std::cout << "accept client connecttion." << std::endl;
		}
	}
	
	// 关闭侦听socke
	close(listenfd);
	
	return 0;
}