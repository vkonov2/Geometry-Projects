#include <iostream>
#include <string>
#include <cmath>

#include "functions.h"
#include "point.h"
using namespace std;

int solve (Point * points, int n)
{
	FILE* f;
    int count_x, count_y, i;

    f = fopen ("line_coefs.txt", "w");
    if (!f)
    {
    	printf(">>>can't open file -> line_coefs.txt\n");
    	return -1;
    }

    count_x = 0;
    count_y = 0;

    for (i = 0; i < n; i++)
    {
    	if (fabs(points[i].x) > 1e-16)
    		count_x++;

    	if (fabs(points[i].y) > 1e-16)
    		count_y++;
    }

    if ((count_x == 0)&&(count_y == 0))
    {
    	printf ("\nfound line:\n");
    	printf ("%d * x + %d * y = %d\n\n", 0, 0, 0);

    	fprintf(f, "%d %d %d\n", 0, 0, 0);
    	fclose (f);

    	return 1;
    }
    else if (count_x == 0)
    {
    	printf ("\nfound line:\n");
    	printf ("%d * x + %d * y = %d\n\n", 1, 0, 0);

    	fprintf(f, "%d %d %d\n", 1, 0, 0);
    	fclose (f);

    	return 1;
    }
    else if (count_y == 0)
    {
    	printf ("\nfound line:\n");
    	printf ("%d * x + %d * y = %d\n\n", 0, 1, 0);

    	fprintf(f, "%d %d %d\n", 0, 1, 0);
    	fclose (f);

    	return 1;
    }
    else
    {
    	double sum_xx, sum_xy, sum_yy, sum_x, sum_y;

    	double a, b, d;

    	sum_xx = 0;
    	sum_xy = 0;
    	sum_yy = 0;
    	sum_x = 0;
    	sum_y = 0;

    	for (i = 0; i < n; i++)
    	{
    		sum_xx += points[i].x * points[i].x;
    		sum_xy += points[i].x * points[i].y;
    		sum_yy += points[i].y * points[i].y;
    		sum_x += points[i].x;
    		sum_y += points[i].y;
    	}

    	a = 1 / (sqrt( 1 + ((-sum_xx/sum_x+sum_xy/sum_y)/(-sum_yy/sum_y+sum_xy/sum_x)) * ((-sum_xx/sum_x+sum_xy/sum_y)/(-sum_yy/sum_y+sum_xy/sum_x)) ));

    	b = a * ((-sum_xx/sum_x+sum_xy/sum_y)/(-sum_yy/sum_y+sum_xy/sum_x));

    	d = -a * ( ((-sum_xx/sum_x+sum_xy/sum_y)/(-sum_yy/sum_y+sum_xy/sum_x)) * (-sum_xy/sum_x) - sum_xx/sum_x );

    	printf ("\nfound line:\n");
    	printf ("%.2lf * x + %.2lf * y = %.2lf\n\n", a, b, d);

    	fprintf(f, "%lf %lf %lf\n", a, b, d);
    	fclose (f);

    	return 1;
    }

    return 1;
}
