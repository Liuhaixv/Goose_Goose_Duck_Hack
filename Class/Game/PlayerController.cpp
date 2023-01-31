﻿#pragma once
#pragma warning(disable : 4996)

#include "PlayerController.h"

#include "../Data/offsets.hpp"
#include "../Client.h"
#include"string.hpp"

extern Client g_client;
extern Memory memory;
PlayerController::PlayerController()
{
}

PlayerController::~PlayerController()
{
}

PlayerController& PlayerController::operator=(PlayerController& o)
{
    this->b_isSilenced = o.b_isSilenced;
    this->b_isInfected = o.b_isInfected;
    this->b_isPlayerRoleSet = o.b_isPlayerRoleSet;
    this->b_inVent = o.b_inVent;
    this->b_hasBomb = o.b_hasBomb;
    this->b_isGhost = o.b_isGhost;
    this->b_isLocal = o.b_isLocal;
    this->b_isSpectator = o.b_isSpectator;
    this->b_isRemoteSpectating = o.b_isRemoteSpectating;
    // 本轮是否杀过人
    this->b_hasKilledThisRound = o.b_hasKilledThisRound;

    this->i_playerRoleId = o.i_playerRoleId;
    this->invisibilityDistance = o.invisibilityDistance;
    this->i_timeOfDeath = o.i_timeOfDeath;

    this->nickname = o.nickname;
    this->roleName = o.roleName;

    this->v3_position = o.v3_position;

    return *this;
}

void PlayerController::reset()
{
    this->address = 0;

    resetMemberFields();
}

void PlayerController::resetMemberFields()
{
    b_hasRecordedPlayersNearby = false;
    if (this->playersNearbyOnDeath.size() > 0)
        this->playersNearbyOnDeath.clear();

    playerRole = NULL;

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

    i_readyState = ReadyState::ReadyState_NotReady;
}

/// <summary>
/// 添加其他玩家的数据到playersNearbyOnDeath向量保存起来
/// </summary>
/// <param name="suspectKiller">可能的凶手</param>
void PlayerController::addPlayersNearby(IN PlayerController* suspectKiller)
{
    // TODO
    if (suspectKiller == NULL)
    {
        return;
    }

    PlayerController suspect;
    suspect = *(suspectKiller);

    this->playersNearbyOnDeath.push_back(suspect);
}

/// <summary>
/// 传送玩家到指定的点
/// </summary>
/// <param name="position"></param>
bool PlayerController::teleportTo(IN const Vector2& to)
{
    // 无效指针
    // invalid pointer address
    if (this->address == NULL)
    {
        return false;
    }

    // 检查是否为本地玩家
    if (!this->b_isLocal)
    {
        return false;
    }

    std::vector<int64_t> offsets = {
        Offsets::PlayerController::ptr_Rigidbody2D,
        Offsets::Rigidbody2D::ptr_UnknownClass0,
        Offsets::Rigidbody2D::UnknownClass0::ptr_UnknownFields,
        Offsets::Rigidbody2D::UnknownClass0::UnknownFields::v2_position };

    int64_t position = memory.FindPointer(this->address, offsets);

    // 读取要写入的位置的地址无效
    if (position == NULL)
    {
        return false;
    }

    try
    {
        memory.write_mem<Vector2>(position, to);
    }
    catch (...)
    {
        return false;
    }

    return true;
}

bool PlayerController::hasReadied()
{
    return this->i_readyState == ReadyState::ReadyState_Ready;
}

/// <summary>
/// 更新玩家坐标信息<para/>
/// Update player's position and returns true if data valid
/// </summary>
/// <param name="PlayerController"></param>
/// <returns></returns>
bool PlayerController::updatePosition()
{
    // 无效指针
    // invalid pointer address
    if (this->address == NULL)
    {
        return false;
    }
    try
    {
        this->v3_position = memory.read_mem<Vector3>(this->address + Offsets::PlayerController::v3_position, { 0.0f, 0.0f, 0.0f });
    }
    catch (...)
    {
        return false;
    }

    return true;
}

// TODO: 导致程序崩溃？
void PlayerController::updateNickname()
{
    if (!PlayerController::validateAddress(this->address))
    {
        return;
    }

    int64_t nickname_Addr = memory.read_mem<int64_t>(this->address + Offsets::PlayerController::ptr_nickname, NULL);

    this->nickname = string(nickname_Addr).get_std_string();
}

/// <summary>
/// Reset and update after replacing address
/// </summary>
/// <param name="address"></param>
/// <returns></returns>
bool PlayerController::update(IN int64_t address)
{
    if (address == NULL)
    {
        return false;
    }

    if (!validateAddress(address))
    {
        return false;
    }

    if (address != this->address)
    {
        reset();
        this->address = address;
    }

    return update();
}

