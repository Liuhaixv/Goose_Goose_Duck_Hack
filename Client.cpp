#pragma once

#include"Client.h"

#include"Class/Hack.hpp"
#include "Class/DebugConsole.h"
#include "Class/HttpDataUpdater.h"

extern Hack hack;
extern HackSettings hackSettings;
extern DebugConsole debugConsole;

extern HttpDataUpdater httpDataUpdater;

Client::Client()
{
    for (int i = 0; i < n_players; i++) {
        playerControllers.push_back(new PlayerController());
    }
}

Client::~Client() {
    for (auto ptr_playerController : playerControllers) {
        delete ptr_playerController;
    }
    if (playerControllers.size() > 0)
        playerControllers.clear();
}

/// <summary>
/// 重置GUI设置
/// </summary>
void Client::resetGuiSettings() {
    GuiSettings* guiSettings = &hackSettings.guiSettings;

    //reset player's target speed
    guiSettings->f_movementSpeed = hackSettings.gameOriginalData.f_baseMovementSpeed;
    guiSettings->b_alwaysEnableNoclip = false;
    guiSettings->b_disableFogOfWar = false;
}

/// <summary>
/// 重置所有玩家的数据
/// </summary>
void Client::resetAllPlayersData() {

    debugConsole.log(DebugType::FUNCTION, "ResetAllPlayersData");

    for (auto playerController : this->playerControllers) {
        playerController->resetMemberFields();
    }
}

void Client::onLocalPlayerQuitGame() {

    //TODO: bug
    //debugConsole.log(DebugType::EVENT, "OnLocalPlayerQuitGame");

    Client::onGameEnded();
}

void Client::onEnteringRoom()
{
    //TODO: 不知道干啥
    debugConsole.log(DebugType::EVENT, "OnEnteringRoom");
}

/// <summary>
/// 游戏开始
/// </summary>
void Client::onGameStarted() {

    debugConsole.log(DebugType::EVENT, "OnGameStarted");

    //更新游戏内初始数据
    //updateGameOriginalData();

    //更新游戏开始时间
    time(&this->time_gameStart);

    UserInfo userInfo = getUserInfo();

    //上传用户信息
    httpDataUpdater.addHttpTask(std::bind(HttpTask::uploadUserInfo, userInfo));
}

/// <summary>
/// 游戏结束
/// </summary>
void Client::onGameEnded() {

    debugConsole.log(DebugType::EVENT, "OnGameEnded");

    //TODO: 重置玩家数据，例如死亡时附近的玩家
    resetAllPlayersData();
    //TODO: 重置hack中的激活状态
    hack.resetActivationStates();
    //重置Gui设置
    resetGuiSettings();

    this->time_gameStart = NULL;
}

/// <summary>
/// 有玩家死亡
/// </summary>
/// <param name="playerController"></param>
void Client::onPlayerDeath(IN PlayerController* playerController) {
    //TODO: 统计死亡位置附近的玩家
}

bool Client::gameHasStarted()
{
    return this->time_gameStart != NULL;
}

bool Client::inGameScene()
{
    return lobbySceneHandler.b_InGameScene;
}

bool Client::localPlayerReadied()
{
    return this->localPlayer.playerController.hasReadied();
}

bool Client::teleportTo(IN const Vector2& to) {
    if (this->localPlayer.address == NULL) {
        return false;
    }

    PlayerController* playerController = &this->localPlayer.playerController;

    if (playerController == NULL || playerController->address == NULL) {
        return false;
    }

    return playerController->teleportTo(to);
}

void Client::updateGameOriginalData() {
    //hackSettings.gameOriginalData.f_baseMovementSpeed = localPlayer.getBaseMovementSpeed();
}

//Private


UserInfo Client::getUserInfo()
{
    UserInfo userInfo;

    std::string GaggleID = "";
    //野猪佩奇
    std::string nickname = "";
    //w4rhMT8ZJseijNoZqJC8szCucXr2
    std::string userId = "";

    return UserInfo();
}