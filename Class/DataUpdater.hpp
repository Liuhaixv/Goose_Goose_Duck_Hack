#pragma once

#include<list>

#include"../Client.h"
#include"../Class/Hack.hpp"
#include"LocalPlayer.hpp"
#include"../utils.hpp"

extern Hack hack;
extern Utils utils;

/// <summary>
/// Thread functions that update data.
/// </summary>
class DataUpdater {
public:
    DataUpdater(Client* client) {
        this->client = client;
        this->memory = client->getMemory();
    }

    int validPlayersNum = 0;

    /// <summary>
    /// 更新玩家信息的线程函数<para/>
    /// Thread that keeps updating data of PlayerControllers using RPM
    /// </summary>
    /// <param name="playerControllers"></param>
    /// <param name="num">Number of players</param>
    void playerControllerUpdater() {
        //PlayerController* playerControllers = client->playerControllers;
        auto playerControllers = client->playerControllers;
        LocalPlayer* localPlayer = &client->localPlayer;
        int players = client->n_players;

        while (true) {

            bool localPlayerUpdated = false;
            updateLocalPlayer(localPlayer, &localPlayerUpdated);

            if (localPlayerUpdated) {
                updatePlayerController(playerControllers, players);
            }
            else {
                //TODO: LocalPlayer更新失败，重置所有玩家
                for (auto ptr_playerController : playerControllers) {
                    if (ptr_playerController->address) {
                        ptr_playerController->reset();
                    }
                }
            }

            Sleep(30);
        }
    }
private:
    Client* client = nullptr;
    Memory* memory = nullptr;

    bool b_isPlayerRoleSet = false;

    /// <summary>
    /// 更新本地玩家
    /// </summary>
    /// <param name="playerController"></param>
    void updateLocalPlayer(LocalPlayer* localPlayer, OUT bool* localPlayerUpdated) {
        //获取内存中对应玩家槽位的实例地址
        std::vector<int64_t> offsets = GameAssembly::localPlayer();
        //offsets.pop_back();
        //offsets.push_back(Offsets::LocalPlayer::ptr_playerController);
        //offsets.push_back(0x0);

        int64_t localPlayerAddr = memory->FindPointer(memory->gameAssemblyBaseAddress, offsets);

        if (localPlayerAddr == NULL) {
            *localPlayerUpdated = false;
            return;
        }

        //Update localplayer
        *localPlayerUpdated = localPlayer->update(localPlayerAddr);

        int64_t localPlayerController = memory->read_mem<int64_t>(localPlayerAddr + Offsets::LocalPlayer::ptr_playerController, NULL);

        //Update playerController
        updatePlayerController(&localPlayer->playerController, localPlayerController);

        if (localPlayer->playerController.address == NULL) {
            localPlayer->playerController.reset();
        }

        if (localPlayer->playerController.b_isPlayerRoleSet) {
            //游戏刚开始
            if (!b_isPlayerRoleSet) {
                client->onGameStarted();
                b_isPlayerRoleSet = true;
            }
        }
        else {
            //游戏刚结束
            if (b_isPlayerRoleSet) {
                client->onGameEnded();
                b_isPlayerRoleSet = false;
                b_isPlayerRoleSet = false;
            }
        }

        if (localPlayer->playerController.address == NULL) {
            return;
        }

        //TODO: 穿墙没有正常工作, 另外需要在每次游戏开始的时候重置修改值
        hack.removeFogOfWar(&localPlayer->playerController);
        hack.noclip(&localPlayer->playerController);//TODO: Bugged 会导致游戏崩溃
        hack.speedHack(localPlayer);
    }

    bool updatePlayerController(PlayerController* dst, int64_t address) {
        //内存中当前玩家槽位不存在数据
        //Data not available
        if (address == NULL) {
            dst->reset();
            return false;
        }

        //槽位对应的内存地址不符，可能是因为有玩家退出游戏导致槽位玩家变更
        //if (ptr_playerController->address != playerControllerAddr) {
        //    ptr_playerController->update(playerControllerAddr);
        //}

        //读取内存并更新
        //Update from memory
        bool successUpdated = dst->update(address);

        return successUpdated;
    }

