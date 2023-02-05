#include "CodeCave.h"
#include"../Memory/Memory.h"
#include "CompleteOneTask.h"
#include "GetReady.h"
#include"../../utils.hpp"

extern Memory memory;
extern Utils utils;

std::vector<byte> checkIfShouldCall(int staticFieldIndex) {
    return utils.combineVectors({
        //TODO
        //mov rax, rbx
        std::vector<byte>{0x48, 0x8B, 0xC3},
        //add rax, staticFieldIndex * 0x10
        {0x48, 0x05}, utils.addressToLittleEndianBytes(staticFieldIndex * 0x10),

        });
}

void CodeCave::StaticField::flushUpdatedIntoMemory()
{
    memory.write_bytes(this->codeCave->staticFieldEntry, this->staticFieldBytes.get(), this->staticFieldSize);
}

std::vector<byte> CodeCave::buildCodeCave()
{
    std::vector<byte> result;

    CompleteOneTask completeOneTask;
    GetReady getReady;

    std::vector<byte> beginBytes = {
        //push rbx
        0x53,
        //sub rsp, 0x40
        0x48,0x83,0xEC,0x40,
        //获取当前函数地址，即codeEntry
        //lea rax,[rip-7]
        0x48, 0x8D, 0x05, 0xF9, 0xFF, 0xFF, 0xFF,

    };

     //TODO: 添加各个要执行的函数，以及执行前的判断


        //
     std::vector endBytes = {
        //add rsp, 0x40
        0x48,0x83,0xc4,0x40,
        //pop rbx
        0x5B
     };

    return std::vector<byte>();
}
