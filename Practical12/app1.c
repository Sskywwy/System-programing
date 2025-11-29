#include <stdio.h>
#include "utils.h"

int main()
{
    char word[] = "Development";
    int len = string_length(word);

    printf("Word: %s\n", word);
    printf("Length: %d\n", len);
    return 0;
}
