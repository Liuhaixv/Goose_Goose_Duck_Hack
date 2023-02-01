#pragma once

#include"../Data/offsets.hpp"
#include "../memory.h"
#include "../Struct/HackSettings.hpp"
#include"../utils.hpp"
#include "../Client.h"
#include"Updater.h"

extern HackSettings hackSettings;
extern Utils utils;
extern Memory memory;
extern Client g_client;

/// <summary>
/// 负责patch汇编区指令字节。
/// 由于目前没有使用到hook框架，该类暂时也负责维护hook功能
/// </summary>
class BytesPatchUpdater : public Updater {
public:
    void unhookAll() {
        //LocalPlayer.Update
        if (this->b_has_hooked_autoCompleteTasks || this->b_has_hooked_autoReady) {
            this->unhook_LocalPlayer_Update();
        }
    }

    void bytesPatchUpdater() {
        while (true) {
            Sleep(100);

            if (this->paused) {
                Sleep(100);
                continue;
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

                    if (!memory.write_bytes(address,
                        { GameAssembly::BytesPatch::QuitGame::disableQuitGame })
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

                    const byte* patchBytes = GameAssembly::BytesPatch::CooldownTime::removeCooldownTime;
                    memory.write_bytes(address, patchBytes, GameAssembly::BytesPatch::CooldownTime::bytesNum);
                }
                else if (state == SHOULD_DEACTIVATE_NOW) {
                    //恢复冷却
                    int64_t address = memory.gameAssemblyBaseAddress + GameAssembly::BytesPatch::CooldownTime::address;
                    if (address == NULL) {
                        continue;
                    }

                    const byte* patchBytes = GameAssembly::BytesPatch::CooldownTime::raw;
                    memory.write_bytes(address, patchBytes, GameAssembly::BytesPatch::CooldownTime::bytesNum);
                }
            }

            //自动完成任务以及自动准备判断
            {
                /*
                if (hackSettings.guiSettings.b_autoCompleteTasks) {

                }
                else if(hackSettings.guiSettings.b_autoReady) {

                }
                else if (hackSettings.guiSettings.b_autoCompleteTasks_and_autoReady) {

                }
                */
                bool enabled_autoCompleteTasks_and_autoReady = hackSettings.guiSettings.b_autoCompleteTasks_and_autoReady;
                bool enabled_autoReady = hackSettings.guiSettings.b_autoReady;
                bool enabled_autoCompleteTasks = hackSettings.guiSettings.b_autoCompleteTasks;

                if (enabled_autoCompleteTasks_and_autoReady) {
                    autoCompleteTasks_and_autoReady();
                }
                else if (enabled_autoReady) {
                    autoReady();
                }
                else if (enabled_autoCompleteTasks) {
                    autoCompleteTasks();
                }
                else {
                    //unhook
                    if (this->b_has_hooked_autoCompleteTasks || this->b_has_hooked_autoReady) {
                        unhook_LocalPlayer_Update();
                    }
                }

                /*
                ActivationState state_autoCompleteTasks_and_autoReady = utils.shouldActivateOnce(hackSettings.guiSettings.b_autoCompleteTasks_and_autoReady, &b_has_hooked_autoCompleteTasks_and_autoReady);
                ActivationState state_autoReady = utils.shouldActivateOnce(hackSettings.guiSettings.b_autoReady, &b_has_hooked_autoReady);
                ActivationState state_autoCompleteTasks = utils.shouldActivateOnce(hackSettings.guiSettings.b_autoCompleteTasks, &b_has_hooked_autoCompleteTasks);

                //优先判断是否需要unhook
                if (state_autoCompleteTasks_and_autoReady == SHOULD_DEACTIVATE_NOW
                    || state_autoReady == SHOULD_DEACTIVATE_NOW
                    || state_autoCompleteTasks == SHOULD_DEACTIVATE_NOW) {
                    unhook_LocalPlayer_Update();
                }

                //unhook结束后判断是否需要hook
                if (state_autoCompleteTasks_and_autoReady == SHOULD_ACTIVATE_NOW) {
                    //hook不能共存，将其他选项改为false
                    hackSettings.guiSettings.b_autoCompleteTasks = false;
                    hackSettings.guiSettings.b_autoReady = false;
                    this->b_has_hooked_autoCompleteTasks = false;
                    this->b_has_hooked_autoReady = false;

                    this->hook_autoCompleteTasks_and_autoReady();
                }
                else if (state_autoReady == SHOULD_ACTIVATE_NOW) {

                    hackSettings.guiSettings.b_autoCompleteTasks_and_autoReady = false;
                    hackSettings.guiSettings.b_autoCompleteTasks = false;
                    this->b_has_hooked_autoCompleteTasks_and_autoReady = false;
                    this->b_has_hooked_autoCompleteTasks = false;

                    this->hook_autoReady();
                }
                else if (state_autoCompleteTasks == SHOULD_ACTIVATE_NOW) {

                    hackSettings.guiSettings.b_autoCompleteTasks_and_autoReady = false;
                    hackSettings.guiSettings.b_autoReady = false;
                    this->b_has_hooked_autoCompleteTasks_and_autoReady = false;
                    this->b_has_hooked_autoReady = false;

                    this->hook_autoCompleteTasks();
                }
                */
            }
        }
    }
private:

