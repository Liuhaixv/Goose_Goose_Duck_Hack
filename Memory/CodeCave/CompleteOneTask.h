#pragma once
#include "CallableFunction.h"
class CompleteOneTask :    public CallableFunction
{
private:
    std::vector<byte> getFunctionBytes();
};

