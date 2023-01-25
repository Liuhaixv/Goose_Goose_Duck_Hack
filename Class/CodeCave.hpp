#pragma once
#include<Windows.h>

#define byte unsigned char

/// <summary>
/// 代码洞穴的结构为
/// 1.静态变量，目前保存bool值作为flag，标记注入代码是否被执行过
/// 2.代码区，存放注入代码的逻辑
///
/// 其中，代码区分为2部分：
/// 1.ret返回区：用于跳转回原函数
/// 2.注入区：用于存放原函数被注入跳转后的逻辑
/// </summary>
class CodeCave {
    

    static byte* hook_update_return(OUT int* bytesNum) {
        byte hookedBytes[] = { 0x48};

        //传出字节总数
        *bytesNum = sizeof(hookedBytes) / sizeof(byte);
    }
};