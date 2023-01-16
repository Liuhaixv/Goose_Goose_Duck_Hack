#pragma once

#include "Memory.hpp"
#include "Struct/HackSettings.hpp"
#include "Class/PlayerController.h"
#include"Class/LocalPlayer.hpp"

class Client
{
public:
    HackSettings* hackSettings = nullptr;

    static const int n_players = 16;
    LocalPlayer localPlayer;
    std::vector<PlayerController*> playerControllers;

    Client(Memory* memory, HackSettings* hackSettings);

    ~Client();

    Memory* getMemory() {
        return this->memory;
    }

    /// <summary>
    /// 重置GUI设置
    /// </summary>
    void resetGuiSettings();

    /// <summary>
    /// 重置所有玩家的数据
    /// </summary>
    void resetPlayersData();

    /// <summary>
    /// 本地玩家退出游戏
    /// </summary>
    void onLocalPlayerQuitGame();

    /// <summary>
    /// 游戏开始
    /// </summary>
    void onGameStarted();

    /// <summary>
    /// 游戏结束
    /// </summary>
    void onGameEnded();

    /// <summary>
    /// 有玩家死亡
    /// </summary>
    /// <param name="playerController"></param>
    void onPlayerDeath(IN PlayerController* playerController);

    bool teleportTo(IN const Vector2& to);

    void updateGameOriginalData();

    [[deprecated("Replaced with GUI, won't do anything now")]]
    void printAllPlayersInfo(Utils* utils = nullptr);

private:
    Memory* memory = 0;
};
