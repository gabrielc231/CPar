#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define m 24
#define n 24

double y[m];
double A[m][n] = {{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5}};
double x[n] = {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};

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

void *Pth_mat_vect(void* arg) {
    long my_rank = ((long*) arg)[0];
    int i, j;
    long local_m = m / ((long*) arg)[1];
    if (local_m == 0) {local_m = 1;}
    int my_first_row = my_rank * local_m;
    int my_last_row = (my_rank + 1) * local_m - 1;

    for (i = my_first_row; i <= my_last_row; i++) {
        y[i] = 0.0;
        for (j = 0; j < n; j++) {
            y[i] += A[i][j] * x[j];
        }
    }
    free(arg);
    return NULL;
}

int main(int argc, char* argv[]) {
    // valor padrão para threads
    int thread_count = 8;
    
    // verifica se foi passado um argumento para o número de threads
    if (argc == 2) {
    thread_count = atoi(argv[1]);
    }

    if (argc > 2) {
        printf("Uso: ./main [numero de threads]\n");
        return -1;
    }
    // Cria threads
    pthread_t* thread_handles;
    thread_handles = (pthread_t*) malloc(thread_count * sizeof(pthread_t));
    for (long thread = 0; thread < thread_count; thread++) {
        long* arg = (long*) malloc(2 * sizeof(long));
        arg[0] = thread;
        arg[1] = thread_count;
        if (pthread_create(&thread_handles[thread], NULL, &Pth_mat_vect, (void*) arg) != 0) {
            printf("Erro ao criar thread\n");   
        }
    }

    // Aguarda que todas as threads terminem
    for (long thread = 0; thread < thread_count; thread++) {
        if (pthread_join(thread_handles[thread], NULL) != 0) {
            printf("Erro ao aguardar thread\n");
        }
    }

    // Libera memória alocada para as threads
    free(thread_handles);

    // O resultado está em y
    printf("Vetor resultante: \n");
    print_vector(11,y);


    return 0;
}
