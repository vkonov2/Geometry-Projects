#include <iostream>
#include <string>
#include <cmath>
#include <vector>

#include "Functions.h"
#include "Point.h"
using namespace std;

#define _USE_MATH_DEFINES

double FindApproxDistFromCurve2D (Point point, vector<double> coeffs)
{
    double a = coeffs[0];
    double b = coeffs[1];
    double c = coeffs[2];
    double d = coeffs[3];
    double e = coeffs[4];
    double f = coeffs[5];

    if (fabs (a) < 1e-14 && fabs (b) < 1e-14 && fabs (c) < 1e-14 && fabs (d) < 1e-14 && fabs (e) < 1e-14 && fabs (f) < 1e-14)
    {
        printf ("\n>>> bad curve \n");
        return -1.0;
    }

    f = a*point.x*point.x + b*point.y*point.y + c*point.x*point.y + d*point.x + e*point.y + f;
    d = 2*a*point.x + c*point.y + d;
    e = 2*b*point.y + c*point.x + e;

    double x, y;
    x = 0.0;
    y = 0.0;

    // printf("\ntranfer 1 -- curve: \n%lf*x^2+%lf*y^2+%lf*x*y+%lf*x+%lf*y+%lf=0\n", a, b, c, d, e, f);
    // printf ("\ntranfer 1 -- point:\nx = %lf\ny = %lf\n\n", x, y);

    const int N = 100;
    double h = 1.0;

    double left = -50.0;

    double val1, val2, D;
    double val_x, val1_y, val2_y;

    double dist = 1e+20;
    double dist1_cur, dist2_cur;

    double x_min, y_min;
    int i_min;

    while (h > 1e-6)
    {
        for (int i = 0; i < N; i++)
        {
            val_x = left + i*h;
            D = (c*val_x+e)*(c*val_x+e) - 4*b*(a*val_x*val_x+d*val_x+f);

            if (D < 0)
                continue;
            else
            {
                val1_y = (-(c*val_x+e)+sqrt(D))/(2*b);
                val2_y = (-(c*val_x+e)-sqrt(D))/(2*b);

                val1 = a*val_x*val_x+b*val1_y*val1_y+c*val_x*val1_y+d*val_x+e*val1_y+f;
                val2 = a*val_x*val_x+b*val2_y*val2_y+c*val_x*val2_y+d*val_x+e*val2_y+f;

                if (fabs(val1)<1e-14)
                {
                    dist1_cur = sqrt(val_x*val_x+val1_y*val1_y);

                    if (dist > dist1_cur)
                    {
                        dist = dist1_cur;
                        x_min = val_x;
                        y_min = val1_y;
                        i_min = i;
                    }
                }

                if (fabs(val2)<1e-14)
                {
                    dist2_cur = sqrt(val_x*val_x+val2_y*val2_y);

                    if (dist > dist2_cur)
                    {
                        dist = dist2_cur;
                        x_min = val_x;
                        y_min = val2_y;
                        i_min = i;
                    }
                }
            }
        }

        left += (i_min - 1)*h;
        h = 2*h/double(N);
    }

    // printf ("point: (%lf, %lf)\n", x_min, y_min);

    return dist;
}
