
#ifndef CURVE_GENERATOR_H_
#define CURVE_GENERATOR_H_

#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <fstream>
#include <cmath>

#include <gsl/gsl_sf_ellint.h>

#include "../Geometry/Vector2.h"

#include "../utils.h"

using namespace std;

struct EllipseParams
{
	double Xc;
	double Yc;
	double A;
	double B;
	double Angle;
	double beginAngle;
	double endAngle;
	int DistribCase;
	int FlagDistribAngle;
	int FlagDistribRadius;
	double dstrAngleP1;
	double dstrAngleP2;
	double dstrRadiusP1;
	double dstrRadiusP2;
	string& PATH;
};

struct rhoParams
{
	double Xc;
	double Yc;
	double A;
	double B;
	double Angle;
};

double RHO (double PHI, void* P);

void EllipseGenerator (vector<VECTOR2>& POINTS, const int PointsCount, void* P);

#endif // CURVE_GENERATOR_H_










