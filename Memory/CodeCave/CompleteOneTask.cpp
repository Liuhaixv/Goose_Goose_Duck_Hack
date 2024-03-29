﻿#include "CompleteOneTask.h"
#include "../Memory.cpp"
#include"../../Data/offsets.hpp"
#include"../../utils.hpp"

std::vector<byte> CompleteOneTask::getFunctionBytes()
{
    static std::vector<byte> ASM_mov_rax = { 0x48, 0xB8 };
    static std::vector<byte> ASM_mov_rcx = { 0x48, 0xB9 };
    static std::vector<byte> ASM_jmp_rax = { 0xFF, 0xE0 };
    //call near (ff 15 ????????) then jmp = call addres which we jmp to
    static std::vector<byte> ASM_call_FF15 = { 0xFF, 0x15, 0x02, 0x00, 0x00 , 0x00 , 0xEB , 0x08 };
    static std::vector<byte> ASM_ret = { 0xC3 };

    std::vector<byte> bytes = utils.combineVectors({
        { 0x48,0x83,0xEC,0x48 },
        ASM_mov_rcx, utils.addressToLittleEndianBytes(memory.gameAssemblyBaseAddress + GameAssembly::Class::ptr_LobbySceneHandlerClass),
        std::vector<byte>{0x48, 0x8B, 0x09, 0x48, 0x8B, 0x89, 0xB8, 0x00, 0x00, 0x00, 0x48, 0x8B, 0x09, 0x48, 0x85, 0xC9, 0x0F, 0x84, 0x8A, 0x00, 0x00, 0x00, 0x48, 0x8B, 0x49, 0x40, 0x48, 0x85, 0xC9, 0x0F, 0x84, 0x7D, 0x00, 0x00, 0x00, 0x4C, 0x8B, 0x51, 0x38, 0x4D, 0x8B, 0x52, 0x18, 0x4D, 0x85, 0xD2, 0x0F, 0x84, 0x6C, 0x00, 0x00, 0x00, 0x66, 0x45, 0x8B, 0x4A, 0x18, 0x48, 0x31, 0xC0, 0x66, 0xB8, 0x18, 0x00, 0x66, 0x41, 0xF7, 0xE1, 0x66, 0x41, 0x89, 0xC1, 0x4D, 0x8B, 0xC2, 0x49, 0x83, 0xC0, 0x10, 0x66, 0x45, 0x01, 0xC8, 0x4D, 0x8B, 0xDA, 0x49, 0x83, 0xC3, 0x10, 0x49, 0x83, 0xC3, 0x18, 0x4D, 0x39, 0xC3, 0x0F, 0x8F, 0x39, 0x00, 0x00, 0x00, 0x49, 0x8B, 0x13, 0x48, 0x85, 0xD2, 0x74, 0xEB, 0x48, 0x8B, 0xD2, 0x41, 0xB0, 0x00, 0x41, 0xB1, 0x00, 0xC7, 0x44, 0x24, 0x30, 0x00, 0x00, 0x00, 0x00, 0xC7, 0x44, 0x24, 0x28, 0x01, 0x00, 0x00, 0x00, 0xC7, 0x44, 0x24, 0x20, 0x00, 0x00, 0x00, 0x00},
        ASM_call_FF15, utils.addressToLittleEndianBytes(memory.gameAssemblyBaseAddress + GameAssembly::Method::TasksHandler::CompleteTask),
        std::vector<byte>{0x48, 0xB9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBA, 0x01, 0x00, 0x00, 0x00, 0x4D, 0x31, 0xC0},
        //ASM_call_FF15,utils.addressToLittleEndianBytes(memory.gameAssemblyBaseAddress + GameAssembly::Method::PlayerPropertiesManager::ChangeReadyState),
        std::vector<byte>{0x48, 0x83, 0xC4, 0x48},
        ASM_ret
    });

    return bytes;
}
