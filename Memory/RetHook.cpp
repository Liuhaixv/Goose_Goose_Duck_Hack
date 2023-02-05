#include "RetHook.h"
#include"../Memory/Memory.h"
#include"../utils.hpp"

extern Memory memory;
extern Utils utils;

void CallHook::hook(int64_t funcAddress)
{
    this->hookedBytes = {
        //ASM_mov_rax       
        0x48,0xB8,
        //address
        0,0,0,0,0,0,0,0,
        //ASM_call_rax
        0xFF, 0xD0
    };

    *(int64_t*)(&hookedBytes[0] + 2) = funcAddress;

    memory.write_bytes(this->hookEntry, hookedBytes);
}

void CallHook::unhook()
{
    memory.write_bytes(hookEntry, coveredBytes);
}
