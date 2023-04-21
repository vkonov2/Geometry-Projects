#include <iostream>
#include <string>
#include <cmath>
#include <sys/time.h>

#include "functions.h"
using namespace std;

double norm (double *a, int n, int m)
{
    int column = 0, num = -1, i, j, l, count = 0;
    double val=0;
    double start;

    //print (a, n, n, 5);
    
    start = time_now();
    
    for (i=0;i<n;i++)
    {
        for (j=0;j<m;j++)
            val+=a[i*n+j]*a[i*n+j];
    }

    val=sqrt(val);

    printf("norm time: %lf secs\n\n", time_now()-start);
    printf(">>>norm value = %10.3e\n\n", val);
    
    return val;
}