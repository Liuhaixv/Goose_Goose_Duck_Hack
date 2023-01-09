#pragma once
#pragma warning(disable:4996)

#include"../Class/offsets.hpp"
#include"../Struct/Common.hpp"

#include<Windows.h>

class PlayerController {
public:
    PlayerController() {
        this->address = NULL;
    }

    //Init instance with known PlayerController address
    PlayerController(int64_t ptr_address) {
        this->address = ptr_address;
    }

    ~PlayerController() {
        if (this->address) {
            reset();
        }
    }

    int64_t address = NULL;

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

    int i_playerRoleId = 0;
    int invisibilityDistance = 0;
    int i_timeOfDeath = 0;

    std::string nickname = "";
    char roleName[64] = "";
    Vector3 v3_position{ 0.0f, 0.0f, 0.0f };

    void reset() {
        this->address = 0;

        resetMemberFields();
    }

    void resetMemberFields() {
        b_isSilenced = false;
        b_isInfected = false;
        b_isPlayerRoleSet = false;
        b_inVent = false;
        b_hasBomb = false;
        b_isGhost = false;
        b_isLocal = false;
        b_isSpectator = false;
        b_isRemoteSpectating = false;
        b_hasKilledThisRound = false;

        i_playerRoleId = 0;
        invisibilityDistance = 0;
        i_timeOfDeath = 0;

        nickname = "";
        roleName[0] = '\0';
        v3_position = { 0.0f, 0.0f, 0.0f };
    }

    /// <summary>
    /// 更新玩家坐标信息<para/>
    /// Update player's position and returns true if data valid 
    /// </summary>
    /// <param name="PlayerController"></param>
    /// <returns></returns>
    bool updatePosition() {
        //无效指针
        //invalid pointer address
        if (this->address == NULL) {
            return false;
        }

        try {
            this->v3_position = memory->read_mem<Vector3>(this->address + Offsets::PlayerController::v3_position);
        }
        catch (...) {
            return false;
        }

        return true;
    }



    /// <summary>
    /// 更新玩家数据<para/>
    /// Update player's data and returns true if data valid 
    /// </summary>
    /// <param name="PlayerController"></param>
    /// <returns>玩家数据是否有效</returns>
    bool update() {
        //无效指针
        //invalid pointer address
        if (this->address == NULL) {
            return false;
        }

        //wchar_t tmpNick[42] = L"";
        byte tmpNick[42]{};

        int64_t* nickname_obj = (memory->read_mem<int64_t*>(this->address + Offsets::PlayerController::fl_nickname));
        byte* p_str = (byte*)(nickname_obj)+0x14;

        int64_t length = memory->read_mem<int>(memory->read_mem<int64_t>(this->address + Offsets::PlayerController::fl_nickname) + 0x10);

        byte* p_tmpNick = tmpNick;
        for (int i = 0; i < length * 2 + 1; i++) {
            byte byte_ = memory->read_mem<byte>((int64_t)(p_str + i));
            //byte byte_= *(p_str + i);
            *(p_tmpNick + i) = byte_;
        }

        //memcpy(tmpNick, p_str, length * 2 + 1);
        //memory->copy_bytes((int64_t)&addr, (int64_t)tmpNick, length * 2 + 1);

        int _size = 0;
        //for (int i = 0; i < 64; i++) {
        //	if (tmpNick[i] == 0) {
        //		break;
        //	}
        //	_size++;
        //}

        std::wstring string_to_convert = std::wstring(reinterpret_cast<wchar_t*>(tmpNick), length);

        nickname = Utils::wstring2string(string_to_convert);

        v3_position = memory->read_mem<Vector3>(this->address + Offsets::PlayerController::v3_position);

        b_isPlayerRoleSet = memory->read_mem<bool>(this->address + Offsets::PlayerController::b_isPlayerRoleSet);
        b_isLocal = memory->read_mem<bool>(this->address + Offsets::PlayerController::b_isLocal);

        if (b_isPlayerRoleSet) {
            b_inVent = memory->read_mem<bool>(this->address + Offsets::PlayerController::b_inVent);
            b_hasBomb = memory->read_mem<bool>(this->address + Offsets::PlayerController::b_hasBomb);
            b_isGhost = memory->read_mem<bool>(this->address + Offsets::PlayerController::b_isGhost);
            b_isSpectator = memory->read_mem<bool>(this->address + Offsets::PlayerController::b_isSpectator);
            invisibilityDistance = memory->read_mem<int>(this->address + Offsets::PlayerController::fl_invisibilityDistance);
            b_isRemoteSpectating = memory->read_mem<bool>(this->address + Offsets::PlayerController::b_isRemoteSpectating);
            b_hasKilledThisRound = memory->read_mem<bool>(this->address + Offsets::PlayerController::b_hasKilledThisRound);
            i_playerRoleId = memory->read_mem<int>(memory->read_mem<int64_t>(this->address + Offsets::PlayerController::fl_playerRoleId) + 0x10);

            //更新死亡时间
            i_timeOfDeath = memory->read_mem<int>(this->address + Offsets::PlayerController::i_timeOfDeath);
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
                                //disable fow
                                //set layermask
                                memory->write_mem<int>(fogOfWarHandler + Offsets::FogOfWarHandler::i_layerMask, 0);

                                //7.5 is enough to see the whole screen
                                //f_baseViewDistance * f_viewDistanceMultiplier = 6 * 1.25 = 7.5
                                float f_viewDistanceMultiplier = memory->read_mem<float>(fogOfWarHandler + Offsets::FogOfWarHandler::f_viewDistanceMultiplier);
                                if (f_viewDistanceMultiplier != 0) {
                                    memory->write_mem<float>(fogOfWarHandler + Offsets::FogOfWarHandler::f_baseViewDistance, 7.5 / f_viewDistanceMultiplier);
                                }
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