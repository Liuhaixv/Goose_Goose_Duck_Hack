#pragma once
#include <vector>

typedef unsigned char byte;

class CallableFunction
{
public:
    /// <summary>
    /// 申请可执行内存并存放要执行的函数的所有汇编字节
    /// </summary>
    CallableFunction();
private:
    //函数的开始位置
    int64_t entryAddress = NULL;

    void allocateMemory();

    virtual std::vector<byte> getFunctionBytes() = 0;
};

