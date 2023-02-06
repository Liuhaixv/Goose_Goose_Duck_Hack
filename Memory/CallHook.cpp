#include "CallHook.h"
#include"../Memory/Memory.h"
#include"../utils.hpp"

extern Memory memory;
extern Utils utils;

void CallHook::setCallAddress(int64_t callAddress)
{
    this->callAddress = callAddress;
}

bool CallHook::hook()
{
    if (memory.processHandle == NULL || memory.gameAssemblyBaseAddress == NULL) {
        return false;
    }

    if (this->callAddress == NULL) {
        //TODO: ERR
        return false;
    }

    this->hookedBytes = {
        //ASM_mov_rax       
        0x48,0xB8,
        //address
        0,0,0,0,0,0,0,0,
        //ASM_call_rax
        0xFF, 0xD0
    };

    *(int64_t*)(&hookedBytes[0] + 2) = this->callAddress;

    memory.write_bytes(this->hookEntry, hookedBytes);

    this->b_hasHooked = true;

    return true;
}

bool CallHook::unhook()
{
    if (memory.gameAssemblyBaseAddress == NULL || memory.processHandle == NULL) {
        return false;
    }
    memory.write_bytes(hookEntry, rawBytes);

    this->b_hasHooked = false;
    return true;
}

bool CallHook::init()
{
    if (memory.processHandle == NULL) {
        return false;
    }

    this->hookEntry = memory.gameAssemblyBaseAddress + this->offsetToGameAssembly;

    return true;
}
