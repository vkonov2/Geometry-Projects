#ifndef ELLIPSE_FUNCTIONS_H_
#define ELLIPSE_FUNCTIONS_H_

#define __STDCPP_WANT_MATH_SPEC_FUNCS__ 1

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/QR>

#include <gsl/gsl_integration.h>
#include <gsl/gsl_sf_ellint.h>

#include "../../Src/Eigen/EigenFunctions.h"

#include "../../Src/utils.h"

#include "../../Src/CurveGenerator/CurveGenerator.h"

using namespace std;

using Eigen::VectorXd;
using Eigen::MatrixXd;
using Eigen::VectorXcd;
using Eigen::SelfAdjointEigenSolver;

// Coeffs2MAF converts Coeffs(6) to MatrixAngleForm(X0,Y0,A,B,Angle)
// ||
// VV
void Coeffs2MAF (const vector<double>& COEFFS, VectorXd& MAF);

// MAF2Coeffs converts MatrixAngleForm(X0,Y0,A,B,Angle) to Coeffs(6) 
// ||
// VV
void MAF2Coeffs (const VectorXd& MAF, vector<double>& COEFFS);

double CalcEllipseArcLenght (double beginAngle, double endAngle, void* P, int flag);

double fArc (double PHI, void* P);

double CalcEllipsePerimeter (double A, double B, double beginAngle, double endAngle, int flag);

#endif // ELLIPSE_FUNCTIONS_H_
