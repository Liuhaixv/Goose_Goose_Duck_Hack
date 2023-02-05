#include "CallableFunction.h"
#include"../Memory.h"

extern Memory memory;

void CallableFunction::call()
{
    //TODO: 申请临时空间并在创建线程调用后删除空间
}

void CallableFunction::allocateMemory()
{
    std::vector<byte> bytes = this->getFunctionBytes();

    memory.allocExecutableMemory(0x1000, &this->entryAddress);
    memory.write_bytes(this->entryAddress, bytes);
}
