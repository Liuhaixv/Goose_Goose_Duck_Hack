#pragma once

#include<string>

enum RemoteMasterServerState {
    //未知
    UNKNOWN,
    //服务器连接失败
    DOWN,
    //正常连接
    NORMAL
};

struct  RemoteMasterServerSettings {
    //域名
    std::string domain = "127.0.0.1";
    //端口
    std::string port = "80";

    //服务器连接状态
    //TODO：后端加上ping pong检测连接状态
    RemoteMasterServerState serverState = RemoteMasterServerState::UNKNOWN;

    //获取最新的辅助版本
    const std::string getLatestHackVersion = "/getLatestHackVersion";
    //获取最新的游戏版本
    const std::string getLatestGameVersion = "/getLatestGameVersion";

    RemoteMasterServerSettings() {
    }

    RemoteMasterServerSettings(std::string domain, std::string port) {
        this->domain = domain;
        this->port = port;
    }
};

class RemoteMasterServer {
    RemoteMasterServerSettings serverSettings;

    RemoteMasterServer() {
        checkServerState();
    }

    RemoteMasterServer(std::string domain, std::string port) {
        this->serverSettings.domain = domain;
        this->serverSettings.port = port;
        checkServerState();
    }

    //TODO:
    void checkServerState() {

    }
};