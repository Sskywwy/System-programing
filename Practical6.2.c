#include <stdio.h>
#include <windows.h>
#include <stdlib.h> // Потрібен для malloc та free

#define NUM_THREADS 5 // Кількість потоків, які ми хочемо створити

/**
 * Функція, яку виконуватимуть потоки.
 * Вона приймає вказівник (lpParam) на свій номер.
 */
DWORD WINAPI ThreadFunction(LPVOID lpParam) {
    // 1. "Розпаковуємо" параметр (LPVOID) назад у вказівник на int
    int* pThreadNum = (int*)lpParam;
    int threadNum = *pThreadNum; // Отримуємо наш унікальний номер

    // 2. Виводимо повідомлення
    printf("Hello from Thread %d\n", threadNum);

    /*
     * 3. Звільняємо пам'ять, яку для нас виділив головний потік.
     * Оскільки main виділив її через malloc, потік,
     * який її використав, має її звільнити через free.
     */
    free(pThreadNum);

    return 0; // Код завершення потоку
}

int main() {
    HANDLE hThreads[NUM_THREADS]; // Масив для зберігання дескрипторів потоків
    int i;

    // Створюємо 5 потоків у циклі
    for (i = 0; i < NUM_THREADS; i++) {
        /*
         * Готуємо дані для потоку.
         * Виділяємо пам'ять для одного числа (int).
         */
        int* pThreadNum = (int*)malloc(sizeof(int));
        if (pThreadNum == NULL) {
            fprintf(stderr, "Помилка виділення пам'яті для потоку %d\n", i + 1);
            continue; // Пропускаємо створення цього потоку, якщо пам'ять не виділилась
        }
        
        // Записуємо унікальний номер (1, 2, 3, 4, 5) у виділену пам'ять
        *pThreadNum = i + 1;

        // Створюємо потік і передаємо йому вказівник на його номер
        hThreads[i] = CreateThread(
            NULL,           // Атрибути безпеки (за замовчуванням)
            0,              // Розмір стеку (за замовчуванням)
            ThreadFunction, // Функція, яку потік виконає
            pThreadNum,     // Параметр (вказівник на наш номер)
            0,              // Прапорці створення (запустити одразу)
            NULL            // Нам не потрібен ID потоку
        );

        if (hThreads[i] == NULL) {
            fprintf(stderr, "Помилка створення потоку %d: %lu\n", i + 1, GetLastError());
            free(pThreadNum); // Важливо звільнити пам'ять, якщо потік не створився
        }
    }

    // Головний потік виводить своє повідомлення
    printf("Hello from main (created %d threads)\n", NUM_THREADS);

    /*
     * Чекаємо, поки ВСІ потоки завершать свою роботу.
     * - NUM_THREADS: Кількість об'єктів для очікування.
     * - hThreads: Масив цих об'єктів.
     * - TRUE: Чекати на ВСІ об'єкти (а не на будь-який один).
     * - INFINITE: Чекати нескінченно довго.
     */
    WaitForMultipleObjects(NUM_THREADS, hThreads, TRUE, INFINITE);

    printf("Main: All threads have finished.\n");

    // Закриваємо дескриптори всіх потоків, щоб звільнити ресурси
    for (i = 0; i < NUM_THREADS; i++) {
        if (hThreads[i] != NULL) {
            CloseHandle(hThreads[i]);
        }
    }

    return 0;
}