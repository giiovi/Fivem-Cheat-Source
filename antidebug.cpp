#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <memory>
#include <xorstr.hpp>
#include <iostream>


struct EnumData {
    std::string target_class_name;
    bool found;
};

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    EnumData* data = reinterpret_cast<EnumData*>(lParam);
    char class_name[256];
    GetClassNameA(hwnd, class_name, sizeof(class_name));

    if (data->target_class_name == class_name) {
        data->found = true;
        return FALSE; // Stop enumerating windows
    }

    return TRUE; // Continue enumerating windows
}


inline const std::vector<std::string> blacklisted_strings =
{
  XorStr(("Cheat Engine")),
  XorStr(("Cheat Engine 7.0")),
  XorStr(("Cheat Engine 7.1")),
  XorStr(("Cheat Engine 7.2")),
  XorStr(("Cheat Engine 7.3")),
  XorStr(("Cheat Engine 7.4")),
  XorStr(("HTTP Debugger")),
  XorStr(("HTTP Debugger Pro")),
  XorStr(("x64dbg")),
  XorStr(("x64dbg [Elevated]")),
  XorStr(("Progress Telerik Fiddler Classic")),
  XorStr(("Telerik Fiddler Classic")),
  XorStr(("Fiddler Classic")),
  XorStr(("Fiddler")),
  XorStr(("Unispect - Mono Type Dumper")),
  XorStr(("IDA: Quick start")),
  XorStr(("IDA")),
  XorStr(("HxD")),
  XorStr(("ida64.exe")),
  XorStr(("Unispect.exe")),
  XorStr(("unispect.exe")),
  XorStr(("ida.exe")),
  XorStr(("ida32.exe")),
  XorStr(("reclass.net.exe")),
  XorStr(("reclass.exe")),
  XorStr(("heyrays.exe")),
  XorStr(("InjectorCLIx64.exe")),
  XorStr(("InjectorCLIx86.exe")),
  XorStr(("ScyllaTest_x64.exe")),
  XorStr(("ScyllaTest_x86.exe")),
  XorStr(("lighthouse.exe")),
  XorStr(("cheatengine-x86_64.exe")),
  XorStr(("classinformer.exe")),
  XorStr(("ida-x86emu.exe")),
  XorStr(("cffexplorer.exe")),
  XorStr(("winhex.exe")),
  XorStr(("hiew.exe")),
  XorStr(("fiddler.exe")),
  XorStr(("Fiddler.exe")),
  XorStr(("httpdebugger.exe")),
  XorStr(("httpdebuggerpro.exe")),
  XorStr(("HTTPDebuggerUI.exe")),
  XorStr(("HTTPDebuggerSvc.exe")),
  XorStr(("scylla.exe")),
  XorStr(("Cheat Engine.exe")),
  XorStr(("HxD.exe")),
  XorStr(("dotPeek64.exe")),
  XorStr(("drvmap.exe")),
  XorStr(("x64dbg.exe")),
  XorStr(("processhacker.exe")),
  XorStr(("ProcessHacker.exe")),
  XorStr(("Process Hacker 2")),
  XorStr(("Process Hacker 2")),
  XorStr(("dnSpy v6.1.8 (64-bit, .NET)")),
  XorStr(("KsDumper")),
  XorStr(("Scylla x64")),
  XorStr(("WinDbgFrameClass")),
  XorStr(("Scylla")),
  XorStr(("OllyDbg")),
XorStr(("OllyDbg.exe")),
XorStr(("OLLYDBG.EXE")),
XorStr(("Windbg")),
XorStr(("WinDbg.exe")),
XorStr(("WinDBG.exe")),
XorStr(("Debugging Tools for Windows")),
XorStr(("deBugger")),
XorStr(("deBugger.exe")),
XorStr(("DebugView")),
XorStr(("DebugView.exe")),
XorStr(("Process Explorer")),
XorStr(("ProcessExplorer.exe")),
XorStr(("ProcExp.exe")),
XorStr(("Wireshark")),
XorStr(("Wireshark.exe")),
XorStr(("Wireshark64.exe")),
XorStr(("Wireshark32.exe")),
XorStr(("Immunity Debugger")),
XorStr(("ImmunityDebugger.exe")),
XorStr(("IMMUNITYDEBUGGER.EXE")),
XorStr(("IDA Pro")),
XorStr(("IDA Pro.exe")),
XorStr(("IDA PRO.EXE")),
XorStr(("TcpView")),
XorStr(("TcpView.exe")),
XorStr(("TCPVIEW.EXE")),
XorStr(("AutoIT3")),
XorStr(("AutoIT3.exe")),
XorStr(("AutoIT3 Debugger")),
XorStr(("AutoIT3 Debugger.exe")),
XorStr(("SysInternals")),
XorStr(("Sysinternals Suite")),
XorStr(("Sysinternals Suite.exe")),
XorStr(("SysInternals DebugView")),
XorStr(("SysInternals DebugView.exe")),
XorStr(("FileMon")),
XorStr(("Filemon.exe")),
XorStr(("R-Tools")),
XorStr(("R-Tools.exe")),
XorStr(("Kernel Debugger")),
XorStr(("Kernel Debugger.exe")),
XorStr(("PEiD")),
XorStr(("PEiD.exe")),
XorStr(("PEiD_x64.exe")),
XorStr(("PEiD_x86.exe")),
XorStr(("Resource Hacker")),
XorStr(("Resource Hacker.exe")),
XorStr(("ResourceHacker.exe"))
};

