#include "public.h"
#include "CommonConnectionPool.h"
#include <iostream>
CommonConnectionPool::CommonConnectionPool(){
    // if(!loadConfigFile()){
    //     return;
    // }
    /**
     * 创建初始连接
     */
    for(int i =0; i<conf.getInitSize(); ++i){
        Connection *p = new Connection();
        // p->connect(ip_,port_,username_,password_, dbname_);
        p->connect(conf);
        p->refreshAliveTime();
        connectionQue_.push(p);
        connectionCnt_++;
    }
    //启动新线程，作为连接的生产者
    thread produce(std::bind(&CommonConnectionPool::produceConnectionTask,this));
    produce.detach();//设置成分离线程
    //启动也给定时线程，扫描超过maxIdleTime多余空闲连接， 多余连接回收
    thread scanner(std::bind(&CommonConnectionPool::scannerConnectionTask,this));
    scanner.detach();//设置成分离线程
}

CommonConnectionPool::~CommonConnectionPool(){

}
/**
 * 线程安全的懒汉单例函数接口
 */
CommonConnectionPool* CommonConnectionPool::getCommonConnectionPool(){
    static CommonConnectionPool pool;
    return &pool;
}
//加载数据库配置
// bool CommonConnectionPool::loadConfigFile(){
//     FILE *pf = fopen("mysql.cnf","r");
//     if(pf == nullptr){
//         LOG("加载文件失败");
//         return false;
//     }
//     while(!feof(pf)){
//         char line[1024] = {0};
//         fgets(line, 1024, pf);
//         string str = line;
//         int idx = str.find("=",0);
//         if(idx == -1) continue;
//         int endidx = str.find("\n",idx);
//         string key = str.substr(0,idx);
//         string value = str.substr(idx+1,endidx-idx-1);
//         if(key == "ip"){
//             ip_ = value;
//         }else if(key == "port"){
//             port_ = atoi(value.c_str());
//         }else if(key == "username"){
//             username_ = value;
//         }else if(key == "password"){
//             password_ = value;
//         }else if(key == "dbname"){
//             dbname_ = value;
//         }else if(key == "initSize"){
//             initSize_ = atoi(value.c_str());
//         }else if(key == "maxSize"){
//             maxSize_ = atoi(value.c_str());
//         }else if(key == "maxIdleTime"){
//             maxIdleTime_ = atoi(value.c_str());
//         }else if(key == "connectionTimeOut"){
//             connectionTimeout_ = atoi(value.c_str());
//         }
//     }
//     return true;
// }

void CommonConnectionPool::produceConnectionTask(){

    for(;;){
        unique_lock<mutex> lock(queueMutex_);
        while(!connectionQue_.empty()){
            cv.wait(lock);//队列部位， 此时生产线程进入等待状态
        }
        //连接数量没有达到上线， 继续创建新连接
        if(connectionCnt_ <conf.getMaxSize()){
            Connection *p = new Connection();
            // p->connect(ip_,port_,username_,password_, dbname_);
            p->connect(conf);
            p->refreshAliveTime();
            connectionQue_.push(p);
            connectionCnt_++;
        }
        //唤醒消费线程，可以连接
        cv.notify_all();
    }
}

//获取一个mysql连接
shared_ptr<Connection> CommonConnectionPool::getConnection(){
    unique_lock<mutex> lock(queueMutex_);
    while(connectionQue_.empty()){
        //超时
        if(cv_status::timeout == cv.wait_for(lock,chrono::milliseconds(conf.getConnectionTimeout()))){
            if(connectionQue_.empty()){
                LOG("获取连接超时");
                return nullptr;
            }
        }
    }
    /**
     * shared_ptr析构，会吧connection资源delete掉，会调用connetion的析构函数
     * 需要自定义shared_ptr的资源释放方式， 吧connection直接归还到queue中
     */
    shared_ptr<Connection> sp(connectionQue_.front(),[&](Connection *con){
        //这里是服务i去应用线程调用，要考虑线程队列安全
        unique_lock<mutex> lock(queueMutex_);
        con->refreshAliveTime();
        connectionQue_.push(con);
    });
    connectionQue_.pop();
    //通知其他生产消除可以消费
    cv.notify_all();
    return sp;
}
//启动也给定时线程，扫描超过maxIdleTime多余空闲连接， 多余连接回收
void CommonConnectionPool::scannerConnectionTask(){
    for(;;){
        this_thread::sleep_for(chrono::seconds(conf.getMaxIdleTime()));
        //扫描整个队列，释放多余的连接
        unique_lock<mutex> lock(queueMutex_);
        while(connectionCnt_ > conf.getInitSize()){
            Connection *p = connectionQue_.front();
            if(p->getAliveTime() >=  (conf.getMaxIdleTime() * 1000)){
                connectionQue_.pop();
                delete p;//释放连接
            }else{
                break;//队头没有超过maxIdleTime_时间，其他连接也没有超过
            }
        }
    }
}