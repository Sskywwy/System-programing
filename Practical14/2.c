#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> // Для open()

int main() {
    // 1. Створюємо буфер для даних
    char buffer[128];
    
    // 2. Відкриваємо файл (System Call: open/openat)
    // O_RDONLY - тільки для читання
    int fd = open("secret.txt", O_RDONLY);
    
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    // 3. Читаємо дані (System Call: read)
    // Читаємо з fd у buffer, максимум 100 байт
    ssize_t bytes_read = read(fd, buffer, 100);

    if (bytes_read > 0) {
        // 4. Пишемо на екран (System Call: write)
        // 1 - це стандартний дескриптор stdout (екран)
        write(1, buffer, bytes_read);
        write(1, "\n", 1); // Просто новий рядок
    }

    // 5. Закриваємо (System Call: close)
    close(fd);

    return 0;
}