    /// <summary>
    /// 更新玩家PlayerController信息
    /// </summary>
    /// <param name="playerControllers"></param>
    /// <param name="playersNum"></param>
    void updatePlayerController(std::vector<PlayerController*> playerControllers, int playersNum) {
        int validPlayers = 0;

        //遍历所有PlayerController, 更新数据
        for (int i = 0; i < playersNum; ++i) {
            //获取当前遍历的玩家槽位中的PlayerController
            //Current pointer of iterated PlayerController in array
            PlayerController* ptr_playerController = playerControllers[i];

            //获取内存中对应玩家槽位的实例地址
            std::vector<int64_t> offsets = GameAssembly::playerControllerByIndex(i);
            int64_t playerControllerAddr = memory->FindPointer(memory->gameAssemblyBaseAddress, offsets);

            if (updatePlayerController(ptr_playerController, playerControllerAddr)) {
                validPlayers++;
            }
        }

        //更新有效玩家信息
        //Update valid players' num
        this->validPlayersNum = validPlayers;

        //注意!!!：以下代码已废弃，替换为在玩家死亡事件中处理
        //之所以开两个循环，并在第二个循环才遍历玩家保存死亡数据，是为了防止数据没有被更新

        //判断玩家是否死亡并存储附近玩家数据

        //废弃方法，由于时间复杂度是O(n²)，过于浪费性能。16个玩家就等于要遍历 16*16 = 256次循环
        //替代方法：首先遍历一次所有已经死亡的玩家，将已经死亡但尚未保存过死亡时附近玩家数据的玩家的指针保存起来。
        //接着在第二次遍历中判断每个玩家和已有需要记录死亡数据的玩家的坐标进行范围判断，然后将数据保存到死亡玩家中
        //由于死亡玩家数据只会被记录一次，所以 O(n) ≤时间复杂度≤ O(n²/2)

        /*判断玩家是否死亡并存储附近玩家数据
        for (PlayerController* playerController : g_client->playerControllers) {

            //更新死亡时候附近的玩家
            //当前已死亡
            if (playerController->address && playerController->i_timeOfDeath > 0) {
                //如果没有记录过附近的玩家，开始保存附近玩家数据并保存
                if (!playerController->b_hasRecordedPlayersNearby) {
                    playerController->b_hasRecordedPlayersNearby = true;

                    for (PlayerController* playerController : g_client->playerControllers) {
                        //如果当前遍历的玩家的地址为空或等于当前玩家则continue
                        if (playerController->address == NULL || playerController->address == playerController->address) {
                            continue;
                        }

                        //如果距离过远
                        if (playerController)
                    }
                }
            }
        }
        */

        /*
        static std::vector<PlayerController*> deadPlayersThatHasNotRecordedPlayersNearBy;

        //首先统计需要保存死亡数据的玩家
        for (PlayerController* playerController : playerControllers) {
            if (playerController && playerController->address) {
                //未死亡
                if (playerController->i_timeOfDeath == 0) {
                    continue;
                }

                //已经记录过死亡时附近的玩家。玩家不会死两次，所以跳过该玩家
                //Players can not die twice, right? No need to record other players' info again then.
                if (playerController->b_hasRecordedPlayersNearby) {
                    continue;
                }

                //死亡，且未记录过附近玩家数据
                deadPlayersThatHasNotRecordedPlayersNearBy.push_back(playerController);
            }
        }

        //无人死亡或死亡数据都已记录
        if (deadPlayersThatHasNotRecordedPlayersNearBy.size() == 0) {
            return;
        }

        //开始遍历所有玩家，如果在死亡玩家附近则将自己的数据交给死亡玩家
        for (PlayerController* suspectKiller : playerControllers) {
            if (!suspectKiller || suspectKiller->address == NULL) {
                continue;
            }

            //遍历需要记录数据的死亡玩家
            for (PlayerController* deadPlayer : deadPlayersThatHasNotRecordedPlayersNearBy) {
                //凶手不太可能是自己
                //You may not be the killer yourself
                if (suspectKiller->address == deadPlayer->address) {
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
                    if (suspectKiller->v3_position.distanceTo(deadPlayer->v3_position, true) <= deadPlayer->f_maxRangeRecordingPlayersNearby) {
                        deadPlayer->addPlayersNearby(suspectKiller);
                    }
                    else {
                        //嫌疑人距离过远，不太可能是凶手
                        //Suspect too far from dead player
                    }
                }
            }
        }
        */
    }
};
