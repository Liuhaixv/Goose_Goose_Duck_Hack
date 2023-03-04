#ifndef ANTIAC_HPP
#define ANTIAC_HPP
#pragma once

// #define _WIN32_WINNT 0x0400 // https://docs.microsoft.com/en-us/windows/win32/api/debugapi/nf-debugapi-isdebuggerpresent
#include "Windows.h"

class AntiAC {
private:

    HMODULE module_handle;
    HANDLE process_handle;

public:
    // TODO: Run debugger checks in Thread local storage to make bypassing anti-debug more difficult
    AntiAC() {
        

        check_for_debug();

        // module_handle = GetModuleHandle(NULL);
        // erase_pe_headers();
    }

    void check_loop() {
        while (true) {
            check_for_debug();
            Sleep(1000); // sleep for 1 second before checking again
        }
    }

    void check_for_debug() {
        process_handle = GetCurrentProcess();
        BOOL has_debug_port = TRUE;

        if (IsDebuggerPresent()) ExitProcess(EXIT_SUCCESS); // running in ring-3 debugger
        if (!CheckRemoteDebuggerPresent(process_handle, &has_debug_port)) ExitProcess(EXIT_SUCCESS); // if method fails to execute - exit process
        if (has_debug_port) ExitProcess(EXIT_SUCCESS); // running in ring-3 debugger
    }

    // https://github.com/LordNoteworthy/al-khaser/blob/8ff90a3979face6e29aacb12521b032f2b379073/al-khaser/AntiDump/ErasePEHeaderFromMemory.cpp#L8-L22
    void erase_pe_headers() {
        DWORD OldProtect = 0;

        VirtualProtect((char*)module_handle, 4096, // Assume x86 page size
            PAGE_READWRITE, &OldProtect);

        // Erase the header
        SecureZeroMemory((char*)module_handle, 4096);
    }
};

#endif