#include <iso646.h>
#include "str.h"


int string_length(const char* string) {
    int res = 0;
    for (const char* it = string; *it != '\0'; ++it) {
        ++res;
    }
    return res;
}

int num_digits(const char* string) {
    int res = 0;
    for (const char* it = string; *it != '\0'; ++it) {
        if (('0' <= *it) and (*it <= '9')) {
            ++res;
        }
    }
    return res;
}
