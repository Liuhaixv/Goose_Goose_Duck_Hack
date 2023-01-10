#pragma once

#include<list>
#include<thread>
#include <stdlib.h>
#include <tchar.h>

#include"client.hpp"
#include"utils.hpp"

#include"Struct/HackSettings.hpp"

#include"./Class/DataUpdater.hpp"
#include"./Class/HotkeyUpdater.hpp"
#include"./Class/GameProcessUpdater.hpp"

#include"./Class/PlayerController.hpp"

//Windows
#include<Windows.h>
#include<dwmapi.h>
#include<d3d11.h>
//ImGui
#include<imgui.h>
#include<imgui_impl_dx11.h>
#include<imgui_impl_win32.h>
//UI
#include "UI.h"

void getScaledResolution(int& x, int& y);

Utils utils;

INT APIENTRY WinMain(HINSTANCE instance, HINSTANCE, PSTR, INT cmd_show) {
    //初始化辅助设置类
    //settings
    HackSettings hackSettings;
    {
        //修改设置
        //Edit hacksettings
        hackSettings.disableFogOfWar = false;
    }

    //初始化RPM工具类
    //Init RPM classes
    Memory memory;
    Client client(&memory, &hackSettings);

    //初始化更新类线程
    //Init updaters
    HotkeyUpdater hotkeyUpdater(&hackSettings);
    DataUpdater dataUpdater(&client);
    MemoryUpdater memoryUpdater(&memory, &hackSettings);

    //监听热键
    //Listen to keyboard
    std::thread hackSettingsUpdater(&HotkeyUpdater::hackSettingsUpdater, &hotkeyUpdater);
    //启动游戏内存数据更新线程
    //Game data updater
    std::thread playerControllerUpdater(&DataUpdater::playerControllerUpdater, &dataUpdater);
    //启动游戏进程查找线程
    //Game process finder
    std::thread gameProcessUpdater(&MemoryUpdater::gameProcessUpdater, &memoryUpdater);


    //GUI循环
    UI::Render(&hackSettings, instance, cmd_show);
    //UI::Render(&hackSettings, instance, cmd_show);

    /*
    while (true) {
        while (memory.processHandle != NULL && memory.gameAssemblyBaseAddress ) {
            if (!hackSettings.process) {

                std::cout << "Game found" << std::endl;
                hackSettings.process = true;
            }

            while (client.in_game()) {
                if (!hackSettings.game) {
                    hackSettings.game = true;
                    client.update_gamemory.de();
                    hacks.init();
                }


                int round_index = client.get_round_index();

                //update last_time_round_index_changed
                if (round_index != hackSettings.round_index) {
                    time_t current_time = time(NULL);
                    hackSettings.last_time_round_index_changed = current_time;
                }
                //update round_index
                hackSettings.round_index = round_index;

                // SendMessage(ol.hwnd, WM_PAINT, NULL, NULL);
                Sleep(1000);
            }

            hackSettings.game = false;
            Sleep(5000);
        }

        std::cout << "Game not found, sleeping for 5 secs" << std::endl;

        hackSettings.process = false;
        ++connect_count;
        Sleep(5000);
        if (connect_count >= 2) {
            connect_count = 0;
            memory.~memory.ry();
            client.~Client();
            hacks.~Hacks();

            new(&memory. memory.ry();
            new(&client) Client(&memory.;
        }
    }
    */
}