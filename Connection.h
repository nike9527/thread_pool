/**
 * 实现mysql数据操作
 */
#pragma once
#include "config.h"
#include <string>
#include <time.h>
#include <mysql/mysql.h>
using namespace std;

class Connection
{
public:
    //初始化数据库连接
    Connection();
    //释放数据库连接
    ~Connection();
    //连接数据库
    bool connect(string ip, unsigned short port, string user, string password, string dbname);
    //连接数据库
    bool connect(const config &conf);
    //更新操作
    bool update(string sql);
    //更新空闲时间
    void refreshAliveTime()
    {
        alivetime_ = clock();
    }
    //获取空闲时间
    clock_t getAliveTime(){
        return clock() - alivetime_;
    }
    //查询操作
    MYSQL_RES* query(string sql);
private:
    string ip_; //主机ip
    unsigned short port_;//数据库端口
    string user_;//数据库用户名
    string password_;//数据库密码
    string dbname_;//数据库
    MYSQL *conn_;//MySQL连接
    clock_t alivetime_;//空闲状态的起始存活时间
};


