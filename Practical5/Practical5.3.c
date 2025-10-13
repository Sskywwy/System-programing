#include <windows.h>
#include <stdio.h>
#include <string.h>

// Прототипи функцій
void ParentProcess(void);
void ChildProcess(void);

// "Диспетчер", який викликає потрібну функцію залежно від аргументів
int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "child") == 0) {
        ChildProcess();
    } else {
        ParentProcess();
    }
    return 0;
}

// Функція, що виконується батьківським процесом
void ParentProcess(void) {
    printf("PARENT: Процес запущено.\n");

    HANDLE hReadPipe, hWritePipe;
    SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };

    // 1. Створюємо анонімний канал
    if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
        fprintf(stderr, "PARENT: Помилка створення каналу. Код: %lu\n", GetLastError());
        return;
    }

    // Готуємо структури для запуску дочірнього процесу
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    // Перенаправляємо стандартне введення дочірнього процесу на наш канал для читання
    si.hStdInput = hReadPipe;
    si.dwFlags |= STARTF_USESTDHANDLES;

    // Формуємо команду для запуску копії програми з аргументом "child"
    char szAppName[MAX_PATH];
    GetModuleFileName(NULL, szAppName, MAX_PATH);
    char szCommandLine[MAX_PATH];
    sprintf(szCommandLine, MAX_PATH, "\"%s\" child", szAppName);

    // 2. Створюємо дочірній процес з успадкуванням дескрипторів
    if (!CreateProcess(NULL, szCommandLine, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        fprintf(stderr, "PARENT: Помилка CreateProcess. Код: %lu\n", GetLastError());
        return;
    }
    
    // Батько не буде читати з каналу, тому закриваємо цей дескриптор
    CloseHandle(hReadPipe);

    // 3. Надсилаємо масив чисел
    int numbers_to_send[] = { 10, 25, 3, 12, 50, 100 };
    DWORD bytes_written;
    int count = sizeof(numbers_to_send) / sizeof(int);
    printf("PARENT: Sending %d numbers...\n", count);

    // Записуємо весь масив (як потік байтів) у канал
    if (!WriteFile(hWritePipe, numbers_to_send, sizeof(numbers_to_send), &bytes_written, NULL)) {
        fprintf(stderr, "PARENT: Помилка запису в канал. Код: %lu\n", GetLastError());
    }

    // Закриваємо дескриптор для запису, щоб дочірній процес отримав EOF
    CloseHandle(hWritePipe);

    // 4. Очікуємо завершення дочірнього процесу
    WaitForSingleObject(pi.hProcess, INFINITE);
    printf("PARENT: Child finished. Exiting.\n");

    // Закриваємо дескриптори процесу та потоку
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

// Функція, що виконується дочірнім процесом
void ChildProcess(void) {
    printf("CHILD: Процес запущено.\n");

    int received_numbers[20]; // Буфер для отримання чисел
    DWORD bytes_read;
    
    // Отримуємо дескриптор стандартного введення, який батько перенаправив на канал
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);

    printf("CHILD: Reading numbers from pipe...\n");

    // Читаємо потік байтів з каналу (який тепер є нашим стандартним введенням)
    if (ReadFile(hStdin, received_numbers, sizeof(received_numbers), &bytes_read, NULL) && bytes_read > 0) {
        
        // Визначаємо, скільки чисел ми фактично отримали
        int received_count = bytes_read / sizeof(int);
        long long sum = 0;

        // Обчислюємо суму
        for (int i = 0; i < received_count; i++) {
            sum += received_numbers[i];
        }
        
        printf("CHILD: Received %d numbers. Their sum is: %lld\n", received_count, sum);

    } else {
        fprintf(stderr, "CHILD: Помилка читання з каналу або отримано 0 байт. Код: %lu\n", GetLastError());
    }
}