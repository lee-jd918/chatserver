#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <iostream>
#include <functional>
using namespace muduo;
using namespace std;
using namespace muduo::net;
using namespace placeholders;

class ChatServer
{
public:
    //初始化聊天服务器对象
    ChatServer(EventLoop *loop,               // 事件循环
               const InetAddress &listenAddr, // IP+Port
               const string &nameArg);
    void start();

private:
    void onConnection(const TcpConnectionPtr &);
    
    void onMessage(const TcpConnectionPtr&,//连接
                   Buffer*,          // 缓冲区
                   Timestamp);

    TcpServer _server;//组合的muduo库，实现服务器功能的类对象
    EventLoop *_loop; // epoll 指向事件循环的指针
};

#endif