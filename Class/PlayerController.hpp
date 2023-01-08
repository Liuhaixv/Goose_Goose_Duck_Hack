#pragma once
#pragma warning(disable:4996)

#include"../utils.hpp"
#include"../Class/offsets.hpp"
#include"../Struct/Common.hpp"

#include<Windows.h>

class PlayerController {
public:
    PlayerController(Memory memory) {
        this->memory = &memory;
    }

    PlayerController(Memory memory, Client client) {
        this->memory = &memory;
        this->client = &client;
    }

    ~PlayerController() {
    }


    //Mark if the player is valid(false if no player's info valid)
    //标记当前玩家槽位是否有效(当玩家中途离开或该槽位无人占用导致内存中无有效玩家数据时为false)
    bool valid = false;

    DWORD_PTR ptr_PlayerController = 0;
    int i_playerRoleId = 0;
    bool b_isSilenced = false;
    bool b_isInfected = false;
    bool b_isPlayerRoleSet = false;
    bool b_inVent = false;
    bool b_hasBomb = false;
    bool b_isGhost = false;
    bool b_isLocal = false;
    bool b_isSpectator = false;
    bool b_isRemoteSpectating = false;

    //本轮是否杀过人
    bool b_hasKilledThisRound = false;

    int invisibilityDistance = 0;
    int i_timeOfDeath = 0;

    std::string nickname = "";
    char roleName[64] = "";
    Vector3 pos{ 0.0f, 0.0f, 0.0f };

