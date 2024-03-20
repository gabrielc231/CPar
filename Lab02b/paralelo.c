#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define m 3
#define n 3
#define thread_count 4

double y[m];
double A[m][n] = {{1,2,3},{4,5,6},{7,8,9}};
double x[n] = {1,3,6};

void print_matrix(int l, int c, double A[l][c]) {
    for (int i = 0; i < l; i++) {
        printf("|");
        for (int j = 0; j < c; j++) {
            printf(" %.2lf", A[i][j]);
        }
        printf("|\n");
    }
    printf("\n");
}

void print_vector(int t, double* v) {
    printf("|");
    for (int i = 0; i < t; i++) {
        printf(" %.2lf", v[i]);
    }
    printf(" |\n");
}

void *Pth_mat_vect(void* rank) {
    long my_rank = (long) rank;
    int i, j;
    int local_m = m / thread_count;
    if (local_m == 0) {local_m = 1;}
    int my_first_row = my_rank * local_m;
    int my_last_row = (my_rank + 1) * local_m - 1;

    for (i = my_first_row; i <= my_last_row; i++) {
        y[i] = 0.0;
        for (j = 0; j < n; j++) {
            y[i] += A[i][j] * x[j];
        }
    }

    return NULL;
}

int main() {

    // Cria threads
    pthread_t* thread_handles;
    thread_handles = (pthread_t*) malloc(thread_count * sizeof(pthread_t));
    for (long thread = 0; thread < thread_count; thread++) {
        pthread_create(&thread_handles[thread], NULL, Pth_mat_vect, (void*) thread);
    }

    // Aguarda que todas as threads terminem
    for (long thread = 0; thread < thread_count; thread++) {
        pthread_join(thread_handles[thread], NULL);
    }

    // Libera memória alocada para as threads
    free(thread_handles);

    // O resultado está em y
    printf("Vetor resultante: \n");
    print_vector(3,y);


    return 0;
}
