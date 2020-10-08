//客户端登录时处理请求

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include "mydict.h"

int home_func(int connfd)
{
	int ret;
	char cli_opt[64];
	char title[] = {"*********** Login Page *************\n"};
	char opt_title[] = {"L--login R--register Q--quit h--help\n"};
	char tit[128];
	bzero(tit, sizeof(tit));
	strcpy(tit, title);
	strcat(tit, opt_title);
	while(1){
		usleep(100*1000);
loop:
		bzero(cli_opt, sizeof(cli_opt));
		ret = write(connfd, tit, strlen(tit));
		err_monitor(connfd, ret, "home_func_send header");
		
		usleep(100*1000);
		ret = read(connfd, &cli_opt, sizeof(cli_opt));
		err_monitor(connfd, ret, "home_func_recv cli_opt");
		if(strlen(cli_opt) == 0)
			goto loop;
		switch((int)cli_opt[0]){
			case 'L': { //登录
						  char *ch = strstr(cli_opt, "#");
						  *ch = 0;
						  ret = login(connfd, cli_opt+1, ch+1);
						  if(ret ==0 ){ //登录成功
							  ret = write(connfd, "Login successful!\n", 20);
							  err_monitor(connfd, ret, "home_func_send Login successful");
							  return 0;
						  }
						  else{ //登录失败
							  ret = write(connfd, "Login failed!\n", 16);
							  err_monitor(connfd, ret, "home_func_send Login failed");
						  }
					  }
					  break;
			case 'R': { //注册
						  char *ch = strstr(cli_opt, "#");
						  *ch = 0;
						  ret = regist(connfd, cli_opt+1, ch+1);
						  if(ret == 0){ //注册成功
							  ret = write(connfd, "Regist successful!\n", 16);
							  err_monitor(connfd, ret, "home_func send Regist successful");
						  }
						  else{ //注册失败
							  ret = write(connfd, "Regist failed!\n", 16);
							  err_monitor(connfd, ret, "home_func send Regist failed");
						  }
					  }
					  break;
			case 'Q': { //退出
						  ret = write(connfd, "quit\n", 8);
					      err_monitor(connfd, ret, "home_func_send Q");	

						  close(connfd);
						  exit(0); //退出
					  }
			case 'h': { //帮助
						  char help_buf[128] = {"L--登录，R--注册，Q--退出，h--查看帮助\n"};
						  ret = write(connfd, help_buf, strlen(help_buf));
					  	  break;
					  }
			default: {
						 write(connfd, "无效的输入！\n", 16);
						 break;
					 }
		}
	}

	return 0;
}
