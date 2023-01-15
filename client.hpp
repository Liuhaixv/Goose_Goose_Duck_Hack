#pragma once

#include "Memory.hpp"
#include "Struct/HackSettings.hpp"
#include "Class/PlayerController.h"
#include"Class/LocalPlayer.hpp"
#include<format>

class Client
{
public:
    HackSettings* hackSettings = nullptr;

    static const int n_players = 16;
    LocalPlayer localPlayer;
    std::vector<PlayerController*> playerControllers;

    Client(Memory* memory, HackSettings* hackSettings)
    {
        this->memory = memory;
        this->hackSettings = hackSettings;

        localPlayer.setMemory(this->memory);
        for (int i = 0; i < n_players; i++) {
            playerControllers.push_back(new PlayerController(this->memory));
        }
    }

    ~Client() {
        for (auto ptr_playerController : playerControllers) {
            delete ptr_playerController;
        }
        playerControllers.clear();
    }

    Memory* getMemory() {
        return this->memory;
    }

    /// <summary>
    /// 重置GUI设置
    /// </summary>
    void resetGuiSettings() {
        GuiSettings* a = &this->hackSettings->guiSettings;
        a->f_movementSpeed = this->hackSettings->gameOriginalData.f_baseMovementSpeed;
    }

    /// <summary>
    /// 游戏开始
    /// </summary>
    void onGameStarted() {
        //更新游戏内初始数据
        updateGameOriginalData();
        resetGuiSettings();
    }

    /// <summary>
    /// 游戏结束
    /// </summary>
    void onGameEnded() {
        //TODO: reset player's speed when game finished
        //TODO: add switch to speedHack
        updateGameOriginalData();
        resetGuiSettings();
    }

    /// <summary>
    /// 有玩家死亡
    /// </summary>
    /// <param name="playerController"></param>
    void onPlayerDeath(IN PlayerController* playerController) {
        //TODO: 统计死亡位置附近的玩家
    }
    
    bool teleportTo(IN const Vector2& to) {
        if (this->localPlayer.address == NULL) {
            return false;
        }

        PlayerController* playerController = &this->localPlayer.playerController;

        if (playerController == NULL || playerController->address == NULL) {
            return false;
        }

        return playerController->teleportTo(to);
    }

    void updateGameOriginalData() {
        hackSettings->gameOriginalData.f_baseMovementSpeed = localPlayer.getBaseMovementSpeed();
    }

    void printAllPlayersInfo(Utils* utils = nullptr) {

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

private:
    Memory* memory = 0;
};