bool is_blacklisted_process_or_window(const std::string& string) {
    // Check for blacklisted processes
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 pe32 = { sizeof(PROCESSENTRY32) };
        if (Process32First(snapshot, &pe32)) {
            do {
                if (string == std::string(pe32.szExeFile)) {
                    CloseHandle(snapshot);
                    return true;
                }
            } while (Process32Next(snapshot, &pe32));
        }
        CloseHandle(snapshot);
    }

    // Check for blacklisted window titles
    if (FindWindowA(0, string.c_str()) != NULL || FindWindowA(string.c_str(), 0) != NULL) {
        return true;
    }

    // Check for blacklisted window class names
    EnumData data;
    data.target_class_name = string;
    data.found = false;

    EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&data));

    return data.found;
}


void unhook_dll(const char* dll_name) {
    HMODULE hModule = GetModuleHandleA(dll_name);
    if (hModule) {
        PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)hModule;
        PIMAGE_NT_HEADERS pNTHeaders = (PIMAGE_NT_HEADERS)((LPBYTE)hModule + pDOSHeader->e_lfanew);

        // Get the import directory
        PIMAGE_DATA_DIRECTORY pImportDirectory = &pNTHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
        if (pImportDirectory->Size) {
            PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)((LPBYTE)hModule + pImportDirectory->VirtualAddress);

            // Iterate through the import descriptors
            for (; pImportDescriptor->Name; ++pImportDescriptor) {
                PIMAGE_THUNK_DATA pThunkData = (PIMAGE_THUNK_DATA)((LPBYTE)hModule + pImportDescriptor->FirstThunk);
                for (; pThunkData->u1.Function; ++pThunkData) {
                    DWORD oldProtect;
                    VirtualProtect(&pThunkData->u1.Function, sizeof(DWORD), PAGE_READWRITE, &oldProtect);
                    pThunkData->u1.Function = 0;
                    VirtualProtect(&pThunkData->u1.Function, sizeof(DWORD), oldProtect, &oldProtect);
                }
            }
        }
    }
}

std::string generate_random_id(size_t length) {
    static const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string random_id(length, ' ');

    for (size_t i = 0; i < length; ++i) {
        random_id[i] = characters[rand() % characters.size()];
    }

    return random_id;
}






std::string path()
{
    char shitter[_MAX_PATH]; // defining the path
    GetModuleFileNameA(NULL, shitter, _MAX_PATH); // getting the path
    return std::string(shitter); //returning the path
}


void watch_and_unhook() {
    while (true) {
        for (const auto& string : blacklisted_strings) {
            if (is_blacklisted_process_or_window(string)) {

                
               
                exit(1);
            }
        }

        // Unhook DLLs
        unhook_dll("ntdll.dll");

        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    }
}



void start_protection() {
    std::thread([]() { watch_and_unhook(); }).detach();
}
