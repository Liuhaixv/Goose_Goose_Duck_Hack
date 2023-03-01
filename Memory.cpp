#include "Memory.h"

#include<TlHelp32.h>
#include <iostream>
#include <tchar.h> // _tcscmp

#include"utils.hpp"

#include <initializer_list>
#include "Class/Hack.hpp"
#include "Struct/UserSettings.hpp"
#include "Class/HotkeyUpdater.hpp"
#include "Class/BytesPatchUpdater.hpp"
#include "Class/GameProcessUpdater.hpp"
#include "Class/DataUpdater.hpp"
#include "Client.h"

extern Utils utils;
extern Hack hack;
extern Client g_client;
extern UserSettings userSettings;

extern HotkeyUpdater hotkeyUpdater;
extern DataUpdater dataUpdater;
extern BytesPatchUpdater bytesUpdater;
extern MemoryUpdater memoryUpdater;

extern std::vector<Updater*> updaters;
//Public

void Memory::reset() {

    //首先检查是否需要unhook
    bytesUpdater.unhookAll();

    //暂停写入
    pauseWriteToMemory();

    this->pID = NULL;
    this->processHandle = NULL;
    this->gameAssemblyBaseAddress = NULL;
}

void Memory::searchGameProcessAndAttach() {
    searchGameProcess();

    //默认附加到最后一个搜索到的游戏进程
    if (pIDs.size() > 0) {
        hackSettings.gameStateSettings.openProcessState = attachToGameProcess(pIDs[pIDs.size() - 1]);
    }
}

void Memory::searchGameProcess() {
    get_procId_by_name(processName, &pIDs);
    DWORD pid = NULL;
    if (this->pIDs.size() == 0) {
        pid = NULL;
    }
    else {
        pid = this->pIDs[0];
    }
}

OpenProcessState Memory::attachToGameProcess(DWORD pid) {
    bool firstTimeAttach = true;

    //本次附加进程非第一次，需要重新初始化整个程序
    if (this->pID != NULL) {
        firstTimeAttach = false;
        this->reset();
    }

    if (pid == NULL) {
        //utils.print("Please Launch the game before running this debug tool!", "请在打开辅助前运行游戏！");
        //std::cout << std::endl;
        hackSettings.gameStateSettings.openProcessState = OpenProcessState::GameNotFound;
        return OpenProcessState::GameNotFound;
    }
    //utils.print("Detected game pid:", "检测到游戏进程pid:");
    //std::cout << pID << std::endl;

    processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (processHandle == INVALID_HANDLE_VALUE || processHandle == NULL) { // error handling
        //-1: Game found but failed to open
        //std::cout << "Failed to open process" << std::endl;
        hackSettings.gameStateSettings.openProcessState = OpenProcessState::GameFoundButFailedToOpen;
        return OpenProcessState::GameFoundButFailedToOpen;
    }

    char gameAssemblyModuleName[] = "GameAssembly.dll";
    gameAssemblyBaseAddress = GetModuleBaseAddress(_T(gameAssemblyModuleName), pid);

    //Game found but failed to load dll module
    if (gameAssemblyBaseAddress == NULL) {
        hackSettings.gameStateSettings.openProcessState = OpenProcessState::FailedToLoadDLL;
        return OpenProcessState::FailedToLoadDLL;
    }

    //Success
    this->pID = pid;
    hackSettings.gameStateSettings.openProcessState = OpenProcessState::GameFoundAndLoadedDLL;

    if (!firstTimeAttach) {
        //重置所有extern

        new(&utils) Utils();
        new(&hack) Hack();
        new(&hackSettings) HackSettings();
        new(&g_client) Client();
        hack.setClient(&g_client);
        new(&userSettings) UserSettings();

        new(&hotkeyUpdater) HotkeyUpdater(&hackSettings);
        new(&dataUpdater) DataUpdater(&g_client);

        bytesUpdater.~BytesPatchUpdater();
        new(&bytesUpdater) BytesPatchUpdater();

        new(&memoryUpdater) MemoryUpdater(&g_client, &hackSettings);

        //恢复写入 
        this->resumeWriteToMemory();
    }

    return OpenProcessState::GameFoundAndLoadedDLL;
}

