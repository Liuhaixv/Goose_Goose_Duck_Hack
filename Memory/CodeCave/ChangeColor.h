#pragma once
#include "CallableFunction.h"
class ChangeColor :
    public CallableFunction
{
public:
    ChangeColor() {
        allocateMemory();
    }

private:
    std::vector<byte> getFunctionBytes();
};

