#pragma once

#include"./GuiSettings.hpp"
#include"./GameStateSettings.hpp"
struct HackSettings {
    bool disableFogOfWar = false;
    bool enableNoclip = false;

    GameStateSettings gameStateSettings;
    GuiSettings guiSettings;
};
