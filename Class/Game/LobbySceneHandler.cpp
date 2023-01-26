#include "LobbySceneHandler.h"

#include "../memory.hpp"
#include"../utils.hpp"
#include "../Data/offsets.hpp"
#include "../../Client.h"

extern Client* g_client;
extern Memory memory;
void LobbySceneHandler::resetMemberFields()
    {
        tasksHandler.reset();
        b_InGameScene = false;
        b_ExploreMode = false;
        i_CurrentMap = -1;
        b_IsPublicGame = false;
        b_IsMicEnabled = false;

        b_gameStarted = false;
    }

bool LobbySceneHandler::update()
{
    //无效指针
        //invalid pointer address
    if (this->address == NULL) {
        return false;
    }

    static std::vector<int64_t> offsets = GameAssembly::lobbySceneHandler_staticFiled();

    int64_t staticFiled_Addr = memory.FindPointer(memory.gameAssemblyBaseAddress, offsets);

    if (staticFiled_Addr == NULL) {
        return false;
    }

    int64_t tasksHandler_Addr = memory.read_mem<int64_t>(this->address + Offsets::LobbySceneHandler::ptr_tasksHandler, NULL);

    if (tasksHandler_Addr == NULL) {
        return false;
    }

    this->tasksHandler.update(tasksHandler_Addr);

    if (memory.read_mem<bool>(staticFiled_Addr + Offsets::LobbySceneHandler::Class::StaticField::b_InGameScene, false)) {
        //刚进入房间/游戏
        if (b_InGameScene == false) {
            g_client->onEnteringRoom();
        }
        b_InGameScene = true;
    }
    else {
        //刚退出游戏/房间
        if (b_InGameScene == true) {
            g_client->onLocalPlayerQuitGame();
        }
        b_InGameScene = false;
    }
    b_ExploreMode = memory.read_mem<bool>(staticFiled_Addr + Offsets::LobbySceneHandler::Class::StaticField::b_ExploreMode, false);
    i_CurrentMap = memory.read_mem<int>(staticFiled_Addr + Offsets::LobbySceneHandler::Class::StaticField::i_CurrentMap, false);
    b_IsPublicGame = memory.read_mem<bool>(staticFiled_Addr + Offsets::LobbySceneHandler::Class::StaticField::b_IsPublicGame, false);
    b_IsMicEnabled = memory.read_mem<bool>(staticFiled_Addr + Offsets::LobbySceneHandler::Class::StaticField::b_IsMicEnabled, false);

    if (memory.read_mem<bool>(this->address + Offsets::LobbySceneHandler::b_gameStarted, false)) {
        //非教程模式下
        if (!b_ExploreMode && b_gameStarted == false) {
            g_client->onGameStarted();
        }
        b_gameStarted = true;
    }
    else {
        //非教程模式下
        if (!b_ExploreMode && b_gameStarted == true) {
            g_client->onGameEnded();
        }
        b_gameStarted = false;
    }

    //在房间或游戏中
    if (b_InGameScene) {
        //修改挂机最大时长
        if (hackSettings.guiSettings.b_antiIdleKick) {
            memory.write_mem<float>(this->address + Offsets::LobbySceneHandler::f_inactiveTimeThreshold, 1000000000.0f);
            memory.write_mem<float>(this->address + Offsets::LobbySceneHandler::f_privateInactiveTimeThreshold, 1000000000.0f);
        }
    }

    return true;
}

bool LobbySceneHandler::validateAddress(IN int64_t address) {

    int64_t lobbySceneHandlerClass = memory.read_mem<int64_t>(memory.gameAssemblyBaseAddress + GameAssembly::Class::ptr_LobbySceneHandlerClass, NULL);

    if (lobbySceneHandlerClass == NULL) {
        //Error finding class
        return false;
    }

    int64_t lobbySceneHandlerClass_ = memory.read_mem<int64_t>(address, NULL);

    return lobbySceneHandlerClass == lobbySceneHandlerClass_;
}