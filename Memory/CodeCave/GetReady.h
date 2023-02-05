#pragma once
#include"CallableFunction.h"
class GetReady : public CallableFunction
{
public:
    GetReady() {
        this->allocateMemory();
    }

private:
    std::vector<byte> getFunctionBytes();
};