//
// Created by Варвара Семенова on 30.11.2023.
//

#ifndef EIGENVALUESTASKS_FUNCTIONS_H
#define EIGENVALUESTASKS_FUNCTIONS_H

#include <iostream>
#include <cstring>
#include <cmath>

using namespace std;

enum class io_status
{
    successful,
    error_read,
    error_open,
    none
};

class ARGS
{
public:
    int n = 0;
    double *A = nullptr;
    double *x = nullptr;
    double eps = 0;

    double *a = nullptr;
    double *b = nullptr;
    double *c = nullptr;
    double *xk = nullptr;
    double *y = nullptr;
    double *z = nullptr;

    double norm = 0;

    int its = 0;
    double three_diagonal_time = 0;
    double eigenvalues_time = 0;

    ~ARGS()
    {
        delete[] a;
        delete[] b;
        delete[] c;
        delete[] xk;
        delete[] y;
        delete[] z;
    }
};

//basic_functions.cpp
double get_full_time();
double get_CPU_time();
int toInt(const char* str, int* ptr);
int toDouble(const char* str, double* ptr);

//matrix_operations.cpp
int matrixSubtraction(double* A1, double *A2, double *C, int n, int m);
void matrix_product(double *A, double* B, double* C, int n, int s, int m);
double vector_norm(double *x, int n);
int vector_division(double *a1, int n, double alpha);
void vector_multiplying(double *a1, int n, double alpha);
void E(double* block, int m);
double matrix_max_norm(double *A, int n);

//matrix_input
int fileMatrixInput(double* A, char* filename, int n);
void f(double *A, int s, int n);
void get_column(double *A, double *a, int n, int k);
bool is_symmetric(double *A, int n);

//matrix_output
void matrixOutput(double *matrix, int l, int n, int r = 3);

//results.cpp
double r1(double *A, int n, double *x);
double r2(double *A, int n, double *x);

//eigenvalues.cpp
int find_eigenvalues(ARGS *arg);
int three_diagonal(ARGS *arg);
void UAUt(ARGS *arg);
int LU_decomposition(ARGS *arg, double alpha = 0); //lambda = xk, l = y, u = z
int sign_changes(ARGS *arg, double alpha, bool f = false);

#endif //EIGENVALUESTASKS_FUNCTIONS_H
