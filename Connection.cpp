#include "public.h"
#include "Connection.h"
#include <iostream>
using namespace std;
//初始化数据库连接
Connection::Connection()
{
    conn_ = mysql_init(nullptr);
}
//释放数据库连接
Connection::~Connection(){
    if(conn_ != nullptr)
        mysql_close(conn_);
}
//连接数据库
bool Connection::connect(string ip, unsigned short port, string user, string password, string dbname){
    MYSQL *p = mysql_real_connect(conn_,ip.c_str(), user.c_str(), 
                                    password.c_str(), dbname.c_str(),port, nullptr, 0
                                );
    // LOG(mysql_get_client_info());
    return p != nullptr;
}
//连接数据库
bool Connection::connect(const config &conf){
    MYSQL *p = mysql_real_connect(conn_, conf.getIp().c_str(),
                                    conf.getUsername().c_str(),
                                    conf.getPassword().c_str(),
                                    conf.getDbname().c_str(),
                                    conf.getPort(),nullptr,0);
    // LOG(mysql_get_client_info());
    return p != nullptr;
}
//更新操作
bool Connection::update(string sql){
    if(mysql_query(conn_, sql.c_str())){
        LOG("更新失败:"+sql+","+ mysql_error(conn_));
        return false;
    }
    return true;
}
//查询操作
MYSQL_RES* Connection::query(string sql){
   if(mysql_query(conn_,sql.c_str())){
        LOG("查询失败:"+sql);
        return nullptr;
    }
    return mysql_use_result(conn_);
}