#include "redis.hpp"
#include <iostream>
#include <thread>
using namespace std;
Redis::Redis() : _publishContext(nullptr), _subcribeContext(nullptr)
{
}

Redis::~Redis()
{
    if (_publishContext != nullptr)
    {
        redisFree(_publishContext);
    }
    if (_subcribeContext != nullptr)
    {
        redisFree(_subcribeContext);
    }
}

bool Redis::connect()
{
    _publishContext = redisConnect("127.0.0.1", 6379);
    if (nullptr == _publishContext)
    {
        cout << "connect redis failed!" << endl;
        return false;
    }

    _subcribeContext = redisConnect("127.0.0.1", 6379);
    if (nullptr == _subcribeContext)
    {
        cerr << "connect redis failed!" << endl;
        return false;
    }
    //在单独的线程中，监听通道上的事件，有消息给业务层进行上班
    thread t([&]()
             { observer_channel_message(); });
    t.detach();
    cout << "connect redis-server success!" << endl;
    return true;
}

bool Redis::publish(int channel, string message)
{
    redisReply *reply = (redisReply *)redisCommand(_publishContext, "PUBLISH %d %s", channel, message.c_str());
    if (nullptr == reply)
    {
        cerr << "publish command failed!" << endl;
        return false;
    }
    freeReplyObject(reply);
    return true;
}

bool Redis::subcribe(int channel)
{
    // SUBSCRIBE命令本身会造成线程阻塞等待通道里面发生消息，这里只做订阅通道，不接收通道消息
    // 通道消息的接收专门在observer_channel_message函数中的独立线程中进行
    // 只负责发送命令，不阻塞接收redis server响应消息，否则和notifyMsg线程抢占响应资源
    
    //write a formatted command to the output buffer
    if (REDIS_ERR == redisAppendCommand(this->_subcribeContext, "SUBSCRIBE %d", channel))
    {
        cerr << "subscribe command failed!" << endl;
        return false;
    }
    int done = 0;
    while (!done)
    {
        //Write the output buffer to the socket.
        if (REDIS_ERR == redisBufferWrite(this->_subcribeContext, &done))
        {
            cerr << "subscribe command failed!" << endl;
            return false;
        }
    }
    return true;
}

bool Redis::unsubcribe(int channel)
{
    if (REDIS_ERR == redisAppendCommand(this->_subcribeContext, "UNSUBSCRIBE %d"), channel)
    {
        cerr << "unsubscribe command failed!" << endl;
        return false;
    }
    //redisBufferWrite可以循环发送缓冲区，直到缓冲区数据发送完毕（done被置为1）
    int done = 0;
    while (!done)
    {
        if (REDIS_ERR == redisBufferWrite(this->_subcribeContext, &done))
        {
            cerr << "unsubscribe command failed!" << endl;
            return false;
        }
    }
    return true;
}

//在独立线程中订阅通道中的消息
void Redis::observer_channel_message()
{
    redisReply *reply = nullptr;
    while (REDIS_OK == redisGetReply(this->_subcribeContext, (void **)&reply))
    {
        if (reply != nullptr && reply->element[2] != nullptr && reply->element[2]->str != nullptr)
        {
            char *test = reply->element[2]->str;
            _notify_message_handler(atoi(reply->element[1]->str), reply->element[2]->str);
        }
        freeReplyObject(reply);
    }
    cerr << ">>>>>>>>>>>> observe_channel_message quit <<<<<<<<<<<<" << endl;
}

//初始化上报消息的回调函数
void Redis::init_notify_handler(function<void(int, string)> fn)
{
    this->_notify_message_handler = fn;
}