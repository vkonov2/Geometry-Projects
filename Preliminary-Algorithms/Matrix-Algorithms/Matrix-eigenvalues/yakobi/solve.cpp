#include <iostream>
#include <string>
#include <cmath>

#include "functions.h"
using namespace std;

int solve (int n, int m, double eps, double *A)
{
   	print (A, n, n, m);

   	int i, j;
   	double val;
   	int max_i, max_j;

   	val = A[1];
   	max_i = 0, max_j = 1;

   	for (i = 0; i < n; i++)
   	{
   		for (j = i+1; j < n; j++)
   		{
   			if (i != j)
   			{
   				if (fabs(A[i*n+j]) > fabs(val))
   				{
   					val = A[i*n+j];
   					max_i = i;
   					max_j = j;
   				}
   			}
   		}
   	}

   	while (fabs(val) > eps)
   	{

   		yakobi (A, n, max_i, max_j);


	   	val = A[1];
	   	max_i = 0, max_j = 1;

	   	for (i = 0; i < n; i++)
	   	{
	   		for (j = i+1; j < n; j++)
	   		{
	   			if (i != j)
	   			{
	   				if (fabs(A[i*n+j]) > fabs(val))
	   				{
	   					val = A[i*n+j];
	   					max_i = i;
	   					max_j = j;
	   				}
	   			}
	   		}
	   	}

   	}

   	//print (A, n, n, m);

   	for (i = 0; i < m; i++)
   		printf("%10.3e\n", A[i*n+i]);

    return 1;
}
