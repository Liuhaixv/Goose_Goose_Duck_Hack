#pragma once

#include "memory.hpp"
#include "Struct/HackSettings.hpp"

class Client
{
public:
    HackSettings* hackSettings = nullptr;

    PlayerController playerControllers[16];

	Client(Memory memory)
	{
		this->memory = &memory;
	}

    Client(Memory memory, HackSettings hackSettings)
    {
        this->memory = &memory;
        this->hackSettings = &hackSettings;
    }

    Memory* getMemory() {
        return this->memory;
    }

  
    //
    //穿墙模式

    /// <summary>
    /// Enable noclip to disable collider, which makes you able to walk through any obstacle like walls and tables<paragm/>
    /// 1
    /// </summary>
    /// <param name="localPlayer"></param>
    /// <param name="enable"></param>
    static void noclip(PlayerController* localPlayer, bool enable = true) {
    
    }

    static void printAllPlayersInfo(std::list<PlayerController> playerControllers, Utils* utils = nullptr) {
        std::list<PlayerController>::iterator iterator = playerControllers.begin();

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

        for (int i = 0; iterator != playerControllers.end(); ++iterator, ++i) {
            if (!(*iterator).address) {
                continue;
            }

            bool killedThisRound = (*iterator).b_hasKilledThisRound;
            int deathTime = (*iterator).i_timeOfDeath;

            std::cout << std::format("{:25}{:15}{:15}{:10}",
                (*iterator).nickname,
                (*iterator).roleName,
                killedThisRound ? (utils ? utils->str("Yes", "是") : "是") : "",
                (*iterator).i_timeOfDeath ? std::to_string(((*iterator).i_timeOfDeath)) : ""
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
