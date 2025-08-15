#include "chatservice.hpp"
#include "chatserver.hpp"
#include "json.hpp"
#include <string>

using json=nlohmann::json;

ChatServer::ChatServer(EventLoop *loop,               // 事件循环
                       const InetAddress &listenAddr, // IP+Port
                       const string &nameArg)
    : _server(loop, listenAddr, nameArg),
      _loop(loop) // 服务器名字
{
    // 给服务器注册用户连接的创建和断开回调
        _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));
        // 给服务器注册用户读写事件回调
        _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));
        // 设置服务器的线程数      1个I/O线程和3个worker线程
        _server.setThreadNum(4); // 4个线程,主reactor负责新用户连接，子reactor负责连接用户的读写事件处理

}

void ChatServer::start()
{
    _server.start(); // 启动服务器
}
//上报链接相关信息的回调函数
void ChatServer::onConnection(const TcpConnectionPtr& conn)
{
    //客户端断开链接
    if(!conn->connected())
    {
        ChatService::instance()->clientCloseException(conn);//客户端异常关闭
        conn->shutdown();
    }
}

void ChatServer::onMessage(const TcpConnectionPtr& conn, //连接
                          Buffer*  buffer,          // 缓冲区
                          Timestamp time)         //接收到数据的时间信息
{
    string buf=buffer->retrieveAllAsString(); //获取缓冲区中的数据
    json js = json::parse(buf);//解析json数据
    //完全解耦网络模块代码和业务模块代码
    //通过js["msgid"]->业务handler
    auto msgHandler = ChatService::instance()->getHandler(js["msgid"].get<int>());
    msgHandler(conn,js,time);//回调消息绑定好的事件处理器，来执行相应的业务处理
}