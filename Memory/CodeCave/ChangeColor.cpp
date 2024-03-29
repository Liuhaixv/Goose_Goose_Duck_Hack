﻿#include "ChangeColor.h"
#include "../Memory.cpp"
#include"../../Data/offsets.hpp"
#include"../../utils.hpp"

std::vector<byte> ChangeColor::getFunctionBytes()
{
    std::vector<byte> ASM_MOV_RAX = { 0x48,0xB8};
    std::vector<byte> ASM_FF15_CALL = { 0xFF,0x15,0x02,0x00,0x00,0x00,0xEB,0x08, };

    std::vector<byte> bytes = utils.combineVectors({
        {0x50,0x50,0x53,0x51,0x52,0x55,0x57,0x56,0x41,0x50,0x41,0x51,0x41,0x52,0x41,0x53,0x41,0x54,0x41,0x55,0x41,0x56,0x41,0x57,0x48,0x83,0xEC,0x28,0x45,0x31,0xC0,0xBA,0x15,0x00,0x00,0x00,0x31,0xC9,},
        ASM_FF15_CALL, utils.addressToLittleEndianBytes(memory.gameAssemblyBaseAddress + GameAssembly::Method::UnityEngine::Random::RandomRangeInt),//UnityEngine.Random.Range
        {0x50},
        ASM_MOV_RAX, utils.addressToLittleEndianBytes(memory.gameAssemblyBaseAddress + GameAssembly::Class::ptr_PlayerCustomizationPanelHandlerClass),//GameAssembly.dll+ptr_PlayerCustomizationPanelHandlerClass
        {0x48,0x8B,0x00,0x48,0x8B,0x80,0xB8,0x00,0x00,0x00,0x48,0x85,0xC0,0x0F,0x84,0x2F,0x00,0x00,0x00,0x48,0x8B,0x00,0x48,0x8B,0xD8,0x48,0x85,0xC0,0x0F,0x84,0x20,0x00,0x00,0x00,0x48,0x8B,0xC8,0x5A,0x45,0x31,0xC0,},
        ASM_FF15_CALL, utils.addressToLittleEndianBytes(memory.gameAssemblyBaseAddress + GameAssembly::Method::PlayerCustomizationPanelHandler::ChangeColor),//Handlers.LobbyHandlers.PlayerCustomizationPanelHandler.ChangeColor
        {0x48,0x83,0xC4,0x28,0xE9,0x09,0x00,0x00,0x00,0x48,0x83,0xC4,0x30,0xE9,0x00,0x00,0x00,0x00,0x41,0x5F,0x41,0x5E,0x41,0x5D,0x41,0x5C,0x41,0x5B,0x41,0x5A,0x41,0x59,0x41,0x58,0x5E,0x5F,0x5D,0x5A,0x59,0x5B,0x58,0x58,0xC3,}
     });

    return bytes;
}
