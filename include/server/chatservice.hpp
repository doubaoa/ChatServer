#ifndef CHATSERVICE_H
#define CHATSERVICE_H
#include <muduo/net/TcpConnection.h>
#include <unordered_map>
#include <functional>
#include <mutex>
#include "json.hpp"
#include "UserModel.hpp"
#include "friendmodel.hpp"
#include "groupmodel.hpp"
#include "redis.hpp"
#include "offlinemessagemodel.hpp"
using json = nlohmann::json;
using namespace muduo;
using namespace muduo::net;
using namespace std;
using MsgHandler = function<void(const TcpConnectionPtr &conn, json &js, Timestamp)>;

class ChatService
{
public:
    static ChatService *instance();
    void login(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void loginout(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void reg(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void clientCloseException(const TcpConnectionPtr &conn);
    void handleRedisSubscribeMessage(int userid, string msg);
    void reset();
    MsgHandler getHandler(int msgid);

private:
    ChatService();
    //<消息类型id:回调函数>
    unordered_map<int, MsgHandler> _msgHandlerMap;
    //用户表操作
    UserModel _userModel;
    //朋友关系表操作
    FriendModel _friendModel;
    //离线消息表操作
    OfflineMsgModel _OfflineMsgModel;
    //群组关系操作
    GroupModel _groupModel;
    //存储当前用户id和对应的tcp连接
    unordered_map<int, TcpConnectionPtr> _userConnMap;
    //单例模式 _userConnMap是共享资源 需要加锁
    mutex connMutex;
    //redis
    Redis _redis;
};

#endif