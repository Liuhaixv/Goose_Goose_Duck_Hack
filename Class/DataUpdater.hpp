#pragma once

#include<list>

#include"../Client.hpp"
#include"../Class/Hack.hpp"
#include"LocalPlayer.hpp"

extern Hack hack;

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

    void enableFogOfWar(PlayerController* playerController) {
        //
        // MaskLayer int:131090
        //修改fog of war
        if (playerController->b_isLocal) {
            if (this->client && this->client->hackSettings) {
                if (!this->client->hackSettings->disableFogOfWar) {
                    //memory->write_mem<bool>(PlayerController + Offsets::PlayerController::b_fogOfWarEnabled, false);

                    int64_t fogOfWarHandler_addr = memory->FindPointer(memory->gameAssemblyBaseAddress, GameAssembly::localPlayer()) + Offsets::LocalPlayer::ptr_fogOfWarHandler;
                    int64_t fogOfWarHandler = memory->read_mem<int64_t>(fogOfWarHandler_addr, NULL);

                    memory->write_mem<int>(fogOfWarHandler + Offsets::FogOfWarHandler::i_layerMask, 131090);

                    //7.5 is enough to see the whole screen
                    //f_baseViewDistance * f_viewDistanceMultiplier = 6 * 1.25 = 7.5
                    // float f_viewDistanceMultiplier = memory->read_mem<float>(fogOfWarHandler + Offsets::FogOfWarHandler::f_viewDistanceMultiplier);
                    //if (f_viewDistanceMultiplier != 0) {
                    //    memory->write_mem<float>(fogOfWarHandler + Offsets::FogOfWarHandler::f_baseViewDistance, 7.5 / f_viewDistanceMultiplier);
                    //}
                }
            }
        }
    }



    /// <summary>
    /// 更新本地玩家
    /// </summary>
    /// <param name="playerController"></param>
    void updateLocalPlayer(LocalPlayer* localPlayer, bool* localPlayerUpdated) {
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

        hack.removeFogOfWar(&localPlayer->playerController);
        //TODO: Bugged 会导致游戏崩溃
        //hack.noclip(&localPlayer->playerController);
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
    }

    /// <summary>
    /// 更新玩家PlayerController信息
    /// </summary>
    /// <param name="playerControllers"></param>
    /// <param name="playersNum"></param>
    void updatePlayerController(std::vector<PlayerController*> playerControllers, int playersNum) {
        int validPlayers = 0;

        //遍历所有PlayerController
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
    }
};
