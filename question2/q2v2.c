#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s source dest\n", argv[0]);
        return 1;
    }

    FILE *src = fopen(argv[1], "rb");
    FILE *dest = fopen(argv[2], "wb");

    if (!src || !dest) {
        perror("File error");
        return 1;
    }

    char buffer[BUFFER_SIZE];
    size_t bytes;

    clock_t start = clock();

    while ((bytes = fread(buffer, 1, BUFFER_SIZE, src)) > 0) {
        fwrite(buffer, 1, bytes, dest);
    }

    clock_t end = clock();

    printf("High-level copy time: %f seconds\n",
           (double)(end - start) / CLOCKS_PER_SEC);

    fclose(src);
    fclose(dest);

    return 0;
}