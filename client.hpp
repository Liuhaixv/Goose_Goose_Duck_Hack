#pragma once

#include "memory.hpp"
#include "Struct/HackSettings.hpp"
#include "Class/PlayerController.hpp"
#include<format>

class Client
{
public:
    HackSettings* hackSettings = nullptr;

    static const int n_players = 16;
    PlayerController localPlayer;
    PlayerController playerControllers[n_players];

    Client(Memory* memory, HackSettings* hackSettings = nullptr)
    {
        this->memory = memory;
        this->hackSettings = hackSettings;

        localPlayer.setMemory(this->memory);
        for (int i = 0; i < n_players; i++) {
            playerControllers[i].setMemory(this->memory);
        }
    }

    Memory* getMemory() {
        return this->memory;
    }

    //穿墙模式

    /// <summary>
    /// 开启穿墙模式
    /// Enable noclip to disable collider, which makes you able to walk through any obstacle like walls and tables<paragm/>
    /// </summary>
    /// <param name="localPlayer"></param>
    /// <param name="enable"></param>
    bool noclip(PlayerController* localPlayer, bool enable = true) {
        std::vector<int64_t> offsets{
                Offsets::PlayerController::ptr_bodyCollider,
                Offsets::CapsuleCollider2D::i_unknownClass0,
                Offsets::CapsuleCollider2D::UnknownClass0::b_enableCollider };

        int64_t b_enableCollider = this->memory->FindPointer(localPlayer->address,
            offsets);

        if (b_enableCollider == NULL) {
            return false;
        }

        this->memory->write_mem<bool>(b_enableCollider, !enable);
    }

    void printAllPlayersInfo(Utils* utils = nullptr) {

        const char separator = '-';
        const int nameWidth = 15;

        std::cout << std::format("{:25}{:15}{:15}{:10}\n\n",
            utils ? utils->str("Nickname", "玩家昵称") : "玩家昵称",
            utils ? utils->str("Rolename", "角色") : "角色",
            utils ? utils->str("KilledThisRound", "本轮杀过人") : "本轮杀过人",
            utils ? utils->str("DeadTime", "死亡时间") : "死亡时间");

        /*
        std::cout << std::left << std::setw(25) << std::setfill(separator) << (utils ? utils->str("Nickname", "玩家昵称") : "玩家昵称");
        std::cout << std::left << std::setw(15) << std::setfill(separator) << (utils ? utils->str("Rolename", "角色") : "角色");
        std::cout << std::left << std::setw(15) << std::setfill(separator) << (utils ? utils->str("KilledThisRound", "本轮杀过人") : "本轮杀过人");
        std::cout << std::left << std::setw(10) << std::setfill(separator) << (utils ? utils->str("DeadTime", "死亡时间") : "死亡时间");
        */
        //std::cout << std::left << std::setfill(separator) << (utils ? utils->str("Pos", "坐标") : "坐标");

        for (int i = 0; i < Client::n_players; ++i) {
            PlayerController* ptr_PlayerController = &(this->playerControllers[i]);

            if (ptr_PlayerController->address == NULL) {
                continue;
            }

            bool killedThisRound = ptr_PlayerController->b_hasKilledThisRound;
            int deathTime = ptr_PlayerController->i_timeOfDeath;

            std::cout << std::format("{:25}{:15}{:15}{:10}",
                ptr_PlayerController->nickname,
                ptr_PlayerController->roleName,
                killedThisRound ? (utils ? utils->str("Yes", "是") : "是") : "",
                ptr_PlayerController->i_timeOfDeath ? std::to_string((ptr_PlayerController->i_timeOfDeath)) : ""
            );
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
