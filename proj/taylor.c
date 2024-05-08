#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define n_terms 2400000

void printVec(long double* vec, int size) {
  printf("[ ");
  for (int i = 0; i < size; i++) {
    printf("%.0Lf ", vec[i]);
  }
  printf("]\n");
}

void taylor(int n, long double* factTable,long double* result_ptr) {
  int my_rank = omp_get_thread_num();
  int local_thread_count = omp_get_num_threads();
  int local_n = n / local_thread_count;
  int local_start = my_rank * local_n;

  long double local_result = 0.0;

  for (int i = 0; i < local_n; i++) {
    local_result += 1.0/factTable[local_start + i];
  }
  *result_ptr += local_result;
}

long double* createFactVec(int n) {
  long double* factTable = (long double*) malloc(n_terms * sizeof(long double));
  factTable[0] = 1;
  factTable[1] = 1;

  for (int i = 2; i < n; i++) {
    factTable[i] = factTable[i-1] * (long double)i;
  }
  return factTable;
}


int main(int argc, char* argv[]) {
  double start = omp_get_wtime();
  long double result = 0.0;
  int thread_count= 1;
  if (argc > 1) {thread_count = atoi(argv[1]);}
  long double* factTable = createFactVec(n_terms);

  #pragma omp parallel num_threads(thread_count)
  taylor(n_terms, factTable, &result);

  double end = omp_get_wtime();

  printf("thread_count = %d\n", thread_count);
  printf("num termos = %d\n", n_terms);
  printf("resultado aproximado: %.18Lf\n", result);
  printf("tempo: %.8lf\n", end-start);
  
  free(factTable);
  return 0;
}
