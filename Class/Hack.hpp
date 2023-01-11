#pragma once

#include"../client.hpp"
#include"../Class/PlayerController.hpp"

class Hack {
public:
    //Must set client
    Hack() {
    }

    Hack(Client* client) {
        this->client = client;
    }

    void setClient(Client* client) {
        this->client = client;
    }

    void removeFogOfWar(PlayerController* playerController) {
        //修改fog of war
        if (playerController->b_isLocal) {
            if (this->client && this->client->hackSettings) {
                if (this->client->hackSettings->disableFogOfWar) {

                    //memory->write_mem<bool>(PlayerController + Offsets::PlayerController::b_fogOfWarEnabled, false);
                    Memory* memory = this->client->getMemory();

                    int64_t fogOfWarHandler_addr = memory->FindPointer(memory->gameAssemblyBaseAddress, Offsets::GameAssembly::localPlayer()) + Offsets::LocalPlayer::ptr_fogOfWarHandler;
                    int64_t fogOfWarHandler = memory->read_mem<int64_t>(fogOfWarHandler_addr);

                    if (memory->read_mem<bool>(fogOfWarHandler + Offsets::FogOfWarHandler::b_targetPlayerSet)) {
                        //disable fow
                        //set layermask
                        memory->write_mem<int>(fogOfWarHandler + Offsets::FogOfWarHandler::i_layerMask, 0);

                        //7.5 is enough to see the whole screen
                        //f_baseViewDistance * f_viewDistanceMultiplier = 6 * 1.25 = 7.5
                        float f_viewDistanceMultiplier = memory->read_mem<float>(fogOfWarHandler + Offsets::FogOfWarHandler::f_viewDistanceMultiplier);
                        if (f_viewDistanceMultiplier != 0) {
                            memory->write_mem<float>(fogOfWarHandler + Offsets::FogOfWarHandler::f_baseViewDistance, 7.5 / f_viewDistanceMultiplier);
                        }
                    }
                }
            }
        }
    }

    void noclip(PlayerController* playerController) {
        //开启穿墙
        if (playerController->b_isLocal) {
            if (this->client && this->client->hackSettings) {
                if (this->client->hackSettings->guiSettings.b_alwaysEnableNoclip) {
                    client->noclip(playerController, 1);
                }
                else {
                    client->noclip(playerController, this->client->hackSettings->enableNoclip);
                }
            }
        }
    }

private:
    Client* client = nullptr;
};