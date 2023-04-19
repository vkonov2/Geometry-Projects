#ifndef FUNCTIONS_H 
#define FUNCTIONS_H

#include "point.h"

double check_input ();
void get_data (Point * points, int n, FILE*f);
int solve (Point * points, int n);

double time_now(void);

double _max (double a, double b);

#endif
