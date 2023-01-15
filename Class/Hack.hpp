#pragma once

#include"../Client.hpp"
#include"../Class/PlayerController.h"
#include"../Data/offsets.hpp"

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

    void removeFogOfWar(PlayerController* localPlayerController) {
        //修改fog of war
        if (localPlayerController->b_isLocal) {
            if (this->client && this->client->hackSettings) {
                if (this->client->hackSettings->disableFogOfWar) {

                    //memory->write_mem<bool>(PlayerController + Offsets::PlayerController::b_fogOfWarEnabled, false);
                    Memory* memory = this->client->getMemory();

                    int64_t fogOfWarHandler_addr = memory->FindPointer(memory->gameAssemblyBaseAddress, GameAssembly::localPlayer()) + Offsets::LocalPlayer::ptr_fogOfWarHandler;
                    int64_t fogOfWarHandler = memory->read_mem<int64_t>(fogOfWarHandler_addr, NULL);

                    if (memory->read_mem<bool>(fogOfWarHandler + Offsets::FogOfWarHandler::b_targetPlayerSet, false)) {
                        //disable fow
                        //set layermask
                        memory->write_mem<int>(fogOfWarHandler + Offsets::FogOfWarHandler::i_layerMask, 0);

                        //7.5 is enough to see the whole screen
                        //f_baseViewDistance * f_viewDistanceMultiplier = 6 * 1.25 = 7.5
                        float f_viewDistanceMultiplier = memory->read_mem<float>(fogOfWarHandler + Offsets::FogOfWarHandler::f_viewDistanceMultiplier, 0);
                        if (f_viewDistanceMultiplier != 0) {
                            memory->write_mem<float>(fogOfWarHandler + Offsets::FogOfWarHandler::f_baseViewDistance, 7.5 / f_viewDistanceMultiplier);
                        }
                    }
                }
            }
        }
    }

    /// <summary>
    /// 开启穿墙模式
    /// Enable noclip to disable collider, which makes you able to walk through any obstacle like walls and tables<paragm/>
    /// </summary>
    /// <param name="localPlayer"></param>
    /// <param name="enable"></param>
    bool enableNoclip(PlayerController* localPlayer, bool enable = true) {
        bool shouldEnableCollider = !enable;

        Memory* memory = this->client->getMemory();
        std::vector<int64_t> offsets{
                Offsets::PlayerController::ptr_bodyCollider,
                Offsets::CapsuleCollider2D::ptr_unknownClass0,
                Offsets::CapsuleCollider2D::UnknownClass0::b_enableCollider };

        int64_t b_enableCollider = memory->FindPointer(localPlayer->address,
            offsets);

        if (b_enableCollider == NULL) {
            return false;
        }

        bool colliderEnabled = memory->read_mem<bool>(b_enableCollider, false);

        //enable就是启用穿墙，即禁用碰撞
        if (colliderEnabled != shouldEnableCollider) {
            memory->write_mem<bool>(b_enableCollider, shouldEnableCollider);
        }
    }

    void noclip(PlayerController* localPlayerController) {
        //开启穿墙
        if (localPlayerController && localPlayerController->b_isLocal) {
            if (this->client && this->client->hackSettings) {
                if (this->client->hackSettings->guiSettings.b_alwaysEnableNoclip) {
                    enableNoclip(localPlayerController);
                }
                else {
                    enableNoclip(localPlayerController, this->client->hackSettings->enableNoclip);
                }
            }
        }
    }

    //TODO: reset player's speed when game finished
    //TODO: reset player's speed when disabled speedHack
    void speedHack(LocalPlayer* localPlayer) {
        if (!this->client->hackSettings->guiSettings.b_enableSpeedHack) {
            return;
        }

        float targetSpeed = this->client->hackSettings->guiSettings.f_baseMovementSpeed;

        if (targetSpeed < 0) {
            targetSpeed = this->client->hackSettings->gameOriginalData.f_baseMovementSpeed;
            this->client->hackSettings->guiSettings.f_baseMovementSpeed = targetSpeed;
        }else{
            Memory* memory = this->client->getMemory();
            //开启穿墙
            if (localPlayer) {
                std::vector<int64_t> offsets = {
                    Offsets::LocalPlayer::ptr_Class,
                    Offsets::LocalPlayer::Class::ptr_staticFields,
                    Offsets::LocalPlayer::Class::StaticField::f_movementSpeed
                };
                int64_t baseMovementSpeed_addr = memory->FindPointer(localPlayer->address, offsets);

                memory->write_mem<float>(baseMovementSpeed_addr, targetSpeed);
            }
        }
    }

private:
    Client* client = nullptr;
};