std::vector<PlayerController*> PlayerController::getPlayersNearby()
{
}

// 检查该地址是PlayerController实例
bool PlayerController::validateAddress(IN int64_t address)
{

    int64_t playerControllerClass = memory.read_mem<int64_t>(memory.gameAssemblyBaseAddress + GameAssembly::Class::ptr_PlayerControllerClass, NULL);

    if (playerControllerClass == NULL)
    {
        // Error finding class
        return false;
    }

    int64_t playerControllerClass_ = memory.read_mem<int64_t>(address, NULL);

    return playerControllerClass == playerControllerClass_;
}

/// <summary>
/// 更新玩家数据<para/>
/// Update player's data and returns true if data valid
/// </summary>
/// <param name="PlayerController"></param>
/// <returns>玩家数据是否有效</returns>
bool PlayerController::update()
{
    // 无效指针
    // invalid pointer address
    if (this->address == NULL)
    {
        return false;
    }

    // 更新昵称
    if (this->nickname[0] == 0)
    {
        updateNickname();
    }

    // TODO: ??? 游戏没开始这个值也是1 局内角色已确定(游戏开始)
    b_isPlayerRoleSet = memory.read_mem<bool>(this->address + Offsets::PlayerController::b_isPlayerRoleSet, false);
    // 是否为本地玩家
    b_isLocal = memory.read_mem<bool>(this->address + Offsets::PlayerController::b_isLocal, false);

    i_readyState = memory.read_mem<int>(this->address + Offsets::PlayerController::i_readyState, ReadyState::ReadyState_NotReady);

    if (b_isPlayerRoleSet)
    {
        // 更新玩家坐标
        updatePosition();

        b_inVent = memory.read_mem<bool>(this->address + Offsets::PlayerController::b_inVent, false);
        b_hasBomb = memory.read_mem<bool>(this->address + Offsets::PlayerController::b_hasBomb, false);
        b_isGhost = memory.read_mem<bool>(this->address + Offsets::PlayerController::b_isGhost, false);
        b_isSpectator = memory.read_mem<bool>(this->address + Offsets::PlayerController::b_isSpectator, false);
        invisibilityDistance = memory.read_mem<int>(this->address + Offsets::PlayerController::fl_invisibilityDistance, -1);
        b_isRemoteSpectating = memory.read_mem<bool>(this->address + Offsets::PlayerController::b_isRemoteSpectating, false);
        b_hasKilledThisRound = memory.read_mem<bool>(this->address + Offsets::PlayerController::b_hasKilledThisRound, false);
        playerRole = memory.read_mem<int64_t>(this->address + Offsets::PlayerController::ptr_playerRole, NULL);
        i_playerRoleId = memory.read_mem<int>(playerRole + 0x10, 0);

        float timeOfDeath = memory.read_mem<int>(this->address + Offsets::PlayerController::i_timeOfDeath, 0);

        // 玩家刚死亡
        if (i_timeOfDeath == 0 && timeOfDeath > 0)
        {
            this->onDeath();
        }

        // 更新死亡时间
        i_timeOfDeath = timeOfDeath;

        std::u8string rolename = utils.getRoleName(i_playerRoleId);
        roleName = std::string(rolename.begin(), rolename.end());

    }

    return true;
}

/// <summary>
/// 玩家死亡事件
/// </summary>
void PlayerController::onDeath()
{
    for (PlayerController* suspectKiller : g_client.playerControllers) {
        if (!suspectKiller || suspectKiller->address == NULL) {
            continue;
        }

        //凶手不太可能是自己
        //You may not be the killer yourself
        if (suspectKiller->address == this->address) {
            continue;
        }

        //是否需要记录死亡玩家？暂时选择记录
        //TODO: Should we record players who died near another dead player?
        static bool shouldWeRecordDeadSuspect = true;

        if (suspectKiller->i_timeOfDeath > 0) {
            //嫌疑人已死亡
            //Suspect who is near dead player is dead too
            if (!shouldWeRecordDeadSuspect) {
                continue;
            }
        }
        else {
            //嫌疑人是活人
            //判断距离
            if (suspectKiller->v3_position.distanceTo(this->v3_position, true) <= this->f_maxRangeRecordingPlayersNearby) {
                //在第一个位置放入死亡玩家的信息拷贝，防止死亡玩家乱跑导致位置错误
                if (this->playersNearbyOnDeath.size() == 0) {
                    this->addPlayersNearby(this);
                }
                this->addPlayersNearby(suspectKiller);
            }
            else {
                //嫌疑人距离过远，不太可能是凶手
                //Suspect too far from dead player
            }
        }
    }
}