    bool b_noSkillCooldown = false;

    //bool b_has_hooked_autoCompleteTasks_and_autoReady = false;
    bool b_has_hooked_autoCompleteTasks = false;
    bool b_has_hooked_autoReady = false;

    bool b_allocatedMemory_autoCompleteTasks = false;
    bool b_allocatedMemory_autoReady = false;

    /// <summary>
    /// 是否已经启用反反作弊
    /// </summary>
    bool b_antiAC_enabled = false;

    bool b_bypassNormalBan = false;

    void unhook_LocalPlayer_Update() {
        //当前没有hook过
        //if (!this->b_has_hooked_autoCompleteTasks && !this->b_has_hooked_autoCompleteTasks_and_autoReady && !this->b_has_hooked_autoReady) {
        //    return;
        //}

        //unhook
        //取消注入代码
        std::vector<byte> originalBytes = { 0x45,0x0F,0x28,0x7B,0x90,//movaps xmm15,[r11-70]
                                            0x49,0x8B,0xE3,//mov rsp,r11
                                            0x41,0x5F,
                                            0x41,0x5E,
                                            0x5F,
                                            0x5E,
                                            0x5B,
                                            0xC3 };

        //定位LocalPlayer的Update函数的注入点
        int64_t injectEntry_Addr = memory.gameAssemblyBaseAddress + GameAssembly::Method::LocalPlayer::Update + 0x1043;
        memory.write_bytes(injectEntry_Addr, { originalBytes });

        //b_has_hooked_autoCompleteTasks_and_autoReady = false;
        b_has_hooked_autoCompleteTasks = false;
        b_has_hooked_autoReady = false;
    }

    void autoCompleteTasks() {
        //游戏已经开始
        if (g_client.gameHasStarted()) {
            //已经超过延迟时间，允许hook
            if (g_client.timeSinceGameStarted() > hackSettings.guiSettings.f_delayedEnableTime) {
                if (!this->b_has_hooked_autoCompleteTasks) {
                    hook_autoCompleteTasks();
                }
            }
            else {
                //尚未超过延迟时间，如果hook则需要unhook
                if (this->b_has_hooked_autoCompleteTasks) {
                    unhook_LocalPlayer_Update();
                }
            }


            //TODO: 判断任务是否已经完成，需要unhook
        }
        else {
            //游戏未开始不需要做任务，这里尝试unhook
            if (this->b_has_hooked_autoCompleteTasks) {
                unhook_LocalPlayer_Update();
            }
        }
    }

