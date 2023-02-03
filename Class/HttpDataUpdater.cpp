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
            Sleep(10);
            if (this->tasks.size() == 0) {
                Sleep(50);
                continue;
            }
            else {
                std::function<void()> callable = tasks.front();
                tasks.pop();
                callable();
            }
        }
    }

//private


namespace HttpTask {

    void connectToServer() {
        PingApi pingApi;
        httplib::Client cli(pingApi.domain, pingApi.port);

        auto res = cli.Get(pingApi.getPath());
        if (res->status == 200) {
            //success
            hackSettings.remoteServerSettings.serverState = RemoteMasterServerState::NORMAL;
            //checkBan
            //TODO
        }
        else {
            hackSettings.remoteServerSettings.serverState = RemoteMasterServerState::DOWN;
        }
    }

    void checkLatestVersions() {
        if (hackSettings.remoteServerSettings.serverState != NORMAL) {
            return;
        }

        //已经更新过版本信息
        if (hackSettings.latestVersions.hasUpdatedLatestVersions()) {
            return;
        }

        static GetVersionsApi getVersionsApi;

        httplib::Client cli(getVersionsApi.domain, getVersionsApi.port);
        auto res = cli.Get(getVersionsApi.getPath());
        if (res->status == 200) {
            try {
                //TODO: fix json
                JsonStruct::LatestVersionsJson latestVersions = nlohmann::json::parse(res->body).get<JsonStruct::LatestVersionsJson>();
                hackSettings.latestVersions.update(latestVersions);
            }
            catch (...) {
                hackSettings.remoteServerSettings.serverState = RemoteMasterServerState::DOWN;
            }
        }
        else {
            return;
        }
    }
}