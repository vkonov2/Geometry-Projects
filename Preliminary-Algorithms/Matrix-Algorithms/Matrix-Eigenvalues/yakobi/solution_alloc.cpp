#include <iostream>
#include <string>
#include <cmath>

#include "functions.h"
using namespace std;

int solution_alloc (int n, int m, double eps, int k, const char* filename)
{	
	FILE *f;
	double *A;
	double val, start;
	int i, j, err;

	A = new double[n*n];
	if (!A)
	{
		printf(">>>allocation error -> a\n");
		return -1;
	}

	if (k == 0)
	{
		f = fopen (filename, "r");
		if (!f)
		{
			printf(">>>can't open file -> %s\n", filename);
			delete [] A;
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
			delete [] A;
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

	start = time_now();
    err = solve (n, m, eps, A);
    if (err < 0)
    {
    	printf("\n>>>error in func -> solve\n");
    	delete A;
        return err;
    }
    printf("\ntime of solution: %10.3e secs\n", time_now()-start);

    err = norm (n, k, filename, A);
    if (err < 0)
    {
    	printf("\n>>>error in func -> norm\n");
    	delete A;
        return err;
    }

    delete A;

    return 1;
}
