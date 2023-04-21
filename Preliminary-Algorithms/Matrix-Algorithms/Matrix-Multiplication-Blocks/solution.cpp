#include <iostream>
#include <string>
#include <cmath>

#include "functions.h"
using namespace std;

int solution (int n, int m, int k, const char* filename)
{
    double *a, *a_mem, *e;
    int i, j, val, count;

    a = (double*)malloc(n*n*sizeof(double));
    if (!a)
    {
        printf (">>>allocation error\n");
        return -2;
    }

    a_mem = (double*)malloc(n*n*sizeof(double));
    if (!a_mem)
    {
        printf (">>>allocation error\n");
        return -2;
    }

    e = (double*)malloc(n*n*sizeof(double));
    if (!e)
    {
        printf (">>>allocation error\n");
        return -2;
    }


    if (k == 0)
    {
        FILE *f;
        f = fopen(filename, "r");
        if (!f)
        {
            printf (">>>can't open file\n");
            return -1;
        }

        count = 0;
        while ((fscanf(f, "%lf", &a[count]) == 1)&&(count <= n*n))
            count++;

        for (i=0;i<n*n;i++)
            a_mem[i]=a[i];

        if (count < n*n)
        {
            printf(">>>incorrect data in file\n");
            fclose (f);
            free (a);
            free (a_mem);
            free (e);
            return -2;
        }

        fclose (f);
    }

    if (k != 0)
    {
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
                a[i*n + j] = formula (k, n, i, j);
        }

        for (i=0;i<n*n;i++)
            a_mem[i]=a[i];
    }



    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (i == j)
                e[i*n + j] = 1;
            else
                e[i*n + j] = 0;
        }
    }

    print (a, n, n, m);

    inverse (a, e, n, m);

    check (a_mem, e, n, m);

    free (a);
    free (e);

    return 1;
}