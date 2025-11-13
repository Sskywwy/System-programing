#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int numstud = 3;
typedef struct
{
    char name[20];
    int age;    
} Student;


int main()
{
    Student* college = calloc(numstud, sizeof(Student));
    strcpy(college[0].name, "Roma");
    college[0].age = 20;
    strcpy(college[1].name, "boma");
    college[1].age = 10;
    strcpy(college[2].name, "Doma");
    college[2].age = 111;
    for (int i = 0; i < numstud; i++) {
        printf("Student %d: Name=%s, Age=%d\n", 
               i, college[i].name, college[i].age);
    } 
    return 0;
}
