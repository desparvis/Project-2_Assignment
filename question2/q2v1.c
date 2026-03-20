#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s source dest\n", argv[0]);
        return 1;
    }

    int src = open(argv[1], O_RDONLY);
    int dest = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0644);

    if (src < 0 || dest < 0) {
        perror("File error");
        return 1;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes;

    clock_t start = clock();

    while ((bytes = read(src, buffer, BUFFER_SIZE)) > 0) {
        write(dest, buffer, bytes);
    }

    clock_t end = clock();

    printf("Low-level copy time: %f seconds\n",
           (double)(end - start) / CLOCKS_PER_SEC);

    close(src);
    close(dest);

    return 0;
}