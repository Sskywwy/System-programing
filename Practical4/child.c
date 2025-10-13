#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

DWORD factorial(int n) {
    DWORD result = 1;
    for(int i = 2; i <= n; i++)
        result *= i;
    return result;
}

int main(int argc, char *argv[])
{
   if (argc != 2) {
        printf("Використання: child.exe <число>\n");
        return 1;
    }
    int n = atoi(argv[1]);
    DWORD result = factorial(n);

    printf("Дочірній процес (PID: %lu) обчислив factorial(%d) = %lu\n", GetCurrentProcessId(), n, result);

    return (int)(result % 256);
}
