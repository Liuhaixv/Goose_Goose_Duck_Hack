#include "CodeCave.h"
#include"../Memory/Memory.h"
#include "CompleteOneTask.h"
#include "GetReady.h"
#include"../../utils.hpp"
#include "../JmpHook.h"

extern Memory memory;
extern Utils utils;

/// <summary>
/// 构建汇编代码，判断是否要call函数，如果值大于0则减1并执行
/// </summary>
/// <param name="staticFieldIndex"></param>
/// <param name="functionAddress"></param>
/// <returns></returns>
std::vector<byte> checkIfShouldCallAndDecreaseBy1(int staticFieldIndex, int64_t functionAddress) {
    const int size = 31;
    byte bytesJE[size] = {
        //cmp cmp [rbx+0], 0
        0x83, 0xBB, 0,0,0,0, 0x00,
        //je +12
        0x74, 0x12,
        //mov eax, [rbx]
        0x8B, 0x03,
        //dec eax
        0xff,0xc8,
        //mov [rbx], eax
        0x89,0x03,
        //mov rax, 0x0000000000000000
        0x48, 0xB8 ,0,0,0,0,0,0,0,0,
        //call rax
        0xFF,0xD0,
        //add rbx, 0x10
        0x48, 0x83, 0xC3, 0x10,
    };

    //*(int32_t*)(bytesJE + 2) = staticFieldIndex * 0x10;
    *(int64_t*)(bytesJE + 17) = functionAddress;

    return std::vector<byte>(bytesJE, bytesJE + size);
}

/// <summary>
/// 判断不为0则执行
/// </summary>
/// <param name="staticFieldIndex"></param>
/// <param name="functionAddress"></param>
/// <returns></returns>
std::vector<byte> checkIfShouldCall(int staticFieldIndex, int64_t functionAddress) {
    const int size = 25;
    byte bytesJE[size] = {
        //cmp cmp [rbx+0], 0
        0x83, 0xBB, 0x00,0x00,0x00,0x00, 0x00,
        //je +0xC
        0x74, 0xC,
        //mov rax, 0x0000000000000000
        0x48, 0xB8 ,0,0,0,0,0,0,0,0,
        //call rax
        0xFF,0xD0,
        //add rbx, 0x10
        0x48, 0x83, 0xC3, 0x10,
    };

    *(int64_t*)(bytesJE + 11) = functionAddress;

    return std::vector<byte>(bytesJE, bytesJE + size);
}

void CodeCave::StaticField::writeField(int _FunctionOrder, int data)
{
    memory.write_mem<int>(this->codeCave->staticFieldEntry + _FunctionOrder * 0x10, data);
}

CodeCave::~CodeCave()
{
    memory.FreeMemory(this->staticFieldEntry);

    for (auto fun : this->callableFunctions) {
        fun->~CallableFunction();
    }
}

bool CodeCave::buildCodeCave(CallHook* callHook)
{
    if (memory.processHandle == NULL) {
        return false;
    }

    this->completeOneTask = std::make_unique<CompleteOneTask>();
    this->getReady = std::make_unique<GetReady>();
    this->changeColor = std::make_unique<ChangeColor>();
    

    this->callableFunctions.clear();
    this->callableFunctions.push_back(completeOneTask.get());//Fn_CompleteOneTask,
    this->callableFunctions.push_back(getReady.get());//Fn_GetReady
    this->callableFunctions.push_back(changeColor.get());//Fn_ChangeColor

    std::vector<byte> beginBytes;

    std::vector<byte> push_all = {
        //rax rbx rcx rdx rbp rdi rsi r8 r9 r10 r11 r12 r13 r14 r15
        0x50,
        0x53,
        0x51,
        0x52,
        0x55,
        0x57,
        0x56,
        0x41, 0x50,
        0x41, 0x51,
        0x41, 0x52,
        0x41, 0x53,
        0x41, 0x54,
        0x41, 0x55,
        0x41, 0x56,
        0x41, 0x57 };

    std::vector<byte> pop_all = {
      { 0x41, 0x5F,
        0x41, 0x5E,
        0x41, 0x5D,
        0x41, 0x5C,
        0x41, 0x5B,
        0x41, 0x5A,
        0x41, 0x59,
        0x41, 0x58,
        0x5E,
        0x5F,
        0x5D,
        0x5A,
        0x59,
        0x5B,
        0x58 }
    };

    /*
    if (callHook->b_hookAfterCoveredRawBytes) {
        for (int i = 0; i < CallHook::coveredBytes; i++) {
            beginBytes.push_back(callHook->rawBytes[i]);
        }
        for (int i = CallHook::coveredBytes; i < callHook->rawBytes.size(); i++) {
            beginBytes.push_back(0x90);//nop
        }
    }
    */

    //先执行被覆盖的前置代码part1
    if (callHook->rawBytesPart1.size() != 0)
        for (auto b : callHook->rawBytesPart1) {
            beginBytes.push_back(b);
        }

    beginBytes = utils.combineVectors({
            beginBytes,
            push_all,
            //获取当前函数地址，即codeEntry
            //lea rbx,[rip-8-16]
            { 0x48, 0x8D, 0x1D, 0xCE, 0xFF, 0xFF, 0xFF,
            //sub rbx, 0x1000
            0x48, 0x81,0xEB},utils.addressToLittleEndianBytes(this->staticField.staticFieldSize),
            //sub rsp, 0x40
            {0x48,0x83,0xEC,0x40}
        });

    //添加各个要执行的函数，以及执行前的判断
    std::vector<byte> functionsCallCheck = utils.combineVectors({
            checkIfShouldCallAndDecreaseBy1(Fn_CompleteOneTask, this->completeOneTask.get()->getEntryAddress()),
            checkIfShouldCallAndDecreaseBy1(Fn_GetReady, this->getReady.get()->getEntryAddress()),
            checkIfShouldCall(Fn_ChangeColor,this->changeColor.get()->getEntryAddress())
        });

    std::vector<byte> endBytes = utils.combineVectors({
        //add rsp, 0x40
        std::vector<byte>{ 0x48,0x83,0xc4,0x40 },
        pop_all
        });

    /*
    if (!callHook->b_hookAfterCoveredRawBytes) {
        for (int i = 0; i < CallHook::coveredBytes; i++) {
            endBytes.push_back(callHook->rawBytes[i]);
        }
        for (int i = CallHook::coveredBytes; i < callHook->rawBytes.size(); i++) {
            endBytes.push_back(0x90);//nop
        }
    }
    */

    //执行后置代码
    if (callHook->rawBytesPart2.size() != 0)
        for (auto b : callHook->rawBytesPart2) {
            endBytes.push_back(b);
        }

    endBytes.push_back(0xC3);//ret

    std::vector<byte> result = utils.combineVectors({
        beginBytes,
        functionsCallCheck,
        endBytes
        }
    );

    memory.allocExecutableMemory(0x2000, &this->staticFieldEntry);
    this->codeEntry = this->staticFieldEntry + this->staticField.staticFieldSize;

    memory.write_bytes(this->codeEntry, result);

    return true;
}

