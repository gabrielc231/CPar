#include <stdio.h>
#include <math.h>
#include <omp.h>

double a = 0;   // ponto a
double b = 100; // ponto b
int n = 8;      // número de trapézios

int main()
{
    double h = (b - a) / n;
    double approx = (sin(a) + sin(b)) / 2.0;

#pragma omp parallel
    {
        for (int i = 0; i < n; i++)
        {
            int x_i = a + i * h;
            approx += sin(x_i);
        }
    }

    approx = h * approx;
}
