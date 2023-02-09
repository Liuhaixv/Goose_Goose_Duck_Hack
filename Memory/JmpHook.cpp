#include "JmpHook.h"
#include"../Memory/Memory.h"
#include"../utils.hpp"

extern Memory memory;
extern Utils utils;

void JmpHook::setJmpAddress(int64_t jmpAddress)
{
    this->jmpAddress = jmpAddress;
}

bool JmpHook::hook()
{
    if (memory.processHandle == NULL || memory.gameAssemblyBaseAddress == NULL) {
        return false;
    }

    if (this->jmpAddress == NULL) {
        //TODO: ERR
        return false;
    }

    this->hookedBytes = {
        //ASM_mov_rax       
        0x48,0xB8,
        //address
        0,0,0,0,0,0,0,0,
        //ASM_jmp_rax
        0xFF, 0xE0
    };

    for (int i = JmpHook::jmpASMbytes; i < this->coveredBytes.size(); i++) {
        //补全nop
        this->hookedBytes.push_back(0x90);//nop
    }

    *(int64_t*)(&hookedBytes[0] + 2) = this->jmpAddress;

    memory.write_bytes(this->hookEntry, hookedBytes);

    this->b_hasHooked = true;

    return true;
}

bool JmpHook::unhook()
{
    if (memory.gameAssemblyBaseAddress == NULL || memory.processHandle == NULL) {
        return false;
    }
    memory.write_bytes(hookEntry, coveredBytes);

    this->b_hasHooked = false;
    return true;
}

bool JmpHook::init()
{
    if (memory.processHandle == NULL) {
        return false;
    }

    this->hookEntry = memory.gameAssemblyBaseAddress + this->offsetToGameAssembly;
    this->jmpBackAddress = memory.gameAssemblyBaseAddress + this->jmpBackAddressOffsetToGameAssembly;

    return true;
}
