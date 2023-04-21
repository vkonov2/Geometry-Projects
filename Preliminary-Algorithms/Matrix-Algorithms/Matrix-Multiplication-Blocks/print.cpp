#include <iostream>
#include <string>
#include <cmath>

#include "functions.h"
using namespace std;

void print (double *a , int l, int n, int m)
{
    // int i;
    // // printf ("=================================================================\n");
    // for (i = 0; i < m; i++)
    // {
    //     if ((i)%n == 0)
    //     {
    //         printf("\n");
    //         printf(" %10.3e", a[i]);
    //     }
    //     else
    //         printf(" %10.3e", a[i]); 
    // }
    // printf ("\n=================================================================\n");
    // return ;

    int i, j;
    printf ("=================================================================\n");
    for (i=0;i<m;i++)
    {
        for (j=0;j<m;j++)
            printf(" %10.3e", a[i*n+j]);
        printf("\n");
    }
    printf ("\n=================================================================\n");
    return ;
}