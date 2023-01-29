#pragma once

#include"../Client.h"
#include"../Class/Game/PlayerController.h"
#include"../Data/offsets.hpp"
#include"../Enum/ActivationState.hpp"

#include"../Struct/ObscuredTypes.hpp"

extern Memory memory;
extern Utils utils;
extern HackSettings hackSettings;

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

    /// <summary>
    /// 重置当前激活状态
    /// </summary>
    void resetActivationStates() {
        this->b_hasDisabledFOW = false;
        this->b_hasEnabledNoclip = false;
    }

    //只需要激活一次
    void removeFogOfWar(PlayerController* localPlayerController) {

        ActivationState state = utils.shouldActivateOnce(hackSettings.guiSettings.b_disableFogOfWar, &this->b_hasDisabledFOW);
        if (state == IDLE_DO_NOTHING) {
            return;
        }

        bool shouldEnableCollider = false;
        if (state == SHOULD_ACTIVATE_NOW) {
            //修改fog of war
            if (localPlayerController->b_isLocal) {
                if (this->client) {
                    //memory.write_mem<bool>(PlayerController + Offsets::PlayerController::b_fogOfWarEnabled, false);

                    int64_t fogOfWarHandler_addr = memory.FindPointer(memory.gameAssemblyBaseAddress, GameAssembly::localPlayer()) + Offsets::LocalPlayer::ptr_fogOfWarHandler;
                    int64_t fogOfWarHandler = memory.read_mem<int64_t>(fogOfWarHandler_addr, NULL);

                    if (memory.read_mem<bool>(fogOfWarHandler + Offsets::FogOfWarHandler::b_targetPlayerSet, false)) {
                        //disable FOW
                        //set layermask
                        //由于layerMask已被加密，需要同时修改加密的数值

                        //TODO
                        //memory.read_mem<ObscuredInt>(fogOfWarHandler + Offsets::FogOfWarHandler::struct_obscured_layerMask, NULL);

                        memory.write_mem<int>(fogOfWarHandler + Offsets::FogOfWarHandler::i_layerMask, 0);

                        


                        //7.5 is enough to see the whole screen
                        //f_baseViewDistance * f_viewDistanceMultiplier = 6 * 1.25 = 7.5
                        //TODO:测试 数值已加密，暂时无法读写
                        //float f_viewDistanceMultiplier = memory.read_mem<float>(fogOfWarHandler + Offsets::FogOfWarHandler::f_viewDistanceMultiplier, 0);
                        //if (f_viewDistanceMultiplier != 0) {
                        //    memory.write_mem<float>(fogOfWarHandler + Offsets::FogOfWarHandler::f_baseViewDistance, 7.5 / f_viewDistanceMultiplier);
                        //}
                    }

                }
            }
        }
        else if (state == SHOULD_DEACTIVATE_NOW) {
            //TODO: 反激活，启用战争迷雾
            if (localPlayerController->b_isLocal) {
                if (this->client) {
                    //memory.write_mem<bool>(PlayerController + Offsets::PlayerController::b_fogOfWarEnabled, false);

                    int64_t fogOfWarHandler_addr = memory.FindPointer(memory.gameAssemblyBaseAddress, GameAssembly::localPlayer()) + Offsets::LocalPlayer::ptr_fogOfWarHandler;
                    int64_t fogOfWarHandler = memory.read_mem<int64_t>(fogOfWarHandler_addr, NULL);

                    if (memory.read_mem<bool>(fogOfWarHandler + Offsets::FogOfWarHandler::b_targetPlayerSet, false)) {
                        //enable fow
                        //set layermask
                        memory.write_mem<int>(fogOfWarHandler + Offsets::FogOfWarHandler::i_layerMask, 131090);
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
    bool enableNoclip(PlayerController* localPlayer, bool shouldEnable) {
        ActivationState state = utils.shouldActivateOnce(shouldEnable, &this->b_hasEnabledNoclip);
        if (state == IDLE_DO_NOTHING) {
            return false;
        }
        bool shouldEnableCollider = false;
        if (state == SHOULD_ACTIVATE_NOW) {
            shouldEnableCollider = false;
        }
        else if (state == SHOULD_DEACTIVATE_NOW) {
            shouldEnableCollider = true;
        }

        std::vector<int64_t> offsets{
                Offsets::PlayerController::ptr_playerCollider,
                Offsets::CapsuleCollider2D::ptr_unknownClass0,
                Offsets::CapsuleCollider2D::UnknownClass0::b_enableCollider };

        int64_t b_enableCollider = memory.FindPointer(localPlayer->address,
            offsets);

        if (b_enableCollider == NULL) {
            return false;
        }

        bool colliderEnabled = memory.read_mem<bool>(b_enableCollider, false);

        //enable就是启用穿墙，即禁用碰撞
        if (colliderEnabled != shouldEnableCollider) {
            memory.write_mem<bool>(b_enableCollider, shouldEnableCollider);
        }
    }

    void noclip(PlayerController* localPlayerController) {
        //开启穿墙
        if (localPlayerController && localPlayerController->b_isLocal) {
            if (this->client) {
                enableNoclip(localPlayerController,
                    hackSettings.guiSettings.b_alwaysEnableNoclip || hackSettings.tempEnableNoclip);
            }
        }
    }

    bool speedHack(LocalPlayer* localPlayer) {
        //目标速度
        float targetSpeed = -1.0f;

        //检查是否启用移速hack
        ActivationState state = utils.shouldActivateOnce(hackSettings.guiSettings.b_enableSpeedHack, &this->b_enabledSpeedHackLastTime);

        switch (state) {
            case SHOULD_DEACTIVATE_NOW:
                //TODO: 用户关闭了speedhack，恢复正常移速

                targetSpeed = hackSettings.gameOriginalData.f_baseMovementSpeed;
                break;

                //speedhack没有被切换开关
            case IDLE_DO_NOTHING:
                //没有开启
                if (!hackSettings.guiSettings.b_enableSpeedHack) {
                    return false;
                }
                else {
                    //继续向下执行逻辑
                    [[fallthrough]];
                }

                //用户刚刚开启speedhack
            case SHOULD_ACTIVATE_NOW:
                //TODO: 检查启用状态下当前目标速度是否更新
                targetSpeed = hackSettings.guiSettings.f_movementSpeed;

                //和上一次设置的速度一样，无变化，直接返回
                if (this->f_lastTimeSetMovementSpeed == hackSettings.guiSettings.f_movementSpeed) {
                    return false;
                }
                break;
        }

        //修改移速逻辑从这里开始       

        if (targetSpeed < 0) {
            targetSpeed = hackSettings.gameOriginalData.f_baseMovementSpeed;
        }

        //修改移速
        if (localPlayer) {
            std::vector<int64_t> offsets = {
                Offsets::LocalPlayer::ptr_Class,
                Offsets::LocalPlayer::Class::ptr_staticFields,
                Offsets::LocalPlayer::Class::StaticField::f_movementSpeed
            };
            int64_t movementSpeed_addr = memory.FindPointer(localPlayer->address, offsets);

            memory.write_mem<float>(movementSpeed_addr, targetSpeed);

            //更新GUI显示的设置速度
            hackSettings.guiSettings.f_movementSpeed = targetSpeed;
            //更新上一次设置的速度
            this->f_lastTimeSetMovementSpeed = targetSpeed;
        }
    }

    void zoomHack(LocalPlayer* localPlayer)
    {
        float targetZoomSize = hackSettings.guiSettings.f_zoomSize;

        //未启用zoomHack
        if (!hackSettings.guiSettings.b_enableZoomHack) {
            return;
        }

        //检查是否启用移速hack
        ActivationState state = utils.shouldActivateOnce(hackSettings.guiSettings.f_zoomSize, &this->f_lastTimeZoomSize);

        if (state == ActivationState::SHOULD_ACTIVATE_NOW) {
            //Override覆写相机距离
            overrideOrthographicSize(localPlayer, targetZoomSize);
        }
    }

    /// <summary>
    /// 设置相机的远近距离
    /// </summary>
    /// <param name="size"></param>
    /// <returns></returns>
    bool overrideOrthographicSize(LocalPlayer* localPlayer, float targetZoomSize)
    {
        if (localPlayer) {
            //启用Override
            std::vector<int64_t> offsets1 = {
                Offsets::LocalPlayer::ptr_cinemachineStateDrivenCamera,
                Offsets::CinemachineStateDrivenCamera::ptr_m_AnimatedTarget
            };
            int64_t changeValue_addr = memory.FindPointer(localPlayer->address, offsets1);

            memory.write_mem<int64_t>(changeValue_addr, 0);

            //修改zoomSize
            std::vector<int64_t> offsets2 = {
                Offsets::LocalPlayer::ptr_cinemachineVirtualCamera,
                Offsets::CinemachineVirtualCamera::f_m_zoomSize
            };
            int64_t zoomSize_addr = memory.FindPointer(localPlayer->address, offsets2);

            memory.write_mem<float>(zoomSize_addr, targetZoomSize);

            return true;
        }
        return false;
    }

private:
    Client* client = nullptr;

    bool b_hasDisabledFOW = false;
    bool b_hasEnabledNoclip = false;

    //上一次设置移速hack是否开启
    bool b_enabledSpeedHackLastTime = false;
    //上一次设置的玩家移速
    float f_lastTimeSetMovementSpeed = -1;
    float f_lastTimeZoomSize = -1;
};