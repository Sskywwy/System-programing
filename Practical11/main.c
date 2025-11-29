#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main() {
    void *handle;              // "Ручка" від відкритої бібліотеки
    int (*calc_func)(int, int); // Вказівник, куди ми збережемо адресу функції


    handle = dlopen("libmult.so", RTLD_LAZY);

    
    calc_func = dlsym(handle, "multiply");


    int result = calc_func(7, 7);
    printf("Результат динамічного виклику: %d\n", result);


    dlclose(handle);

    return 0;
}