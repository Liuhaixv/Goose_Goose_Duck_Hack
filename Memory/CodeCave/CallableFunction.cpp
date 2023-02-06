#include "CallableFunction.h"
#include"../Memory.h"

extern Memory memory;

CallableFunction::~CallableFunction()
{
    if (this->entryAddress != NULL) {
        memory.FreeMemory(this->entryAddress);
        this->entryAddress = NULL;
    }
}

bool CallableFunction::init()
{
    if (memory.processHandle == NULL) {
        return false;
    }

    allocateMemory();

    return true;
}

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
