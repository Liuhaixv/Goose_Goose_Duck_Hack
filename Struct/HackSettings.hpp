#pragma once

#include"./GuiSettings.hpp"
#include"./GameStateSettings.hpp"
#include"./GameOriginalData.hpp"
struct HackSettings {
    //退出程序标志
    bool b_quitProgram = false;

    bool disableFogOfWar = false;
    bool enableNoclip = false;

    GameOriginalData gameOriginalData;
    GameStateSettings gameStateSettings;
    GuiSettings guiSettings;
};
