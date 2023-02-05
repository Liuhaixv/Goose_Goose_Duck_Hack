#pragma once
#include <map>
#include<vector>
#include<memory>

typedef unsigned char byte;

/// <summary>
/// CodeCave只维护要执行的逻辑，以及由于hook覆盖的源字节数组，不维护具体hook的位置，并且执行完后会直接ret
/// 0x0 Allocated 0x1000 memory begin:
/// 0x0 staticFieldEntry
/// 0x0 static field begin:=========================
/// 0x0  entry-0x50 bool bool0;                     | static filed size = 0x50 bytes
/// 0x10 entry-0x40 int int0;                       | 
/// 0x20 entry-0x30 float float0;                   | entry
/// 0x30 entry-0x20 double double0;                 |
/// 0x40 entry-0x10 int int1;                       |
/// 0x50 static field end;==========================
/// 0x50 entry;*************************************
/// 0x50 下面是伪代码演示
/// 0x50 codeEntry
/// 
/// if([entry-50]) {
///   [entry-50] = false;
///   call func0;
/// }
/// if([entry-0x40] > 2){
///   //减去1，然后执行函数
///   mov rax, [entry-0x40]
///   sub byte rax, 1
///   mov [entry-0x40], rax
///   call func1;
/// }
/// ...
/// if(...){
///   ...
///   cal funcN;
/// }
/// //返回被hook的函数
/// 0x??? ret
/// 
///
///
/// 0x0FFE ...
/// 0x0FFF ...
/// 0x1000 Allocated 0x1000 memory end;
/// </summary>
class CodeCave {
public:
    class StaticField {
    public:
        CodeCave::StaticField(CodeCave* codeCave) {
            this->codeCave = codeCave;
        }
        //将更新过的静态区写入CodeCave
        void flushUpdatedIntoMemory();

        template <typename var>
        void setField(int index, var value) {
            byte* bytes = this->staticFieldBytes.get();
            *(var*)(bytes + (index * 0x10)) = value;
        }
    private:
        CodeCave* codeCave;

        //静态变量区的大小
        const int staticFieldSize = 0x500;
        //静态区
        std::unique_ptr<byte[]> staticFieldBytes = std::make_unique<byte[]>(staticFieldSize);
    };

    CodeCave() : staticField(this) {

    }

    int64_t staticFieldEntry = NULL;
    int64_t codeEntry = NULL;

    CodeCave::StaticField staticField;

    /// <summary>
    /// 构建代码洞穴
    /// </summary>
    /// <returns></returns>
    std::vector<byte> buildCodeCave();

private:

    //保存所有要调用的函数的绝对地址
    std::vector<int64_t> functionsToCallInOrder;

    /// <summary>
    /// 
    /// </summary>
    /// <param name="vector"></param>
    /// <param name="bytes"></param>
    void jneCheckCallFunction(std::vector<byte> bytes, int64_t functionAddr) {

    }
};


