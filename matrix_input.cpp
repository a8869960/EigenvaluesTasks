//
// Created by varsem on 15.11.23.
//
#include <cstdio>
#include <stdlib.h>

#include <fstream>

#include "functions.h"

int fileMatrixInput(double* A, char* filename, int n)
{
    FILE* file;
    ifstream mFile (filename);

    //Посчитаем количество строк в файле
    if(mFile.is_open())
    {
        int count = 0;
        string line;

        while(mFile.peek()!=EOF)
        {
            getline(mFile, line);
            count++;
        }

        mFile.close();

        if(count != n)
        {
            cout << "Wrong style of input data." << endl;
            return -1;
        }
    }
    else
        cout << "Can't open the file" << endl;

    file = fopen(filename, "r");

    if(!file)
    {
        cout << "Can't open file with matrix." << endl;
        return -1;
    }

    //Проверим количество данных в файле
    double helper = 0;
    size_t size = 0;

    while(fscanf(file, "%lf", &helper) == 1)
        size++;

    if(fscanf(file, "%lf", &helper) != EOF)
    {
        cout << "Wrong input data in the file." << endl;
        fclose(file);
        return -1;
    }

    if(n*n != size)
    {
        cout << "Wrong size of A in the file." << endl;
        fclose(file);
        return -1;
    }

    rewind(file);

    for(int i = 1; i <= n; i++)
    {
        for(int j = 1; j <= n; j++)
        {
            if(fscanf(file, "%lf", &A[(i - 1) * n + j -1]) != 1)
            {
                cout << "Cant read element from file." << endl;
                return -1;
            }
        }
    }

    fclose(file);

    return 0;
}

void f(double *A, int s, int n)
{
    for(int i = 1; i <= n; i++)
    {
        for(int j = 1; j <= n; j++)
        {
            switch(s) {
                case 1:
                    A[(i - 1) * n + j - 1] = n - max(i, j) + 1;
                    break;

                case 2:
                    if(i == j)
                        A[(i - 1) * n + j - 1] = 2.;
                    else if(abs(i - j) == 1)
                        A[(i - 1) * n + j - 1] = -1;
                    else
                        A[(i - 1) * n + j - 1] = 0;
                    break;

                case 3:
                    if(i == j and i != n - 1)
                        A[(i - 1) * n + j - 1] = 1;
                    else if(j == n - 1)
                        A[(i - 1) * n + j - 1] = i;
                    else if(i == n - 1)
                        A[(i - 1) * n + j - 1] = j;
                    else
                        A[(i - 1) * n + j - 1] = 0;
                    break;

                case 4:
                    A[(i - 1) * n + j - 1] = 1. / (i + j - 1);
                    break;
            }
        }
    }
}

int max(int i, int j)
{
    return (i > j) ? i : j;
}

void get_column(double *A, double *a1, int n, int k)
{
    memset(a1, 0, sizeof(double) * n);

    for(int i = k + 1; i < n; i++)
        a1[i] = A[i * n + k];
}