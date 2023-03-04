#pragma once

//Windows
#include<Windows.h>

#include<list>
#include<thread>
#include <stdlib.h>
#include <tchar.h>

#include"Client.h"
#include"utils.hpp"

//Struct
#include"Struct/UserSettings.hpp"
#include"Struct/HackSettings.hpp"

//Updaters
#include"./Class/DataUpdater.hpp"
#include"./Class/HotkeyUpdater.hpp"
#include"./Class/GameProcessUpdater.hpp"
#include"./Class/BytesPatchUpdater.hpp"

#include"./Class/Game/PlayerController.h"

#include<dwmapi.h>
#include<d3d11.h>
//ImGui
#include<imgui.h>
#include<imgui_impl_dx11.h>
#include<imgui_impl_win32.h>
//UI
#include "./UI/UI.h"
#include "Class/HttpDataUpdater.h"
#include "Memory/CodeCave/CodeCave.h"
#include "Class/DebugConsole.h"

#include "antiAC.hpp"

#ifndef DEVELOP
AntiAC antiAC;
#endif
Hack hack;

HackSettings hackSettings;

//初始化辅助设置类
//settings
Client g_client;

//全局变量保存用户配置
UserSettings userSettings;

Utils utils;

//初始化RPM工具类
//Init RPM classes
Memory memory;

//初始化更新类线程
//Init updaters
//TODO: 处理NULL
HotkeyUpdater hotkeyUpdater(&hackSettings);
DataUpdater dataUpdater(&g_client);
BytesPatchUpdater bytesUpdater;
MemoryUpdater memoryUpdater(&g_client, &hackSettings);
HttpDataUpdater httpDataUpdater;

std::vector<Updater*> updaters;

CodeCave codeCave;

DebugConsole debugConsole;

INT APIENTRY WinMain(HINSTANCE instance, HINSTANCE, PSTR, INT cmd_show) {
    {
        //修改设置
        //Edit hacksettings
        hackSettings.guiSettings.b_disableFogOfWar = false;
    }
    hack.setClient(&g_client);

    //存放所有Updater
    updaters.push_back(&hotkeyUpdater);
    updaters.push_back(&dataUpdater);
    updaters.push_back(&bytesUpdater);
    updaters.push_back(&memoryUpdater);
    updaters.push_back(&httpDataUpdater);

#ifndef DEVELOP
    std::thread antiACThread(&AntiAC::check_loop, &antiAC);
#endif
    //监听热键
    //Listen to keyboard
    std::thread hackSettingsUpdaterThread(&HotkeyUpdater::hackSettingsUpdater, &hotkeyUpdater);
    //启动游戏内存数据更新线程
    //Game data updater
    std::thread playerControllerUpdaterThread(&DataUpdater::playerControllerUpdater, &dataUpdater);
    std::thread lobbySceneHandlerUpdaterThread(&DataUpdater::lobbySceneHandlerUpdater, &dataUpdater);
    //启动字节补丁线程
    //Game process finder
    std::thread bytesPatchUpdaterThread(&BytesPatchUpdater::bytesPatchUpdater, &bytesUpdater);
    //启动游戏进程查找线程
    //Game process finder
    std::thread gameProcessUpdaterThread(&MemoryUpdater::gameProcessUpdater, &memoryUpdater);
    //启动http客户端更新线程
    std::thread httpDataUpdaterThread(&HttpDataUpdater::httpDataUpdaterThread, &httpDataUpdater);

    //GUI循环
    //GUI loop
    UI::Render( instance, cmd_show);   
}