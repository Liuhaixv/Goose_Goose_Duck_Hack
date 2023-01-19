﻿#pragma once

#include"../utils.hpp"
#include "../memory.hpp"
#include "../Data/offsets.hpp"

class LobbySceneHandler
{
public:
    int64_t address = NULL;

    int64_t ptr_tasksHandler = NULL;
    bool  b_InGameScene = false;
    bool  b_ExploreMode = false;
    int   i_CurrentMap = -1;//当前的地图
    bool  b_IsPublicGame = false;
    bool  b_IsMicEnabled = false;

    LobbySceneHandler() {
        this->memory = nullptr;
    }

    LobbySceneHandler(IN Memory* memory) {
        this->memory = memory;
    }

    void setMemory(IN Memory* memory) {
        this->memory = memory;
    }

    void reset() {
        this->address = 0;

        resetMemberFields();
    }

    void resetMemberFields() {
        ptr_tasksHandler = NULL;
        b_InGameScene = false;
        b_ExploreMode = false;
        i_CurrentMap = -1;
        b_IsPublicGame = false;
        b_IsMicEnabled = false;
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

    bool update() {
        //无效指针
        //invalid pointer address
        if (this->memory == NULL || this->address == NULL) {
            return false;
        }

        static std::vector<int64_t> offsets = GameAssembly::lobbySceneHandler_staticFiled();

        int64_t staticFiled_Addr = memory->FindPointer(memory->gameAssemblyBaseAddress, offsets);

        if (staticFiled_Addr == NULL) {
            return false;
        }

        ptr_tasksHandler = memory->read_mem<int64_t>(this->address + Offsets::LobbySceneHandler::ptr_tasksHandler, NULL);

        b_InGameScene = memory->read_mem<bool>(staticFiled_Addr + Offsets::LobbySceneHandler::Class::StaticField::b_InGameScene, false);
        b_ExploreMode = memory->read_mem<bool>(staticFiled_Addr + Offsets::LobbySceneHandler::Class::StaticField::b_ExploreMode, false);
        i_CurrentMap = memory->read_mem<int>(staticFiled_Addr + Offsets::LobbySceneHandler::Class::StaticField::i_CurrentMap, false);
        b_IsPublicGame = memory->read_mem<bool>(staticFiled_Addr + Offsets::LobbySceneHandler::Class::StaticField::b_IsPublicGame, false);
        b_IsMicEnabled = memory->read_mem<bool>(staticFiled_Addr + Offsets::LobbySceneHandler::Class::StaticField::b_IsMicEnabled, false);

        return true;
    }

    //检查该地址是LobbySceneHandler实例
    bool validateAddress(IN int64_t address) {

        int64_t lobbySceneHandlerClass = memory->read_mem<int64_t>(memory->gameAssemblyBaseAddress + GameAssembly::Class::ptr_LobbySceneHandlerClass, NULL);

        if (lobbySceneHandlerClass == NULL) {
            //Error finding class
            return false;
        }

        int64_t lobbySceneHandlerClass_ = memory->read_mem<int64_t>(address, NULL);

        return lobbySceneHandlerClass == lobbySceneHandlerClass_;
    }
};