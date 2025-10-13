#include <windows.h>
#include <stdio.h>

int main() {
    HANDLE hReadPipe, hWritePipe;
    SECURITY_ATTRIBUTES sa;

    // 1. Створюємо канал (pipe) з можливістю успадкування дескрипторів
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
        fprintf(stderr, "Pipe Creation Failed\n");
        return 1;
    }

    // --- Налаштування та запуск першого процесу (dir) ---
    STARTUPINFO si1;
    PROCESS_INFORMATION pi1;
    ZeroMemory(&si1, sizeof(si1));
    si1.cb = sizeof(si1);
    ZeroMemory(&pi1, sizeof(pi1));

    // Перенаправляємо стандартний вивід (stdout) першого процесу в наш канал
    si1.hStdOutput = hWritePipe;
    si1.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    si1.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    si1.dwFlags |= STARTF_USESTDHANDLES;

    char cmd1[] = "cmd.exe /c dir";
    
    // Створюємо перший дочірній процес
    if (!CreateProcess(NULL, cmd1, NULL, NULL, TRUE, 0, NULL, NULL, &si1, &pi1)) {
        fprintf(stderr, "CreateProcess #1 Failed\n");
        return 1;
    }
    printf("PARENT: Launched process #1 (dir).\n");

    // Першому процесу більше не потрібен кінець для запису, він його успадкував.
    // Батько ПОВИНЕН закрити свою копію, інакше другий процес буде чекати вічно.
    CloseHandle(hWritePipe);

    // --- Налаштування та запуск другого процесу (findstr) ---
    STARTUPINFO si2;
    PROCESS_INFORMATION pi2;
    ZeroMemory(&si2, sizeof(si2));
    si2.cb = sizeof(si2);
    ZeroMemory(&pi2, sizeof(pi2));

    // Перенаправляємо стандартний ввід (stdin) другого процесу з нашого каналу
    si2.hStdInput = hReadPipe;
    si2.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    si2.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE); // Вивід findstr йде на екран
    si2.dwFlags |= STARTF_USESTDHANDLES;

    char cmd2[] = "cmd.exe /c findstr .c";
    
    // Створюємо другий дочірній процес
    if (!CreateProcess(NULL, cmd2, NULL, NULL, TRUE, 0, NULL, NULL, &si2, &pi2)) {
        fprintf(stderr, "CreateProcess #2 Failed\n");
        return 1;
    }
    printf("PARENT: Launched process #2 (findstr).\n");

    // Другому процесу більше не потрібен кінець для читання, він його успадкував.
    CloseHandle(hReadPipe);

    // --- Очікування завершення дочірніх процесів ---
    printf("PARENT: Waiting for children to finish...\n");
    WaitForSingleObject(pi1.hProcess, INFINITE);
    WaitForSingleObject(pi2.hProcess, INFINITE);
    printf("PARENT: Both children finished.\n");

    // Закриваємо дескриптори процесів
    CloseHandle(pi1.hProcess);
    CloseHandle(pi1.hThread);
    CloseHandle(pi2.hProcess);
    CloseHandle(pi2.hThread);

    return 0;
}