#pragma once

#include"Client.h"

#include"Class/Hack.hpp"

extern Hack hack;

Client::Client(Memory* memory, HackSettings* hackSettings)
{
    this->memory = memory;
    this->hackSettings = hackSettings;

    localPlayer.setMemory(this->memory);
    for (int i = 0; i < n_players; i++) {
        playerControllers.push_back(new PlayerController(this->memory));
    }
}

Client::~Client() {
    for (auto ptr_playerController : playerControllers) {
        delete ptr_playerController;
    }
    playerControllers.clear();
}

/// <summary>
/// 重置GUI设置
/// </summary>
void Client::resetGuiSettings() {
    HackSettings* hackSettings = this->hackSettings;
    GuiSettings* guiSettings = &hackSettings->guiSettings;


    //reset player's target speed
    guiSettings->f_movementSpeed = this->hackSettings->gameOriginalData.f_baseMovementSpeed;
    guiSettings->b_alwaysEnableNoclip = false;
    guiSettings->b_disableFogOfWar = false;
}

/// <summary>
/// 重置所有玩家的数据
/// </summary>
void Client::resetPlayersData() {
    for (auto playerController : this->playerControllers) {
        playerController->resetMemberFields();
    }
}

void Client::onLocalPlayerQuitGame() {
    Client::onGameEnded();
}

/// <summary>
/// 游戏开始
/// </summary>
void Client::onGameStarted() {
    //更新游戏内初始数据
    updateGameOriginalData();
}

/// <summary>
/// 游戏结束
/// </summary>
void Client::onGameEnded() {
    //TODO: 重置玩家数据，例如死亡时附近的玩家
    resetPlayersData();
    //TODO: 重置hack中的激活状态
    hack.resetActivationStates();
    //重置Gui设置
    resetGuiSettings();
}

/// <summary>
/// 有玩家死亡
/// </summary>
/// <param name="playerController"></param>
void Client::onPlayerDeath(IN PlayerController* playerController) {
    //TODO: 统计死亡位置附近的玩家
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
    hackSettings->gameOriginalData.f_baseMovementSpeed = localPlayer.getBaseMovementSpeed();
}

[[deprecated("Replaced with GUI, won't do anything now")]]
void Client::printAllPlayersInfo(Utils* utils) {

    const char separator = '-';
    const int nameWidth = 15;

    /*
    std::cout << std::format("{:25}{:15}{:15}{:10}\n\n",
        utils ? utils->str("Nickname", "玩家昵称") : "玩家昵称",
        utils ? utils->str("Rolename", "角色") : "角色",
        utils ? utils->str("KilledThisRound", "本轮杀过人") : "本轮杀过人",
        utils ? utils->str("DeadTime", "死亡时间") : "死亡时间");
    */

    /*
    std::cout << std::left << std::setw(25) << std::setfill(separator) << (utils ? utils->str("Nickname", "玩家昵称") : "玩家昵称");
    std::cout << std::left << std::setw(15) << std::setfill(separator) << (utils ? utils->str("Rolename", "角色") : "角色");
    std::cout << std::left << std::setw(15) << std::setfill(separator) << (utils ? utils->str("KilledThisRound", "本轮杀过人") : "本轮杀过人");
    std::cout << std::left << std::setw(10) << std::setfill(separator) << (utils ? utils->str("DeadTime", "死亡时间") : "死亡时间");
    */
    //std::cout << std::left << std::setfill(separator) << (utils ? utils->str("Pos", "坐标") : "坐标");

    for (int i = 0; i < Client::n_players; ++i) {
        PlayerController* ptr_PlayerController = this->playerControllers[i];

        if (ptr_PlayerController->address == NULL) {
            continue;
        }

        bool killedThisRound = ptr_PlayerController->b_hasKilledThisRound;
        int deathTime = ptr_PlayerController->i_timeOfDeath;

        /*

        std::cout << std::format("{:25}{:15}{:15}{:10}",
            ptr_PlayerController->nickname,
            ptr_PlayerController->roleName,
            killedThisRound ? (utils ? utils->str("Yes", "是") : "是") : "",
            ptr_PlayerController->i_timeOfDeath ? std::to_string((ptr_PlayerController->i_timeOfDeath)) : ""
        );
        */

        /*
        std::cout << std::left << std::setw(25) << std::setfill(separator) << (*iterator).nickname;
        std::cout << std::left << std::setw(15) << std::setfill(separator) << (*iterator).roleName;
        std::cout << std::left << std::setw(15) << std::setfill(separator) << (killedThisRound ? (utils ? utils->str("Yes", "是") : "是") : "");
        std::cout << std::left << std::setw(10) << std::setfill(separator);
        */

        //    std::cout << std::left << std::setfill(separator) << "(" << (*iterator).pos.x << ", " << (*iterator).pos.y << ")";
        std::cout << '\n';
    }
    std::cout << std::endl;
}