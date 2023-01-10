#pragma once

#include <Windows.h>
#include "../memory.hpp"
#include "../Struct/HackSettings.hpp"

/// <summary>
/// Thread functions that update data.
/// </summary>
class MemoryUpdater {
public:
    //查找游戏进程的间隔时间(秒)
    //Min time before searching the game process again
    double interval = 5.0;
    MemoryUpdater(Memory* memory, HackSettings* hackSettings) {
        this->memory = memory;
        this->hackSettings = hackSettings;
    }

    /// <summary>
    /// 查找游戏进程并更新Memory<para/>
    /// Thread that search for game process and update Memory instance
    /// </summary>
    void gameProcessUpdater() {
        while (true)
        {
            Sleep(2);            
        }
    }
private:
    Memory* memory = nullptr;
    HackSettings* hackSettings = nullptr;
};
