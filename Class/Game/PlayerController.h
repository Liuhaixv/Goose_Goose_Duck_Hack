﻿#pragma once

#include<Windows.h>
#include<vector>
#include<string>
#include"../Struct/Common.hpp"

#include "../../Enum/ReadyState.hpp"

class Client;
class PlayerController {
public:
    PlayerController();

    ~PlayerController();

    PlayerController& operator=(PlayerController& o);

    //最大记录范围
    const float f_maxRangeRecordingPlayersNearby = 2000000.0f;

    //记录死亡时附近的玩家信息
    bool b_hasRecordedPlayersNearby = false;
    std::vector<PlayerController> playersNearbyOnDeath;

    int64_t address = NULL;

    int64_t playerRole = NULL;

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

    //准备状态
    int i_readyState = ReadyState::ReadyState_NotReady;

    std::string nickname = "";
    std::string roleName = "";

    Vector3 v3_position{ 0.0f, 0.0f, 0.0f };

    void reset();

    void resetMemberFields();

    /// <summary>
    /// 添加其他玩家的数据到playersNearbyOnDeath向量保存起来
    /// </summary>
    /// <param name="suspectKiller">可能的凶手</param>
    void addPlayersNearby(IN PlayerController* suspectKiller);

    /// <summary>
    /// 传送玩家到指定的点
    /// </summary>
    /// <param name="position"></param>
    bool teleportTo(IN const Vector2& to);

    /// <summary>
    /// 是否已经准备
    /// </summary>
    /// <returns></returns>
    bool hasReadied();

    /// <summary>
    /// 更新玩家坐标信息<para/>
    /// Update player's position and returns true if data valid 
    /// </summary>
    /// <param name="PlayerController"></param>
    /// <returns></returns>
    bool updatePosition();

    //TODO: 导致程序崩溃？
    void updateNickname();

    /// <summary>
    /// Reset and update after replacing address
    /// </summary>
    /// <param name="address"></param>
    /// <returns></returns>
    bool update(IN int64_t address);

private:
    std::vector<PlayerController*> getPlayersNearby();

    //检查该地址是PlayerController实例
    bool validateAddress(IN int64_t address);

    /// <summary>
    /// 更新玩家数据<para/>
    /// Update player's data and returns true if data valid 
    /// </summary>
    /// <param name="PlayerController"></param>
    /// <returns>玩家数据是否有效</returns>
    bool update();

    /// <summary>
    /// 玩家死亡事件
    /// </summary>
    void onDeath();
};