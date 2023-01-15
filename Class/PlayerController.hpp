#pragma once
#pragma warning(disable:4996)

#include"../Data/offsets.hpp"
#include"../Struct/Common.hpp"
#include"../Memory.hpp"

#include<Windows.h>

class PlayerController {
public:
    PlayerController() {
        this->memory = nullptr;
    }

    PlayerController(IN Memory* memory) {
        this->memory = memory;
    }

    ~PlayerController() {

    }

    //最大记录范围
    const float f_maxRangeRecordingPlayersNearby = 5.0f;

    //记录死亡时附近的玩家信息
    bool b_hasRecordedPlayersNearby = false;
    std::vector<PlayerController> playersNearbyOnDeath;

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
    std::string roleName = "";

    Vector3 v3_position{ 0.0f, 0.0f, 0.0f };

    void setMemory(IN Memory* memory) {
        this->memory = memory;
    }

    void reset() {
        this->address = 0;

        resetMemberFields();
    }

    void resetMemberFields() {
        b_hasRecordedPlayersNearby = false;
        this->playersNearbyOnDeath.clear();

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
        roleName = "";
        v3_position = { 0.0f, 0.0f, 0.0f };
    }

    /// <summary>
    /// 添加其他玩家的数据到playersNearbyOnDeath向量保存起来
    /// </summary>
    /// <param name="suspectKiller">可能的凶手</param>
    void addPlayersNearby(IN PlayerController* suspectKiller) {
        //TODO
    }

    /// <summary>
    /// 传送玩家到指定的点
    /// </summary>
    /// <param name="position"></param>
    bool teleportTo(IN const Vector2& to) {
        //无效指针
        //invalid pointer address
        if (this->address == NULL) {
            return false;
        }

        //检查是否为本地玩家
        if (!this->b_isLocal) {
            return false;
        }

        std::vector<int64_t> offsets = {
            Offsets::PlayerController::ptr_Rigidbody2D,
            Offsets::Rigidbody2D::ptr_UnknownClass0,
            Offsets::Rigidbody2D::UnknownClass0::ptr_UnknownFields,
            Offsets::Rigidbody2D::UnknownClass0::UnknownFields::v2_position
        };

        int64_t position = memory->FindPointer(this->address, offsets);

        //读取要写入的位置的地址无效
        if (position == NULL) {
            return false;
        }

        try {
            memory->write_mem<Vector2>(position, to);
        }
        catch (...) {
            return false;
        }

        return true;
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
            this->v3_position = memory->read_mem<Vector3>(this->address + Offsets::PlayerController::v3_position, {0.0f, 0.0f, 0.0f});
        }
        catch (...) {
            return false;
        }

        return true;
    }

    //TODO: 导致程序崩溃？
    void updateNickname() {
        if (!PlayerController::validateAddress(this->address)) {
            return;
        }

        int64_t nickname = memory->read_mem<int64_t>(this->address + Offsets::PlayerController::fl_nickname, NULL);
        int64_t firstChar = nickname + 0x14;

        //字符个数
        int length = memory->read_mem<int>(nickname + 0x10, 0);

        char16_t buffer[40];


        if (length == 0) {
            buffer[0] = 0;
            return;
        }

        for (int i = 0; i < length; i++) {
            char16_t c = memory->read_mem<char16_t>(firstChar + sizeof(char16_t) * i, NULL);
            //byte byte_= *(p_str + i);
            buffer[i] = c;
        }
        buffer[length] = 0;

        //std::wstring wstr(reinterpret_cast<wchar_t*>(buffer), length);
        //this->nickname = utils.wstring2string(wstr);

        this->nickname = utils.u8From16(buffer);
    }

    /// <summary>
    /// Reset and update after replacing address
    /// </summary>
    /// <param name="address"></param>
    /// <returns></returns>
    bool update(IN int64_t address) {
        if (address == NULL) {
            return false;
        }

        if (!validateAddress(address)) {
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

    std::vector<PlayerController*> getPlayersNearby() {

    }

    //检查该地址是PlayerController实例
    bool validateAddress(IN int64_t address) {


        int64_t playerControllerClass = memory->read_mem<int64_t>(memory->gameAssemblyBaseAddress + GameAssembly::Class::ptr_PlayerControllerClass, NULL);


        if (playerControllerClass == NULL) {
            //Error finding class
            return false;
        }

        int64_t playerControllerClass_ = memory->read_mem<int64_t>(address, NULL);

        return playerControllerClass == playerControllerClass_;
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
        if (this->memory == NULL || this->address == NULL) {
            return false;
        }

        //更新昵称
        if (this->nickname[0] == 0) {
            updateNickname();
        }

        //局内角色已确定(游戏开始)
        b_isPlayerRoleSet = memory->read_mem<bool>(this->address + Offsets::PlayerController::b_isPlayerRoleSet, false);
        //是否为本地玩家
        b_isLocal = memory->read_mem<bool>(this->address + Offsets::PlayerController::b_isLocal, false);

        if (b_isPlayerRoleSet) {
            //更新玩家坐标
            updatePosition();

            b_inVent = memory->read_mem<bool>(this->address + Offsets::PlayerController::b_inVent, false);
            b_hasBomb = memory->read_mem<bool>(this->address + Offsets::PlayerController::b_hasBomb, false);
            b_isGhost = memory->read_mem<bool>(this->address + Offsets::PlayerController::b_isGhost, false);
            b_isSpectator = memory->read_mem<bool>(this->address + Offsets::PlayerController::b_isSpectator, false);
            invisibilityDistance = memory->read_mem<int>(this->address + Offsets::PlayerController::fl_invisibilityDistance, -1);
            b_isRemoteSpectating = memory->read_mem<bool>(this->address + Offsets::PlayerController::b_isRemoteSpectating, false);
            b_hasKilledThisRound = memory->read_mem<bool>(this->address + Offsets::PlayerController::b_hasKilledThisRound, false);
            i_playerRoleId = memory->read_mem<int>(memory->read_mem<int64_t>(this->address + Offsets::PlayerController::fl_playerRoleId, 0) + 0x10, 0);

            float timeOfDeath = memory->read_mem<int>(this->address + Offsets::PlayerController::i_timeOfDeath, 0);

            //玩家刚死亡
            if (i_timeOfDeath == 0 && timeOfDeath > 0) {
                this->onDeath();
            }

            //更新死亡时间
            i_timeOfDeath = timeOfDeath;

            std::u8string rolename = utils.getRoleName(i_playerRoleId);
            roleName = std::string(rolename.begin(), rolename.end());
        }
        return true;
    }

    /// <summary>
    /// 玩家死亡事件
    /// </summary>
    void onDeath() {
        //g_client->onPlayerDeath(this);
    }
};