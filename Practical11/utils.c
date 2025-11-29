#include "utils.h"

int string_length(const char *str){
    int length = 0;
    while (str[length] != '\0')
    {
        length++;
    }
    return length;
}

int is_palindrome(const char *str){
    int len = string_length(str);
    int start = 0;
    int end = len - 1;
    while (start < end)
    {
        if (str[start] != str[end]){
            return 0;
        }
        start++;
        end--;
    }
    
}