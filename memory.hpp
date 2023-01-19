#pragma once

#include <Windows.h>
#include<TlHelp32.h>
#include <iostream>
#include <tchar.h> // _tcscmp
#include <vector>

#include"Struct/HackSettings.hpp"

#include"utils.hpp"
#include "Enum/OpenProcessState.hpp"
#include <map>

extern Utils utils;
extern HackSettings hackSettings;

class Memory {
public:
    std::string processName = "Goose Goose Duck.exe";
    DWORD pID = NULL;
    HANDLE processHandle = NULL;
    int64_t gameAssemblyBaseAddress = NULL;

    Memory() {
        searchGameProcess();
    }

    /// <summary>
    /// 搜索游戏进程
    /// </summary>
    /// <returns></returns>
    OpenProcessState searchGameProcess() {
        pID = get_porcId_by_name(processName);
        //TODO: 转移到GUI
        if (pID == NULL) {
            utils.print("Please Launch the game before running this debug tool!", "请在打开辅助前运行游戏！");
            std::cout << std::endl;
            return OpenProcessState::GameNotFound;
        }
        utils.print("Detected game pid:", "检测到游戏进程pid:");
        std::cout << pID << std::endl;

        processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
        if (processHandle == INVALID_HANDLE_VALUE || processHandle == NULL) { // error handling
            //-1: Game found but failed to open
            std::cout << "Failed to open process" << std::endl;
            return OpenProcessState::GameFoundButFailedToOpen;
        }

        char gameAssemblyModuleName[] = "GameAssembly.dll";
        gameAssemblyBaseAddress = GetModuleBaseAddress(_T(gameAssemblyModuleName), pID);

        //Game found but failed to load dll module
        if (gameAssemblyBaseAddress == NULL) {
            return OpenProcessState::FailedToLoadDLL;
        }

        //Success
        return OpenProcessState::GameFoundAndLoadedDLL;
    }

    bool write_bytes(IN int64_t address, IN const byte* bytes, IN const int bytesNum) {
        if (hackSettings.b_debug_disableWriteMemory) {
            return true;
        }
        return WriteProcessMemory(processHandle, (LPVOID)address, bytes, bytesNum, NULL);
    }

    template <typename var>
    bool write_mem(IN int64_t address, IN var value) {
        if (hackSettings.b_debug_disableWriteMemory) {
            return true;
        }
        return WriteProcessMemory(processHandle, (LPVOID)address, &value, sizeof(var), NULL);
    }

    template <typename var>
    var read_mem(IN int64_t address, IN var defaultValue) {
        var value;
        if (!this->isAddressInMemoryRegions(address)) {
            return defaultValue;
        }
        ReadProcessMemory(processHandle, (LPCVOID)address, &value, sizeof(var), NULL);
        return value;
    }

    //void copy_bytes(int64_t src_address, int64_t dst_address, int64_t numOfBytes) {
    //    ReadProcessMemory(processHandle, (LPCVOID)src_address, &dst_address, numOfBytes, NULL);
    //}

    int64_t FindPointer(IN int64_t moduleBaseAddress, IN int offset_num, IN int64_t offsets[])
    {
        if (offset_num <= 0) {
            return NULL;
        }

        int64_t Address = moduleBaseAddress + offsets[0];
        Address = read_mem<int64_t>(Address, 0);
        //ReadProcessMemory(processHandle, (LPCVOID)Address, &Address, sizeof(DWORD), NULL);

        if (Address == 0) {
            return NULL;
        }

        for (int i = 1; i < offset_num; i++) //Loop trough the offsets
        {
            Address = read_mem<int64_t>(Address, 0);
            //ReadProcessMemory(processHandle, (LPCVOID)Address, &Address, sizeof(DWORD), NULL);
            if (Address == 0) {
                return NULL;
            }
            Address += offsets[i];
        }
        return Address;
    }

    int64_t FindPointer(IN int64_t baseAddress, IN std::vector<int64_t> offsets)
    {
        if (offsets.size() == 0) {
            return NULL;
        }

        int64_t Address = baseAddress + offsets[0];
        Address = read_mem<int64_t>(Address, 0);
        //ReadProcessMemory(processHandle, (LPCVOID)Address, &Address, sizeof(DWORD), NULL);

        if (offsets.size() == 1) {
            if (isAddressInMemoryRegions(Address)) {
                return Address;
            }
            return NULL;
        }

        Address += offsets[1];

        for (int i = 2; i < offsets.size(); i++) //Loop trough the offsets
        {
            Address = read_mem<int64_t>(Address, 0);

            //ReadProcessMemory(processHandle, (LPCVOID)Address, &Address, sizeof(DWORD), NULL);

            if (Address == 0 || !isAddressInMemoryRegions(Address)) {
                return NULL;
            }
            Address += offsets[i];
        }
        return Address;
    }



private:

    /// <summary>
    /// 获取64位进程的模块的基址<para/>
    /// Get baseAddress of x64 process's module
    /// </summary>
    /// <param name="lpszModuleName">Name of module 模块的名称</param>
    /// <param name="pID">pid of process 进程pid</param>
    /// <returns>address 地址</returns>
    int64_t GetModuleBaseAddress(TCHAR* lpszModuleName, DWORD pID) {
        int64_t dwModuleBaseAddress = NULL;
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID); // make snapshot of all modules within process
        MODULEENTRY32 ModuleEntry32 = { 0 };
        ModuleEntry32.dwSize = sizeof(MODULEENTRY32);

        if (Module32First(hSnapshot, &ModuleEntry32)) //store first Module in ModuleEntry32
        {
            do {
                if (_tcscmp(ModuleEntry32.szModule, lpszModuleName) == 0) // if Found Module matches Module we look for -> done!
                {
                    dwModuleBaseAddress = (int64_t)ModuleEntry32.modBaseAddr;
                    break;
                }
            } while (Module32Next(hSnapshot, &ModuleEntry32)); // go through Module entries in Snapshot and store in ModuleEntry32


        }
        CloseHandle(hSnapshot);
        return dwModuleBaseAddress;
    }

    /// <summary>
    /// 判断地址是否在进程的内存范围内
    /// </summary>
    /// <param name="address"></param>
    /// <returns></returns>
    bool isAddressInMemoryRegions(IN int64_t address) {
        MEMORY_BASIC_INFORMATION info;
        VirtualQueryEx(this->processHandle, (LPCVOID)address, &info, sizeof(info));
        return info.State == MEM_COMMIT;
    }

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
    static DWORD get_porcId_by_name(const std::string_view targetProcess) {
        DWORD procId = 0;
        HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnap != INVALID_HANDLE_VALUE)
        {
            PROCESSENTRY32 procEntry;
            procEntry.dwSize = sizeof(procEntry);
            if (Process32First(hSnap, &procEntry))
            {
                do
                {
                    if (!targetProcess.compare(procEntry.szExeFile))
                    {
                        procId = procEntry.th32ProcessID;
                        //std::cout << "found pID:" << procId << std::endl;
                        //break;
                    }
                } while (Process32Next(hSnap, &procEntry));
            }
        }
        CloseHandle(hSnap);

        return procId;
    }
};
