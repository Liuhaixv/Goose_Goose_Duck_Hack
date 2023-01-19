#pragma once

#include "Memory.hpp"
#include "Struct/HackSettings.hpp"
#include "Class/Game/PlayerController.h"
#include"Class/Game/LocalPlayer.hpp"
#include "Class/Game/LobbySceneHandler.h"

class Client
{
public:
    HackSettings* hackSettings = nullptr;

    static const int n_players = 16;
    LobbySceneHandler lobbySceneHandler;
    LocalPlayer localPlayer;
    std::vector<PlayerController*> playerControllers;

    Client(HackSettings* hackSettings);

    ~Client();

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
};
