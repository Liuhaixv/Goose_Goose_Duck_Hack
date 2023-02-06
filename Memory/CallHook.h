#pragma once
#include<Windows.h>
#include<vector>

/// <summary>
/// Hook类不需要申请可执行空间，只负责覆写字节，来跳转到对应的CodeCave的函数
/// Call another func, so it can return to original code later when ret
/// </summary>
class CallHook
{
public:
    CallHook(int64_t offsetToGameAssembly, std::vector<byte> rawBytes) {
        this->offsetToGameAssembly = offsetToGameAssembly;
        this->rawBytes = rawBytes;
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="callAddress"></param>
    void setCallAddress( int64_t callAddress);

    //开始hook覆盖的第一个字节
    int64_t hookEntry = NULL;
    //hook跳转的call函数地址
    int64_t callAddress = NULL;

    //是：执行被覆盖的字节后再call调用函数
    //否：call调用完函数后再执行被覆盖的字节，然后再ret
    //默认true
    bool b_hookAfterCoveredRawBytes = true;

    //被hook覆盖的字节
    std::vector<byte> rawBytes;

    bool b_hasHooked = false;

    bool hook();

    bool unhook();

    //计算entryAddress
    bool init();

private:
    int64_t offsetToGameAssembly = NULL;
    std::vector<byte> hookedBytes;

};

