#pragma once

#include "../Geometry/Vector2.h"
#include "../Geometry/Polyhedr.h"

void Get_lines(double * l, int N);
void centrelize(const int n, VECTOR2 * p);
double get_polar_intersection(VECTOR2 p1, VECTOR2 p2, double k);
double get_curr_angle(VECTOR2 p);
void get_func_values ( const int nHull, int * Hull, VECTOR2 * Polygone, double * l, int N, double * f);


class Polar
{
public:
	int N;
	//double a_0, a_last;
	double * f;
	
public:
	Polar();
	//net_func(const int n, double (*func)(double));
	Polar(const int n, const int nPoints, VECTOR2 * Polygone, const int nHullPoints, int * HullInd);
	// net_func(const net_func & f, const net_func & g);
	// Polar(const Polar & h);

	double L2Diff(Polar & f);

	~Polar();


};