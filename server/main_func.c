//程序功能主页面

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include "mydict.h"

int main_func(int connfd)
{
	int ret;
	char cli_opt[64]; //用户请求
	char menu_buf[64] = {"S--search H--history Q--quit h--help\n"};
	char menu_line[64] = {"************ Main Page *************\n"};
	char tit[128];
	bzero(tit, 128);
	sprintf(tit, "%s", menu_line);
	strcat(tit, menu_buf);
	while(1){
loop:
		usleep(100*1000); //设置延时，避免粘包
		
		ret = write(connfd, tit, strlen(tit));
		err_monitor(connfd, ret, "main_func send menu_buf");	

		ret = recv(connfd, &cli_opt, sizeof(cli_opt), 0);
		err_monitor(connfd, ret, "main_func recv cli_opt");
		//printf("cli_opt--%s\n", cli_opt);
		if(strlen(cli_opt) == 0)
			goto loop;
		switch((int)cli_opt[0]){
			case 'S': { //搜索
						  //printf("cli_opt--%s\n", cli_opt);
						  search(connfd, cli_opt+1);
						  break;
					  }
			case 'H': { //历史记录
						  history(connfd);
						  break;
					  }
			case 'Q': { //退出
						  ret = send(connfd, "quit\n", 8, 0);
					      err_monitor(connfd, ret, "main_func_send Q");	
						  
						  close(connfd);
						  exit(0); //退出
					  }
			case 'h': {
						  char help_buf[128] = {"S--搜索单词，H--查看历史记录，Q--退出登录，h--查看帮助\n"};
						  ret = write(connfd, help_buf, strlen(help_buf));
						  break;
					  }
			default: {
						 //printf("main_func error\n");
						 write(connfd, "无效的输入！\n", 16);
						 break;
					 }
		}
	}

	return 0;
}
