//
// Created by Варвара Семенова on 30.11.2023.
//
#include <math.h>

double r1(double *A, int n, double *x)
{
    double trace = 0, lambda = 0;

    for(int i = 0; i < n; i++)
    {
        trace += A[i * n + i];
        lambda += x[i];
    }

    return abs(trace - lambda);
}

double r2(double *A, int n, double *x)
{
    double A_lenght = 0, x_lenght = 0;

    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            A_lenght += A[i * n + j] * A[i * n + j];

    for(int i = 0; i < n; i++)
        x_lenght += x[i] * x[i];

    return abs(sqrt(A_lenght) - sqrt(x_lenght));
}