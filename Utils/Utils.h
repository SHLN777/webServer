#ifndef UTILS
#define UTILS

#include "../Timer/timer.h"

class Utils
{
public:
    Utils() {}
    ~Utils() {}

    void init(int timeslot);

    int setnonblocking(int fd); //非阻塞

    
    void addfd(int epollfd, int fd, bool one_shot, int TRIGMode); //将事件注册到事件表， ET/LT

    static void sig_handler(int sig);  //信号处理

    void addsig(int sig, void(handler)(int), bool restart = true); //信号捕捉

    
    void timer_handler();    //处理定时任务

    void show_error(int connfd, const char *info);  //发送错误信息

public:
    static int *u_pipefd;
    sort_timer_lst m_timer_lst;
    static int u_epollfd;
    int m_TIMESLOT;
};

void cb_func(client_data *user_data);  //定时器的处理函数




#endif