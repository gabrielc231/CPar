#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

void trapezoid(double point_a, double point_b, int trapezoid_number, double* result_ptr)
{
    long local_trapezoid_number;
    double trapezoid_height;
    double local_x; // entrada
    double local_a;
    double local_b;
    double local_result;
    int my_rank = omp_get_thread_num();
    int local_thread_count = omp_get_num_threads();
    
    trapezoid_height = (point_b - point_a) / trapezoid_number;
    local_trapezoid_number = trapezoid_number / local_thread_count;
    local_a = point_a + my_rank * local_trapezoid_number * trapezoid_height;
    local_b = local_a + local_trapezoid_number * trapezoid_height;
    local_result = (sin(local_a) + sin(local_b)) / 2.0;

    for (int i = 1; i <= local_trapezoid_number - 1; i++)
    {
        local_x = local_a + i * trapezoid_height;
        local_result += sin(local_x);
    }

    local_result *= trapezoid_height;
    
    double aux = *result_ptr; //testar condicao de corrida
    
    for (int i = 0; i < 10000; i++) *result_ptr = i; //testar condicao de corrida
    
    *result_ptr = aux + local_result;
}

int main(int argc, char* argv[])
{
    int thread_count = 1;   // numero de threads
    int n = 2400000;        // número de trapézios
    double result = 0; // resultado da aproximação
    double a = 0;      // limite inferior da integral
    double b = 100;    // limite superior da integral

    if (argc > 1) // caso queira colocar um valor para número de threads
    {
        thread_count = strtol(argv[1], NULL, 10);
    }
	
    #pragma omp parallel num_threads(thread_count)
    trapezoid(a, b, n, &result);

    printf("thread_count = %d\n", thread_count);
    printf("n = %d\n", n);
    printf("a = %.2f\n", a);
    printf("b = %.2f\n", b);
    printf("\nresultado aproximado: %.10lf...\n", result);
   
}
