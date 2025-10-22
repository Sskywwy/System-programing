#include <windows.h>
#include <stdio.h>

int main() {
    HANDLE hReadPipe, hWritePipe;
    SECURITY_ATTRIBUTES sa;

    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
        fprintf(stderr, "Pipe Creation Failed\n");
        return 1;
    }

    STARTUPINFO si1;
    PROCESS_INFORMATION pi1;
    ZeroMemory(&si1, sizeof(si1));
    si1.cb = sizeof(si1);
    ZeroMemory(&pi1, sizeof(pi1));

    si1.hStdOutput = hWritePipe;
    si1.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    si1.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    si1.dwFlags |= STARTF_USESTDHANDLES;

    char cmd1[] = "cmd.exe /c dir";

    if (!CreateProcess(NULL, cmd1, NULL, NULL, TRUE, 0, NULL, NULL, &si1, &pi1)) {
        fprintf(stderr, "CreateProcess #1 Failed\n");
        return 1;
    }
    printf("PARENT: Launched process #1 (dir).\n");

    CloseHandle(hWritePipe);

    STARTUPINFO si2;
    PROCESS_INFORMATION pi2;
    ZeroMemory(&si2, sizeof(si2));
    si2.cb = sizeof(si2);
    ZeroMemory(&pi2, sizeof(pi2));

    si2.hStdInput = hReadPipe;
    si2.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    si2.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE); 
    si2.dwFlags |= STARTF_USESTDHANDLES;

    char cmd2[] = "cmd.exe /c findstr .c";
    
    if (!CreateProcess(NULL, cmd2, NULL, NULL, TRUE, 0, NULL, NULL, &si2, &pi2)) {
        fprintf(stderr, "CreateProcess #2 Failed\n");
        return 1;
    }
    printf("PARENT: Launched process #2 (findstr).\n");

    CloseHandle(hReadPipe);

    printf("PARENT: Waiting for children to finish...\n");
    WaitForSingleObject(pi1.hProcess, INFINITE);
    WaitForSingleObject(pi2.hProcess, INFINITE);
    printf("PARENT: Both children finished.\n");

    CloseHandle(pi1.hProcess);
    CloseHandle(pi1.hThread);
    CloseHandle(pi2.hProcess);
    CloseHandle(pi2.hThread);

    return 0;
}