#pragma once
#include<Windows.h>
#include<vector>

/// <summary>
/// Call another func, so it can return to original code later when ret
/// </summary>
class CallHook
{
public:
    CallHook() {

    }
    CallHook(bool b_hookAfterCoveredBytes) {
        this->b_hookAfterCoveredBytes = b_hookAfterCoveredBytes;

    }

    //开始hook覆盖的第一个字节
    int64_t hookEntry = NULL;
    //hook跳转的call函数地址
    int64_t callAddress = NULL;

    //是：执行被覆盖的字节后再call调用函数
    //否：call调用完函数后再执行被覆盖的字节，然后再ret
    bool b_hookAfterCoveredBytes = true;

    //被hook覆盖的字节
    std::vector<byte> coveredBytes;

    void hook(int64_t funcAddress);

    void unhook();

private:
    std::vector<byte> hookedBytes;
};

