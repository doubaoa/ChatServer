#include "chatserver.hpp"
#include "chatservice.hpp"
#include "json.hpp"
#include <functional>
using json = nlohmann::json;
using namespace std;
using namespace placeholders;

//tcpserver 初始化， 给server绑定建立连接的回调函数和消息的回调函数
ChatServer::ChatServer(EventLoop *loop,
                       const InetAddress &listenAddr,
                       const string &nameArg)
    : _server(loop, listenAddr, nameArg), _loop(loop)
{
    _server.setConnectionCallback(bind(&ChatServer::onConnection, this, _1));
    _server.setMessageCallback(bind(&ChatServer::onMessage, this, _1, _2, _3));
    _server.setThreadNum(4);
}

//tcpserver开启
void ChatServer::start()
{
    _server.start();
}

//具体的连接回调函数
void ChatServer::onConnection(const TcpConnectionPtr &conn)
{
    if(!conn->connected())
    {
        //客户端异常关闭
        ChatService::instance()->clientCloseException(conn);
        conn->shutdown();
    }
}

//具体的消息处理回调函数
void ChatServer::onMessage(const TcpConnectionPtr &conn,
               Buffer *buffer,
               Timestamp time)
{
    string buf = buffer->retrieveAllAsString();
    json js = json::parse(buf);
    //通过js["msgid"]获取-》业务handler-》conn js time 根据不同业务类型调用不同的函数
    auto handler =  ChatService::instance()->getHandler(js["msgid"].get<int>());
    handler(conn, js, time);
}