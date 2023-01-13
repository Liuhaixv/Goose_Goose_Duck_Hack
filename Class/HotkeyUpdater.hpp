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

            //长按类
            {
                //Left ALT to enable noclip
                //左ALT键开启穿墙
                if (GetAsyncKeyState(VK_LMENU))
                {
                    hackSettings->enableNoclip = true;
                }
                else {
                    hackSettings->enableNoclip = false;
                }
            }

            //开关类

            //Insert switch main menu
            checkSwitchHotkey(VK_INSERT, &this->pressingInsert, &hackSettings->guiSettings.b_enableMenu);
            //Delete to switch minimap
            checkSwitchHotkey(VK_DELETE,&this->pressingDel, &hackSettings->guiSettings.b_enableMinimap );
            //TODO:END key to quit program
            //END键结束辅助
            checkSwitchHotkey(VK_END,&this->pressingEnd, &hackSettings->b_quitProgram );              
        }
    }
private:
    HackSettings* hackSettings = nullptr;

    bool pressingInsert = GetAsyncKeyState(VK_LMENU);
    bool pressingDel = GetAsyncKeyState(VK_DELETE);
    bool pressingEnd = GetAsyncKeyState(VK_END);

    void checkSwitchHotkey(int VK, bool* wasPressingKey, bool* b_switch) {
        bool pressingAtThisTime = GetAsyncKeyState(VK);

        //当前正在按下热键
        if (pressingAtThisTime)
        {
            //之前没有按过热键，说明现在是刚刚按下热键，此时修改开关的bool值
            if (!(*wasPressingKey)) {
                //之前没按过，现在已经是按下的状态了
                *wasPressingKey = true;

                //切换开关
                *b_switch = !(*b_switch);
            }
        }
        else {
            //没有按热键
            *wasPressingKey = false;
        }
    }
};
