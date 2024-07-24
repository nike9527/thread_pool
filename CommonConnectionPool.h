#include <string>
#include <queue>
#include <mutex>
#include <atomic>
#include <thread>
#include <memory>
#include <functional>
#include <map>
#include <condition_variable>
#include "Connection.h"
#include "config.h"
using namespace std;
/**
 * 实现连接池
 */
class CommonConnectionPool
{
public:
    //获取连接池对象实例
    static CommonConnectionPool* getCommonConnectionPool();
    //获取一个mysql连接
    shared_ptr<Connection> getConnection();
private:
  
    CommonConnectionPool(/* args */);
    ~CommonConnectionPool();
    //加载配置文件
    // bool loadConfigFile();
    void produceConnectionTask();
    //启动也给定时线程，扫描超过maxIdleTime多余空闲连接， 多余连接回收
    void scannerConnectionTask();
    // string ip_; //mysql IP地址
    // unsigned short port_; //mysql 的端口好
    // string username_; // mysql d登录用户名
    // string password_; // mysql 登录密码
    // string dbname_;//数据库名字
    // int initSize_; // 连接池的初始连接量
    // int maxSize_;// 连接池的最大连接量
    // int maxIdleTime_;// 连接池的最大空闲时间
    // int connectionTimeout_;// 连接池的获取超时时间

    const config conf; //mysql相关配置
    queue<Connection*> connectionQue_; //存储mysql连接队列
    mutex queueMutex_;//线程安全互斥锁

    atomic_int connectionCnt_;// 记录创建的connection连接总数量

    condition_variable cv;//连接生产线程和消费线程通信

};


