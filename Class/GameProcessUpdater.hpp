#pragma once

#include <Windows.h>
#include "../Memory.hpp"
#include "../Struct/HackSettings.hpp"

/// <summary>
/// Thread functions that update data.
/// </summary>
class MemoryUpdater {
public:
    //查找游戏进程的间隔时间(秒)
    //Min time before searching the game process again
    double interval = 2.0;
    MemoryUpdater(Memory* memory, Client* client, HackSettings* hackSettings) {
        this->memory = memory;
        this->hackSettings = hackSettings;
    }

    /// <summary>
    /// 查找游戏进程并更新Memory<para/>
    /// Thread that search for game process and update Memory instance
    /// </summary>
    void gameProcessUpdater() {

        int connect_count = 0;
        while (true)
        {
            Sleep(2);

            if (memory->processHandle && memory->gameAssemblyBaseAddress) {
                if (hackSettings->gameStateSettings.b_gameProcessRunning == false) {
                    std::cout << "Game found" << std::endl;

                    hackSettings->gameStateSettings.b_gameProcessRunning = true;
                    hackSettings->gameStateSettings.openProcessState = OpenProcessState::GameFoundAndLoadedDLL;
                    //暂时没有添加检测游戏退出，所以这里直接返回结束线程
                    return;
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

                    //搜索游戏进程
                    hackSettings->gameStateSettings.openProcessState = memory->searchGameProcess();
                }
            }
        }
    }
private:
    Client* client = nullptr;
    Memory* memory = nullptr;
    HackSettings* hackSettings = nullptr;
};
