#include <iostream>
#include <string>
#include <cmath>

#include "functions.h"
using namespace std;

double check_input ()
{
	double x;

    do
	{
		if (scanf("%lf", &x)!=1)
		{
			printf (">>>entered wrong data\n");
			printf (">>>shutdown\n");
			return -0.1;
		}

		if (fabs(trunc(x)-x)<1e-14)
			break;
		else
		{
			printf (">>>entered a non-integer number\n");
			printf (">>>enter a new integer number:\n");
		}

	} while (fabs(trunc(x)-x)>1e-14);

	return x;
}
