#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#define BUFFER_SIZE 4096
typedef struct
{
    char name[30];
    int age;
    float grade;
}Student;

int main(int argc, char *argv[])
{
    // FILE *fptr;
    // fptr = fopen("test.txt", "r");
    // if (fptr == NULL){
    //     printf("Error. FILE NOT EXIST");
    // }
    // fclose(fptr);

    // FILE *fptr;
    // fptr = fopen("text.txt", "r");
    // char buffer[100];
    // fgets(buffer, 100, fptr);
    // printf("%s", buffer);
    // fclose(fptr);

    // HANDLE file;
    // DWORD written;
    // char *text = "Blablabla blabla";
    // file = CreateFile("text-windows.txt", GENERIC_READ|GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    // WriteFile(file, text, (DWORD) strlen(text), &written, NULL);
    // CloseHandle(file);

    Student students[3]={
        {"Ivan", 20, 85.5},
        {"Olena", 19, 90.0},
        {"Petro", 21, 76.3}
    };
    
    // FILE *file;
    // FILE *file1;
    // Student buffer[3];
    // file = fopen("Student.bin", "wb");
    // file1 = fopen("Student.txt", "w");
    // fwrite(students, sizeof(Student), 3, file);
    // fwrite(students, sizeof(Student), 3, file1);
    // fclose(file);
    // fclose(file1);
    // file = fopen("Student.bin", "rb");
    // fread(buffer, sizeof(Student), 3, file);
    // fclose(file);
    // for(int i=0; i<3; i++){
    //     printf("name: %s\n age: %d\n grade: %.2f\n", buffer[i].name, buffer[i].age, buffer[i].grade);
    // }

    FILE *FileToCopy = fopen(argv[1], "rb");
    FILE *FileToPaste = fopen(argv[2], "wb");
    char buffer[BUFFER_SIZE];
    size_t bytesread;
    while((bytesread = fread(buffer, 1, BUFFER_SIZE, FileToCopy)) > 0){
        fwrite(buffer, 1, BUFFER_SIZE, FileToPaste);
    };
    fclose(FileToCopy);
    fclose(FileToPaste);
    printf("Успіх");
    return 0;
}
