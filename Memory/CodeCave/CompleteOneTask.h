﻿#pragma once
#include "CallableFunction.h"
class CompleteOneTask :    public CallableFunction
{
public:
    CompleteOneTask() {
        this->allocateMemory();
    }
private:
    std::vector<byte> getFunctionBytes();
};