bool Memory::write_bytes(IN const int64_t address, IN const byte* bytes, IN const int bytesNum) {
    if (hackSettings.b_debug_disableWriteMemory) {
        return false;
    }
    return WriteProcessMemory(processHandle,
        (LPVOID)address,
        bytes,
        bytesNum,
        NULL);
}

bool Memory::write_bytes(IN const int64_t address, IN std::initializer_list<std::vector<byte>> bytes_vectors) {

    if (hackSettings.b_debug_disableWriteMemory) {
        return false;
    }

    std::vector<byte> bytes;
    //遍历拼接Vector
    for (auto v : bytes_vectors) {
        bytes.insert(bytes.end(), v.begin(), v.end());
    }

    const byte* bytesArr = &bytes[0];

    return WriteProcessMemory(processHandle, (LPVOID)address, bytesArr, bytes.size(), NULL);
}

/*
template <typename var>
bool Memory::write_mem(IN int64_t address, IN var value) {
    if (hackSettings.b_debug_disableWriteMemory) {
        return true;
    }
    return WriteProcessMemory(processHandle, (LPVOID)address, &value, sizeof(var), NULL);
}

template <typename var>
var Memory::read_mem(IN int64_t address, IN var defaultValue) {
    var value;
    if (!this->isAddressInMemoryRegions(address)) {
        return defaultValue;
    }
    ReadProcessMemory(processHandle, (LPCVOID)address, &value, sizeof(var), NULL);
    return value;
}*/

int64_t Memory::FindPointer(IN int64_t moduleBaseAddress, IN int offset_num, IN int64_t offsets[])
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

int64_t Memory::FindPointer(IN int64_t baseAddress, IN std::vector<int64_t> offsets)
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

bool Memory::allocExecutableMemory(SIZE_T size, int64_t* address) {
    if (this->processHandle == NULL) {
        return false;
    }

    *address = (int64_t)VirtualAllocEx(
        //游戏的进程句柄
        this->processHandle,
        //不指定内存地址
        NULL,
        //内存大小
        size,
        //提交
        MEM_COMMIT | MEM_RESERVE,
        //可执行可读写
        PAGE_EXECUTE_READWRITE
    );

    if (*address == NULL) {
        return false;
    }

    return true;
}

//Private

void Memory::pauseWriteToMemory() {
    hackSettings.b_debug_disableWriteMemory = true;
    //暂停所有Updater
    for (Updater* updater : updaters) {
        if (updater == nullptr || updater->paused) {
            continue;
        }
        updater->pause();
    }
}

void Memory::resumeWriteToMemory() {
    hackSettings.b_debug_disableWriteMemory = false;
    //恢复所有Updater
    for (Updater* updater : updaters) {
        if (updater == nullptr || !updater->paused) {
            continue;
        }
        updater->unpause();
    }
}


int64_t Memory::GetModuleBaseAddress(TCHAR* lpszModuleName, DWORD pID) {
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

bool Memory::isAddressInMemoryRegions(IN int64_t address) {
    MEMORY_BASIC_INFORMATION info;
    VirtualQueryEx(this->processHandle, (LPCVOID)address, &info, sizeof(info));
    return info.State == MEM_COMMIT;
}

DWORD Memory::get_procId_by_name(IN const std::string_view targetProcess) {
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

void Memory::get_procId_by_name(IN const std::string_view targetProcess, OUT std::vector<DWORD>* pIDs) {
    pIDs->clear();
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
                    pIDs->push_back(procId);
                    //std::cout << "found pID:" << procId << std::endl;
                    //break;
                }
            } while (Process32Next(hSnap, &procEntry));
        }
    }
    CloseHandle(hSnap);
}