/*
	*��֤������ģʽ��send��������Ϊ�Ŀͻ���
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 3000
#define SEND_DATA "helloworld"

int main(int argc, char* argv[])
{
	//1.����һ��socket
	int clientfd = socket(AF_INET, SOCK_STREAM, 0);
	if(clientfd == -1)
	{
		std::cout << "create client error." << std::endl;
		return -1;
	}
	
	//2.���ӷ�����
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
	serveraddr.sin_port = htons(SERVER_PORT);
	if(connect(clientfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1)
	{
		std::cout << "connect server error." << std::endl;
		close(clientfd);
		return -1;
	}
	
	  //���ӳɹ��Ժ������ٽ� clientfd ���óɷ�����ģʽ��
    //�����ڴ���ʱ�����ã�������Ӱ�쵽 connect ��������Ϊ
    int oldSocketFlag = fcntl(clientfd, F_GETFL, 0);
    int newSocketFlag = oldSocketFlag | O_NONBLOCK;
    if(fcntl(clientfd, F_SETFL, newSocketFlag) == -1)
    {
   		std::cout << "set scoket to nonblock error." << std::endl;
   		close(clientfd);
   		return -1; 	
   	}
   	
   	// 3.������������������ݣ����߳����˳�
   	int count = 0;
   	while(true)
   	{
        int ret = send(clientfd, SEND_DATA, strlen(SEND_DATA), 0);
        if (ret == -1) 
        {
            //������ģʽ��send��������TCP����̫С������ȥ���ݣ���������EWOULDBLOCK
            if (errno == EWOULDBLOCK)
            {
                std::cout << "send data error as TCP Window size is too small." << std::endl;
                continue;
            } 
            else if (errno == EINTR)
            {
                //������ź��жϣ����Ǽ�������
                std::cout << "sending data interrupted by signal." << std::endl;
                continue;
            } 
            else 
            {
                std::cout << "send data error." << std::endl;
                break;
            }
        }
        if (ret == 0)
        {
            //�Զ˹ر������ӣ�����Ҳ�ر�
            std::cout << "send data error." << std::endl;
            close(clientfd);
            break;
        } 
        else
        {
            count ++;
            std::cout << "send data successfully, count = " << count << std::endl;
        }
   	}
   	
   	
    //5. �ر�socket
    close(clientfd);
	
	
	return 0;
}