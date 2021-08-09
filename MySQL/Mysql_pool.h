#ifndef _MYSQL_POOL_
#define _MYSQL_POOL_

#include <stdio.h>
#include <list>
#include <string>
#include <error.h>
#include <mysql/mysql.h>
#include "../lock/locker.h"

using namespace std;

class mysql_pool
{
public:
    static mysql_pool *getInstance();   //单例
    void init(string url, string User, string PassWord, string DatabaseName, int port, int Maxconn);  //初始化连接池

    MYSQL* GetConnection();   //获得一个连接
    bool ReleaseConnection(MYSQL* conn);  //释放连接
    int GetFreeConn();    //获得空闲连接数
    void DestoryConn();   //摧毁所有连接

private:
    msyql_pool();
    ~mysql_pool();

    int m_MaxConn;  //最大连接数
    int m_CurConn;  //当前连接的数
    int m_FreeConn; //空闲的连接数
    locker lock;   //互斥的使用连接池
    list<MYSQL *> ConnPool_list;   //连接链表
    sem reserve;

    static mysql_pool* m_istance;   //单例

};

#endif