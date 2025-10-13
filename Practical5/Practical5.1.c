#include <windows.h>
#include <stdio.h>

int main() {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    char command[] = "cmd.exe /c dir";

    printf("PARENT: Зараз я запущу команду '%s' у новому процесі.\n", command);

    if (!CreateProcess(
            NULL,           // Ім'я програми (використовуємо командний рядок)
            command,        // Командний рядок для виконання
            NULL,           // Атрибути безпеки процесу
            NULL,           // Атрибути безпеки потоку
            FALSE,          // Дескриптори НЕ успадковуються
            0,              // Флаги створення
            NULL,           // Використовувати середовище батька
            NULL,           // Використовувати поточний каталог батька
            &si,            // Вказівник на STARTUPINFO
            &pi             // Вказівник на PROCESS_INFORMATION
        )) {
        fprintf(stderr, "Помилка CreateProcess: %lu\n", GetLastError());
        return 1;
    }

    printf("PARENT: Дочірній процес запущено з ID: %lu.\n", pi.dwProcessId);
    printf("PARENT: Чекаю на його завершення...\n");
    printf("-------------------------------------------\n");

    WaitForSingleObject(pi.hProcess, INFINITE);

    printf("-------------------------------------------\n");
    printf("PARENT: Дочірній процес завершився.\n");

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}