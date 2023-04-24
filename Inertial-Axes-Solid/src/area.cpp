#include <iostream>
#include <string>
#include <cmath>

#include "functions.h"
#include "point.h"
using namespace std;

int area (Points* points, Planes* planes, Bones* bones, int nv, int np, int nb)
{
	// struct Planes
	// {
	// 	int p;
	//     int v1, v2, v3, v4;
	//     double x, y, z, s;
	//	   double a, b, c, d;
	// };

	int i, j, k;
	double sum_x = 0, sum_y = 0, sum_z = 0;
	double vec1_x, vec1_y, vec1_z;
	double vec2_x, vec2_y, vec2_z;
	double n_x, n_y, n_z, n;

	for (i = 0; i < np; i++)
	{
		sum_x = 0, sum_y = 0, sum_z = 0;

		sum_x += points[planes[i].v1].x;
		sum_x += points[planes[i].v2].x;
		sum_x += points[planes[i].v3].x;
		sum_x += points[planes[i].v4].x;

		sum_y += points[planes[i].v1].y;
		sum_y += points[planes[i].v2].y;
		sum_y += points[planes[i].v3].y;
		sum_y += points[planes[i].v4].y;

		sum_z += points[planes[i].v1].z;
		sum_z += points[planes[i].v2].z;
		sum_z += points[planes[i].v3].z;
		sum_z += points[planes[i].v4].z;

		planes[i].x = sum_x / 4.0;
		planes[i].y = sum_y / 4.0;
		planes[i].z = sum_z / 4.0;

		vec1_x = points[planes[i].v2].x - points[planes[i].v1].x;
		vec1_y = points[planes[i].v2].y - points[planes[i].v1].y;
		vec1_z = points[planes[i].v2].z - points[planes[i].v1].z;

		vec2_x = points[planes[i].v3].x - points[planes[i].v1].x;
		vec2_y = points[planes[i].v3].y - points[planes[i].v1].y;
		vec2_z = points[planes[i].v3].z - points[planes[i].v1].z;

		n_x = vec1_y * vec2_z - vec2_y * vec1_z;
		n_y = - (vec1_x * vec2_z - vec1_z * vec2_x);
		n_z = vec1_x * vec2_y - vec1_y * vec2_x;
		n = n_x * n_x + n_y * n_y + n_z * n_z;

		if (fabs(n) > 1e-14)
		{
			planes[i].a = n_x / n;
			planes[i].b = n_y / n;
			planes[i].c = n_z / n;
		}

		
	}

    return 1;
}