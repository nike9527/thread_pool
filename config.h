#pragma once
#include<string>
#include "public.h"
using namespace std;
class config
{
private:
    /* data */
public:
     //加载配置文件
    config();
    bool loadConfigFile();
    string getIp() const {return ip_;} 
    unsigned short getPort() const {return port_;} 
    string getUsername() const {return username_;} 
    string getPassword() const {return password_;} 
    string getDbname() const {return dbname_;} 
    int getInitSize() const {return initSize_;} 
    int getMaxSize() const {return maxSize_;} 
    int getMaxIdleTime() const {return maxIdleTime_;} 
    int getConnectionTimeout() const {return connectionTimeout_;} 
private:
    string ip_; //mysql IP地址
    unsigned short port_; //mysql 的端口好
    string username_; // mysql d登录用户名
    string password_; // mysql 登录密码
    string dbname_;//数据库名字
    int initSize_; // 连接池的初始连接量
    int maxSize_;// 连接池的最大连接量
    int maxIdleTime_;// 连接池的最大空闲时间
    int connectionTimeout_;// 连接池的获取超时时间
};