

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sqlite3.h>
#include "mydict.h"

int connfd2;

int callback_history(void *para, int f_num, char **f_value, char **f_name)
{
	int ret;
	char buf[256];
	bzero(buf, sizeof(buf));
	if(f_num == 0){
		sprintf(buf, "history not exist!\n");
		ret = write(connfd2, buf, strlen(buf));
		err_monitor(connfd2, ret, "callback_history send text");
	}
	else{
	//	printf("%s--", f_value[0]);
	//	printf("%s", f_value[1]);
		sprintf(buf, "%s--", f_value[0]);
		strcat(buf, f_value[1]);
		write(connfd2, buf, strlen(buf));
	}
	return 0;
}

void history(int connfd)
{
	connfd2 = connfd;
	int ret;
	sqlite3 *db;
	char sql[256], *errmsg; //sql长度过小可能会导致stack smashing detected错误
	bzero(sql, sizeof(sql));

	ret = sqlite3_open("E-dict.db", &db); //打开数据库
	if(ret != SQLITE_OK){
		printf("open error : %s\n", sqlite3_errmsg(db));
		exit(-1);
	}
	sprintf(sql, "select * from history;");

	//查询单词
	ret = sqlite3_exec(db, sql, callback_history, NULL, &errmsg);
	if(ret != SQLITE_OK){
		printf("sqlite3_exec querry error : %s\n", errmsg);
		exit(-1);
	}
	write(connfd, "end", 4);
}
