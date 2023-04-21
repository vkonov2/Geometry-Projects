#include <iostream>
#include <string>
#include <cmath>

#include "functions.h"
using namespace std;

void swap_str (double *a, int n, int str1, int str2)
{
	double val;

	for (int j = 0; j < n; j++)
	{
		val = a[str1*n + j];
		a[str1*n + j] = a[str2*n + j];
		a[str2*n + j] = val;
	}

	return ;
}