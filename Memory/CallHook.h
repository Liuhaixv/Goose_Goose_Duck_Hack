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
    /// <param name="offsetToGameAssembly"></param>
    /// <param name="rawBytesPart1">hook跳转后立即执行的代码</param>
    /// <param name="rawBytesPart2">hook跳转后执行完逻辑再执行的代码</param>
    CallHook(int64_t offsetToGameAssembly, std::vector<byte> rawBytesPart1, std::vector<byte> rawBytesPart2) {
        this->offsetToGameAssembly = offsetToGameAssembly;
        this->rawBytesPart1 = rawBytesPart1;
        this->rawBytesPart2 = rawBytesPart2;

        this->rawBytes = rawBytesPart1;
        this->rawBytes.insert(this->rawBytes.end(), this->rawBytesPart2.begin(), this->rawBytesPart2.end());
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
    //否：call调用完函数后再执行被覆盖的字节，然后再ret,
    //默认fasle，因为原函数很可能修改关于rsp等数值
    bool b_hookAfterCoveredRawBytes = false;

    //被hook覆盖的字节
    std::vector<byte> rawBytes;

    //分前后两次执行
    std::vector<byte> rawBytesPart1;
    std::vector<byte> rawBytesPart2;

    bool b_hasHooked = false;

    bool hook();

    bool unhook();

    //计算entryAddress
    bool init();

    static const int coveredBytes = 12;

private:
    int64_t offsetToGameAssembly = NULL;
    //包括因为覆盖后多余无效的多余指令
    std::vector<byte> hookedBytes;

};

