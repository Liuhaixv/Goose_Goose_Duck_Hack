#pragma once

#include<list>

#include"../memory.hpp"
#include"PlayerController.hpp"

/// <summary>
/// Thread functions that update data.
/// </summary>
class DataUpdater {
public:
    DataUpdater(Client* client) {
        this->memory = client->getMemory();
    }

    int validPlayersNum = 0;

    /// <summary>
    /// 更新玩家信息的线程函数<para/>
    /// Thread that keeps updating data of PlayerControllers using RPM
    /// </summary>
    /// <param name="playerControllers"></param>
    /// <param name="num">Number of players</param>
    void playerControllerUpdater(PlayerController playerControllers[], int num) {
        while (true) {
            updatePlayerController(playerControllers, num);
            Sleep(1000);
        }
    }
private:
    Memory* memory;

    /// <summary>
    /// 更新玩家PlayerController信息
    /// </summary>
    /// <param name="playerIndex">玩家的索引，范围为[0,15],当index为0时为LocalPlayer</param>
    /// <param name="playerController">要更新的PlayerController</param>
    void updatePlayerController(PlayerController playerControllers[], int num) {
        int validPlayers = 0;

        //遍历所有PlayerController
        for (int i = 0; i < num; ++i) {
            //获取当前遍历的玩家槽位中的PlayerController
            //Current pointer of iterated PlayerController in array
            PlayerController* ptr_playerController = &(playerControllers[i]);

            //获取内存中对应玩家槽位的实例地址
            std::vector<int64_t> offsets = Offsets::GameAssembly::playerControllerByIndex(i);
            int64_t playerControllerAddr = memory->FindPointer(memory->gameAssemblyBaseAddress, offsets);

            //内存中当前玩家槽位不存在数据
            //Data not available
            if (playerControllerAddr == NULL) {
                continue;
            }

            //槽位对应的内存地址不符，可能是因为有玩家退出游戏导致槽位玩家变更
            //if (ptr_playerController->address != playerControllerAddr) {
            //    ptr_playerController->update(playerControllerAddr);
            //}

            //读取内存并更新
            //Update from memory
            bool successUpdated = ptr_playerController->update(playerControllerAddr);

            if (successUpdated) {
                validPlayers++;
            }
        }
        //更新有效玩家信息
        //Update valid players' num
        this->validPlayersNum = validPlayers;
    }
};