    void autoReady() {
        //游戏未开始
        if (!g_client.gameHasStarted()) {

            //判断玩家是否已经准备，防止hook影响性能
            //游戏未开始才需要准备
            //在房间中
            if (g_client.inGameScene()) {
                if (!g_client.localPlayerReadied()) {
                    if (!this->b_has_hooked_autoReady) {
                        hook_autoReady();
                    }
                }
                else {
                    if (this->b_has_hooked_autoCompleteTasks || this->b_has_hooked_autoReady) {
                        unhook_LocalPlayer_Update();
                    }
                }
            }
        }
        else {
            //游戏已经开始，不必准备，尝试unhook
            if (this->b_has_hooked_autoReady) {
                unhook_LocalPlayer_Update();
            }
        }
    }

    void autoCompleteTasks_and_autoReady() {
        //游戏未开始
        if (!g_client.gameHasStarted()) {
            //判断玩家是否已经准备，防止hook影响性能
            //游戏未开始才需要准备
            //在房间中
            if (g_client.inGameScene()) {
                if (!g_client.localPlayerReadied()) {
                    if (!this->b_has_hooked_autoReady) {
                        hook_autoReady();
                    }
                }
                else {
                    if (this->b_has_hooked_autoCompleteTasks || this->b_has_hooked_autoReady) {
                        unhook_LocalPlayer_Update();
                    }
                }
            }
        }
        else {
            //游戏已经开始，判断完成任务的函数
            //已经超过延迟时间，允许hook
            if (g_client.timeSinceGameStarted() > hackSettings.guiSettings.f_delayedEnableTime) {
                if (!this->b_has_hooked_autoCompleteTasks) {
                    hook_autoCompleteTasks();
                }
                //TODO: 判断任务是否已经完成，需要unhook
            }
            else {
                //尚未超过延迟时间，如果hook则需要unhook
                if (this->b_has_hooked_autoCompleteTasks) {
                    unhook_LocalPlayer_Update();
                }
            }
        }
    }

