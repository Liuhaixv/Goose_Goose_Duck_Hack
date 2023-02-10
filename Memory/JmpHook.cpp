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

    /*会改变rax，弃用
    this->hookedBytes = {
        //ASM_mov_rax       
        0x48,0xB8,
        //address
        0,0,0,0,0,0,0,0,
        //ASM_jmp_rax
        0xFF, 0xE0
    };
    */

    this->hookedBytes = {
        //push rax   
        0x50,
        //mov rax, [address]
        0x48, 0xB8,
        //address
        0,0,0,0,0,0,0,0,
        //xchg qword ptr ss:[rsp], rax 交换寄存器
        0x48, 0x87, 0x04, 0x24,
        //ret
        0xC3
    };


    for (int i = JmpHook::jmpASMbytes; i < this->coveredBytes.size(); i++) {
        //补全nop
        this->hookedBytes.push_back(0x90);//nop
    }

    *(int64_t*)(&hookedBytes[0] + 3) = this->jmpAddress;

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
