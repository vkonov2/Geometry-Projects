#include <iostream>
#include <string>
#include <cmath>
#include <sys/time.h>

#include "functions.h"
using namespace std;

double norm (int n, int k, const char* filename, double* A)
{
    FILE *f;
    int i, j;
    double val, norm1 = 0, norm2 = 0;
    double trace_beg = 0, trace_fin = 0;
    double len_beg = 0, len_fin = 0;

    for (i = 0; i < n; i++)
    {
        val = A[i*n+i];

        trace_fin += val;

        len_fin += val*val;
    }

    len_fin = sqrt (len_fin);
    
    if (k == 0)
    {
        f = fopen (filename, "r");
        if (!f)
        {
            printf(">>>can't open file -> %s\n", filename);
            return -2;
        }

        i = 0;
        while (
            (fscanf(f, "%lf", &val) == 1)
            &&
            (i < n*n)
            )
        {
            A[i] = val;
            i++;
        }

        if (i < n*n)
        {
            printf(">>>wrong data in file -> not enough numbers\n");
            fclose (f);
            return -3;
        }

        fclose (f);
    }

    if (k != 0)
    {
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
                A[i*n+j] = formula (k, n, i+1, j+1);
        }
    }
    
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            val = A[i*n+j];

            if (i == j)
                trace_beg += val;

            len_beg += val*val;
        }
    }

    len_beg = sqrt (len_beg);

    norm1 = fabs (trace_beg - trace_fin);

    norm2 = fabs (len_beg - len_fin);

    printf("\nresidual in 1st invar: %10.3e\n", norm1);

    printf("\nresidual in 2nd invar: %10.3e\n", norm2);
    
    return 1;
}
