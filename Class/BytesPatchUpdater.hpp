#pragma once

#include"../Data/offsets.hpp"
#include "../Memory/memory.h"
#include "../Struct/HackSettings.hpp"
#include"../utils.hpp"
#include "../Client.h"
#include"Updater.h"
#include "../Memory/CallHook.h"
#include "../Memory/CodeCave/CodeCave.h"
#include "../Memory/JmpHook.h"

extern HackSettings hackSettings;
extern Utils utils;
extern Memory memory;
extern Client g_client;

extern CodeCave codeCave;

/// <summary>
/// 负责patch汇编区指令字节。
/// 由于目前没有使用到hook框架，该类暂时也负责维护hook功能
/// </summary>
class BytesPatchUpdater : public Updater {
public:
    void unhookAll() {
        //LocalPlayer.Update
        if (this->localPlayer_update_hook.b_hasHooked) {
            this->localPlayer_update_hook.unhook();
        }
    }

    void bytesPatchUpdater() {
        while (true) {
            Sleep(100);

            if (this->paused || memory.processHandle == NULL) {
                Sleep(100);
                continue;
            }

            if (this->localPlayer_update_hook.hookEntry == NULL) {
                if (this->localPlayer_update_hook.init()) {
                    if (codeCave.staticFieldEntry == NULL || codeCave.codeEntry == NULL) {
                        codeCave.buildCodeCave(&this->localPlayer_update_hook);
                    }
                }

            }

            //anti AC
            {
                //强制启用anti AC，目前不加入判断是否需要启用
                if (!this->b_antiAC_enabled) {
                    int64_t address = NULL;

                    //禁止退出游戏
                    address = memory.gameAssemblyBaseAddress + GameAssembly::BytesPatch::QuitGame::address;
                    if (memory.gameAssemblyBaseAddress == NULL) {
                        continue;
                    }

                    if (!memory.write_bytes(address, GameAssembly::BytesPatch::QuitGame::disableQuitGame)
                        ) {
                        continue;
                    }


                    //禁止随机种子
                    /*
                    address = memory.gameAssemblyBaseAddress + GameAssembly::BytesPatch::RandomSeed::address;
                    if (address == NULL) {
                        continue;
                    }

                    if (!memory.write_bytes(address,
                        { GameAssembly::BytesPatch::RandomSeed::disableRandomSeed })
                        ) {
                        continue;
                    }*/

                    b_antiAC_enabled = true;
                }
            }

            //不断更换随机彩虹颜色
            {
                ActivationState state = utils.shouldActivateOnce(hackSettings.guiSettings.b_enableRainbowColors, &b_enableRainbowColors);

                if (state == SHOULD_ACTIVATE_NOW) {
                    enableAutoChangeColor();
                }
                else if (state == SHOULD_DEACTIVATE_NOW) {
                    disableAutoChangeColor();
                }
            }

            //one-tap完成任务，一键完成任务
            {
                ActivationState state = utils.shouldActivateOnce(hackSettings.guiSettings.b_oneTapCompletingTask, &b_hasEnabledOneTapCompletingTask);

                if (state == SHOULD_ACTIVATE_NOW) {
                    
                    int64_t address = memory.gameAssemblyBaseAddress + GameAssembly::BytesPatch::SkipPlayingGameToCompleteTask::address;
                    if (address == NULL) {
                        continue;
                    }

                    memory.write_bytes(address,
                            GameAssembly::BytesPatch::SkipPlayingGameToCompleteTask::oneTapCompleteTask                        
                    );
                }
                else if (state == SHOULD_DEACTIVATE_NOW) {
                    //恢复任务
                    int64_t address = memory.gameAssemblyBaseAddress + GameAssembly::BytesPatch::SkipPlayingGameToCompleteTask::address;
                    if (address == NULL) {
                        continue;
                    }

                    memory.write_bytes(address,
                        GameAssembly::BytesPatch::SkipPlayingGameToCompleteTask::raw
                    );
                }
            }

            //Bypass version check
            {
                ActivationState state = utils.shouldActivateOnce(hackSettings.guiSettings.b_bypassVersionCheck, &b_bypassVersionCheck);

                if (state == SHOULD_ACTIVATE_NOW) {

                    if (memory.unityPlayerBaseAddress == NULL) {
                        continue;
                    }

                    //过检测:步骤1 构建CodeCave
                    int64_t address = memory.unityPlayerBaseAddress + UnityPlayer::BytesPatch::BypassVersionCheck::BuildCodeCave::address;

                    memory.write_bytes(address, UnityPlayer::BytesPatch::BypassVersionCheck::BuildCodeCave::bytes);

                    //过检测:步骤2 篡改Jmp至CodeCave
                    address = memory.unityPlayerBaseAddress + UnityPlayer::BytesPatch::BypassVersionCheck::JmpToCodeCave::address;

                    memory.write_bytes(address, UnityPlayer::BytesPatch::BypassVersionCheck::JmpToCodeCave::bypass);
                }
                else if (state == SHOULD_DEACTIVATE_NOW) {
                    //恢复检测
                    if (memory.unityPlayerBaseAddress == NULL) {
                        continue;
                    }

                    //恢复版本检测
                    int64_t address = memory.unityPlayerBaseAddress + UnityPlayer::BytesPatch::BypassVersionCheck::JmpToCodeCave::address;

                    memory.write_bytes(address, UnityPlayer::BytesPatch::BypassVersionCheck::JmpToCodeCave::raw);
                }
            }

            //Bypass normal ban
            {
                ActivationState state = utils.shouldActivateOnce(hackSettings.guiSettings.b_bypassNormalBan, &b_bypassNormalBan);

                if (state == SHOULD_ACTIVATE_NOW) {

                    if (memory.gameAssemblyBaseAddress == NULL) {
                        continue;
                    }

                    //过检测:步骤1
                    int64_t address = memory.gameAssemblyBaseAddress + GameAssembly::BytesPatch::BypassBan::Step1::address;

                    auto patchBytes_step1 = GameAssembly::BytesPatch::BypassBan::Step1::bypassBan;
                    memory.write_bytes(address, { patchBytes_step1 });

                    //过检测:步骤2
                    address = memory.gameAssemblyBaseAddress + GameAssembly::BytesPatch::BypassBan::Step2::address;

                    auto patchBytes_step2 = GameAssembly::BytesPatch::BypassBan::Step2::bypassBan;
                    memory.write_bytes(address, { patchBytes_step2 });
                }
                else if (state == SHOULD_DEACTIVATE_NOW) {
                    //恢复检测
                    if (memory.gameAssemblyBaseAddress == NULL) {
                        continue;
                    }

                    //恢复检测:步骤1
                    int64_t address = memory.gameAssemblyBaseAddress + GameAssembly::BytesPatch::BypassBan::Step1::address;

                    auto patchBytes_step1 = GameAssembly::BytesPatch::BypassBan::Step1::raw;
                    memory.write_bytes(address, { patchBytes_step1 });

                    //恢复检测:步骤2
                    address = memory.gameAssemblyBaseAddress + GameAssembly::BytesPatch::BypassBan::Step2::address;

                    auto patchBytes_step2 = GameAssembly::BytesPatch::BypassBan::Step2::raw;
                    memory.write_bytes(address, { patchBytes_step2 });
                }
            }

            //移除技能冷却
            {
                ActivationState state = utils.shouldActivateOnce(hackSettings.b_removeSkillCoolDown, &b_noSkillCooldown);

                if (state == SHOULD_ACTIVATE_NOW) {
                    //移除冷却
                    int64_t address = memory.gameAssemblyBaseAddress + GameAssembly::BytesPatch::CooldownTime::address;
                    if (address == NULL) {
                        continue;
                    }

                    memory.write_bytes(address,
                        {
                            GameAssembly::BytesPatch::CooldownTime::removeCooldownTime
                        }
                    );
                }
                else if (state == SHOULD_DEACTIVATE_NOW) {
                    //恢复冷却
                    int64_t address = memory.gameAssemblyBaseAddress + GameAssembly::BytesPatch::CooldownTime::address;
                    if (address == NULL) {
                        continue;
                    }

                    memory.write_bytes(address,
                        {
                            GameAssembly::BytesPatch::CooldownTime::raw
                        }
                    );
                }
            }

            //自动完成任务以及自动准备判断
            {
                if (hackSettings.guiSettings.b_enable_autoTasks_and_autoReady) {

                    bool enabled_autoReady = hackSettings.guiSettings.b_autoReady;
                    bool enabled_autoCompleteTasks = hackSettings.guiSettings.b_autoCompleteTasks;

                    autoReady(enabled_autoReady);
                    autoCompleteTasks(enabled_autoCompleteTasks);

                    if (!enabled_autoCompleteTasks && !enabled_autoReady) {
                        unhook_LocalPlayer_Update();
                    }
                }
                else {
                    if (this->localPlayer_update_hook.b_hasHooked) {
                        //this->unhook_LocalPlayer_from_CodeCave();
                        this->unhook_LocalPlayer_Update();
                        this->b_has_hooked_autoCompleteTasks = false;
                        this->b_has_hooked_autoReady = false;
                    }
                }
            }
        }
    }
private:

