//chat_ser服务端

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/signal.h>
#include <arpa/inet.h>
#include <sys/types.h>          
#include <sys/wait.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include "mydict.h"

//搭建tcp服务器 （1.处理客户的连接  2.处理客户的请求） 
//tcp并发服务器：  子进程负责：客户的请求  父进程负责:连接请求

int client_count = 0; //记录在线连接数，

//处理用户请求
int do_client(int connfd) //处理客户的请求
{
	int ret;
	int flag = 0;//判断用户是否登录标记

	while(1){ //循环处理客户端请求，直到客户端退出
		switch(flag){ //根据用户请求，做出响应
			case 0: {
						ret = home_func(connfd);//客户端登录主界面, 记录用户名
						if(ret == 0)
							flag = 1; //记录用户登录成功,下一次进入程序主页面
					}break;
			case 1: {
						main_func(connfd); //主程序界面
					}break;
			default: printf("Client Request Error!\n"); break; //出错
		}
	}

	return 0;
}

//捕捉僵尸信号，处理僵尸进程
void signal_handler(int sig)
{
	printf("signal_handler,sig=%d\n", sig);
	//pid = wait(NULL);
	while(waitpid(-1, NULL, WNOHANG) > 0); //>0说明有僵尸进程，继续回收
	printf("pid %d is killed\n", getpid());
	client_count--; //在线人数减一
	return ;
}

//初始化socket连接
int tcp_init(int port, char *ip)
{
	int sockfd,ret;
	sockfd = socket(AF_INET, SOCK_STREAM  ,0); 

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);  

	//设置地址重用
	int on = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	ret = bind(sockfd,(struct sockaddr *)&addr,sizeof(addr));
	if(ret==-1){
		perror("bind");
		exit(-1);
	}

	listen(sockfd,32);  
	return sockfd;
}

int main(int argc, char * argv[])
{
	signal(SIGCHLD, signal_handler); //SIGCHLD:产生僵尸进程就会产生这个信号

	int sockfd, connfd;

	char ip_buf[24] = {"0"};
	if(argc>1)
		sockfd = tcp_init(6660+atoi(argv[1]), ip_buf);
	else
		sockfd = tcp_init(6660, ip_buf);

	while(1){
		printf("wait...\n");
		connfd = accept(sockfd,NULL,NULL);  
		if(connfd == -1){
			if(errno == EINTR){ //被singal信号干扰
				continue;
			}
		}
		printf("connect a client--%d\n", ++client_count);

		pid_t pid = fork(); 
		if(pid==0){
			signal(SIGINT, signal_handler); //接收到CTRL+C信号
			//printf("connfd--%d\n", connfd);
			close(sockfd);
			//子进程负责：客户请求
			//fwrite(&connfd, 1, sizeof(connfd), fp); //保存用户地址
			do_client(connfd);
		}
		else{
			close(connfd);
			//父进程负责：客户连接
			continue;
		}
	}
	close(sockfd);

	return 0;
}

