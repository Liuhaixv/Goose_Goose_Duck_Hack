#pragma once
#include "CallableFunction.h"
class CompleteOneTask :    public CallableFunction
{
public:
    CompleteOneTask() {
        allocateMemory();
    }
private:
    std::vector<byte> getFunctionBytes();
};

