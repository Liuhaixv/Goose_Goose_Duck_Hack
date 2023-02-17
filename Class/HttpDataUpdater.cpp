#include "HttpDataUpdater.h"
#include "Updater.h"
#include "../Struct/HackSettings.hpp"

#include"../httplib.h"
#include"../http/http_api/GetVersionsApi.h"

#include"../http/json_stuct/LatestVersionsJson.h"

extern HackSettings hackSettings;

//public
void HttpDataUpdater::addHttpTask(IN std::function<void()> callable) {
    this->tasks.push(callable);
}

/// <summary>
/// 从hackSettings中读取判断是否需要更新对应的数据<para/>
/// Thread that keeps updating hackSettings by listening to keyboard
/// </summary>
void HttpDataUpdater::httpDataUpdaterThread() {
    while (true) {
        if (this->paused) {
            Sleep(200);
            continue;
        }

        Sleep(10);
        //检查服务器状态
        switch (hackSettings.remoteServerSettings.serverState) {
        case RemoteMasterServerState::BANNED:
            //TODO: banned
            exit(0);
        case RemoteMasterServerState::UNKNOWN:
        case RemoteMasterServerState::DOWN:
            HttpTask::connectToServer();
            break;
        case RemoteMasterServerState::NORMAL:
            break;
        }

        if (this->tasks.size() == 0) {
            Sleep(50);
            continue;
        }
        else {
            std::function<void()> callable = tasks.front();
            callable();
            tasks.pop();
        }
    }
}

//private
bool getVersionsJson(IN std::string str, OUT JsonStruct::LatestVersionsJson* latestVersions) {
    std::cout << str << std::endl;

    json jsonStr;
    json latestVersionJson;

    try {
        jsonStr = json::parse(str);
        if (!jsonStr.contains("success") || !jsonStr.contains("data") || jsonStr["success"] != true) {
            std::cout << "Parse json failed!!";
            return false;
        }

        latestVersionJson = jsonStr["data"];
    }
    catch (...) {
        std::cout << "Parse json failed!!";
        return false;
    }

    try {
        latestVersions->update(latestVersionJson.get<JsonStruct::LatestVersionsJson>());
    }
    catch (...) {
        std::cout << "Parse json failed!!";
        return false;
    }

    return true;
    /*
    std::cout << "HackVersion:" << latestVersions.latestHackVersion << std::endl;
    std::cout << "Url:" << latestVersions.url << std::endl;
    */
}

namespace HttpTask {

    void connectToServer() {
        static int maxRetryTime = 20;
        static int triedTimes = 0;
        static time_t lastTimeConnect = -1;
        static time_t minInterval = 3;//3秒

        //尝试连接次数过多，禁止尝试连接服务器
        if (triedTimes >= maxRetryTime) {
            return;
        }

        if (time(NULL) - lastTimeConnect <= minInterval) {
            //间隔过短
            return;
        }

        PingApi pingApi;
        httplib::Client cli(pingApi.domain);

        try {
            httplib::Result res = cli.Get(pingApi.getPath());

            if (res.error() != httplib::Error::Success) {
                throw res;
            }

            if (res->status == 200) {
                //success
                hackSettings.remoteServerSettings.serverState = RemoteMasterServerState::NORMAL;

                getVersionsJson(res->body, &hackSettings.latestVersions);
                //checkBan
                //TODO
                // 
                //更新时间
                time(&lastTimeConnect);
            }
            else {
                triedTimes++;
                //加大重连间隔
                if (triedTimes >= (maxRetryTime / 3)) {
                    minInterval *= 2;
                }

                hackSettings.remoteServerSettings.serverState = RemoteMasterServerState::DOWN;
            }
        }
        catch (...) {
            //TODO: ERR

            triedTimes++;
            //加大重连间隔
            if (triedTimes >= (maxRetryTime / 3)) {
                minInterval *= 2;
            }

            hackSettings.remoteServerSettings.serverState = RemoteMasterServerState::UNKNOWN;
        }
    }

    void checkLatestVersions(bool forceReconnect = false) {
        if (hackSettings.remoteServerSettings.serverState != NORMAL && forceReconnect == false) {
            return;
        }

        //已经更新过版本信息
        if (hackSettings.latestVersions.hasUpdatedLatestVersions()) {
            return;
        }

        static GetVersionsApi getVersionsApi;

        httplib::Client cli(getVersionsApi.domain);

        try {
            //
            auto res = cli.Get(getVersionsApi.getPath());

            if (res.error() != httplib::Error::Success) {
                throw res;
            }

            if (res->status == 200) {

                //success
                if (!getVersionsJson(res->body, &hackSettings.latestVersions)) {
                    hackSettings.remoteServerSettings.serverState = RemoteMasterServerState::UNKNOWN;
                }
                else {
                    //更新服务器状态
                    hackSettings.remoteServerSettings.serverState = RemoteMasterServerState::NORMAL;
                }

                //睡眠1秒观看加载动画
                //Sleep(1000);
            }
            else {
                return;
            }           
        }
        catch (...) {
            hackSettings.remoteServerSettings.serverState = RemoteMasterServerState::DOWN;
        }
    }
    void uploadUserInfo(UserInfo userInfo)
    {

    }
}

