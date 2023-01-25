﻿#pragma once

#include"../Data/offsets.hpp"
#include "../memory.hpp"
#include "../Struct/HackSettings.hpp"
#include"../utils.hpp"

extern HackSettings hackSettings;
extern Utils utils;
extern Memory memory;

/// <summary>
/// 负责patch汇编区指令字节。
/// 由于目前没有使用到hook框架，该类暂时也负责维护hook功能
/// </summary>
class BytesPatchUpdater {
public:

    void bytesPatchUpdater() {
        while (true) {
            Sleep(100);

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
                ActivationState state_autoCompleteTasks_and_autoReady = utils.shouldActivateOnce(hackSettings.guiSettings.b_autoCompleteTasks_and_autoReady, &b_autoCompleteTasks_and_autoReady);
                ActivationState state_autoReady = utils.shouldActivateOnce(hackSettings.guiSettings.b_autoReady, &b_autoReady);
                ActivationState state_autoCompleteTasks = utils.shouldActivateOnce(hackSettings.guiSettings.b_autoCompleteTasks, &b_autoCompleteTasks);

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
                    this->b_autoCompleteTasks = false;
                    this->b_autoReady = false;

                    this->activate_autoCompleteTasks_and_autoReady();
                }
                else if (state_autoReady == SHOULD_ACTIVATE_NOW) {

                    hackSettings.guiSettings.b_autoCompleteTasks_and_autoReady = false;
                    hackSettings.guiSettings.b_autoCompleteTasks = false;
                    this->b_autoCompleteTasks_and_autoReady = false;
                    this->b_autoCompleteTasks = false;

                    this->activate_autoReady();
                }
                else if (state_autoCompleteTasks == SHOULD_ACTIVATE_NOW) {

                    hackSettings.guiSettings.b_autoCompleteTasks_and_autoReady = false;
                    hackSettings.guiSettings.b_autoReady = false;
                    this->b_autoCompleteTasks_and_autoReady = false;
                    this->b_autoReady = false;

                    this->activate_autoCompleteTasks();
                }
            }
        }
    }
private:

    bool b_noSkillCooldown = false;
    bool b_autoCompleteTasks_and_autoReady = false;
    bool b_autoCompleteTasks = false;
    bool b_autoReady = false;

    void unhook_LocalPlayer_Update() {
        //TODO:unhook
        //取消注入代码
        std::vector<byte> originalBytes = { 0x45,0x0F,0x28,0x7B,0x90,0x49,0x8B,0xE3,0x41,0x5F,0x41,0x5E,0x5F,0x5E,0x5B,0xC3 };

        //定位LocalPlayer的Update函数的注入点
        int64_t injectEntry_Addr = memory.gameAssemblyBaseAddress + GameAssembly::Method::LocalPlayer::Update + 0xFDD;
        memory.write_bytes(injectEntry_Addr, { originalBytes });
    }

    void activate_autoCompleteTasks() {
        //TODO
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
        int64_t injectEntry_Addr = memory.gameAssemblyBaseAddress + GameAssembly::Method::LocalPlayer::Update + 0xFDD;

        //篡改跳转用的汇编指令字节
        //mov rax 0x????????
        //jmp rax
        std::vector<byte> injectCodesBaseAddress_LittleEndianBytes = utils.addressToLittleEndianBytes(injectCodesBaseAddress);

        memory.write_bytes(injectEntry_Addr, {
                        ASM_mov_rax,
                        injectCodesBaseAddress_LittleEndianBytes,
                        ASM_jmp_rax
            });
    }

    void activate_autoReady() {
        //TODO
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
                    //ASM_call_FF15,utils.addressToLittleEndianBytes(memory.gameAssemblyBaseAddress + GameAssembly::Method::TasksHandler::CompleteTask),
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
        int64_t injectEntry_Addr = memory.gameAssemblyBaseAddress + GameAssembly::Method::LocalPlayer::Update + 0xFDD;

        //篡改跳转用的汇编指令字节
        //mov rax 0x????????
        //jmp rax
        std::vector<byte> injectCodesBaseAddress_LittleEndianBytes = utils.addressToLittleEndianBytes(injectCodesBaseAddress);

        memory.write_bytes(injectEntry_Addr, {
                        ASM_mov_rax,
                        injectCodesBaseAddress_LittleEndianBytes,
                        ASM_jmp_rax
            });
    }

    void activate_autoCompleteTasks_and_autoReady() {
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
        int64_t injectEntry_Addr = memory.gameAssemblyBaseAddress + GameAssembly::Method::LocalPlayer::Update + 0xFDD;

        //篡改跳转用的汇编指令字节
        //mov rax 0x????????
        //jmp rax
        std::vector<byte> injectCodesBaseAddress_LittleEndianBytes = utils.addressToLittleEndianBytes(injectCodesBaseAddress);

        memory.write_bytes(injectEntry_Addr, {
                        ASM_mov_rax,
                        injectCodesBaseAddress_LittleEndianBytes,
                        ASM_jmp_rax
            });
    }
};
