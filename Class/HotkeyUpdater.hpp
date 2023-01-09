#pragma once

#include"../Struct/HackSettings.hpp"
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
        }
    }
private:
    HackSettings* hackSettings = nullptr;
};
