#pragma once
#include <vector>

#undef byte
typedef unsigned char byte;

class CallableFunction
{
public:
    /// <summary>
    /// 申请可执行内存并存放要执行的函数的所有汇编字节
    /// </summary>
    CallableFunction() {

    }

    bool init();

    void call();

    int64_t getEntryAddress() {
        return this->entryAddress;
    }

protected:
    //申请内存并写入字节数组
    void allocateMemory();

private:
    //函数的开始位置
    int64_t entryAddress = NULL;

    virtual std::vector<byte> getFunctionBytes() = 0;
};

