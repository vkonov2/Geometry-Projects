#include <iostream>
#include <cstdlib>

#include "functions.h"

void mult(double *A, double *B, double *C, int m, int n, int l)
{
	int i, j, k;
	double val;

	for (i = 0; i < m; i ++)
	{
		//printf(">>>i = %d\n", i);
		for (j = 0 ; j < l; j ++)
		{
			//printf(">>>j = %d\n", j);
			val = 0;
			//C[i*l+j] = 0;
			
			for (k = 0; k < n; k ++)
			{
				//C[i*l+j] += A[i*n+k] * B[k*l+j];
				val += A[i*n+k] * B[k*l+j];
				// printf (">>>A[%d] = %10.2lf\n", i*n+k, A[i*n+k]);
				// printf (">>>B[%d] = %10.2lf\n", k*l+j, B[k*l+j]);
				// printf (">>>A[%d] * B[%d] = %10.2lf\n", i*n+k, k*l+j, A[i*n+k] * B[k*l+j]);
				// printf (">>>C[%d] = %10.2lf\n", i*l+j, C[i*l+j]);
				// printf (">>>C[%d] = %10.2lf\n", i*l+j, val);
			}
			
			C[i*l+j] = val;
		}
	}

	return ;
}

