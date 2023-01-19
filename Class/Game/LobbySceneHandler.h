#pragma once

#include"../utils.hpp"
#include "../memory.hpp"
#include "../Data/offsets.hpp"

class LobbySceneHandler
{
public:



    LobbySceneHandler() {
        this->memory = nullptr;
    }

    LobbySceneHandler(IN Memory* memory) {
        this->memory = memory;
    }

    int64_t address = NULL;

    void setMemory(IN Memory* memory) {
        this->memory = memory;
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

    bool update() {
        //无效指针
        //invalid pointer address
        if (this->memory == NULL || this->address == NULL) {
            return false;
        }

        int64_t playerController = memory->read_mem<int64_t>(this->address + Offsets::LocalPlayer::ptr_playerController, NULL);

        return true;
    }

    //检查该地址是LocalPlayer实例
    bool validateAddress(IN int64_t address) {

        int64_t localPlayerClass = memory->read_mem<int64_t>(memory->gameAssemblyBaseAddress + GameAssembly::Class::ptr_LocalPlayerClass, NULL);

        if (localPlayerClass == NULL) {
            //Error finding class
            return false;
        }

        int64_t localPlayerClass_ = memory->read_mem<int64_t>(address, NULL);

        return localPlayerClass == localPlayerClass_;
    }
};