#pragma once

#include<string>
#include"../http/http_api/Ping.h"

enum RemoteMasterServerState {
    //未知
    UNKNOWN,
    //服务器连接失败
    DOWN,
    //正常连接
    NORMAL,
    //已封禁
    BANNED
};

struct  RemoteMasterServerSettings {
    //服务器连接状态
    //TODO：后端加上ping pong检测连接状态
    RemoteMasterServerState serverState = RemoteMasterServerState::UNKNOWN;
};