bool CodeCave::buildCodeCave(JmpHook* jmpHook)
{
    //TODO
    if (memory.processHandle == NULL) {
        return false;
    }

    this->completeOneTask = std::make_unique<CompleteOneTask>();
    this->getReady = std::make_unique<GetReady>();
    this->changeColor = std::make_unique<ChangeColor>();


    this->callableFunctions.clear();
    this->callableFunctions.push_back(completeOneTask.get());//Fn_CompleteOneTask,
    this->callableFunctions.push_back(getReady.get());//Fn_GetReady
    this->callableFunctions.push_back(changeColor.get());//Fn_ChangeColor

    std::vector<byte> beginBytes;

    std::vector<byte> push_all = {
        //rax rbx rcx rdx rbp rdi rsi r8 r9 r10 r11 r12 r13 r14 r15
        0x50,
        0x53,
        0x51,
        0x52,
        0x55,
        0x57,
        0x56,
        0x41, 0x50,
        0x41, 0x51,
        0x41, 0x52,
        0x41, 0x53,
        0x41, 0x54,
        0x41, 0x55,
        0x41, 0x56,
        0x41, 0x57 };

    std::vector<byte> pop_all = {
      { 0x41, 0x5F,
        0x41, 0x5E,
        0x41, 0x5D,
        0x41, 0x5C,
        0x41, 0x5B,
        0x41, 0x5A,
        0x41, 0x59,
        0x41, 0x58,
        0x5E,
        0x5F,
        0x5D,
        0x5A,
        0x59,
        0x5B,
        0x58 }
    };

    beginBytes = utils.combineVectors({
            beginBytes,
            /*push_all,*/
            //获取当前函数地址，即codeEntry
            //lea rbx,[rip-8-16]
            { 0x48, 0x8D, 0x1D, 0xF9, 0xFF, 0xFF, 0xFF,
            //sub rbx, 0x1000
            0x48, 0x81,0xEB},utils.addressToLittleEndianBytes(this->staticField.staticFieldSize),
            //sub rsp, 0x40
            {0x48,0x83,0xEC,0x40}
        });

    //添加各个要执行的函数，以及执行前的判断
    std::vector<byte> functionsCallCheck = utils.combineVectors({
            checkIfShouldCallAndDecreaseBy1(Fn_CompleteOneTask, this->completeOneTask.get()->getEntryAddress()),
            checkIfShouldCallAndDecreaseBy1(Fn_GetReady, this->getReady.get()->getEntryAddress()),
            checkIfShouldCall(Fn_ChangeColor,this->changeColor.get()->getEntryAddress())
        });

    std::vector<byte> endBytes = utils.combineVectors({
        //add rsp, 0x40
        std::vector<byte>{ 0x48,0x83,0xc4,0x40 },
        /*pop_all*/ });

            //执行后置代码
    for (auto b : jmpHook->coveredBytes) {
        endBytes.push_back(b);
    }

    endBytes = utils.combineVectors({
        endBytes,
        //ASM_mov_rax       
        { 0x48,0xB8},
        //address
        utils.addressToLittleEndianBytes(jmpHook->jmpBackAddress),
        //ASM_jmp_rax
        {0xFF, 0xE0}
    });
      

    std::vector<byte> result = utils.combineVectors({
        beginBytes,
        functionsCallCheck,
        endBytes
        }
    );

    memory.allocExecutableMemory(0x2000, &this->staticFieldEntry);
    this->codeEntry = this->staticFieldEntry + this->staticField.staticFieldSize;

    memory.write_bytes(this->codeEntry, result);

    return true;
}
