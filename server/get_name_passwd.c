#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "mydict.h"

void get_name_passwd(int connfd, char *name, char *passwd)
{
	usleep(100*1000);
	int ret;
	char buf_name[24];
	bzero(buf_name, sizeof(buf_name));
	ret = send(connfd, "input your name: ", 24, 0);
	err_monitor(connfd, ret, "get_name_passwd send input name");
	ret = recv(connfd, name, 24, 0);
	err_monitor(connfd, ret, "get_name_passwd recv input name");

	usleep(100*1000);
	bzero(buf_name, sizeof(buf_name));
	sprintf(buf_name, "%s\n", name);
	ret = send(connfd, buf_name, 24, 0);
	err_monitor(connfd, ret, "get_name_passwd send input name");
	
	usleep(100*1000);
	ret = send(connfd, "input your passwd: \n", 24, 0);
	err_monitor(connfd, ret, "get_name_passwd send input passwd");
	ret = recv(connfd, passwd, 24, 0);
	err_monitor(connfd, ret, "get_name_passwd recv input passwd");
}
