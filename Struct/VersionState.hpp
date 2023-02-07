#pragma once
#include <string>

struct VersionState {
    //检查过版本更新
    bool b_checkedVersion = false;
    //是否为最新版本
    bool b_isLatestHackVersion = false;

    //游戏版本是否正确
    bool b_gameVersionMatch = false;

    //TODO
    bool checkGameVersion(std::string gameVersion1, std::string gameVersion2) {

    }

    //TODO
    bool isLatestHackVersion(std::string currentHackVersion, std::string remoteServerLatestHackVersion) {

    }
};