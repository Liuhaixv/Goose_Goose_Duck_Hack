#include "CodeCave.h"
#include"../Memory/Memory.h"

extern Memory memory;
void CodeCave::StaticField::flushUpdatedIntoMemory()
{
    memory.write_bytes(this->codeCave->staticFieldEntry, this->staticFieldBytes.get(), this->staticFieldSize);
}

std::vector<byte> CodeCave::buildCodeCave()
{
    std::vector<byte> result;

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
