#pragma once

#include "TasksHandler.h"

class LobbySceneHandler
{
public:
    int64_t address = NULL;

    TasksHandler tasksHandler;
    bool  b_InGameScene = false;
    bool  b_ExploreMode = false;
    int   i_CurrentMap = -1;//当前的地图
    bool  b_IsPublicGame = false;
    bool  b_IsMicEnabled = false;

    bool b_gameStarted = false;

    LobbySceneHandler() {
    }

    void reset() {
        this->address = 0;

        resetMemberFields();
    }

    void resetMemberFields();

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
    bool update();

    //检查该地址是LobbySceneHandler实例
    bool validateAddress(IN int64_t address);
};