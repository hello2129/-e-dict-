
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <sys/socket.h>
#include "mydict.h"

int callback_regist(void * para, int f_num, char **f_value, char **f_name)
{
	(*(int *)para)++;
	return 0;
}

int regist(int connfd, char *name, char *passwd) //响应客户端登录请求
{
	int ret, count=0; //count：为1说明用户存在,注册失败
	
	sqlite3 *db;
	char sql[256], *errmsg; //sql长度过小可能会导致stack smashing detected错误
	bzero(sql, sizeof(sql));

	ret = sqlite3_open("user.db", &db); //打开数据库
	if(ret != SQLITE_OK){
		printf("open error : %s\n", sqlite3_errmsg(db));
		exit(-1);
	}

	sprintf(sql, "select * from user where name='%s';", name);

	ret = sqlite3_exec(db, sql, callback_regist, &count, &errmsg);//查询用户是否存在
	if(ret != SQLITE_OK){
		printf("sqlite3_exec querry error : %s\n", errmsg);
		exit(-1);
	}
	if(count == 0){ //用户不存在，将用户插入user表
		bzero(sql, sizeof(sql));
		sprintf(sql, "insert into user values(NULL, '%s', '%s');",\
				name, passwd);
		ret = sqlite3_exec(db, sql, NULL, NULL, &errmsg); 
		if(ret != SQLITE_OK){
			printf("regist insert error : %s\n", errmsg);
			exit(-1);
		}
		sqlite3_close(db);
		return 0;
	}

	sqlite3_close(db);
	return -1; //注册失败
}
