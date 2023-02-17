#pragma once

#include "Memory.h"
#include "Struct/HackSettings.hpp"
#include "Class/Game/PlayerController.h"
#include"Class/Game/LocalPlayer.h"
#include "Class/Game/LobbySceneHandler.h"
#include "Struct/UserInfo.h"

class Client
{
public:

    static const int n_players = 16;
    LobbySceneHandler lobbySceneHandler;
    LocalPlayer localPlayer;
    std::vector<PlayerController*> playerControllers;

    Client();

    ~Client();

    /// <summary>
    /// 重置GUI设置
    /// </summary>
    void resetGuiSettings();

    /// <summary>
    /// 重置所有玩家的数据
    /// </summary>
    void resetAllPlayersData();

    /// <summary>
    /// 本地玩家退出游戏
    /// </summary>
    void onLocalPlayerQuitGame();

    /// <summary>
    /// 本地玩家进入游戏房间，或直接进入游戏（比如休息室）
    /// </summary>
    void onEnteringRoom();

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

    /// <summary>
    /// 获取游戏已进行时间
    /// </summary>
    /// <returns></returns>
    int timeSinceGameStarted() {
        if (this->time_gameStart == NULL) {
            return -1;
        }

        return time(NULL) - this->time_gameStart;
    }

    /// <summary>
    /// 游戏是否已经开始
    /// </summary>
    /// <returns></returns>
    bool gameHasStarted();

    /// <summary>
    /// 玩家是否在房间/游戏中
    /// </summary>
    /// <returns></returns>
    bool inGameScene();

    /// <summary>
    /// 返回本地玩家是否已经准备
    /// </summary>
    /// <returns></returns>
    bool localPlayerReadied();

    void updateGameOriginalData();

    [[deprecated("Replaced with GUI, won't do anything now")]]
    //void printAllPlayersInfo(Utils* utils = nullptr);

    bool teleportTo(IN const Vector2& to);

    /// <summary>
    /// 修改相机远近距离
    /// </summary>
    /// <param name="size"></param>
    /// <returns></returns>
    bool overrideOrthographicSize(float size);

private:
    //上一次游戏开始的时间
    time_t time_gameStart = NULL;

    UserInfo getUserInfo();
};
