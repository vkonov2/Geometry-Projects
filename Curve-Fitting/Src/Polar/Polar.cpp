#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <cmath>
#include <math.h>
#include <string>
#include "Polar.h"


void Get_lines(double * l, int N)
{
	for (int i = 0; i < N; i++)
	{
		/*l[i] = atan(p[i].y/p[i].x);*/
		l[i] = tan(2 * M_PI * i / N);

	}
}

void centrelize(const int n, VECTOR2 * p)
{
	VECTOR2 mass_center;
	mass_center.x = 0;
	mass_center.y = 0;

	for (int i = 0; i < n; i++)
	{
		mass_center.x += p[i].x;
		mass_center.y += p[i].y;
	}

	mass_center.x /= n;
	mass_center.y /= n;

	for (int i = 0; i < n; i++)
	{
		p[i].x -= mass_center.x;
		p[i].y -= mass_center.y;
	}
}

double get_polar_intersection(VECTOR2 p1, VECTOR2 p2, double k)
{
	double x = (-1) * (p1.x * (p2.y - p1.y) - p1.y * (p2.x - p1.x)) / ((p2.x - p1.x) * k + (p1.y - p2.y));
	double y = (-1) * (p1.x * (p2.y - p1.y) - p1.y * (p2.x - p1.x)) * k / ((p2.x - p1.x) * k + (p1.y - p2.y));

	return sqrt(x*x + y*y);
}

double get_curr_angle(VECTOR2 p)
{
	double phi = atan2(p.y,p.x)*180/M_PI;

	if (phi < 0)
		phi += 360; 
	return phi;
}

void get_func_values ( const int nHull, int * Hull, VECTOR2 * Polygone, double * l, int N, double * f)
{
	VECTOR2 p1, p2;
	double phi1=0, phi2=0, max, min;
	double inter = 0;
	int first_ray=0, last_ray=nHull-1;
	int i,j,k;
	/* initialize f with nulls */
	for ( k = 0; k < N; k++)
	{
		f[k] = 0;
	}

	int i0, i1;

	for (i = 0; i < nHull - 1; i++)
	{
		i0 = Hull[i % nHull];
		i1 = Hull[(i + 1) % nHull];

		//printf("i = %d\n", i0);

		p1 = Polygone[i0];
		p2 = Polygone[i1];
		phi1 = get_curr_angle(p1);
		phi2 = get_curr_angle(p2);
		if(phi2 > phi1)
		{
			max = phi2;
			min = phi1;
		}
		else
		{
			max = phi1;
			min = phi2;
		}
		
		/* last pair case */
		if(( (min >= 0)&&(min < 90) ) && ((max >= 180)&&(max < 360) ))
		{
			first_ray = (int)(ceil(max / (360.0/N)));
			last_ray = (int)(floor(min / (360.0/N)));
			
			for ( j = 0; j < last_ray + 1; j++)
			{
				inter = get_polar_intersection(p1, p2, l[j]);
				if (inter > f[j])
					f[j] = inter;
			}
			for ( j = first_ray; j < N; j++)
			{
				inter = get_polar_intersection(p1, p2, l[j]);
				if (inter > f[j])
					f[j] = inter;
			}

		}
		/* regular case */
		else
		{
			
			first_ray = (int)(ceil(min / (360.0/N)));
			last_ray = (int)(floor(max / (360.0/N)));
			
			for ( j = first_ray; j < last_ray + 1; j++)
			{
				inter = get_polar_intersection(p1, p2, l[j]);
				if (inter > f[j])
					f[j] = inter;
			}
		}
		
	}
}

Polar::Polar()
{
	N = 0;
	f = 0;
}

/* changes Polygone , so give a cpoy of Polygone as arg */
Polar::Polar(const int n, const int nPoints, VECTOR2 * Polygone, const int nHullPoints, int * HullInd) {

    N = n;

    double * l;
    
    l = new double[N];
	f = new double[N];

	for (int i = 0; i < N; ++i)
		f[i] = 0.0;

	if(!f)
	{
		printf("Create f err\n");
	}
    /* put poly to the (0,0)*/
    // centrelize(nPoints, Polygone);

    /* create lines to intersect with poly */
    Get_lines(l, N);

    get_func_values(nHullPoints, HullInd, Polygone, l, N, f);

	delete[] l;
}

Polar::~Polar()
{
	if(f)
		delete[] f;
}
double Polar:: L2Diff(Polar & func) 
{
    if (N != func.N) {
        printf("Different sizes\n");
        return -1;
    }

	double diff = 0;

	//printf("func.N = %d", func.N);
    for (int i = 0; i < func.N; i ++) 
	{
		///printf("f[i] = %lf, func.f[i] = %lf\n", f[i], func.f[i]);
        diff += (f[i] - func.f[i])*(f[i] - func.f[i]);
    }

	printf("\n");

    return sqrt(diff);
}

//int main (void) {
//
//    VECTOR2 * square;
//    int * Hull;
//    int N = 4;
//    square = (VECTOR2 *) malloc (4 * sizeof(VECTOR2));
//    Hull = (int *) malloc (4 * sizeof(int));
//
//    for (int i = 0; i < 4; i++) {
//        Hull[i] = i;
//        
//    }
//
//    square[0].x = 1;
//    square[0].y = 1;
//    square[1].x = -1;
//    square[1].y = 1;
//    square[2].x = -1;
//    square[2].y = -1;
//    square[3].x = 1;
//    square[3].y = -1;
//
//    Polar p_s(N, 4, square, 4, Hull);
//
//    for (int k = 0; k < 4; k ++) {
//        cout << p_s.f[k] << "\n";
//    }
//
//
//    return 0;
//}