    bool b_noSkillCooldown = false;

    bool b_has_hooked_autoCompleteTasks = false;
    bool b_has_hooked_autoReady = false;

    bool b_allocatedMemory_autoCompleteTasks = false;
    bool b_allocatedMemory_autoReady = false;

    bool b_hasEnabledOneTapCompletingTask = false;

    time_t lastTimeTaskComplete = -1;

    /// <summary>
    /// 是否已经启用反反作弊
    /// </summary>
    bool b_antiAC_enabled = false;

    bool b_bypassNormalBan = false;

    bool b_bypassVersionCheck = false;

    bool b_enableRainbowColors = false;
    /*
    CallHook localPlayer_update_hook{ GameAssembly::Method::LocalPlayer::Update + 0xF70,
                                    {0x41,0x0F,0x28,0x7B,0xE0,
                                    0x45,0x0F,0x28,0x43,0xD0,
                                    0x49,0x8B,0xE3,
                                    0x41,0x5F,
                                    0x41,0x5E,
                                    0x5F,
                                    0x5E,
                                    0x5B},{}
    };*/


    JmpHook localPlayer_update_hook{ GameAssembly::Method::LocalPlayer::Update,
                                        {0x48,0x8B,0xC4,
        0x48,0x89,0x48,0x08,
        0x53,
        0x56,
        0x57,
        0x41,0x56,
        0x41, 0x57,
        0x48,0x81,0xEC, 0x0, 0x1, 0x0,0x0},
                                        GameAssembly::Method::LocalPlayer::Update + JmpHook::jmpASMbytes
    };

