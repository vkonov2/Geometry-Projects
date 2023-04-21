#include <iostream>
#include <string>
#include <cmath>
#include <sys/time.h>

#include "functions.h"
using namespace std;

void inverse (double *a, double *e, int n, int m)
{
    int column = 0, num = -1, i, j, l;
    double val, k;
    double start;
    
    start = time_now();
    
    for (j = column; j < n; j++)
    {
    	val = a[i*column + column];
        for (i = column; i < n; i++)
        {
            if (fabs(val) < fabs(a[i*n+column]))
            {
            	val = a[i*n+column];
                num = i;
            }
        }
        
        if (fabs(val-0) > 1e-14)
        {	
        	if (column < num)
        	{
        		swap_str (a, n, column, num);
        		swap_str (e, n, column, num);
        	}

        	for (i = 0; i < column; i++)
            {
                k = a[i*n + j] / val;
                
                for (l = 0; l < n; l++)
                {
                    a[i*n + l] -= k * a[column*n + l];
                    
                    e[i*n + l] -= k * e[column*n + l];
                }
            }

            for (i = column+1; i < n; i++)
            {
                k = a[i*n + j] / val;
                
                for (l = 0; l < n; l++)
                {
                    a[i*n + l] -= k * a[column*n + l];
                    
                    e[i*n + l] -= k * e[column*n + l];
                }
            }
            
            column++;
        }
        else
        {
            printf("Inverse matrix does not exist\n");

            return ;
        }
    }
    
    k = 0;
    
    for (i = 0; i < n; i++)
    {
        if (fabs (a[i*n + i] - 0) > 1e-14)
        {
            k = 1/a[i*n + i];
            
            a[i*n + i] = 1;
            
            for (j = 0; j < n; j++)
                e[i*n + j] *= k;
        }
        else
        {
            printf("Inverse matrix does not exist\n");
            
            return ;
        }
    }

    print (e, n, n, m);

    printf("inversion time: %lf secs\n", time_now()-start);
    
    return ;
}