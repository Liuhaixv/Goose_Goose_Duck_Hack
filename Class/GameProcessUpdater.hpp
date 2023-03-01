﻿#pragma once

#include <Windows.h>
#include "../Memory.h"
#include "../Struct/HackSettings.hpp"
#include "Updater.h"
#include"../Client.h"


/// <summary>
/// Thread functions that update data.
/// </summary>
class MemoryUpdater :public Updater {
public:
    //查找游戏进程的间隔时间(秒)
    //Min time before searching the game process again
    double interval = 2.0;
    MemoryUpdater(Client* client, HackSettings* hackSettings) {
        this->hackSettings = hackSettings;
    }

    /// <summary>
    /// 查找游戏进程并更新Memory。只适用于第一次自动连接到游戏<para/>
    /// Thread that search for game process and update Memory instance
    /// </summary>
    void gameProcessUpdater() {

        int connect_count = 0;
        while (true)
        {
            Sleep(2);

            if (this->paused) {
                Sleep(100);
                continue;
            }

            if (memory.processHandle && memory.gameAssemblyBaseAddress) {
                if (hackSettings->gameStateSettings.b_gameProcessRunning == false) {
                    //std::cout << "Game found" << std::endl;

                    hackSettings->gameStateSettings.b_gameProcessRunning = true;
                    //hackSettings->gameStateSettings.openProcessState = OpenProcessState::GameFoundAndLoadedDLL;
                    //TODO: 添加检测游戏退出
                    continue;
                }
            }
            else {
                //Game not found, sleeping for $interval$ secs
                hackSettings->gameStateSettings.b_gameProcessRunning = false;
                Sleep(5000);

                ++connect_count;
                Sleep(interval * 1000);

                if (connect_count >= 2) {
                    connect_count = 0;

                    //搜索游戏进程并尝试附加
                    memory.searchGameProcessAndAttach();
                }
            }
        }
    }
private:
    Client* client = nullptr;
    HackSettings* hackSettings = nullptr;
};
