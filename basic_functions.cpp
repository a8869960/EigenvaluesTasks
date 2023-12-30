//
// Created by Варвара Семенова on 30.11.2023.
//
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#include <cstdio>
#include <cstdlib>
#include <sys/sysinfo.h>
#include <sys/time.h>
#include <sys/resource.h>

double get_full_time()
{
    struct timeval buf;
    gettimeofday(&buf, NULL);
    return buf.tv_sec + buf.tv_usec / 1.e6;
}

double get_CPU_time()
{
    struct rusage buf;
    getrusage(RUSAGE_THREAD, &buf);
    return buf.ru_utime.tv_sec + buf.ru_utime.tv_usec / 1.e6;
}

int toInt(const char* str, int* ptr)
{
    long L;
    char* e;

    errno = 0;
    L = strtol(str, &e, 10);

    if (!errno && *e == '\0')
        if (INT_MIN <= L && L <= INT_MAX)
        {
            *ptr = (int)L;
            return 0;
        }
        else
            return -1;
    else
        return -1;
}

int toDouble(const char* str, double* ptr)
{
    char* e;

    errno = 0;
    *ptr = strtod(str, &e);

    if (!errno && *e == '\0')
        return 0;
    else
        return -1;
}
