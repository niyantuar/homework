#include <stdio.h>
#include "str.h"


int main(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        const char* arg = argv[i];
        printf("%d %d\n", string_length(arg), num_digits(arg));
    }
}
