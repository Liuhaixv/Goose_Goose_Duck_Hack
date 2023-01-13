#pragma once

#include"../Struct/HackSettings.hpp"
#include"../UI.h"
#include <Windows.h>

/// <summary>
/// Thread functions that update data.
/// </summary>
class HotkeyUpdater {
public:
    HotkeyUpdater(HackSettings* hackSettings) {
        this->hackSettings = hackSettings;
    }

    /// <summary>
    /// 监听键盘热键修改设置<para/>
    /// Thread that keeps updating hackSettings by listening to keyboard
    /// </summary>
    void hackSettingsUpdater() {
        while (true)
        {
            Sleep(2);

            //Left ALT to enable noclip
            //左ALT键开启穿墙
            if (GetAsyncKeyState(VK_LMENU))
            {
                hackSettings->enableNoclip = true;
            }
            else {
                hackSettings->enableNoclip = false;
            }

            bool insert = GetAsyncKeyState(VK_INSERT);
            //Insert to enable GUI menu
            //Insert键开关菜单
            if (insert)
            {
                if (!pressingInsert) {
                    pressingInsert = true;
                    //switch
                    hackSettings->guiSettings.b_enableMenu = !hackSettings->guiSettings.b_enableMenu;
                    //UI::makeWindowClickable(UI::hwnd, hackSettings->guiSettings.b_enableMenu);
                }
            }
            else {
                pressingInsert = false;
            }

            bool del = GetAsyncKeyState(VK_DELETE);
            //DEL键开关地图
            if (del)
            {
                if (!pressingDel) {
                    pressingDel = true;

                    hackSettings->guiSettings.b_enableMinimap = !hackSettings->guiSettings.b_enableMinimap;

                }
            }
            else {
                pressingDel = false;
            }
        }
    }
private:
    HackSettings* hackSettings = nullptr;

    bool pressingInsert = GetAsyncKeyState(VK_LMENU);
    bool pressingDel = GetAsyncKeyState(VK_DELETE);
};
