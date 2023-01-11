#pragma once

#include"../Enum/OpenProcessState.hpp"

struct GameStateSettings {
    //游戏是否正在运行
    //Indicate whether the game process is running
    bool b_gameProcessRunning = false;
    //(未使用)指示localplayer是否在游戏局内
    //Not used yet
    bool b_ingame = false;

    OpenProcessState openProcessState = OpenProcessState::GameNotFound;
};