    void hook_autoCompleteTasks() {
        //Hook逻辑
        //开始注入代码                   

        //标记当前是否已经申请内存
        //static bool b_allocatedMemory = false;
        //申请的内存基址
        static int64_t injectCodesBaseAddress = 0;

        static std::vector<byte> ASM_mov_rax = { 0x48, 0xB8 };
        static std::vector<byte> ASM_mov_rcx = { 0x48, 0xB9 };
        static std::vector<byte> ASM_jmp_rax = { 0xFF, 0xE0 };
        static std::vector<byte> ASM_call_FF15 = { 0xFF, 0x15, 0x02, 0x00, 0x00 , 0x00 , 0xEB , 0x08 };
        static std::vector<byte> ASM_ret = { 0xC3 };

        //尚未申请内存
        if (!b_allocatedMemory_autoCompleteTasks) {
            bool result = memory.allocExecutableMemory(1024, &injectCodesBaseAddress);

            //申请内存成功
            if (result == true) {
                b_allocatedMemory_autoCompleteTasks = true;
                //将注入代码写入申请的内存中
                //byte injectAsmCodes[] = { 0x45,0x0F,0x28,0x7B,0x90,0x49,0x8B,0xE3,0x41,0x5F,0x41,0x5E,0x5F,0x5E,0x5B,0x48,0x83,0xEC,0x48,0x48,0xB9,0xD0,0xA8,0xEA,0x41,0xFC,0x7F,0x00,0x00,0x48,0x8B,0x09,0x48,0x8B,0x89,0xB8,0x00,0x00,0x00,0x48,0x8B,0x09,0x48,0x85,0xC9,0x0F,0x84,0x8A,0x00,0x00,0x00,0x48,0x8B,0x49,0x40,0x48,0x85,0xC9,0x0F,0x84,0x7D,0x00,0x00,0x00,0x4C,0x8B,0x51,0x38,0x4D,0x8B,0x52,0x18,0x4D,0x85,0xD2,0x0F,0x84,0x6C,0x00,0x00,0x00,0x66,0x45,0x8B,0x4A,0x18,0x48,0x31,0xC0,0x66,0xB8,0x18,0x00,0x66,0x41,0xF7,0xE1,0x66,0x41,0x89,0xC1,0x4D,0x8B,0xC2,0x49,0x83,0xC0,0x10,0x66,0x45,0x01,0xC8,0x4D,0x8B,0xDA,0x49,0x83,0xC3,0x10,0x49,0x83,0xC3,0x18,0x4D,0x39,0xC3,0x0F,0x8F,0x39,0x00,0x00,0x00,0x49,0x8B,0x13,0x48,0x85,0xD2,0x74,0xEB,0x48,0x8B,0xD2,0x41,0xB0,0x00,0x41,0xB1,0x00,0xC7,0x44,0x24,0x30,0x00,0x00,0x00,0x00,0xC7,0x44,0x24,0x28,0x01,0x00,0x00,0x00,0xC7,0x44,0x24,0x20,0x00,0x00,0x00,0x00,0xFF,0x15,0x02,0x00,0x00,0x00,0xEB,0x08,0xD0,0x55,0x0A,0x3F,0xFC,0x7F,0x00,0x00,0x48,0xB9,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xBA,0x01,0x00,0x00,0x00,0x4D,0x31,0xC0,0xFF,0x15,0x02,0x00,0x00,0x00,0xEB,0x08,0xA0,0xB1,0xA8,0x3E,0xFC,0x7F,0x00,0x00,0x48,0x83,0xC4,0x48,0xC3 };

                memory.write_bytes(injectCodesBaseAddress, {
                    std::vector<byte>{0x45,0x0F,0x28,0x7B,0x90,0x49,0x8B,0xE3,0x41,0x5F,0x41,0x5E,0x5F,0x5E,0x5B,0x48,0x83,0xEC,0x48},
                    ASM_mov_rcx, utils.addressToLittleEndianBytes(memory.gameAssemblyBaseAddress + GameAssembly::Class::ptr_LobbySceneHandlerClass),
                    std::vector<byte>{0x48,0x8B,0x09,0x48,0x8B,0x89,0xB8,0x00,0x00,0x00,0x48,0x8B,0x09,0x48,0x85,0xC9,0x0F,0x84,0x8A,0x00,0x00,0x00,0x48,0x8B,0x49,0x40,0x48,0x85,0xC9,0x0F,0x84,0x7D,0x00,0x00,0x00,0x4C,0x8B,0x51,0x38,0x4D,0x8B,0x52,0x18,0x4D,0x85,0xD2,0x0F,0x84,0x6C,0x00,0x00,0x00,0x66,0x45,0x8B,0x4A,0x18,0x48,0x31,0xC0,0x66,0xB8,0x18,0x00,0x66,0x41,0xF7,0xE1,0x66,0x41,0x89,0xC1,0x4D,0x8B,0xC2,0x49,0x83,0xC0,0x10,0x66,0x45,0x01,0xC8,0x4D,0x8B,0xDA,0x49,0x83,0xC3,0x10,0x49,0x83,0xC3,0x18,0x4D,0x39,0xC3,0x0F,0x8F,0x39,0x00,0x00,0x00,0x49,0x8B,0x13,0x48,0x85,0xD2,0x74,0xEB,0x48,0x8B,0xD2,0x41,0xB0,0x00,0x41,0xB1,0x00,0xC7,0x44,0x24,0x30,0x00,0x00,0x00,0x00,0xC7,0x44,0x24,0x28,0x01,0x00,0x00,0x00,0xC7,0x44,0x24,0x20,0x00,0x00,0x00,0x00},
                    ASM_call_FF15,utils.addressToLittleEndianBytes(memory.gameAssemblyBaseAddress + GameAssembly::Method::TasksHandler::CompleteTask),
                    std::vector<byte>{0x48,0xB9,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xBA,0x01,0x00,0x00,0x00,0x4D,0x31,0xC0},
                    //ASM_call_FF15,utils.addressToLittleEndianBytes(memory.gameAssemblyBaseAddress + GameAssembly::Method::PlayerPropertiesManager::ChangeReadyState),
                    std::vector<byte>{0x48,0x83,0xC4,0x48},
                    ASM_ret
                    });
            }
        }
        else {
            //已经申请过内存
        }

        //篡改汇编指令跳转到注入代码地址
        //定位LocalPlayer的Update函数的注入点
        int64_t injectEntry_Addr = memory.gameAssemblyBaseAddress + GameAssembly::Method::LocalPlayer::Update + 0x1043;

        //篡改跳转用的汇编指令字节
        //mov rax 0x????????
        //jmp rax
        std::vector<byte> injectCodesBaseAddress_LittleEndianBytes = utils.addressToLittleEndianBytes(injectCodesBaseAddress);

        memory.write_bytes(injectEntry_Addr, {
                        ASM_mov_rax,
                        injectCodesBaseAddress_LittleEndianBytes,
                        ASM_jmp_rax
            });

        this->b_has_hooked_autoCompleteTasks = true;
    }

