#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>

int main() {
    // DWORD pid = GetCurrentProcessId();
    // DWORD ppid = 0;

    // HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    // PROCESSENTRY32 entry;
    // entry.dwSize = sizeof(PROCESSENTRY32);

    // if (Process32First(snapshot, &entry)) {
    //     do {
    //         if (entry.th32ProcessID == pid) {
    //             ppid = entry.th32ParentProcessID;
    //             break;
    //         }
    //     } while (Process32Next(snapshot, &entry));
    // }

    // CloseHandle(snapshot);

    // printf("PID: %lu\n", pid);
    // printf("PPID: %lu\n", ppid);



    // STARTUPINFO si;
    // PROCESS_INFORMATION pi;

    // ZeroMemory(&si, sizeof(si));
    // si.cb = sizeof(si);
    // ZeroMemory(&pi, sizeof(pi));

    
    // if (CreateProcess(
    //     NULL,               
    //     "notepad.exe",      
    //     NULL, NULL, FALSE,
    //     0,                  
    //     NULL, NULL,         
    //     &si, &pi)) {        
    //     printf("Процес notepad.exe успішно запущено!\n");
    //     printf("PID процесу: %lu\n", pi.dwProcessId);

        
    //     CloseHandle(pi.hProcess);
    //     CloseHandle(pi.hThread);
    // } else {
    //     printf("Не вдалося запустити notepad.exe. Код помилки: %lu\n", GetLastError());
    // }








    // STARTUPINFO si;
    // PROCESS_INFORMATION pi;

    // ZeroMemory(&si, sizeof(si));
    // si.cb = sizeof(si);
    // ZeroMemory(&pi, sizeof(pi));

    
    // if (CreateProcess(
    //     NULL,               
    //     "child.exe",        
    //     NULL, NULL, FALSE,
    //     0, NULL, NULL,
    //     &si, &pi)) {

    //     printf("Батьківський процес (PID: %lu) чекає дочірнього...\n", GetCurrentProcessId());

        
    //     WaitForSingleObject(pi.hProcess, INFINITE);

        
    //     DWORD exitCode;
    //     GetExitCodeProcess(pi.hProcess, &exitCode);
    //     printf("Дочірній процес завершився з кодом: %lu\n", exitCode);

    //     CloseHandle(pi.hProcess);
    //     CloseHandle(pi.hThread);

    // } else {
    //     printf("Помилка створення дочірнього процесу: %lu\n", GetLastError());
    // }






    #define CHILD_COUNT 2
    STARTUPINFO si[CHILD_COUNT];
    PROCESS_INFORMATION pi[CHILD_COUNT];
    char cmd[CHILD_COUNT][50] = { "child.exe 5", "child.exe 7"};
    for (int i=0; i<CHILD_COUNT; i++){
        ZeroMemory(&si[i], sizeof(si[i]));
        si[i].cb = sizeof(si[i]);
        ZeroMemory(&pi[i], sizeof(pi[i]));
    }
    
    for(int i=0; i<CHILD_COUNT; i++) {
        if (!CreateProcess(NULL, cmd[i], NULL, NULL, FALSE, 0, NULL, NULL, &si[i], &pi[i])) {
            printf("Помилка створення дочірнього процесу %d: %lu\n", i, GetLastError());
            return 1;
        }
        printf("Батько створив процес %d (PID: %lu)\n", i, pi[i].dwProcessId);
    }

    for(int i=0; i<CHILD_COUNT; i++) {
        WaitForSingleObject(pi[i].hProcess, INFINITE);
        DWORD exitCode;
        GetExitCodeProcess(pi[i].hProcess, &exitCode);
        printf("Дочірній процес %d завершився з кодом (результат %d %% 256): %lu\n", i, atoi(&cmd[i][10]), exitCode);

        CloseHandle(pi[i].hProcess);
        CloseHandle(pi[i].hThread);
    }

    printf("Батьківський процес: всі дочірні завершені.\n");

    return 0;
}
