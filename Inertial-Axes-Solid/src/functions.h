#ifndef FUNCTIONS_H 
#define FUNCTIONS_H

#include "point.h"

struct Points
{
	int v;
    double x, y, z;
    int b1, b2, b3;
};

struct Planes
{
	int p;
    int v1, v2, v3, v4;
    double x, y, z, s;
    double a, b, c, d;
};

struct Bones
{
    int b;
    int p1, p2;
    int v1, v2;
};

int solve (Points* points, Planes* planes, Bones* bones, int nv, int np, int nb);

int get_data (Points * points, Planes * planes, Bones * bones, int nv, int np, int nb);

int area (Points * points, Planes * planes, Bones * bones, int nv, int np, int nb);

int eigen (int n, int m, double eps, double *A);

int yakobi (double *A, int n, int i, int j);

int check (Points* points, Planes* planes, Bones* bones, int nv, int np, int nb);

double time_now(void);

#endif