    void hook_autoReady() {
        //TODO
        //Hook逻辑
        //开始注入代码                   

        //标记当前是否已经申请内存
        //static bool b_allocatedMemory = false;
        //申请的内存基址
        static int64_t injectCodesBaseAddress = 0;

        static std::vector<byte> ASM_mov_rax = { 0x48, 0xB8 };
        static std::vector<byte> ASM_mov_rcx = { 0x48, 0xB9 };
        static std::vector<byte> ASM_jmp_rax = { 0xFF, 0xE0 };
        static std::vector<byte> ASM_call_FF15 = { 0xFF, 0x15, 0x02, 0x00, 0x00 , 0x00 , 0xEB , 0x08 };
        static std::vector<byte> ASM_ret = { 0xC3 };

        //尚未申请内存
        if (!b_allocatedMemory_autoReady) {
            bool result = memory.allocExecutableMemory(1024, &injectCodesBaseAddress);

            //申请内存成功
            if (result == true) {
                b_allocatedMemory_autoReady = true;
                //将注入代码写入申请的内存中
                //byte injectAsmCodes[] = { 0x45,0x0F,0x28,0x7B,0x90,0x49,0x8B,0xE3,0x41,0x5F,0x41,0x5E,0x5F,0x5E,0x5B,0x48,0x83,0xEC,0x48,0x48,0xB9,0xD0,0xA8,0xEA,0x41,0xFC,0x7F,0x00,0x00,0x48,0x8B,0x09,0x48,0x8B,0x89,0xB8,0x00,0x00,0x00,0x48,0x8B,0x09,0x48,0x85,0xC9,0x0F,0x84,0x8A,0x00,0x00,0x00,0x48,0x8B,0x49,0x40,0x48,0x85,0xC9,0x0F,0x84,0x7D,0x00,0x00,0x00,0x4C,0x8B,0x51,0x38,0x4D,0x8B,0x52,0x18,0x4D,0x85,0xD2,0x0F,0x84,0x6C,0x00,0x00,0x00,0x66,0x45,0x8B,0x4A,0x18,0x48,0x31,0xC0,0x66,0xB8,0x18,0x00,0x66,0x41,0xF7,0xE1,0x66,0x41,0x89,0xC1,0x4D,0x8B,0xC2,0x49,0x83,0xC0,0x10,0x66,0x45,0x01,0xC8,0x4D,0x8B,0xDA,0x49,0x83,0xC3,0x10,0x49,0x83,0xC3,0x18,0x4D,0x39,0xC3,0x0F,0x8F,0x39,0x00,0x00,0x00,0x49,0x8B,0x13,0x48,0x85,0xD2,0x74,0xEB,0x48,0x8B,0xD2,0x41,0xB0,0x00,0x41,0xB1,0x00,0xC7,0x44,0x24,0x30,0x00,0x00,0x00,0x00,0xC7,0x44,0x24,0x28,0x01,0x00,0x00,0x00,0xC7,0x44,0x24,0x20,0x00,0x00,0x00,0x00,0xFF,0x15,0x02,0x00,0x00,0x00,0xEB,0x08,0xD0,0x55,0x0A,0x3F,0xFC,0x7F,0x00,0x00,0x48,0xB9,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xBA,0x01,0x00,0x00,0x00,0x4D,0x31,0xC0,0xFF,0x15,0x02,0x00,0x00,0x00,0xEB,0x08,0xA0,0xB1,0xA8,0x3E,0xFC,0x7F,0x00,0x00,0x48,0x83,0xC4,0x48,0xC3 };

                memory.write_bytes(injectCodesBaseAddress, {
                    std::vector<byte>{0x45,0x0F,0x28,0x7B,0x90,0x49,0x8B,0xE3,0x41,0x5F,0x41,0x5E,0x5F,0x5E,0x5B,0x48,0x83,0xEC,0x48},
                    ASM_mov_rcx, utils.addressToLittleEndianBytes(memory.gameAssemblyBaseAddress + GameAssembly::Class::ptr_LobbySceneHandlerClass),
                    std::vector<byte>{0x48,0x8B,0x09,0x48,0x8B,0x89,0xB8,0x00,0x00,0x00,0x48,0x8B,0x09,0x48,0x85,0xC9,0x0F,0x84,0x7A,0x00,0x00,0x00,0x48,0x8B,0x49,0x40,0x48,0x85,0xC9,0x0F,0x84,0x6D,0x00,0x00,0x00,0x4C,0x8B,0x51,0x38,0x4D,0x8B,0x52,0x18,0x4D,0x85,0xD2,0x0F,0x84,0x5C,0x00,0x00,0x00,0x66,0x45,0x8B,0x4A,0x18,0x48,0x31,0xC0,0x66,0xB8,0x18,0x00,0x66,0x41,0xF7,0xE1,0x66,0x41,0x89,0xC1,0x4D,0x8B,0xC2,0x49,0x83,0xC0,0x10,0x66,0x45,0x01,0xC8,0x4D,0x8B,0xDA,0x49,0x83,0xC3,0x10,0x49,0x83,0xC3,0x18,0x4D,0x39,0xC3,0x0F,0x8F,0x29,0x00,0x00,0x00,0x49,0x8B,0x13,0x48,0x85,0xD2,0x74,0xEB,0x48,0x8B,0xD2,0x41,0xB0,0x00,0x41,0xB1,0x00,0xC7,0x44,0x24,0x30,0x00,0x00,0x00,0x00,0xC7,0x44,0x24,0x28,0x01,0x00,0x00,0x00,0xC7,0x44,0x24,0x20,0x00,0x00,0x00,0x00,0x48,0xB9,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xBA,0x01,0x00,0x00,0x00,0x4D,0x31,0xC0},
                    ASM_call_FF15,utils.addressToLittleEndianBytes(memory.gameAssemblyBaseAddress + GameAssembly::Method::PlayerPropertiesManager::ChangeReadyState),
                    std::vector<byte>{0x48,0x83,0xC4,0x48},
                    ASM_ret
                    });
            }
        }
        else {
            //已经申请过内存
        }

        //篡改汇编指令跳转到注入代码地址
        //定位LocalPlayer的Update函数的注入点
        int64_t injectEntry_Addr = memory.gameAssemblyBaseAddress + GameAssembly::Method::LocalPlayer::Update + 0x1043;

        //篡改跳转用的汇编指令字节
        //mov rax 0x????????
        //jmp rax
        std::vector<byte> injectCodesBaseAddress_LittleEndianBytes = utils.addressToLittleEndianBytes(injectCodesBaseAddress);

        memory.write_bytes(injectEntry_Addr, {
                        ASM_mov_rax,
                        injectCodesBaseAddress_LittleEndianBytes,
                        ASM_jmp_rax
            });

        this->b_has_hooked_autoReady = true;
    }

