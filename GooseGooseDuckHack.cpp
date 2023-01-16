#pragma once

#include<list>
#include<thread>
#include <stdlib.h>
#include <tchar.h>

#include"Client.hpp"
#include"utils.hpp"

#include"Struct/HackSettings.hpp"

//Updaters
#include"./Class/DataUpdater.hpp"
#include"./Class/HotkeyUpdater.hpp"
#include"./Class/GameProcessUpdater.hpp"
#include"./Class/BytesPatchUpdater.hpp"

#include"./Class/PlayerController.h"

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

Utils utils;
Hack hack;

//初始化辅助设置类
//settings
HackSettings hackSettings;
Client* g_client;

INT APIENTRY WinMain(HINSTANCE instance, HINSTANCE, PSTR, INT cmd_show) {
    {
        //修改设置
        //Edit hacksettings
        hackSettings.guiSettings.b_disableFogOfWar = false;
    }
  
    //初始化RPM工具类
    //Init RPM classes
    Memory memory;
    Client client(&memory, &hackSettings);
    hack.setClient(&client);

    g_client = &client;

    //初始化更新类线程
    //Init updaters
    HotkeyUpdater hotkeyUpdater(&hackSettings);
    DataUpdater dataUpdater(&client);
    BytesPatchUpdater bytesUpdater(&memory);
    MemoryUpdater memoryUpdater(&memory, &client, &hackSettings);

    //监听热键
    //Listen to keyboard
    std::thread hackSettingsUpdater(&HotkeyUpdater::hackSettingsUpdater, &hotkeyUpdater);
    //启动游戏内存数据更新线程
    //Game data updater
    std::thread playerControllerUpdater(&DataUpdater::playerControllerUpdater, &dataUpdater);
    //启动字节补丁线程
    //Game process finder
    std::thread bytesPatchUpdater(&BytesPatchUpdater::bytesPatchUpdater, &bytesUpdater);
    //启动游戏进程查找线程
    //Game process finder
    std::thread gameProcessUpdater(&MemoryUpdater::gameProcessUpdater, &memoryUpdater);

    //GUI循环
    //GUI loop
    UI::Render( instance, cmd_show);   
}