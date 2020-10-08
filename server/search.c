
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sqlite3.h>
#include "mydict.h"

int connfd2;
sqlite3 *db;

void insert_history(char *buf)
{
	int ret;
	char sql[256], *errmsg; //sql长度过小可能会导致stack smashing detected错误
	bzero(sql, sizeof(sql));

	sprintf(sql, "insert into history values(NULL, '%s');", buf);

	//查询单词
	ret = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
	if(ret != SQLITE_OK){
		printf("sqlite3_exec querry error : %s\n", errmsg);
		exit(-1);
	}
}

int callback_search(void *para, int f_num, char **f_value, char **f_name)
{
	(*(int*)para)++;
	//printf("%s--%s\n", f_value[0], f_value[1]);
	int ret;
	char buf[256];
	bzero(buf, sizeof(buf));
	sprintf(buf, "%s", f_value[1]);
	insert_history(buf);
	usleep(100*1000);
	ret = write(connfd2, buf, strlen(buf));
	err_monitor(connfd2, ret, "callback_search send text");
	return 0;	
}

int search(int connfd, char *word)
{
	connfd2 = connfd;
	int ret;
	int count = 0;

	char sql[256], *errmsg; //sql长度过小可能会导致stack smashing detected错误
	bzero(sql, sizeof(sql));

	ret = sqlite3_open("E-dict.db", &db); //打开数据库
	if(ret != SQLITE_OK){
		printf("open error : %s\n", sqlite3_errmsg(db));
		exit(-1);
	}
	sprintf(sql, "select * from dict where word='%s';", word);

	//查询单词
	ret = sqlite3_exec(db, sql, callback_search, &count, &errmsg);
	if(ret != SQLITE_OK){
		printf("sqlite3_exec querry error : %s\n", errmsg);
		exit(-1);
	}
	sqlite3_close(db);
	char buf[128];
	bzero(buf, sizeof(buf));
	sprintf(buf, "word not exist!\n");
	if(count == 0){
		write(connfd, buf, strlen(buf));	
	}
}
