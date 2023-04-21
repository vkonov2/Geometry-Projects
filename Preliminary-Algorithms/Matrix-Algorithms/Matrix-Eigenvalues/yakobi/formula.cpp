#include <iostream>
#include <string>
#include <cmath>

#include "functions.h"
using namespace std;

double formula (int k, int n, int i, int j)
{
	int max = (i > j ? i : j);

	if (k == 1)
		return (double)(n + 1 - max);
	else if (k == 2)
	{
		if (i == j)
			return 2;
		else if (fabs(fabs(i - j) - 1) < 1e-16)
			return -1;
		else
			return 0;
	}
	else if (k == 3)
	{
		if ((i == j) && (i < n))
			return 1;
		else if (j == n)
			return i;
		else if (i == n)
			return j;
		else
			return 0;
	}
	else if (k == 4)
	{
		if (i + j > 1)
			return 1/((double)(i + j - 1));
		else
			return 0;
	}
	else 
		return 0;
}
