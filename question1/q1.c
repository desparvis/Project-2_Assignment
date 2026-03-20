#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];
    pid_t pid1, pid2;

    // Create pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }

    // First child (ps aux)
    pid1 = fork();
    if (pid1 == 0) {
        // Redirect stdout to pipe write end
        dup2(pipefd[1], STDOUT_FILENO);

        // Close unused ends
        close(pipefd[0]);
        close(pipefd[1]);

        char *args[] = {"ps", "aux", NULL};
        execvp(args[0], args);

        perror("execvp ps");
        exit(1);
    }

    // Second child (grep root)
    pid2 = fork();
    if (pid2 == 0) {
        // Redirect stdin from pipe read end
        dup2(pipefd[0], STDIN_FILENO);

        // Open file for output
        int fd = open("output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (fd < 0) {
            perror("open");
            exit(1);
        }

        // Redirect stdout to file
        dup2(fd, STDOUT_FILENO);

        close(fd);
        close(pipefd[0]);
        close(pipefd[1]);

        char *args[] = {"grep", "root", NULL};
        execvp(args[0], args);

        perror("execvp grep");
        exit(1);
    }

    // Parent process
    close(pipefd[0]);
    close(pipefd[1]);

    wait(NULL);
    wait(NULL);

    // Read and display part of output
    FILE *file = fopen("output.txt", "r");
    if (!file) {
        perror("fopen");
        exit(1);
    }

    char buffer[256];
    printf("---- Output Preview ----\n");

    for (int i = 0; i < 5 && fgets(buffer, sizeof(buffer), file); i++) {
        printf("%s", buffer);
    }

    fclose(file);

    return 0;
}