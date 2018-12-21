/*
	*TCP服务器
	*socket() -> bind() -> listen() -> accept() ->write/send() -> close()
*/

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h> // for htonl()
#include <unistd.h>
#include <string.h>
#include <vector>


int main(int argc, char* argv[])
{
	//1.创建一个侦听socket
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd == -1)
	{
		std::cout << "socket() function return descriptor is error" << std::endl;
		return -1;			
	}
	
	//2.绑定IP地址和端口号(初始化服务器相关信息)
	struct sockaddr_in bindaddr;
	memset(&bindaddr, 0, sizeof(bindaddr));
	bindaddr.sin_family = AF_INET;
	bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bindaddr.sin_port = htons(3000);
	if(bind(listenfd, (struct sockaddr*)&bindaddr, sizeof(bindaddr)) == -1)
	{
		std::cout << "bind() function is error" << std::endl;
		return -1;
	}
	
	//3.启动侦听
	if(listen(listenfd, SOMAXCONN) == -1)
	{
		std::cout << "listen() function is error" << std::endl;
		return -1;
	}
	
	//记录所有客户端连接的容器
	std::vector<int> clientfds;
	while(true)
	{
		struct sockaddr_in clientaddr;
		socklen_t clnaddrlen = sizeof(clientaddr);
		//4.接收客户端连接
		int clientfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clnaddrlen);
		if(clientfd != -1)
		{
			char recvBuf[32] = {0};
			//5.从客户端接收数据
			int ret = recv(clientfd, recvBuf, 32, 0);
			std::cout << "recv data from client." << ret << std::endl;
				
			if(ret > 0)
			{
				std::cout << "recv data from client, data: " << recvBuf << std::endl;
				//6.send发送给客户端
				ret = send(clientfd, recvBuf, strlen(recvBuf), 0);
			  if (ret != strlen(recvBuf))
                  std::cout << "send data error." << std::endl;

              std::cout << "send data to client successfully, data: " << recvBuf << std::endl;
			}
			else
			{
				std::cout << "recv() data error." << std::endl;
			}
			
			clientfds.push_back(clientfd);
			
		}
	}
	
	//7.关闭侦听
	close(listenfd);

	return 0;
}

//可通过命令来查看: lsof -i -Pn

