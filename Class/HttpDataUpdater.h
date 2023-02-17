#pragma once
#include "Updater.h"
#include <queue>
#include <functional>
#include "../Struct/UserInfo.h"

/// <summary>
/// Thread functions that update data from http server
/// </summary>
class HttpDataUpdater :public Updater {
public:
    void addHttpTask(std::function<void()> callable);

    /// <summary>
    /// 从hackSettings中读取判断是否需要更新对应的数据<para/>
    /// Thread that keeps updating hackSettings by listening to keyboard
    /// </summary>
    void httpDataUpdaterThread();

private:
    std::queue<std::function<void()>> tasks;
};

namespace HttpTask {
    void connectToServer();
    void checkLatestVersions(bool forceReconnect);
    void uploadUserInfo(UserInfo userInfo);
}