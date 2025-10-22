#include <stdio.h>
#include <windows.h>


DWORD WINAPI ThreadFunction(LPVOID lpParam) {
    printf("Hello from thread\n");
    return 0; 
}

int main() {
    HANDLE hThread;  
    DWORD dwThreadId; 


    hThread = CreateThread(
        NULL,             
        0,                
        ThreadFunction,   
        NULL,             
        0,                
        &dwThreadId       
    );

    if (hThread == NULL) {
        fprintf(stderr, "Помилка при створенні потоку: %lu\n", GetLastError());
        return 1;
    }

    printf("Hello from main\n");


    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);

    return 0;
}