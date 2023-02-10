#pragma once
#include<Windows.h>
#include<vector>

/// <summary>
/// Hook类不需要申请可执行空间，只负责覆写字节，来跳转到对应的CodeCave的函数
/// Call another func, so it can return to original code later when ret
/// </summary>
class JmpHook
{
public:

    /// <summary>
    /// 从GameAssembly.dll + offsetToGameAssembly处hook，跳转到指定CodeCave，先执行coveredBytes，然后执行codeCave代码,然后jmp至jmpBackAddressOffsetToGameAssembly地址
    /// </summary>
    /// <param name="offsetToGameAssembly"></param>
    /// <param name="coveredBytes"></param>
    /// <param name="jmpBackAddress"></param>
    JmpHook(int64_t offsetToGameAssembly, std::vector<byte> coveredBytes, int64_t jmpBackAddressOffsetToGameAssembly) {
        this->offsetToGameAssembly = offsetToGameAssembly;
        this->coveredBytes = coveredBytes;
        this->jmpBackAddressOffsetToGameAssembly = jmpBackAddressOffsetToGameAssembly;
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="callAddress"></param>
    void setJmpAddress( int64_t jmpAddress);

    //开始hook跳转的地址
    int64_t hookEntry = NULL;
    //hook跳转的call函数地址
    int64_t jmpAddress = NULL;
    int64_t jmpBackAddress = NULL;


    //被hook覆盖失效的字节
    //coveredBytes数量>= jmp指令所用字节数
    std::vector<byte> coveredBytes;

    bool b_hasHooked = false;

    bool hook();

    bool unhook();

    //计算entryAddress
    bool init();

    //jmp指令使用的字节数量
    static const int jmpASMbytes = 0x10;

private:
    int64_t offsetToGameAssembly = NULL;
    int64_t jmpBackAddressOffsetToGameAssembly = NULL;
    //包括因为覆盖后多余无效的多余指令
    std::vector<byte> hookedBytes;

};