    /// <summary>
    /// 由于游戏中准备会导致掉线等bug问题，已弃用
    /// </summary>
    /*
    void hook_autoCompleteTasks_and_autoReady() {
        //Hook逻辑
        //开始注入代码

        //标记当前是否已经申请内存
        static bool b_allocatedMemory = false;
        //申请的内存基址
        static int64_t injectCodesBaseAddress = 0;

        static std::vector<byte> ASM_mov_rax = { 0x48, 0xB8 };
        static std::vector<byte> ASM_mov_rcx = { 0x48, 0xB9 };
        static std::vector<byte> ASM_jmp_rax = { 0xFF, 0xE0 };
        static std::vector<byte> ASM_call_FF15 = { 0xFF, 0x15, 0x02, 0x00, 0x00 , 0x00 , 0xEB , 0x08 };
        static std::vector<byte> ASM_ret = { 0xC3 };

        //尚未申请内存
        if (!b_allocatedMemory) {
            bool result = memory.allocExecutableMemory(1024, &injectCodesBaseAddress);

            //申请内存成功
            if (result == true) {
                b_allocatedMemory = true;
                //将注入代码写入申请的内存中
                //byte injectAsmCodes[] = { 0x45,0x0F,0x28,0x7B,0x90,0x49,0x8B,0xE3,0x41,0x5F,0x41,0x5E,0x5F,0x5E,0x5B,0x48,0x83,0xEC,0x48,0x48,0xB9,0xD0,0xA8,0xEA,0x41,0xFC,0x7F,0x00,0x00,0x48,0x8B,0x09,0x48,0x8B,0x89,0xB8,0x00,0x00,0x00,0x48,0x8B,0x09,0x48,0x85,0xC9,0x0F,0x84,0x8A,0x00,0x00,0x00,0x48,0x8B,0x49,0x40,0x48,0x85,0xC9,0x0F,0x84,0x7D,0x00,0x00,0x00,0x4C,0x8B,0x51,0x38,0x4D,0x8B,0x52,0x18,0x4D,0x85,0xD2,0x0F,0x84,0x6C,0x00,0x00,0x00,0x66,0x45,0x8B,0x4A,0x18,0x48,0x31,0xC0,0x66,0xB8,0x18,0x00,0x66,0x41,0xF7,0xE1,0x66,0x41,0x89,0xC1,0x4D,0x8B,0xC2,0x49,0x83,0xC0,0x10,0x66,0x45,0x01,0xC8,0x4D,0x8B,0xDA,0x49,0x83,0xC3,0x10,0x49,0x83,0xC3,0x18,0x4D,0x39,0xC3,0x0F,0x8F,0x39,0x00,0x00,0x00,0x49,0x8B,0x13,0x48,0x85,0xD2,0x74,0xEB,0x48,0x8B,0xD2,0x41,0xB0,0x00,0x41,0xB1,0x00,0xC7,0x44,0x24,0x30,0x00,0x00,0x00,0x00,0xC7,0x44,0x24,0x28,0x01,0x00,0x00,0x00,0xC7,0x44,0x24,0x20,0x00,0x00,0x00,0x00,0xFF,0x15,0x02,0x00,0x00,0x00,0xEB,0x08,0xD0,0x55,0x0A,0x3F,0xFC,0x7F,0x00,0x00,0x48,0xB9,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xBA,0x01,0x00,0x00,0x00,0x4D,0x31,0xC0,0xFF,0x15,0x02,0x00,0x00,0x00,0xEB,0x08,0xA0,0xB1,0xA8,0x3E,0xFC,0x7F,0x00,0x00,0x48,0x83,0xC4,0x48,0xC3 };

                memory.write_bytes(injectCodesBaseAddress, {
                    std::vector<byte>{0x45,0x0F,0x28,0x7B,0x90,0x49,0x8B,0xE3,0x41,0x5F,0x41,0x5E,0x5F,0x5E,0x5B,0x48,0x83,0xEC,0x48},
                    ASM_mov_rcx, utils.addressToLittleEndianBytes(memory.gameAssemblyBaseAddress + GameAssembly::Class::ptr_LobbySceneHandlerClass),
                    std::vector<byte>{0x48,0x8B,0x09,0x48,0x8B,0x89,0xB8,0x00,0x00,0x00,0x48,0x8B,0x09,0x48,0x85,0xC9,0x0F,0x84,0x8A,0x00,0x00,0x00,0x48,0x8B,0x49,0x40,0x48,0x85,0xC9,0x0F,0x84,0x7D,0x00,0x00,0x00,0x4C,0x8B,0x51,0x38,0x4D,0x8B,0x52,0x18,0x4D,0x85,0xD2,0x0F,0x84,0x6C,0x00,0x00,0x00,0x66,0x45,0x8B,0x4A,0x18,0x48,0x31,0xC0,0x66,0xB8,0x18,0x00,0x66,0x41,0xF7,0xE1,0x66,0x41,0x89,0xC1,0x4D,0x8B,0xC2,0x49,0x83,0xC0,0x10,0x66,0x45,0x01,0xC8,0x4D,0x8B,0xDA,0x49,0x83,0xC3,0x10,0x49,0x83,0xC3,0x18,0x4D,0x39,0xC3,0x0F,0x8F,0x39,0x00,0x00,0x00,0x49,0x8B,0x13,0x48,0x85,0xD2,0x74,0xEB,0x48,0x8B,0xD2,0x41,0xB0,0x00,0x41,0xB1,0x00,0xC7,0x44,0x24,0x30,0x00,0x00,0x00,0x00,0xC7,0x44,0x24,0x28,0x01,0x00,0x00,0x00,0xC7,0x44,0x24,0x20,0x00,0x00,0x00,0x00},
                    ASM_call_FF15,utils.addressToLittleEndianBytes(memory.gameAssemblyBaseAddress + GameAssembly::Method::TasksHandler::CompleteTask),
                    std::vector<byte>{0x48,0xB9,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xBA,0x01,0x00,0x00,0x00,0x4D,0x31,0xC0},
                    ASM_call_FF15,utils.addressToLittleEndianBytes(memory.gameAssemblyBaseAddress + GameAssembly::Method::PlayerPropertiesManager::ChangeReadyState),
                    std::vector<byte>{0x48,0x83,0xC4,0x48},
                    ASM_ret
                    });
            }
        }
        else {
            //已经申请过内存
        }

        //篡改汇编指令跳转到注入代码地址
        //定位LocalPlayer的Update函数的注入点
        int64_t injectEntry_Addr = memory.gameAssemblyBaseAddress + GameAssembly::Method::LocalPlayer::Update + 0xF45;

        //篡改跳转用的汇编指令字节
        //mov rax 0x????????
        //jmp rax
        std::vector<byte> injectCodesBaseAddress_LittleEndianBytes = utils.addressToLittleEndianBytes(injectCodesBaseAddress);

        memory.write_bytes(injectEntry_Addr, {
                        ASM_mov_rax,
                        injectCodesBaseAddress_LittleEndianBytes,
                        ASM_jmp_rax
            });

        this->b_has_hooked_autoCompleteTasks_and_autoReady = true;
    }
    */
};
