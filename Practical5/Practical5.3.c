#include <windows.h>
#include <stdio.h>
#include <string.h>

void ParentProcess(void);
void ChildProcess(void);


int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "child") == 0) {
        ChildProcess();
    } else {
        ParentProcess();
    }
    return 0;
}


void ParentProcess(void) {
    printf("PARENT: Процес запущено.\n");

    HANDLE hReadPipe, hWritePipe;
    SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };

    if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
        fprintf(stderr, "PARENT: Помилка створення каналу. Код: %lu\n", GetLastError());
        return;
    }

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.hStdInput = hReadPipe;
    si.dwFlags |= STARTF_USESTDHANDLES;

    char szAppName[MAX_PATH];
    GetModuleFileName(NULL, szAppName, MAX_PATH);
    char szCommandLine[MAX_PATH];
    sprintf(szCommandLine, MAX_PATH, "\"%s\" child", szAppName);

    if (!CreateProcess(NULL, szCommandLine, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        fprintf(stderr, "PARENT: Помилка CreateProcess. Код: %lu\n", GetLastError());
        return;
    }
    CloseHandle(hReadPipe);

    int numbers_to_send[] = { 10, 25, 3, 12, 50, 100 };
    DWORD bytes_written;
    int count = sizeof(numbers_to_send) / sizeof(int);
    printf("PARENT: Sending %d numbers...\n", count);

    if (!WriteFile(hWritePipe, numbers_to_send, sizeof(numbers_to_send), &bytes_written, NULL)) {
        fprintf(stderr, "PARENT: Помилка запису в канал. Код: %lu\n", GetLastError());
    }

    CloseHandle(hWritePipe);

    WaitForSingleObject(pi.hProcess, INFINITE);
    printf("PARENT: Child finished. Exiting.\n");

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

void ChildProcess(void) {
    printf("CHILD: Процес запущено.\n");

    int received_numbers[20]; 
    DWORD bytes_read;
    
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);

    printf("CHILD: Reading numbers from pipe...\n");

    if (ReadFile(hStdin, received_numbers, sizeof(received_numbers), &bytes_read, NULL) && bytes_read > 0) {
        int received_count = bytes_read / sizeof(int);
        int sum = 0;
        for (int i = 0; i < received_count; i++) {
            sum += received_numbers[i];
        }
        
        printf("CHILD: Received %d numbers. Their sum is: %lld\n", received_count, sum);

    } else {
        fprintf(stderr, "CHILD: Помилка читання з каналу або отримано 0 байт. Код: %lu\n", GetLastError());
    }
}