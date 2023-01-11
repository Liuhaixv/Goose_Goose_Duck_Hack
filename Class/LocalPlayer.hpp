#pragma once

#include"../utils.hpp"
#include"../Data/offsets.hpp"
#include"../Struct/Common.hpp"
#include"../memory.hpp"
#include"PlayerController.hpp"

#include<Windows.h>

class LocalPlayer {
public:
    LocalPlayer() {
        this->memory = nullptr;
        playerController.setMemory(this->memory);
    }

    LocalPlayer(Memory* memory) {
        this->memory = memory;
        playerController.setMemory(this->memory);
    }

    int64_t address = NULL;

    PlayerController playerController;
  
    /// <summary>
    /// 获取移动速度
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
               Offsets::LocalPlayer::Class::StaticField::f_movementSpeed};

            int64_t addr = memory->FindPointer(this->address, offsets);
            if (addr == NULL) {
                return -1;
            }

            float result = memory->read_mem<float>(addr);

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

    void setMemory(Memory* memory) {
        this->memory = memory;
        playerController.setMemory(this->memory);
    }

    void reset() {
        this->address = 0;

        resetMemberFields();
    }

    void resetMemberFields() {
        
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

    bool update() {
        //无效指针
        //invalid pointer address
        if (this->memory == NULL || this->address == NULL) {
            return false;
        }

        int64_t playerController = memory->read_mem<int64_t>(this->address + Offsets::LocalPlayer::ptr_playerController);
        this->playerController.update(playerController);

        return true;
    }
};