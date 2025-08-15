#ifndef CHATSERVICE_H
#define CHATSERVICE_H
#include <unordered_map>
#include <functional>
#include <mutex>
#include <muduo/net/TcpConnection.h>
#include "offlinemessagemodel.hpp"

using namespace std;
using namespace muduo;
using namespace muduo::net;

#include "friendmodel.hpp"
#include <usermodel.hpp>
#include "json.hpp"
#include "groupmodel.hpp"
#include "redis.hpp"
using json = nlohmann::json;
//表示处理消息的事件回调方法类型
using MsgHandler = std::function<void(const TcpConnectionPtr &conn, json &js, Timestamp time)>;

// 聊天服务器业务类
class ChatService
{
public:
    // 获取单例对象的接口函数
    static ChatService* instance();
    // 处理登录业务
    void login(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 处理注册业务
    void reg(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //一对一聊天业务
    void oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //添加好友业务
    void addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //获得消息对应的处理器
    MsgHandler getHandler(int msgid);
    // 处理客户端异常退出
    void clientCloseException(const TcpConnectionPtr &conn);
    //服务器异常，业务重置方法
    void reset();
    // 创建群组业务
    void createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 加入群组业务
    void addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 群组聊天业务
    void groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //处理注销业务
    void loginout(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 从redis消息队列中获取订阅的消息
    void handleRedisSubscribeMessage(int, string);
    
private:
    ChatService();
    unordered_map<int, MsgHandler> _msgHandlerMap; // 存储消息id和对应的处理器
    unordered_map<int,TcpConnectionPtr> _userConnMap; // 存储用户id和连接的映射关系
    
    mutex _connMutex;//定义互斥锁，保证_userConnMap线程安全
    UserModel _userModel; // 用户数据操作类对象

    OfflineMsgModel _offlineMsg;
    FriendModel _friendModel;

    GroupModel _groupModel;
    // redis操作对象
    Redis _redis;
    
};

#endif