#include <iostream>
#include <string>
#include <cmath>

#include "functions.h"
using namespace std;

int check (double *a, double *e, int n, int m)
{
    double *c;
    double val=0;
    int i, j;

    c = (double*)malloc(n*n*sizeof(double));
    if (!c)
    {
        printf (">>>allocation error\n");
        return -2;
    }

    mult (a, e, c, n, n);

    //print (c, n, n, m);

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (i == j)
                c[i*n + j] -= 1;
        }
    }

    print (c, n, n, m);

    //val = det (c, n);
    val = norm (c, n, n);

    return 1;
}