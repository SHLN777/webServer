#include <string.h>
#include "Mysql_pool.h"

mysql_pool::msyql_pool()
{
    m_FreeConn=0;
    m_CurConn=0;
}

mysql_pool mysql_pool::getInstance()   //单例
{
    if(!m_istance)
    {
        msyql_pool* conn = new mysql_pool;
    }

    return m_istance;
}


void mysql_pool::init(string url, string User, string PassWord, string DatabaseName, int port, int Maxconn)  //初始化连接
{
    for(int i=0; i<Maxconn; i++)
    {
        MYSQL* con = NULL;
        con = mysql_init(con);
        if(con==NULL)
        {
            exit(1);
        }

        con = mysql_real_connect(con, url.c_str(), User.c_str(), PassWord.c_str(), DatabaseName.c_str(), port, NULL, 0);  //连接数据库
        if(con==NULL)
        {
            exit(1);
        }

        ConnPool_list.push_back(con);  //放入链表中
        ++m_FreeConn;

    }

    reserve = sem(m_FreeConn);  //信号量
    m_MaxConn = m_FreeConn;
}

MYSQL* mysql_pool::GetConnection()   //获得一个连接
{
    MYSQL* conn = NULL;
    if(0==ConnPool_list.size())
    {
        return NULL;
    }

    reserve.wait();
    lock.lock();

    conn = ConnPool_list.front();
    ConnPool_list.pop_front();

    --m_FreeConn;
    ++m_CurConn;

    lock.unlock();

    return conn;
}

/*
bool mysql_pool::ReleaseConnection(MYSQL *conn)   //释放一个连接
{
    if(NULL==conn)
    {
        return false;
    }


}
*/
void mysql_pool::DestoryConn()  //摧毁所有连接
{
    lock.lock();
    if(ConnPool_list.size()>0)
    {
        for(auto it=ConnPool_list.begin(); it!=ConnPool_list.end(); it++)
        {
            MYSQL* conn = *it;
            mysql_close(conn);
        }
        m_FreeConn=0;
        m_CurConn=0;
    }
    lock.unlock();
}

int mysql_pool::GetFreeConn()
{
    return m_FreeConn;
}

mysql_pool::~mysql_pool()
{
    DestoryConn();
}