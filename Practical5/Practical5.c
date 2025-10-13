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
    SECURITY_ATTRIBUTES sa;

    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
        fprintf(stderr, "PARENT: Помилка створення каналу. Код помилки: %lu\n", GetLastError());
        return;
    }

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    si.hStdInput = hReadPipe;
    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE); 
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    si.dwFlags |= STARTF_USESTDHANDLES;

    ZeroMemory(&pi, sizeof(pi));

    char szAppName[MAX_PATH];
    GetModuleFileName(NULL, szAppName, MAX_PATH);

    char szCommandLine[MAX_PATH];
    sprintf(szCommandLine, "\"%s\" child", szAppName);

    printf("PARENT: Запускаю дочірній процес...\n");
    if (!CreateProcess(
            NULL,           
            szCommandLine,  
            NULL,           
            NULL,           
            TRUE,           
            0,              
            NULL,           
            NULL,           
            &si,            
            &pi             
        )) {
        fprintf(stderr, "PARENT: Помилка CreateProcess. Код помилки: %lu\n", GetLastError());
        return;
    }

    CloseHandle(hReadPipe);

    const char *message = "Hello from parent (Windows)";
    DWORD dwWritten;
    printf("PARENT: Записую в канал: \"%s\"\n", message);

    if (!WriteFile(hWritePipe, message, strlen(message) + 1, &dwWritten, NULL)) {
        fprintf(stderr, "PARENT: Помилка запису в канал. Код помилки: %lu\n", GetLastError());
    }

    CloseHandle(hWritePipe);

    WaitForSingleObject(pi.hProcess, INFINITE);
    printf("PARENT: Дочірній процес завершився. Вихід.\n");

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

void ChildProcess(void) {
    printf("CHILD: Процес запущено.\n");

    char buffer[100];
    DWORD dwRead;
    
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);

    printf("CHILD: Читаю з каналу...\n");
    
    if (ReadFile(hStdin, buffer, sizeof(buffer) - 1, &dwRead, NULL)) {
        buffer[dwRead] = '\0';
        printf("CHILD: Отримано повідомлення: \"%s\"\n", buffer);
    } else {
        fprintf(stderr, "CHILD: Помилка читання з каналу. Код помилки: %lu\n", GetLastError());
    }
}