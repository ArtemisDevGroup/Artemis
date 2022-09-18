#undef UNICODE
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

namespace Artemis {
    namespace Loader {
        namespace Helper {
            public ref class Helpers {
            public:
                static System::UInt32 Inject() {
                    PROCESSENTRY32 pe32 = { NULL };
                    pe32.dwSize = sizeof(pe32);

                    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
                    if (hSnap == INVALID_HANDLE_VALUE) return 1;

                    if (!Process32First(hSnap, &pe32)) {
                        CloseHandle(hSnap);
                        return 2;
                    }

                    DWORD dwProcessId = 0;
                    do {
                        if (!strcmp(pe32.szExeFile, "RainbowSix.exe")) {
                            dwProcessId = pe32.th32ProcessID;
                        }
                    } while (Process32Next(hSnap, &pe32));
                    CloseHandle(hSnap);

                    if (!dwProcessId) return 3;

                    MODULEENTRY32 me32 = { NULL };
                    me32.dwSize = sizeof(me32);

                    hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwProcessId);
                    if (hSnap == INVALID_HANDLE_VALUE) return 4;

                    if (!Module32First(hSnap, &me32)) {
                        CloseHandle(hSnap);
                        return 5;
                    }

                    uintptr_t base = 0;
                    do {
                        if (!strcmp(me32.szModule, "RainbowSix.exe")) {
                            base = (uintptr_t)me32.modBaseAddr;
                        }
                    } while (Module32Next(hSnap, &me32));
                    CloseHandle(hSnap);

                    if (!base) return 6;

                    CHAR szBuffer[MAX_PATH];

                    if (!K32GetModuleFileNameExA(
                        GetCurrentProcess(),
                        GetModuleHandleA(nullptr),
                        szBuffer,
                        MAX_PATH
                    )) return 7;

                    for (; szBuffer[strlen(szBuffer) - 1] != '\\'; szBuffer[strlen(szBuffer) - 1] = '\0');

                    strcat_s(szBuffer, "Artemis.dll");

                    HMODULE hModule = GetModuleHandleA("Kernel32.dll");
                    if (!hModule) return 8;

                    LPTHREAD_START_ROUTINE lpLoadLibraryA = (LPTHREAD_START_ROUTINE)GetProcAddress(hModule, "LoadLibraryA");
                    if (!lpLoadLibraryA) return 9;

                    HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_QUERY_INFORMATION, FALSE, dwProcessId);
                    if (!hProcess) return 10;

                    Sleep(10000);

                    LPVOID lpParameter = VirtualAllocEx(hProcess, nullptr, strlen(szBuffer) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
                    if (!lpParameter) {
                        CloseHandle(hProcess);
                        return 11;
                    }

                    if (!WriteProcessMemory(hProcess, lpParameter, szBuffer, strlen(szBuffer) + 1, nullptr)) {
                        VirtualFreeEx(hProcess, lpParameter, NULL, MEM_RELEASE);
                        CloseHandle(hProcess);
                        return 12;
                    }

                    HANDLE hThread = CreateRemoteThread(hProcess, nullptr, NULL, lpLoadLibraryA, lpParameter, NULL, nullptr);
                    if (!hThread) {
                        VirtualFreeEx(hProcess, lpParameter, NULL, MEM_RELEASE);
                        CloseHandle(hProcess);
                        return 13;
                    }

                    Sleep(10);

                    if (!VirtualFreeEx(hProcess, lpParameter, NULL, MEM_RELEASE)) {
                        CloseHandle(hProcess);
                        return 14;
                    }

                    CloseHandle(hProcess);

                    return 0;
                }
            };
        }
    }
}