    void autoCompleteTasks(bool enableHook) {

        if (!enableHook) {
            if (this->b_has_hooked_autoCompleteTasks) {
                this->localPlayer_update_hook.unhook();
                this->b_has_hooked_autoCompleteTasks = false;
            }
            return;
        }

        //游戏已经开始
        if (g_client.gameHasStarted()) {
            //已经超过延迟时间，允许hook
            if (g_client.timeSinceGameStarted() > hackSettings.guiSettings.f_delayedEnableTime) {
                time_t currentTime = time(NULL);
                //判断任务间隔
                if (currentTime - this->lastTimeTaskComplete < hackSettings.guiSettings.f_minInterval) {
                    //时间过短
                }
                else {
                    //做一次任务
                    this->lastTimeTaskComplete = currentTime;
                    do_autoCompleteTasks();
                }
            }
            else {
                //尚未超过延迟时间，如果hook则需要unhook
                if (this->localPlayer_update_hook.b_hasHooked) {
                    unhook_LocalPlayer_Update();
                }
            }
        }
        else {
            //游戏未开始不需要做任务，如果不需要自动准备则unhook
            if (this->b_has_hooked_autoCompleteTasks && !hackSettings.guiSettings.b_autoReady) {
                unhook_LocalPlayer_Update();
            }
        }
    }

