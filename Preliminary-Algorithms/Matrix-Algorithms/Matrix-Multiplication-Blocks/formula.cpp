#include <iostream>
#include <string>
#include <cmath>

#include "functions.h"
using namespace std;

double formula (int k, int n, int i, int j)
{
	if (k == 1)
		return (double)(n + 1 - _max(i,j));
	else if (k == 2)
		return (double)(_max(i,j));
	else if (k == 3)
		return (double)(fabs(i - j));
	else if (k == 4)
		return 1/((double)(i + j - 1));
	else 
		return 0;
}