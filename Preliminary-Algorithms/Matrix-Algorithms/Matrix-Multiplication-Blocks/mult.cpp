#include <iostream>
#include <cstdlib>

#include "functions.h"

void mult (double * a, double * b, double * c, int n, int m)
{
	for (int i = 0; i < n; i ++)
	{
		for (int j = 0 ; j < m; j ++)
		{
			c[j + i * n] = 0;
			for (int k = 0; k < m; k ++)
				c[j + i * n] += a[k + i * n] * b[j + k * n];

			// if (j%m == 0)
			// 	printf ("\n %10.3lf", c[i*n + j]);
			// else
			// 	printf (" %10.3lf", c[i*n + j]);
			
		}
	}
}

