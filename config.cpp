#include "config.h"
#include <iostream>
using namespace std;
config::config(){
    loadConfigFile();
}
bool config::loadConfigFile(){
  FILE *pf = fopen("mysql.cnf","r");
    if(pf == nullptr){
        LOG("加载文件失败");
        return false;
    }
    while(!feof(pf)){
        char line[1024] = {0};
        fgets(line, 1024, pf);
        string str = line;
        int idx = str.find("=",0);
        if(idx == -1) continue;
        int endidx = str.find("\n",idx);
        string key = str.substr(0,idx);
        string value = str.substr(idx+1,endidx-idx-1);
        if(key == "ip"){
            ip_ = value;
        }else if(key == "port"){
            port_ = atoi(value.c_str());
        }else if(key == "username"){
            username_ = value;
        }else if(key == "password"){
            password_ = value;
        }else if(key == "dbname"){
            dbname_ = value;
        }else if(key == "initSize"){
            initSize_ = atoi(value.c_str());
        }else if(key == "maxSize"){
            maxSize_ = atoi(value.c_str());
        }else if(key == "maxIdleTime"){
            maxIdleTime_ = atoi(value.c_str());
        }else if(key == "connectionTimeOut"){
            connectionTimeout_ = atoi(value.c_str());
        }
    }
    return true;
}