#include <iostream>
#include <string>
#include <cmath>

#include "functions.h"
using namespace std;

void print (double *A , int l, int n, int m)
{
    int i, j;

    if ((m>l||m>n) && n < 1)
    {
        printf(">>>wrong data for print -> m\n");
        exit (-1);
    }
    else if (n > 1)
    {
        printf ("=================================================================\n");

        for (i=0;i<m;i++)
        {
            for (j=0;j<m;j++)
                printf(" %10.3e", A[i*n+j]);
            
            printf("\n");
        }

        printf ("=================================================================\n");
    }
    else if (n == 1)
    {
        printf ("=================================================================\n");

        for (i=0;i<m;i++)
        {
            printf(" %10.3e", A[i]);
            
            printf("\n");
        }

        printf ("=================================================================\n");
    }
    
    return ;
}
