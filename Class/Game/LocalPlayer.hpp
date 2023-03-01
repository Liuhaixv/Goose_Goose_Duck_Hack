﻿#pragma once

#include"../utils.hpp"
#include"../Struct/Common.hpp"
#include"../Memory.h"
#include"PlayerController.h"
#include"../Data/offsets.hpp"

#include"../../Struct/ObscuredTypes.hpp"

#include<Windows.h>

extern Memory memory;
class LocalPlayer {
public:
    int64_t address = NULL;

    PlayerController playerController;

    /// <summary>
    /// 获取当前移动速度
    /// </summary>
    /// <returns></returns>
    float getMovementSpeed() {
        //无效指针
        //invalid pointer address
        if (this->address == NULL) {
            return -1;
        }
        try {
            std::vector<int64_t> offsets = {
                Offsets::LocalPlayer::ptr_Class,
               Offsets::LocalPlayer::Class::ptr_staticFields,
               Offsets::LocalPlayer::Class::StaticField::struct_movementSpeed };

            int64_t movementSpeed_addr = memory.FindPointer(this->address, offsets);

            if (movementSpeed_addr == NULL) {
                return -1;
            }

            ObscuredFloat movementSpeed;
            memory.read_mem_EX<ObscuredFloat>(movementSpeed_addr, movementSpeed);

            float f_movementSpeed = movementSpeed.Decrypt();

            if (f_movementSpeed <= 0) {
                return -1;
            }

            return f_movementSpeed;
        }
        catch (...) {
            return -1;
        }
    }
    /// <summary>
    /// 获取初始速度
    /// </summary>
    /// <returns></returns>
    float getBaseMovementSpeed() {
        //无效指针
        //invalid pointer address
        if (this->address == NULL) {
            return -1;
        }
        try {
            std::vector<int64_t> offsets = {
                Offsets::LocalPlayer::ptr_Class,
               Offsets::LocalPlayer::Class::ptr_staticFields,
               Offsets::LocalPlayer::Class::StaticField::f_baseMovementSpeed };

            int64_t addr = memory.FindPointer(this->address, offsets);
            if (addr == NULL) {
                return -1;
            }

            float result = memory.read_mem<float>(addr, -1.0f);

            if (result <= 0) {
                return -1;
            }

            return result;
        }
        catch (...) {
            return -1;
        }

        return true;
    }

    void reset() {
        this->address = 0;

        resetMemberFields();
    }

    void resetMemberFields() {
        this->playerController.reset();
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
    bool update() {
        //无效指针
        //invalid pointer address
        if (this->address == NULL) {
            return false;
        }

        int64_t playerController = memory.read_mem<int64_t>(this->address + Offsets::LocalPlayer::ptr_playerController, NULL);
        this->playerController.update(playerController);

        return true;
    }

    //检查该地址是LocalPlayer实例
    bool validateAddress(IN int64_t address) {

        int64_t localPlayerClass = memory.read_mem<int64_t>(memory.gameAssemblyBaseAddress + GameAssembly::Class::ptr_LocalPlayerClass, NULL);

        if (localPlayerClass == NULL) {
            //Error finding class
            return false;
        }

        int64_t localPlayerClass_ = memory.read_mem<int64_t>(address, NULL);

        return localPlayerClass == localPlayerClass_;
    }
};