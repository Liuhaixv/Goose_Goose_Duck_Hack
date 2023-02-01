#pragma once

#include"./GuiSettings.hpp"
#include"./GameStateSettings.hpp"
#include"./GameOriginalData.hpp"

#include"RemoteMasterServerSettings.hpp"

struct HackSettings {
    //退出程序标志
    bool b_quitProgram = false;

    bool tempEnableNoclip = false;

    bool b_removeSkillCoolDown = false;

    bool b_debug_disableWriteMemory = false;
    //bool b_debug_disableReadMemory = false;

    RemoteMasterServerSettings remoteServerSettings;

    GameOriginalData gameOriginalData;
    GameStateSettings gameStateSettings;
    GuiSettings guiSettings;
};
