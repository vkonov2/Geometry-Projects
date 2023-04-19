#include <iostream>
#include <string>
#include <cmath>

#include "functions.h"
#include "point.h"
using namespace std;

void get_data (Point* points, int n, FILE*f)
{
	for (int i = 0; i < n; i++) 
	{
		points[i].x = (double)i;
		points[i].y = 8*(double)i - 3 + ((rand()%100)-50)*0.5;

		fprintf( f, "%lf %lf ", points[i].x, points[i].y );
	}
}
