#include <stdio.h>

int main() {
    FILE *file = fopen("marcio.txt", "w");

    for (int i = 0; i < 250; ++i) {
        fwrite("marcio\n", sizeof(char), 7, file);
    }
}