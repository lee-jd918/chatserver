#include <chatserver.hpp>
#include <chatservice.hpp>
#include <iostream>
#include <signal.h>

using namespace std;
//处理服务器ctrl+c结束后，重置user的状态信息
void resetHandler(int)
{
    ChatService::instance()->reset();
    exit(0);
}

int main(int argc,char **argv)
{
    if (argc < 3)
    {
        cerr << "command invalid! example: ./ChatServer 127.0.0.1 6000" << endl;
        exit(-1);
    }
    // 解析通过命令行参数传递的ip和port
    char *ip = argv[1];
    uint16_t port = atoi(argv[2]);

    signal(SIGINT,resetHandler);
    EventLoop loop;//epoll
    InetAddress addr(ip,port);
    ChatServer server(&loop, addr, "ChatServer");

    server.start(); //开启服务器，listenfd通过epoll_ctl添加到epoll上
    loop.loop(); //epoll_wait以阻塞方式等待新用户连接，已连接用户的读写事件等  事件发生回调onConnection和onMessage方法
    return 0;
}