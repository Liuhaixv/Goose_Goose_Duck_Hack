#pragma once

#include"./GuiSettings.hpp"
#include"./GameStateSettings.hpp"
#include"./GameOriginalData.hpp"
struct HackSettings {
    bool disableFogOfWar = false;
    bool enableNoclip = false;

    GameOriginalData gameOriginalData;
    GameStateSettings gameStateSettings;
    GuiSettings guiSettings;
};
