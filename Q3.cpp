#include <iostream>
#include <pthread.h>
#include <cstdint>  // Include for intptr_t

const int n = 1024;
const int num_threads = 4;
int A[n];
int B[n];
pthread_mutex_t mutex_scan;

void* thread_scan(void* arg) {
    intptr_t thread_id = reinterpret_cast<intptr_t>(arg);  // Cast back to intptr_t
    int start = thread_id * (n / num_threads);
    int end = (thread_id + 1) * (n / num_threads);

    if (thread_id > 0) {
        for (int i = start; i < end; i++) {
            B[i] = B[i] + B[start - 1];
        }
    }

    pthread_exit(NULL);
}

int main() {
    // Initialize A array with random numbers

    pthread_t threads[num_threads];
    pthread_mutex_init(&mutex_scan, NULL);

    for (int i = 0; i < n; i++) {
        A[i] = rand() % 100; // Replace with your random number generation
        B[i] = A[i];
    }

    for (int i = 0; i < num_threads; i++) {
        intptr_t thread_id = static_cast<intptr_t>(i);
        pthread_create(&threads[i], NULL, thread_scan, reinterpret_cast<void*>(thread_id));
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex_scan);

    // Display the prefix sum array B
    std::cout << "Prefix Sum Array B:" << std::endl;
    for (int i = 0; i < n; i++) {
        std::cout << B[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
