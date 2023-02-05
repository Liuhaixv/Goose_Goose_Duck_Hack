#include "CallableFunction.h"
#include"../Memory.h"

extern Memory memory;

void CallableFunction::call()
{
    //TODO
}

void CallableFunction::allocateMemory()
{
    std::vector<byte> bytes = this->getFunctionBytes();

    memory.allocExecutableMemory(0x1000, &this->entryAddress);
    memory.write_bytes(this->entryAddress, bytes);
}
