#include "CodeCave.h"
#include"../Memory/Memory.h"
#include "CompleteOneTask.h"
#include "GetReady.h"
#include"../../utils.hpp"

extern Memory memory;
extern Utils utils;

enum FunctionOrder {
    Fn_CompleteOneTask,
    Fn_GetReady
};

std::vector<byte> checkIfShouldCall(int staticFieldIndex, int64_t functionAddress) {
    const int size = 35;
    byte bytesJE[size] = {
        //cmp cmp [rbx+1000], 0
        0x83, 0xBB, 0,0,0,0,0x00,
        //je +1a
        0x74, 0x1A,
        //sub [rbx+1000], 1
        0x83, 0xAB, 0,0,0,0, 0x01,
        //mov [rbx+1000], rax
        0x48, 0x89, 0x83,0 ,0 ,0, 0,
        //mov rax, 0x0000000000000000
        0x48, 0xB8 ,0,0,0,0,0,0,0,0,
        //call rax
        0xFF,0xD0
    };

    *(int32_t*)(bytesJE + 2) = staticFieldIndex * 10;
    *(int32_t*)(bytesJE + 11) = staticFieldIndex * 10;
    *(int32_t*)(bytesJE + 19) = staticFieldIndex * 10;
    *(int64_t*)(bytesJE + 25) = functionAddress;

    return std::vector<byte>(bytesJE, bytesJE + size);
}

void CodeCave::StaticField::flushUpdatedIntoMemory()
{
    memory.write_bytes(this->codeCave->staticFieldEntry, this->staticFieldBytes.get(), this->staticFieldSize);
}

std::vector<byte> CodeCave::buildCodeCave(CallHook* callHook)
{
    if (memory.processHandle == NULL) {
        return {};
    }

    this->completeOneTask = std::make_unique<CompleteOneTask>();
    this->getReady = std::make_unique<GetReady>();

    this->callableFunctions.clear();
    this->callableFunctions.push_back(completeOneTask.get());//Fn_CompleteOneTask,
    this->callableFunctions.push_back(getReady.get());//Fn_GetReady

    std::vector<byte> beginBytes;

    if (callHook->b_hookAfterCoveredRawBytes) {
        beginBytes = callHook->rawBytes;
    }

    beginBytes = utils.combineVectors({
            beginBytes,
            //push rbx
            {0x53,
            //获取当前函数地址，即codeEntry
            //lea rbx,[rip-8]
            0x48, 0x8D, 0x1D, 0xF8, 0xFF, 0xFF, 0xFF,
            //sub rbx, 0x1000
            0x48, 0x81,0xEB},utils.addressToLittleEndianBytes(this->staticField.staticFieldSize),
            //sub rsp, 0x40
            {0x48,0x83,0xEC,0x40}
        });

    //添加各个要执行的函数，以及执行前的判断
    std::vector<byte> functionsCallCheck = utils.combineVectors({
            checkIfShouldCall(Fn_CompleteOneTask, this->completeOneTask.get()->getEntryAddress()),
            checkIfShouldCall(Fn_GetReady, this->getReady.get()->getEntryAddress()),
        });

    std::vector<byte> endBytes = {
        //add rsp, 0x40
        0x48,0x83,0xc4,0x40,
        //pop rbx
        0x5B
    };

    if (!callHook->b_hookAfterCoveredRawBytes) {
        endBytes.insert(endBytes.end(), callHook->rawBytes.begin(), callHook->rawBytes.end());
    }

    endBytes.push_back(0xC3);//ret

    std::vector<byte> result = utils.combineVectors({
        beginBytes,
        functionsCallCheck,
        endBytes
        }
    );

    return result;
}
