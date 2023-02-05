#pragma once
#include"CallableFunction.h"
class GetReday : public CallableFunction
{
private:
    std::vector<byte> getFunctionBytes();
};

