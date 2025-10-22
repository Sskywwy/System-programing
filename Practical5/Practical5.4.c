#include <windows.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 256

void ShowError(const char* message) {
    DWORD error = GetLastError();
    char errorMsg[256];
    FormatMessage(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        errorMsg, sizeof(errorMsg), NULL);
    fprintf(stderr, "%s\nПомилка %lu: %s\n", message, error, errorMsg);
}

void RunWriterProcess(HANDLE hWritePipe) {
    char buffer[BUFFER_SIZE];
    DWORD bytesWritten;

    printf("[Процес 1 - Письменник]: Введіть рядок та натисніть Enter:\n> ");

    // Читаємо рядок з клавіатури
    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
        fprintf(stderr, "Помилка читання вводу.\n");
        CloseHandle(hWritePipe);
        return;
    }

    if (!WriteFile(hWritePipe, buffer, strlen(buffer), &bytesWritten, NULL)) {
        ShowError("Помилка запису в канал.");
    } else {
        printf("[Процес 1 - Письменник]: Рядок успішно передано в канал.\n");
    }

    CloseHandle(hWritePipe);
}


void RunReaderProcess(HANDLE hReadPipe) {
    char buffer[BUFFER_SIZE];
    DWORD bytesRead;
    FILE *file;

    if (fopen_s(&file, "output.txt", "w") != 0) {
        fprintf(stderr, "Не вдалося відкрити файл output.txt для запису.\n");
        CloseHandle(hReadPipe);
        return;
    }

    printf("[Процес 2 - Читач]: Очікую дані з каналу...\n");

    while (ReadFile(hReadPipe, buffer, BUFFER_SIZE - 1, &bytesRead, NULL) && bytesRead > 0) {
        buffer[bytesRead] = '\0'; 
        fprintf(file, "%s", buffer);
        fflush(file); 
    }

    fclose(file);
    CloseHandle(hReadPipe);
    printf("[Процес 2 - Читач]: Дані отримано і записано у файл 'output.txt'.\n");
}

int main(int argc, char* argv[]) {
    if (argc == 3) {
        HANDLE hPipe = (HANDLE)strtoull(argv[2], NULL, 10); 
        if (strcmp(argv[1], "writer") == 0) {
            RunWriterProcess(hPipe);
        } else if (strcmp(argv[1], "reader") == 0) {
            RunReaderProcess(hPipe);
        }
        return 0; 
    }
    printf("[Батьківський процес]: Створення каналу (pipe)...\n");

    HANDLE hReadPipe, hWritePipe;
    SECURITY_ATTRIBUTES sa;

    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
        ShowError("Не вдалося створити канал.");
        return 1;
    }

    SetHandleInformation(hWritePipe, HANDLE_FLAG_INHERIT, 0); // Тимчасово знімаємо флаг успадкування

    PROCESS_INFORMATION piReader;
    STARTUPINFO siReader;
    ZeroMemory(&siReader, sizeof(siReader));
    siReader.cb = sizeof(siReader);

    char cmdReader[128];
    sprintf_s(cmdReader, sizeof(cmdReader), "%s reader %llu", argv[0], (unsigned long long)hReadPipe);

    if (!CreateProcess(NULL, cmdReader, NULL, NULL, TRUE, 0, NULL, NULL, &siReader, &piReader)) {
        ShowError("Не вдалося створити процес-читач.");
        return 1;
    }
    printf("[Батьківський процес]: Процес-читач створено (PID: %lu).\n", piReader.dwProcessId);

    SetHandleInformation(hWritePipe, HANDLE_FLAG_INHERIT, TRUE);
    SetHandleInformation(hReadPipe, HANDLE_FLAG_INHERIT, 0);

    
    PROCESS_INFORMATION piWriter;
    STARTUPINFO siWriter;
    ZeroMemory(&siWriter, sizeof(siWriter));
    siWriter.cb = sizeof(siWriter);

    char cmdWriter[128];

    sprintf_s(cmdWriter, sizeof(cmdWriter), "%s writer %llu", argv[0], (unsigned long long)hWritePipe);

    if (!CreateProcess(NULL, cmdWriter, NULL, NULL, TRUE, 0, NULL, NULL, &siWriter, &piWriter)) {
        ShowError("Не вдалося створити процес-письменник.");
        return 1;
    }
    printf("[Батьківський процес]: Процес-письменник створено (PID: %lu).\n", piWriter.dwProcessId);


    CloseHandle(hReadPipe);
    CloseHandle(hWritePipe);

    printf("[Батьківський процес]: Очікую завершення дочірніх процесів...\n");
    WaitForSingleObject(piWriter.hProcess, INFINITE);
    WaitForSingleObject(piReader.hProcess, INFINITE);

    
    CloseHandle(piWriter.hProcess);
    CloseHandle(piWriter.hThread);
    CloseHandle(piReader.hProcess);
    CloseHandle(piReader.hThread);

    printf("[Батьківський процес]: Роботу завершено. Перевірте файл 'output.txt'.\n");
    return 0;
}