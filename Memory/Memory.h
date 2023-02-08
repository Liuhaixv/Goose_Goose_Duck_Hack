#pragma once

#include <Windows.h>
#include<memory>
#include<string>
#include <vector>

#include "../Enum/OpenProcessState.hpp"

#include"../Struct/HackSettings.hpp"

#include"PatternScanner.h"

extern HackSettings hackSettings;

class Memory {
public:
    std::string processName = "Goose Goose Duck.exe";
    std::vector<DWORD> pIDs;
    DWORD pID = NULL;
    HANDLE processHandle = NULL;
    int64_t gameAssemblyBaseAddress = NULL;
    int64_t unityPlayerBaseAddress = NULL;

    //TODO
    std::unique_ptr<PatternScanner> gameAssemblyPatternScanner;

    Memory() {
        searchGameProcessAndAttach();
    }

    /// <summary>
    /// 重置Memory类，将辅助恢复到刚启动的状态
    /// </summary>
    void reset();

    /// <summary>
    /// 搜索游戏并尝试附加到最后一个找到的游戏进程
    /// </summary>
    void searchGameProcessAndAttach();

    /// <summary>
    /// 搜索游戏进程并更新pid列表
    /// </summary>
    /// <returns></returns>
    void searchGameProcess();

    /// <summary>
    /// 附加读写到游戏进程
    /// </summary>
    /// <returns></returns>
    OpenProcessState attachToGameProcess(DWORD pid);

    /*
    bool write_bytes(IN const int64_t address, IN const std::vector<byte>& bytes) {
        if (hackSettings.b_debug_disableWriteMemory) {
            return true;
        }
        const byte* bytesArr = &bytes[0];

        return WriteProcessMemory(processHandle, (LPVOID)address, bytesArr, bytes.size(), NULL);
    }
    */

    /// <summary>
    /// 创建一个不带参数的远程线程
    /// </summary>
    /// <param name="address"></param>
    bool createRemoteThread(int64_t address);

    bool write_bytes(IN const int64_t address, IN const byte* bytes, IN const int bytesNum);

    bool write_bytes(IN const int64_t address, IN std::vector<byte> bytes_vector);

    bool write_bytes(IN const int64_t address, IN std::initializer_list<std::vector<byte>> bytes_vectors);

    template <typename var>
    bool write_mem(IN const int64_t address, IN var value) {
        if (hackSettings.b_debug_disableWriteMemory) {
            return false;
        }
        SIZE_T bytesWrote = 0;
        WriteProcessMemory(processHandle, (LPVOID)address, &value, sizeof(var), &bytesWrote);
        return bytesWrote != 0;
    }

    /// <summary>
    /// 由于无法返回是否读取成功，已废弃
    /// </summary>
    /// <typeparam name="var"></typeparam>
    /// <param name="address"></param>
    /// <param name="defaultValue"></param>
    /// <returns></returns>
    template <typename var>
    [[DEPRECATED]] var read_mem(IN const int64_t address, IN var defaultValue) {
        var value;
        if (!this->isAddressInMemoryRegions(address)) {
            return defaultValue;
        }
        ReadProcessMemory(processHandle, (LPCVOID)address, &value, sizeof(var), NULL);
        return value;
    }

    //TODO: 用这个函数替换所有使用deprecated的read_mem方法的代码
    template <typename var>
    bool read_mem_EX(IN const int64_t address, OUT var& value) {
        if (!this->isAddressInMemoryRegions(address)) {
            return false;
        }
        SIZE_T bytesRead = 0;

        bool success = ReadProcessMemory(processHandle, (LPCVOID)address, &value, sizeof(var), &bytesRead);

        return bytesRead != 0;
    }

    //void copy_bytes(int64_t src_address, int64_t dst_address, int64_t numOfBytes) {
    //    ReadProcessMemory(processHandle, (LPCVOID)src_address, &dst_address, numOfBytes, NULL);
    //}

    int64_t FindPointer(IN int64_t moduleBaseAddress, IN int offset_num, IN int64_t offsets[]);

    int64_t FindPointer(IN int64_t baseAddress, IN std::vector<int64_t> offsets);

    auto FindPattern(std::vector<byte> Pattern) const->int64_t;

    /// <summary>
    /// 申请可执行内存
    /// </summary>
    bool allocExecutableMemory(IN SIZE_T size,OUT int64_t* address);

    /// <summary>
    /// 释放申请的内存
    /// </summary>
    void FreeMemory(int64_t address);

private:
    //保存GameAssembly的所有字节
    std::unique_ptr<byte[]> gameAssemblyDLLData;
    //GameAssembly的字节数
    const int64_t* gameAssemblyDLLDataSize;

    //读取GameAssembly的所有字节
    bool readModuleBytes(IN const char* moduleName, OUT std::unique_ptr<byte[]> data, OUT int64_t* size);

    /// <summary>
    /// 暂停程序写入内存
    /// </summary>
    void pauseWriteToMemory();

    /// <summary>
    /// 恢复程序写入内存
    /// </summary>
    void resumeWriteToMemory();

    /// <summary>
    /// 获取64位进程的模块的基址<para/>
    /// Get baseAddress of x64 process's module
    /// </summary>
    /// <param name="lpszModuleName">Name of module 模块的名称</param>
    /// <param name="pID">pid of process 进程pid</param>
    /// <returns>address 地址</returns>
    int64_t GetModuleBaseAddress(TCHAR* lpszModuleName, DWORD pID);

    /// <summary>
    /// 判断地址是否在进程的内存范围内
    /// </summary>
    /// <param name="address"></param>
    /// <returns></returns>
    bool isAddressInMemoryRegions(IN int64_t address);

    /*
    //works in x86
    DWORD GetModuleBaseAddress(TCHAR* lpszModuleName, DWORD pID) {
        DWORD dwModuleBaseAddress = 0;
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID); // make snapshot of all modules within process
        MODULEENTRY32 ModuleEntry32 = { 0 };
        ModuleEntry32.dwSize = sizeof(MODULEENTRY32);

        if (Module32First(hSnapshot, &ModuleEntry32)) //store first Module in ModuleEntry32
        {
            do {
                if (_tcscmp(ModuleEntry32.szModule, lpszModuleName) == 0) // if Found Module matches Module we look for -> done!
                {
                    dwModuleBaseAddress = (DWORD)ModuleEntry32.modBaseAddr;
                    break;
                }
            } while (Module32Next(hSnapshot, &ModuleEntry32)); // go through Module entries in Snapshot and store in ModuleEntry32


        }
        CloseHandle(hSnapshot);
        return dwModuleBaseAddress;
    }
    */

    /// <summary>
    /// 通过名称获取进程pid<para/>
    /// Get process' pid by name
    /// </summary>
    /// <param name="targetProcess"></param>
    /// <returns></returns>
    static DWORD get_procId_by_name(IN const std::string_view targetProcess);

    /// <summary>
    /// 通过名称获取进程pid<para/>
    /// Get process' pid by name
    /// </summary>
    /// <param name="targetProcess"></param>
    /// <returns></returns>
    static void get_procId_by_name(IN const std::string_view targetProcess, OUT std::vector<DWORD>* pIDs);
};
