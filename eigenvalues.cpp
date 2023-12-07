//
// Created by Варвара Семенова on 30.11.2023.
//
#include "functions.h"
#define EPS 1e-16

int find_eigenvalues(ARGS *arg)
{
    int n = arg->n;
    double *a = arg->a, *x = arg->x;
    double eps = arg->eps;
    
    //Приведение к трехдиагональному виду
    double time = get_full_time();
    if(three_diagonal(arg) == -1) return -1;
    arg->three_diagonal_time = get_full_time() - time;

//    if(LU_decomposition(arg) == -1) return -1;
//
//    cout << "lambda: " << endl;
//    matrixOutput(arg->xk, 1, n, n);
//    cout << "l:" << endl;
//    matrixOutput(arg->y, 1, n, n);
//    cout << sign_changes(arg) << endl;
//    cout << "u:" << endl;
//    matrixOutput(arg->z, 1, n, n);

    //Вычисляем b0
    double b0 = matrix_max_norm(a, n) + EPS;
    double ai = -b0, bi = b0, c;

    //Вычисляем собственные значения
    time = get_full_time();
    for(int k = 0; k < n; k++)
    {
        bool flag = true;
        while(flag)
        {
            arg->its++;
            if(bi - ai < eps)
            {
                flag = false;
                
                int multiplicity = sign_changes(arg, bi) - sign_changes(arg, ai);
                if(multiplicity == 0)
                {
                    cout << "Multiplicity == 0." << endl;
                    // cout << sign_changes(arg, bi) << " " << sign_changes(arg, ai) << " " << multiplicity << endl;
                    return -1;
                }

                // cout << sign_changes(arg, ai) << multiplicity << endl;
                double lambda = (ai + bi) / 2;
                for(int i = k; i < k + multiplicity; i++)
                    x[i] = lambda;

                k += multiplicity - 1;
            } else
            {
                c = (ai + bi) / 2;
                int n_minus = sign_changes(arg, c);
                // cout << c << " " << n_minus << " " << k + 1 << " " << "[" << ai << " " << bi << "]" << endl;

                if(n_minus < k + 1)
                    ai = c;
                else
                    bi = c;
            }
        }

        bi = b0;
        ai = x[k];
    }
    arg->eigenvalues_time = get_full_time() - time;

    return 0;
}

int three_diagonal(ARGS *arg)
{
    double *a = arg->a, *xk = arg->xk;
    int n = arg->n;
    double norm;

    for(int k = 0; k < n - 2; k++)
    {
        //Считаем x(k)
        get_column(a, xk, n, k);
        // matrixOutput(xk, 1, n, n);
        norm = vector_norm(xk, n);
        xk[k + 1] -= norm;
        norm = vector_norm(xk, n);
        if(vector_division(xk, n, norm) == -1)
            continue;

        //Считаем UAUt СДЕЛАТЬ ЧТОБЫ СЧИТАЛСЯ ТОЛЬКО ВЕРХНИЙ ТРЕУГОЛЬНИК???
        UAUt(arg);
    }

    // cout << "UAUt:" << endl;
    // matrixOutput(a, n, n, n);
    // cout << endl;

    return 0;
}

void UAUt(ARGS *arg)
{
    double *a = arg->a, *b = arg->b, *c = arg->c, *xk = arg->xk, *y = arg->y, *z = arg->z;
    int n = arg->n;

//    cout << "xk:" << endl;
//    matrixOutput(xk, 1, n, n);
//
//    cout << "a: " << endl;
//    matrixOutput(a, n, n, n);

    matrix_product(a, xk, y, n, n, 1); //y = Ax

//    cout << "y = Ax:" << endl;
//    matrixOutput(y, 1, n, n);

    matrix_product(xk, y, z, 1, n, 1); //z[0] = (x; y)
    double xy = z[0];
//    cout << "(x; y) = " << xy << endl;

    for(int i = 0; i < n; i++)
        z[i] = xk[i] * xy; //z = (x; y)x

//    cout << "z = (x; y)x" << endl;
//    matrixOutput(z, 1, n, n);

    matrixSubtraction(y, z, z, 1, n); //z = y - (x; y)x
//    cout << "z = y - (x; y)x" << endl;
//    matrixOutput(z, 1, n, n);

    vector_multiplying(z, n, 2);
//    cout << "z = 2y - 2(x; y)x" << endl;
//    matrixOutput(z, 1, n, n);

    matrix_product(z, xk, c, n, 1, n); //c = zx*
//    cout << "c" << endl;
//    matrixOutput(c, n, n, n);
    matrix_product(xk, z, b, n, 1, n); //b = xz*
//    cout << "b" << endl;
//    matrixOutput(b, n, n, n);
    matrixSubtraction(a, c, a, n, n);
//    cout << "a1" << endl;
//    matrixOutput(a, n, n, n);
    matrixSubtraction(a, b, a, n, n);
//    cout << "a2" << endl;
//    matrixOutput(a, n, n, n);
}

int LU_decomposition(ARGS *arg, double alpha) //lambda = xk, l = y, u = z ?????
{
    int n = arg->n;
    double *a = arg->a, *lambda = arg->xk, *l = arg->y, *u = arg->z;

    for(int i = 0; i < n - 1; i++)
        lambda[i] = a[(i + 1) * n + i];

    l[0] = a[0] - alpha;
    for(int i = 0; i < n - 1; i++)
    {
        if(fabs(l[i]) < EPS) return -1;
        u[i] = lambda[i] / l[i];

        l[i + 1] = a[(i + 1) * n + i + 1] - alpha - lambda[i] * u[i];
    }
    // cout << "l: " << alpha << endl;
    // matrixOutput(l, 1, n, n);

    return 0;
}

int sign_changes(ARGS *arg, double alpha)
{
    int n = arg->n;
    double *l = arg->y;

    if(LU_decomposition(arg, alpha) == -1) return n;

    int count = 0;

    for(int i = 0; i < n; i++)
        if(l[i] < EPS) count++;

    return count;
}