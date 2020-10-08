#ifndef _MYCHAT_H
#define _MYCHAT_H


//用户信息结构体
typedef struct userinfo {
	char username[16];
	char passwd[16];
	struct userinfo *next;
}u_t;

#define userinfo_t u_t

/****** home() ******
 * 功能：程序启动时响应的主页面
 * 参数：无
 * 返回值：返回程序是否正确指向
 *
 * */
int home_func(int); //登录主页

/****** regist() ******
 * 功能：用户注册
 * 参数：无
 * 返回值：0--注册成功，-1--注册失败
 *
 * */
int regist(int, char *, char *); //注册

int login(int, char *, char *); //登录

int quit(char *); //

int main_func(int); //搜索程序主界面

int search(int, char *); //查询单词

void history(int); //历史记录

int quit(); //退出

void err_monitor(int, int, const char []); // 错误检测

void get_name_passwd(int, char *, char *); //获取用户端输入的用户名和密码

#endif
