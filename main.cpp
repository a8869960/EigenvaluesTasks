//
// Created by Варвара Семенова on 30.11.2023.
//
#include "functions.h"

int main(int ac, char* av[])
{
    try
    {
        int n, m, k;
        double eps;
        char *filename = nullptr;

        if(ac != 5 and ac != 6)
        {
            cout << "Wrong parameters." << endl;
            return -1;
        }

        if(toInt(av[1], &n) == -1)
        {
            cout << "Can't read parameter n." << endl;
            return -1;
        }

        if(toInt(av[2], &m) == -1)
        {
            cout << "Can't read parameter m." << endl;
            return -1;
        }

        if(toDouble(av[3], &eps) == -1)
        {
            cout << "Can't read parameter eps." << endl;
            return -1;
        }

        if(toInt(av[4], &k) == -1)
        {
            cout << "Can't read parameter k." << endl;
            return -1;
        }

        if(k == 0 and ac == 6)
            filename = av[5];
        else if((k == 0 and ac == 5) or (k != 0 and ac == 6))
        {
            cout << "Wrong parameter s and filename." << endl;
            return -1;
        }

        //Инициализация матрицы A
        ARGS *arg = new ARGS[1];
        double *A = new double[n * n];
        double *x = new double[n];

        if(ac == 5)
            f(A, k, n);
        else
        {
            if(fileMatrixInput(A, filename, n) == -1)
            {
                delete[] arg;
                delete[] A;
                delete[] x;
                return -1;
            }
        }

        if(!is_symmetric(A, n))
        {
            cout << "Matrix is not symmetric." << endl;
            delete[] arg;
            delete[] A;
            delete[] x;
            return -1;
        }

        if(m > 0) cout << "Matrix A:" << endl;
        matrixOutput(A, n, n, m);

        //Поиск собственных значений
        arg->n = n;
        arg->A = A;
        arg->x = x;
        arg->eps = eps;

        arg->a = new double[n * n];
        arg->b= new double[n * n];
        arg->c = new double[n * n];
        arg->xk = new double[n];
        arg->y = new double[n];
        arg->z = new double[n];

        bool flag = true;

        memcpy(arg->a, A, sizeof(double) * n * n);

        if(find_eigenvalues(arg) == -1)
        {
            flag = false;
            cout << "Can't be used this method." << endl;
        }

        //Results
        double res1 = 0, res2 = 0;
        if(flag)
        {
            if(m > 0) cout << "Result:" << endl;
            matrixOutput(x, 1, n, m);
            res1 = r1(arg->A, n, arg->x);
            res2 = r2(arg->A, n, arg->x);
        }
        int its = arg->its;
        double t1 = arg->three_diagonal_time, t2 = arg->eigenvalues_time;

        //Запись результата
        printf ("%s : Residual1 = %e Residual2 = %e Iterations = %d \
   Iterations1 = %d Elapsed1 = %.2f Elapsed2 = %.2f\n",
                av[0], res1, res2, its, its / n, t1, t2);

        delete[] arg;
        delete[] A;
        delete[] x;

        return 0;
    } catch (const bad_alloc& e)
    {
        cout << "Bad alloc" << endl;
        return -1;
    }
}
