#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <math.h>

// Thread CPU-bound (cálculo de π)
void* cpu_thread(void* arg) {
    printf("Thread CPU-bound %ld iniciada (PID: %d)\n", (long)arg, getpid());
    
    // Cálculo de π usando a série de Leibniz
    long n = 100000000; // número de iterações
    double pi = 0.0;
    for (long i = 0; i < n; i++) {
        pi += (i % 2 == 0 ? 1.0 : -1.0) / (2 * i + 1);
    }
    pi *= 4;
    
    printf("Thread CPU-bound %ld terminou (π ≈ %f)\n", (long)arg, pi);
    return NULL;
}

// Thread I/O-bound (simula espera)
void* io_thread(void* arg) {
    printf("Thread I/O-bound %ld iniciada (PID: %d)\n", (long)arg, getpid());
    sleep(2);  // Simula operação I/O
    printf("Thread I/O-bound %ld terminou\n", (long)arg);
    return NULL;
}

int main() {
    pid_t pid;
    pthread_t thread_cpu[3], thread_io[3];

    // Cria processo filho
    pid = fork();

    if (pid == 0) { // Processo filho
        printf("\nProcesso filho (PID: %d)\n", getpid());

        // Cria 3 threads CPU-bound
        for (int i = 0; i < 3; i++) {
            pthread_create(&thread_cpu[i], NULL, cpu_thread, (void*)(long)(i + 1));
        }

        // Cria 3 threads I/O-bound
        for (int i = 0; i < 3; i++) {
            pthread_create(&thread_io[i], NULL, io_thread, (void*)(long)(i + 1));
        }

        // Espera todas as threads terminarem
        for (int i = 0; i < 3; i++) {
            pthread_join(thread_cpu[i], NULL);
            pthread_join(thread_io[i], NULL);
        }

    } else if (pid > 0) { // Processo pai
        printf("Processo pai (PID: %d)\n", getpid());
        wait(NULL); // Espera filho terminar
    }

    return 0;
}
