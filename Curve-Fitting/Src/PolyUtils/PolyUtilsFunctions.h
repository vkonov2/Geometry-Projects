#pragma once
#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>
#include "Structs.h"
#include "../../../Val_0/Net_func/Utils.h"
#include "../../../Val_0/Net_func/Net_func.h"
#include "../../../Val_0/Net_func/FFTSource.h"
#include "../ComparisonTest02/FunctionsCT02.h"
#include "../PolyUtils/Transform.h"
#include "../Geometry/Vector.h"
#include "../Geometry/_defs.h"
#include "../Geometry/Plane.h"
#include "../Geometry/AllGeometry.h"
#include "../PlaneGeometry/PlaneGeometryFunctions.h"
#include "../Geometry/Polyhedr.h"
#include "../Geometry/Similart.h"
#include "../Geometry/Vector2.h"

using namespace std;

/*------------------------------------------------------------------------------------------------------
void Area(const POLYHEDRON* Poly, int side_index, double& x_sum, double& y_sum, double& z_sum, double& area_sum, PlaneInfo* Info);
	������� ������� ������� ����� � ������� side_index

	Poly - ������������
	side_index - ����� �����
	x_sum = sum_{i = 0}^{n - 1} vertex[i].x
	y_sum = sum_{i = 0}^{n - 1} vertex[i].y
	z_sum = sum_{i = 0}^{n - 1} vertex[i].z 
	area_sum - ����� �������� ���� ������
	Info - ������ ��������, i-�� ������� �������� �������� ���������� �� i-�� ����� (��. Structs.h)

void Volume(const POLYHEDRON* Poly, int side_index, double& volume, PlaneInfo* Info, const VECTOR mass_center);
	������� ������� �������� ����� ����� � ������� side_index

	Poly - ������������
	side_index - ����� �����
    volume - ����� ������� ��� ���� ������
	mass_center - ����� ���� �������������, ����������� �� �������
	Info - ������ ��������, i-�� ������� �������� �������� ����: [������ ���� ��������� ��� �������� � ������]

void FindSolidMassCenter(POLYHEDRON* P, PlaneInfo* Info, VECTOR& mass_center);
	������� ������� ����� ���� �������������, ����������� �� �������� �������

	P - ������������
	Info - ������ ��������, i-�� ������� �������� �������� ����: [������ ���� ��������� ��� �������� � ������]
	mass_center - ����� ���� �������������, ����������� �� �������� �������

int OrtReper(VECTOR l, VECTOR& e_1, VECTOR& e_2, VECTOR& e_3);
	������� ��������� ������ l �� ����������������� ������ {e_1, e_2, e_3} 

int FindOrientation(const VECTOR a, const VECTOR b, const VECTOR c);
	������� ������� ���������� ������ (a, b, c): 1, ���� ������, -1, ���� �����

int ConvexHullOfProjection(const POLYHEDRON* Poly, const VECTOR n, int* hullInds, VECTOR2* projection);
	������� ������� �������� �������� �������� ��������� ������ ������������� �� ���������,
	������� � ������� ���� n

	Poly - ������������
	n - ������� � ���������, �� ������� ������������ ������������
	hullInds - ������� �������� ������ �������������, ������������ �������� ��������
	projection - ��������
------------------------------------------------------------------------------------------------------*/

void AddOnReper(VECTOR& e1, VECTOR& e2, const VECTOR e3);
double RetMatchingError(POLYHEDRON* Pattern, POLYHEDRON* Real, VECTOR Norm);
void FindCrossSection(const POLYHEDRON* Poly, PLANE P, vector <VECTOR3>& CrossSection);
void RightOrientation(POLYHEDRON* Poly, VECTOR V[3]);
void MatchPolyMainSymAx(POLYHEDRON* Pattern, POLYHEDRON* Real, vector<int>& RealRundist, vector<int>& PatternRundist);
void FindSubPlaneInfo(PlaneInfo* Info, POLYHEDRON* Poly, int SideId);
void Area(const POLYHEDRON* Poly, int side_index, double& x_sum, double& y_sum, double& z_sum, double& area_sum, PlaneInfo* Info);
void Volume(const POLYHEDRON* Poly, int side_index, double& volume, PlaneInfo* Info, const VECTOR mass_center);
void FindSolidMassCenter(POLYHEDRON* P, PlaneInfo* Info, VECTOR& mass_center);
void FindCentroid(POLYHEDRON* Poly, PlaneInfo* Info, VECTOR& mass_center);

int OrtReper(VECTOR l, VECTOR& e_1, VECTOR& e_2, VECTOR& e_3);
int FindOrientation(const VECTOR a, const VECTOR b, const VECTOR c);

void FindProjection(const VECTOR n, const vector < VECTOR > Pts, vector < VECTOR2 >& projection);
void FindProjection2(const VECTOR n, const vector < VECTOR > Pts, vector < VECTOR2 >& projection, const VECTOR e2);
int ConvexHullOfProjection(const POLYHEDRON* Poly, const VECTOR n, int* hullInds, VECTOR2* projection);
void FindProjectionSinglePoint(const VECTOR n, VECTOR pt, VECTOR2& projection, const VECTOR e2);



// /*============================ BEGIN ============================
// Brief:
// 	Stereographic projection of points in space               	 */

// void MovePole(vector<VECTOR>& Pts);
// void StereographicProjection(const vector<VECTOR> Pts, vector<VECTOR2>& StereoProj);
// /*============================  END  ============================*/