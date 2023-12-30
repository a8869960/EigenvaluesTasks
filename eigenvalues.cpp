//
// Created by Варвара Семенова on 30.11.2023.
//
#include "functions.h"
#define EPS 1e-16

int sign(double l);
bool is_three_diagonal(ARGS *arg);

int find_eigenvalues(ARGS *arg)
{
    int n = arg->n;
    double *a = arg->a, *x = arg->x;
    double eps = arg->eps;

    arg->norm = matrix_max_norm(a, n);
    double norm = arg->norm;
    
    if (n == 1)
    {
        x[0] = a[0];
        return 0;
    }

    //Приведение к трехдиагональному виду
    double time = get_full_time();
    if(!is_three_diagonal(arg))
    {
//         cout << "not 3" << endl;
        if(three_diagonal(arg) == -1)
        {
            cout << "Pr with three_diagonal" << endl;
            return -1;
        }
        for (int j = 0; j < n - 2; j++)
            for(int i = j + 2; i < n; i++)
            {
                a[i * n + j] = 0;
                a[j * n + i] = 0;
            }
        arg->three_diagonal_time = get_full_time() - time;
    }
//     cout << "UAUt:" << endl;
//     matrixOutput(a, n, n, 10);
//     cout << endl;
    

    //Вычисляем b0
    double b0 = arg->norm;
    double ai = -b0, bi = b0, c = 0;
//    cout << b0 << endl;
//    cout << sign_changes(arg, -1.015e-22) << endl;

    //Вычисляем собственные значения
    time = get_full_time();
    for(int k = 0; k < n; k++)
    {
        bool is_end = false;
        while(bi - ai > eps * norm and !is_end)
        {
            c = (ai + bi) / 2;

            if(bi - c < EPS * norm or c - ai < EPS * norm)
                is_end = true;

            int n_minus = sign_changes(arg, c);
            if(n_minus < k + 1)
                ai = c;
            else
                bi = c;

//            cout << "[" << ai << " " << bi << "]" << endl;
//            cout << n_minus << endl;
            arg->its++;
        }

        double lambda = (ai + bi) / 2;
        int s1 = sign_changes(arg, bi), s2 = sign_changes(arg, ai);
        int multiplicity = s1 - s2;
//        cout << multiplicity << endl;

        if(multiplicity == 0)
        {
            s1 = sign_changes(arg, bi + eps * norm);
            s2 = sign_changes(arg, ai - eps * norm);
            multiplicity = s1 - s2;
            if (multiplicity == 0) multiplicity++;
        }

//        cout << "[" << ai << " " << bi << "]" << endl;
//        cout << "lambda: " << lambda << " | " << multiplicity << " = " << s1 << " - " << s2 << endl;
//        if(is_end) cout << "end" << endl;
        
        for(int i = k; i < k + multiplicity; i++)
        {
           if(i > n - 1) break;
            x[i] = lambda;
        }

        k += multiplicity - 1;
        bi = b0;
        ai = lambda + eps * norm;
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

//     cout << "UAUt:" << endl;
//     matrixOutput(a, n, n, 10);
//     cout << endl;

    return 0;
}

void UAUt(ARGS *arg)
{
    double *a = arg->a, *b = arg->b, *c = arg->c, *xk = arg->xk, *y = arg->y, *z = arg->z;
    int n = arg->n;

    matrix_product(a, xk, y, n, n, 1); //y = Ax
    matrix_product(xk, y, z, 1, n, 1); //z[0] = (x; y)
    double xy = z[0];

    for(int i = 0; i < n; i++)
        z[i] = xk[i] * xy; //z = (x; y)x

    matrixSubtraction(y, z, z, 1, n); //z = y - (x; y)x
    vector_multiplying(z, n, 2);
    matrix_product(z, xk, c, n, 1, n); //c = zx*
    matrix_product(xk, z, b, n, 1, n); //b = xz*
    matrixSubtraction(a, c, a, n, n);
    matrixSubtraction(a, b, a, n, n);
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
        if(fabs(l[i]) < EPS) l[i] = sign(l[i]) * arg->eps;
        u[i] = lambda[i] / l[i];

        l[i + 1] = a[(i + 1) * n + i + 1] - alpha - lambda[i] * u[i];
    }
    // cout << "l: " << alpha << endl;
    // matrixOutput(l, 1, n, n);

    return 0;
}

int sign(double l)
{
    if(l < 0)
        return -1.0;
    else
        return 1.0;
}

bool is_three_diagonal(ARGS *arg)
{
    int n = arg->n;
    double *a = arg->a;
    
    for (int j = 0; j < n - 2; j++)
        for(int i = j + 2; i < n; i++)
            if(fabs(a[i * n + j]) > 1e-100)
                return false;
            
    return true;
}

int sign_changes(ARGS *arg, double b, [[maybe_unused]]bool f)
{
    int n = arg->n;
    double *a = arg->a;
    double epsA = arg->eps;
    if (n <= 0) return 0;
    double up, aii, down;
    int sgn = 1, count = 0;

    aii = a[0] - b;
    if (fabs(aii) < 1e-100) 
        aii = epsA;
    if (sgn != sgn * sign(aii))
        count++;

    up = a[1] / aii;
    sgn = sgn * sign(aii);

    for(int i = 1; i < n - 1; i++)
    {
        down = a[i * n + i - 1];
        aii = a[i * n + i] - b - down * up;
        if (fabs(aii) < 1e-100) aii = epsA * arg->norm;
        if (sgn != sgn * sign(aii))
            count++;

        sgn = sgn * sign(aii);
        up = a[i * n + i + 1] / aii;
    }
    down = a[n * n - 2];
    aii = a[n * n - 1] - b - down * up;
    if (fabs(aii) < 1e-100) aii = epsA;

    if (sgn != sgn * sign(aii))
        count++;
    
    return count;
}
