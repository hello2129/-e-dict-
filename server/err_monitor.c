#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

void err_monitor(int connfd, int ret, char *errmsg) //错误监测
{
	if(0 >= ret){ //0--客户端主动退出
		//perror("");
		fprintf(stderr, "%s:%s\n", errmsg, strerror(errno));
		close(connfd);
		exit(-1);
	}
}