    static void printAllPlayersInfo(std::list<PlayerController> playerControllers, Utils* utils = nullptr) {
        std::list<PlayerController>::iterator iterator = playerControllers.begin();

        const char separator = '-';
        const int nameWidth = 15;

        std::cout << std::format("{:25}{:15}{:15}{:10}\n",
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
        std::cout << '\n';

        for (int i = 0; iterator != playerControllers.end(); ++iterator, ++i) {
            if (!(*iterator).valid) {
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

    void markAsInvalidPlayer() {
        this->valid = false;
    }

    void markAsValidPlayer() {
        this->valid = true;
    }

    void reset() {
        ptr_PlayerController = 0;
        b_isSilenced = false;
        b_isInfected = false;
        i_playerRoleId = -1;
        b_isPlayerRoleSet = false;
        b_inVent = false;
        b_hasBomb = false;
        b_isGhost = false;
        b_isLocal = false;
        invisibilityDistance = 0;
        b_isSpectator = false;
        b_isRemoteSpectating = false;
        nickname = "";
        roleName[0] = '\0';
        pos = { 0.0f, 0.0f };

        b_hasKilledThisRound = false;

        i_timeOfDeath = 0;
    }

    /// <summary>
    /// 更新玩家坐标信息<para/>
    /// Update player's position and returns true if data valid 
    /// </summary>
    /// <param name="PlayerController"></param>
    /// <returns></returns>
    bool updatePosition(int64_t PlayerController) {

        //无效玩家数据
        //invalid data
        if (PlayerController == NULL) {
            markAsInvalidPlayer();
            return false;
        }

        if (PlayerController == ptr_PlayerController) {
            pos = memory->read_mem<Vector3>(PlayerController + Offsets::PlayerController::v3_position);
        }

        return true;
    }

    //https://blog.csdn.net/mercy_ps/article/details/81218608
    std::string wstring2string(std::wstring wstr) {
        std::string result;

        int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
        char* buffer = new char[len + 1];

        WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
        buffer[len] = '\0';

        result.append(buffer);
        delete[] buffer;
        return result;
    }

    /// <summary>
    /// 更新玩家数据<para/>
    /// Update player's data and returns true if data valid 
    /// </summary>
    /// <param name="PlayerController"></param>
    /// <returns>玩家数据是否有效</returns>
    bool update(int64_t PlayerController) {

        //无效玩家数据
        //invalid data
        if (PlayerController == NULL) {
            markAsInvalidPlayer();
            return false;
        }
        else {
            ptr_PlayerController = PlayerController;
            markAsValidPlayer();
        }

        wchar_t tmpNick[42] = L"";

        int64_t addr_0 = memory->read_mem<int64_t>(PlayerController + Offsets::PlayerController::fl_nickname);
        byte* addr = (byte*)memory->read_mem<int64_t>(addr_0 + 0x14);
        int64_t size = sizeof(wchar_t) * memory->read_mem<int>(memory->read_mem<int64_t>(PlayerController + Offsets::PlayerController::fl_nickname) + 0x10) + 1;

        memcpy(tmpNick, &addr, size);

        tmpNick[size] = 0;

        int _size = 0;
        //for (int i = 0; i < 64; i++) {
        //	if (tmpNick[i] == 0) {
        //		break;
        //	}
        //	_size++;
        //}

        std::wstring string_to_convert = std::wstring(reinterpret_cast<wchar_t*>(tmpNick), size);

        nickname = wstring2string(string_to_convert);

        pos = memory->read_mem<Vector3>(PlayerController + Offsets::PlayerController::v3_position);

        b_isPlayerRoleSet = memory->read_mem<bool>(PlayerController + Offsets::PlayerController::b_isPlayerRoleSet);
        b_isLocal = memory->read_mem<bool>(PlayerController + Offsets::PlayerController::b_isLocal);

        if (b_isPlayerRoleSet) {
            b_inVent = memory->read_mem<bool>(PlayerController + Offsets::PlayerController::b_inVent);
            b_hasBomb = memory->read_mem<bool>(PlayerController + Offsets::PlayerController::b_hasBomb);
            b_isGhost = memory->read_mem<bool>(PlayerController + Offsets::PlayerController::b_isGhost);
            b_isSpectator = memory->read_mem<bool>(PlayerController + Offsets::PlayerController::b_isSpectator);
            invisibilityDistance = memory->read_mem<int>(PlayerController + Offsets::PlayerController::fl_invisibilityDistance);
            b_isRemoteSpectating = memory->read_mem<bool>(PlayerController + Offsets::PlayerController::b_isRemoteSpectating);
            b_hasKilledThisRound = memory->read_mem<bool>(PlayerController + Offsets::PlayerController::b_hasKilledThisRound);
            i_playerRoleId = memory->read_mem<int>(memory->read_mem<int64_t>(PlayerController + Offsets::PlayerController::fl_playerRoleId) + 0x10);

            //更新死亡时间
            i_timeOfDeath = memory->read_mem<int>(PlayerController + Offsets::PlayerController::i_timeOfDeath);
            strcpy(roleName, utils.getRoleName(i_playerRoleId));

            //修改fog of war
            if (b_isLocal) {
                if (this->client) {
                    HackSettings* hackSettings = this->client->hackSettings;
                    if (hackSettings) {
                        if (hackSettings->disableFogOfWar) {                        
                            //memory->write_mem<bool>(PlayerController + Offsets::PlayerController::b_fogOfWarEnabled, false);

                            int64_t fogOfWarHandler_addr = memory->FindPointer(memory->gameAssemblyBaseAddress, Offsets::GameAssembly::localPlayer()) + Offsets::LocalPlayer::ptr_fogOfWarHandler;
                            int64_t fogOfWarHandler = memory->read_mem<int64_t>(fogOfWarHandler_addr);

                            if (memory->read_mem<bool>(fogOfWarHandler + Offsets::FogOfWarHandler::b_targetPlayerSet)) {
                                memory->write_mem<int>(fogOfWarHandler + Offsets::FogOfWarHandler::i_layerMask, 0);
                                memory->write_mem<float>(fogOfWarHandler + Offsets::FogOfWarHandler::f_viewDistanceMultiplier, 5.0);
                            }
                        }
                    }
                }
            }     
        }

        return true;
    }

private:

    Utils utils;
    Memory* memory = nullptr;
    Client* client = nullptr;
};