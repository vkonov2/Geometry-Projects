
#ifndef DRAW_CURVE_H_
#define DRAW_CURVE_H_

#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <random>

#include "../Geometry/Vector2.h"

#include "../utils.h"

#include "../CurveGenerator/CurveGenerator.h"

#define _USE_MATH_DEFINES

using namespace std;



// DRAW SECTION

void DrawEllipseArc(double Xc, double Yc, double A, double B, double Angle, double beginAngle, double endAngle, string& FILENAME, string& PATH);



// PLOT SECTION

void PlotOriginEllipseData(double Xc, double Yc, double A, double B, string& GNUPLOTPATH);

void PlotAllPossibleGeneratedData(const int PointsCount, void* P);

void PlotOriginEllipseDatawDistrib(void* P);



#endif // DRAW_CURVE_H_