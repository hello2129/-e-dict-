//mychat 客服端

#include<stdio.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <time.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <errno.h>

//搭建tcp客户端 （1.处理客户的连接  2.处理客户的请求） 
//tcp客服端：  子进程负责：客户的请求  父进程负责:连接请求

int login(int sockfd, char flag)
{
	int ret;
	char name[24], passwd[24], opt[64];
	bzero(name,24);
	bzero(passwd,24);
	bzero(opt,64);
	printf("input your name: ");
	scanf("%s%*c", name);
	printf("input your passwd: ");
	scanf("%s%*c", passwd);

	sprintf(opt, "%c%s#%s", flag, name, passwd);

	ret = write(sockfd, opt, strlen(opt));
	if(ret == -1){
		perror("login write failed");
		exit(-1);
	}
	return 0;
}

void get_word(int sockfd)
{
	int ret;
	char word[24];
	char str[24];
	bzero(word,24);
	bzero(str,24);
	printf("input a word: ");
	scanf("%s%*c", word);

	sprintf(str, "S%s", word);
	ret = write(sockfd, str, sizeof(str));
	if(ret == -1){
		perror("login write failed");
		exit(-1);
	}
}

int tcp_init(int port, char *ip) //初始化tcp连接,用户登录以后连接数据库
{
	int sockfd, ret;
	sockfd = socket(AF_INET, SOCK_STREAM  ,0); 
	if(-1 == sockfd){
		perror("socket");	
		exit(1);
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);  

	ret = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));
	if(-1 == ret){
		perror("connect");
		exit(1);
	}
	printf("connect server success\n");

	return sockfd;
}

int main(int argc, char * argv[])
{
	int sockfd, port;
	if(argc >= 2)
		port = 6660+atoi(argv[1]);
	else
		port = 6660;
	char ip_buf[24] = {"0"};
	sockfd = tcp_init(port, ip_buf);

	int ret;
	char recv_buf[512];
	char send_buf[512];
	while(1){
		usleep(100*1000);
		bzero(recv_buf, sizeof(recv_buf));
		bzero(send_buf, sizeof(send_buf));
		ret = read(sockfd, recv_buf, sizeof(recv_buf));
		if(ret == -1){
			perror("recv");
			exit(-1);
		}
		printf("%s\n", recv_buf);
		if(recv_buf[2] == '*'){
			printf("input: ");
			scanf("%s%*c", send_buf);
		}

		if(0==strcmp(send_buf, "L") || 0==strcmp(send_buf, "R")){ //登录和注册
			login(sockfd, *send_buf);
		}
		else if(0==strcmp(send_buf, "S")){ //搜索
			get_word(sockfd);	
			while(1){
				bzero(recv_buf, sizeof(recv_buf));
				ret = read(sockfd, recv_buf, sizeof(recv_buf));
				if(recv_buf[1] == '*');
				else{
					printf("%s", recv_buf);
					break;
				}
			}
		}
		else if(0==strcmp(send_buf, "Q")){
			printf("quit\n");
			exit(0);
		}
		else if(0==strcmp(send_buf, "H")){ //历史记录,登录成功后
			ret = write(sockfd, send_buf, strlen(send_buf));
			while(1){
				bzero(recv_buf, sizeof(recv_buf));
				ret = read(sockfd, recv_buf, sizeof(recv_buf));
				//printf("ret--%d\n", ret);
				if(recv_buf[2] == '*');
				else if(ret==128)
					break;
				else
					printf("%s\n", recv_buf);
				if(strstr(recv_buf, "end"))
					break;
			}
		}
		else{
			ret = write(sockfd, send_buf, strlen(send_buf));
			if(ret == -1){
				perror("recv");
				exit(-1);
			}
			if(0 == strcmp(send_buf, "quit")){
				exit(0);	
			}
		}
	}

	return 0;
}
