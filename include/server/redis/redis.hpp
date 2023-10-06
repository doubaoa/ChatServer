#ifndef REDIS_H
#define REDIS_H
#include<string>
#include<functional>
#include<hiredis/hiredis.h>
using namespace std;
class Redis
{
    public:
        Redis();
        ~Redis();
        bool connect();
        bool publish(int channel, string message);
        bool subcribe(int channel);
        bool unsubcribe(int channel);
        void observer_channel_message();
        void init_notify_handler(function<void(int, string)> fn);
    private:
        //redis连接上下文 publish和subscribe要分开
        redisContext *_publishContext;
        redisContext *_subcribeContext;
        //消息上报回调函数
        function<void(int, string)> _notify_message_handler;
};

#endif