#include "chatserver.hpp"
#include "chatservice.hpp"
#include <iostream>
#include <signal.h>
using namespace std;

//客户端下线，在数据库中将客户端状态修改为offline
void resetHandler(int)
{
    ChatService::instance()->reset();
    exit(0);
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        cerr << "command invalid! example: ./ChatServer 127.0.0.1 6000" << endl;
        exit(-1);
    }

    // 解析通过命令行参数传递的ip和port
    char *ip = argv[1];
    uint16_t port = atoi(argv[2]);
    //捕捉ctrl c信号，并设置处理回调函数
    signal(SIGINT, resetHandler);

    EventLoop loop;
    InetAddress add(ip, port);
    //网络层
    ChatServer server(&loop, add, "ChatServer");
    server.start();
    loop.loop();
    return 0;
}