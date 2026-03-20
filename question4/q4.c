#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX_LINE 1024

char *keyword;
FILE *output_file;
pthread_mutex_t mutex;

// Structure to pass file name to thread
typedef struct {
    char *filename;
} thread_data;

// Count occurrences of keyword in a line
int count_occurrences(char *line, char *keyword) {
    int count = 0;
    char *pos = line;

    while ((pos = strstr(pos, keyword)) != NULL) {
        count++;
        pos += strlen(keyword);
    }

    return count;
}

// Thread function
void* search_file(void *arg) {
    thread_data *data = (thread_data*)arg;
    FILE *file = fopen(data->filename, "r");

    if (!file) {
        perror("File open error");
        return NULL;
    }

    char line[MAX_LINE];
    int total = 0;

    while (fgets(line, sizeof(line), file)) {
        total += count_occurrences(line, keyword);
    }

    fclose(file);

    // Write to shared output file safely
    pthread_mutex_lock(&mutex);
    fprintf(output_file, "%s: %d\n", data->filename, total);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Usage: %s keyword output.txt file1 file2 ... num_threads\n", argv[0]);
        return 1;
    }

    keyword = argv[1];
    output_file = fopen(argv[2], "w");
    if (!output_file) {
        perror("Output file error");
        return 1;
    }

    int num_threads = atoi(argv[argc - 1]);
    int num_files = argc - 4;

    pthread_t threads[num_threads];
    thread_data data[num_files];

    pthread_mutex_init(&mutex, NULL);

    int thread_index = 0;

    for (int i = 0; i < num_files; i++) {
        data[i].filename = argv[i + 3];

        pthread_create(&threads[thread_index], NULL, search_file, &data[i]);
        thread_index++;

        // If reached max threads, wait and reuse
        if (thread_index == num_threads || i == num_files - 1) {
            for (int j = 0; j < thread_index; j++) {
                pthread_join(threads[j], NULL);
            }
            thread_index = 0;
        }
    }

    pthread_mutex_destroy(&mutex);
    fclose(output_file);

    return 0;
}