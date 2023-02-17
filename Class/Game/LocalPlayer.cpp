#include"LocalPlayer.h"

#include"../Struct/Common.hpp"
#include"../Memory/Memory.h"
#include"../Data/offsets.hpp"

#include"../../Struct/ObscuredTypes.hpp"

extern Memory memory;

/// <summary>
/// 获取当前移动速度
/// </summary>
/// <returns></returns>
float LocalPlayer::getMovementSpeed() {
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

float LocalPlayer::getBaseMovementSpeed() {
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

bool LocalPlayer::update() {
    //无效指针
    //invalid pointer address
    if (this->address == NULL) {
        return false;
    }

    int64_t playerController = memory.read_mem<int64_t>(this->address + Offsets::LocalPlayer::ptr_playerController, NULL);
    this->playerController.update(playerController);

    return true;
}

bool LocalPlayer::validateAddress(IN int64_t address) {

    int64_t localPlayerClass = memory.read_mem<int64_t>(memory.gameAssemblyBaseAddress + GameAssembly::Class::ptr_LocalPlayerClass, NULL);

    if (localPlayerClass == NULL) {
        //Error finding class
        return false;
    }

    int64_t localPlayerClass_ = memory.read_mem<int64_t>(address, NULL);

    return localPlayerClass == localPlayerClass_;
}