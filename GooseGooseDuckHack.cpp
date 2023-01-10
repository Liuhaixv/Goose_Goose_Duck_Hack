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

INT APIENTRY WinMain(HINSTANCE instance, HINSTANCE, PSTR, INT cmd_show) {
    //初始化工具类
    Utils utils;

    //关闭快速编辑
    //Disable the quick-edit mode
    //utils.disableQuickEdit();

    //settings
    HackSettings hackSettings;
    hackSettings.disableFogOfWar = true;

    //初始化RPM工具类
    Memory memory;
    Client client(&memory, &hackSettings);

    HotkeyUpdater hotkeyUpdater(&hackSettings);
    DataUpdater dataUpdater(&client);

    //GUI



    //检测PID
    if (memory.pID != NULL) {
        //utils.print("Goose Goose Duck hack running...", "鹅鸭杀辅助运行中...");
        //std::cout << std::endl;

        //启动数据更新线程
        std::thread playerControllerUpdater(&DataUpdater::playerControllerUpdater, &dataUpdater);
        //监听热键
        std::thread hackSettingsUpdater(&HotkeyUpdater::hackSettingsUpdater, &hotkeyUpdater);

        /*
        //循环打印数据
        while (true) {
            if (dataUpdater.validPlayersNum > 0) {
                //clear console
                system("cls");
                //开始打印
                client.printAllPlayersInfo(&utils);
            }
            Sleep(1000);
        }
        */

        //GUI线程循环
        UI::Render(instance, cmd_show);
    }
    else {
        //utils.print("Not detected game, closing now... Please try again and run the game before starting the hack.", "未检测到游戏，正在关闭......请先运行游戏再打开该辅助。");
        //std::cout << std::endl;
        //utils.print("If your have launched game but still see this message, it may because you ran the game (or steam) with Administrator privilege. In that case, you must run the hack with Administrator privilege too.", "如果你的游戏已经打开但仍一直看到该信息出现，很可能是你以管理员权限运行了游戏或Steam，那么你需要同样使用管理员权限来运行该辅助。");
        //std::cout << std::endl << std::endl;
        //system("pause");
    }
}