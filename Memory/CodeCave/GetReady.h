#pragma once
#include"CallableFunction.h"
class GetReady : public CallableFunction
{
public:
    GetReady() {
        allocateMemory();
    }

private:
    std::vector<byte> getFunctionBytes();
};