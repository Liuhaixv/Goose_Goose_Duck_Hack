#pragma once

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

//Windows
#include<Windows.h>
#include<dwmapi.h>
#include<d3d11.h>
//ImGui
#include<imgui.h>
#include<imgui_impl_dx11.h>
#include<imgui_impl_win32.h>
//UI
#include "./UI/UI.h"


Utils utils;
Hack hack;

HackSettings hackSettings;

//初始化辅助设置类
//settings
Client g_client;

//全局变量保存用户配置
UserSettings userSettings;

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

std::vector<Updater*> updaters;

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

    //监听热键
    //Listen to keyboard
    std::thread hackSettingsUpdater(&HotkeyUpdater::hackSettingsUpdater, &hotkeyUpdater);
    //启动游戏内存数据更新线程
    //Game data updater
    std::thread playerControllerUpdater(&DataUpdater::playerControllerUpdater, &dataUpdater);
    std::thread lobbySceneHandlerUpdater(&DataUpdater::lobbySceneHandlerUpdater, &dataUpdater);
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