    void autoReady(bool enableHook) {
        if (!enableHook) {
            if (this->b_has_hooked_autoReady) {
                this->localPlayer_update_hook.unhook();
                this->b_has_hooked_autoReady = false;
            }
            return;
        }

        //游戏未开始
        if (!g_client.gameHasStarted()) {

            //判断玩家是否已经准备，防止hook影响性能
            //游戏未开始才需要准备
            //在房间中
            if (g_client.inGameScene()) {
                //尚未准备
                if (!g_client.localPlayerReadied()) {
                    do_autoReady();
                }
                /*
                //已经准备
                else {
                    if (this->localPlayer_update_hook.b_hasHooked) {
                        unhook_LocalPlayer_Update();
                    }
                }*/
            }
        }
        else {
            //游戏已经开始，如果不需要自动做任务则unhook
            if (this->b_has_hooked_autoReady && !hackSettings.guiSettings.b_autoCompleteTasks) {
                unhook_LocalPlayer_Update();
            }
        }
    }

    void unhook_LocalPlayer_from_CodeCave() {
        this->localPlayer_update_hook.unhook();
    }

    //TODO: 历史遗留代码，需要删除
    void unhook_LocalPlayer_Update() {

        b_has_hooked_autoCompleteTasks = false;
        b_has_hooked_autoReady = false;

        codeCave.staticField.writeField(Fn_GetReady, 0);
        codeCave.staticField.writeField(Fn_CompleteOneTask, 0);
    }

    void enableAutoChangeColor() {
        //检查hook
        if (!this->localPlayer_update_hook.b_hasHooked) {
            //连接到CodeCave
            this->localPlayer_update_hook.setJmpAddress(codeCave.codeEntry);
            this->localPlayer_update_hook.hook();
            this->localPlayer_update_hook.b_hasHooked = true;
        }

        this->b_enableRainbowColors = true;

        codeCave.staticField.writeField(Fn_ChangeColor, 1);
    }

    void disableAutoChangeColor() {
        //检查hook
        if (!this->localPlayer_update_hook.b_hasHooked) {
            //连接到CodeCave
            this->localPlayer_update_hook.setJmpAddress(codeCave.codeEntry);
            this->localPlayer_update_hook.hook();
            this->localPlayer_update_hook.b_hasHooked = true;
        }

        this->b_enableRainbowColors = false;

        codeCave.staticField.writeField(Fn_ChangeColor, 0);
    }

    void do_autoCompleteTasks() {
        //检查hook
        if (!this->localPlayer_update_hook.b_hasHooked) {
            //连接到CodeCave
            this->localPlayer_update_hook.setJmpAddress(codeCave.codeEntry);
            this->localPlayer_update_hook.hook();
            this->localPlayer_update_hook.b_hasHooked = true;
        }

        this->b_has_hooked_autoReady = false;
        this->b_has_hooked_autoCompleteTasks = true;


        codeCave.staticField.writeField(Fn_CompleteOneTask, 1);
    }

    void do_autoReady() {
        //检查是否已经hook
        if (!this->localPlayer_update_hook.b_hasHooked) {
            //连接到CodeCave
            this->localPlayer_update_hook.setJmpAddress(codeCave.codeEntry);
            this->localPlayer_update_hook.hook();
            this->localPlayer_update_hook.b_hasHooked = true;
        }

        //互斥
        this->b_has_hooked_autoReady = true;
        this->b_has_hooked_autoCompleteTasks = false;

        codeCave.staticField.writeField(Fn_GetReady, 1);
    }
};
