#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <sys/socket.h>
#include "mydict.h"

int callback(void * para, int f_num, char **f_value, char **f_name)
{
	(*(int *)para)++;
	return 0;
}

int login(int connfd, char *name, char *passwd) //响应客户端登录请求
{
	int ret, count=0; //count：为1说明用户存在且密码正确，登录成功
	sqlite3 *db;
	char sql[256], *errmsg; //sql长度过小可能会导致stack smashing detected错误
	bzero(sql, sizeof(sql));

	ret = sqlite3_open("user.db", &db); //打开数据库
	if(ret != SQLITE_OK){
		printf("open error : %s\n", sqlite3_errmsg(db));
		exit(-1);
	}
	sprintf(sql, "select * from user where name='%s'\
			and passwd='%s';", name, passwd);

	ret = sqlite3_exec(db, sql, callback, &count, &errmsg);//查询用户名和密码是否正确
	if(ret != SQLITE_OK){
		printf("sqlite3_exec querry error : %s\n", errmsg);
		exit(-1);
	}
	if(count == 1){ //登录成功,设置该用户状态为在线，并记录其connfd
		//strcpy(record_name, name); //记录登录的用户名
		sqlite3_close(db);
		return 0;
	}

	sqlite3_close(db);
	return -1; //登录失败
}
