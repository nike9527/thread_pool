#include <iostream>
#include <stdio.h>
#include "Connection.h"
#include "CommonConnectionPool.h"
int main(){
#if 0
    clock_t begin = clock();
    for(int i = 0; i<5000; i++){
        Connection conn;
        char sql[1024] = {0};
        sprintf(sql,"insert into user1(name,age,sex) values('%s',%d,'%s')","zhang san",20, "male");
        conn.connect("127.0.0.1",3306,"zz","342511%Abc","chat");
        conn.update(sql);
    }
    clock_t end = clock();
    cout << end-begin<< "ms" << endl;
#endif
#if 0
    clock_t begin = clock();
    CommonConnectionPool *cp  = CommonConnectionPool::getCommonConnectionPool(); 
    for(int i = 0; i<1000; i++){
         shared_ptr<Connection> conn = cp->getConnection(); 
        char sql[1024] = {0};
        sprintf(sql,"insert into user1(name,age,sex) values('%s',%d,'%s')","zhang san",20, "male");
        conn->update(sql);
    }
    clock_t end = clock();
    cout << end-begin<< "ms" << endl;
#endif
#if 0
    Connection conn;
    char sql[1024] = {0};
    sprintf(sql,"insert into user1(name,age,sex) values('%s',%d,'%s')","zhang san",20, "male");
    conn.connect("127.0.0.1",3306,"zz","342511%Abc","chat");
    conn.update(sql);
    //==================================================
    clock_t begin = clock();
    CommonConnectionPool *cp = CommonConnectionPool::getCommonConnectionPool();
    for(int i = 0; i<1000; i++){
        shared_ptr<Connection> conn = cp->getConnection(); 
        char sql[1024] = {0};
        sprintf(sql,"insert into user(name,age,sex) values('%s',%d,'%s')","zhang san",20, "male");
        conn->update(sql);
    }
    clock_t end = clock();
    cout << end-begin << "ms" << endl;
#endif
    clock_t begin = clock();
    thread t1([](){
        CommonConnectionPool *cp = CommonConnectionPool::getCommonConnectionPool();
        for(int i = 0; i<2500; i++){
            shared_ptr<Connection> conn = cp->getConnection(); 
            char sql[1024] = {0};
            sprintf(sql,"insert into user1(name,age,sex) values('%s',%d,'%s')","zhang san",20, "male");
            conn->update(sql);
        }
    });
    thread t2([](){
        CommonConnectionPool *cp = CommonConnectionPool::getCommonConnectionPool();
        for(int i = 0; i<2500; i++){
            shared_ptr<Connection> conn = cp->getConnection(); 
            char sql[1024] = {0};
            sprintf(sql,"insert into user1(name,age,sex) values('%s',%d,'%s')","zhang san",20, "male");
            conn->update(sql);
        }
    });
    thread t3([](){
        CommonConnectionPool *cp = CommonConnectionPool::getCommonConnectionPool();
        for(int i = 0; i<2500; i++){
            shared_ptr<Connection> conn = cp->getConnection(); 
            char sql[1024] = {0};
            sprintf(sql,"insert into user1(name,age,sex) values('%s',%d,'%s')","zhang san",20, "male");
            conn->update(sql);
        }
    });
    thread t4([](){
        CommonConnectionPool *cp = CommonConnectionPool::getCommonConnectionPool();
        for(int i = 0; i<2500; i++){
            shared_ptr<Connection> conn = cp->getConnection(); 
            char sql[1024] = {0};
            sprintf(sql,"insert into user1(name,age,sex) values('%s',%d,'%s')","zhang san",20, "male");
            conn->update(sql);
        }
    });
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    clock_t end = clock();
    cout << end-begin<< "ms" << endl;
}