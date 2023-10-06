#ifndef PUBLIC_H
#define PUBLIC_H

enum EnMsgType
{
    LOGIN_MSG = 1,//登录
    LOGIN_MSG_ACK,
    REG_MSG,//注册
    REG_MSG_ACK,
    ONE_CHAT_MSG,//一对一聊天
    ADD_FRIEND_MSG,//添加好友
    CREATE_GROUP_MSG,//创建群组
    ADD_GROUP_MSG,//加群
    GROUP_CHAT_MSG,//群聊天
    LOGINOUT_MSG//登出
};

#endif