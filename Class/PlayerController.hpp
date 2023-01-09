#pragma once
#pragma warning(disable:4996)

#include"../Class/offsets.hpp"
#include"../Struct/Common.hpp"
#include"../memory.hpp"

#include<Windows.h>

class PlayerController {
public:
    PlayerController() {
        this->memory = nullptr;
    }

    PlayerController(Memory* memory) {
        this->memory = memory;
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

    void setMemory(Memory* memory) {
        this->memory = memory;
    }

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

    void updateNickname() {
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

        std::wstring string_to_convert = std::wstring(reinterpret_cast<wchar_t*>(tmpNick), length);

        this->nickname = Utils::wstring2string(string_to_convert);
    }

    /// <summary>
    /// Reset and update after replacing address
    /// </summary>
    /// <param name="address"></param>
    /// <returns></returns>
    bool update(int64_t address) {
        if (address == NULL) {
            return false;
        }

        if (address != this->address) {
            reset();
            this->address = address;
        }

        return update();
    }



private:
    Memory* memory = nullptr;

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

        //更新昵称
        if (this->nickname == "") {
            updateNickname();
        }

        //局内角色已确定(游戏开始)
        b_isPlayerRoleSet = memory->read_mem<bool>(this->address + Offsets::PlayerController::b_isPlayerRoleSet);
        //是否为本地玩家
        b_isLocal = memory->read_mem<bool>(this->address + Offsets::PlayerController::b_isLocal);

        if (b_isPlayerRoleSet) {
            //更新玩家坐标
            updatePosition();

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
        }
        return true;
    }
};