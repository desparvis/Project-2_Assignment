#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define NUM_THREADS 16
#define MAX_NUM 200000

// Shared counter for primes
int total_primes = 0;
pthread_mutex_t mutex;

// Function to check if a number is prime
int is_prime(int n) {
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    int sqrt_n = (int)sqrt(n);
    for (int i = 3; i <= sqrt_n; i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

// Thread function
void* count_primes(void* arg) {
    int thread_id = *(int*)arg;
    int numbers_per_thread = MAX_NUM / NUM_THREADS;
    int start = thread_id * numbers_per_thread + 1;
    int end = (thread_id == NUM_THREADS - 1) ? MAX_NUM : start + numbers_per_thread - 1;

    int local_count = 0;
    for (int i = start; i <= end; i++) {
        if (is_prime(i)) local_count++;
    }

    // Update the shared total safely
    pthread_mutex_lock(&mutex);
    total_primes += local_count;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    pthread_mutex_init(&mutex, NULL);

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        if (pthread_create(&threads[i], NULL, count_primes, &thread_ids[i]) != 0) {
            perror("Failed to create thread");
            exit(1);
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    printf("The synchronized total number of prime numbers between 1 and %d is %d\n", MAX_NUM, total_primes);
    return 0;
}