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
	функция находит площадь грани с номером side_index

	Poly - многогранник
	side_index - номер грани
	x_sum = sum_{i = 0}^{n - 1} vertex[i].x
	y_sum = sum_{i = 0}^{n - 1} vertex[i].y
	z_sum = sum_{i = 0}^{n - 1} vertex[i].z 
	area_sum - сумма площадей всех граней
	Info - массив структур, i-ый элемент которого содержит информацию об i-ой грани (см. Structs.h)

void Volume(const POLYHEDRON* Poly, int side_index, double& volume, PlaneInfo* Info, const VECTOR mass_center);
	функция находит телесный объем грани с номером side_index

	Poly - многогранник
	side_index - номер грани
    volume - сумма объемов для всех граней
	mass_center - центр масс многогранника, посчитанный по пощадям
	Info - массив структур, i-ый элемент которого содержит поля: [обязан быть определен при передаче в фунцию]

void FindSolidMassCenter(POLYHEDRON* P, PlaneInfo* Info, VECTOR& mass_center);
	функция находит центр масс многогранника, посчитанный по телесным объемам

	P - многогранник
	Info - массив структур, i-ый элемент которого содержит поля: [обязан быть определен при передаче в фунцию]
	mass_center - центр масс многогранника, посчитанный по телесным объемам

int OrtReper(VECTOR l, VECTOR& e_1, VECTOR& e_2, VECTOR& e_3);
	функция дополняет вектор l до ортонормированной тройки {e_1, e_2, e_3} 

int FindOrientation(const VECTOR a, const VECTOR b, const VECTOR c);
	функция находит ориентацию тройки (a, b, c): 1, если правая, -1, если левая

int ConvexHullOfProjection(const POLYHEDRON* Poly, const VECTOR n, int* hullInds, VECTOR2* projection);
	Функция находит выпуклую оболочку проекции множества вершин многогранника на плоскость,
	нормаль к которой есть n

	Poly - многогранник
	n - нормаль к плоскости, на которую проецируется многогранник
	hullInds - спискок индексов вершин многогранника, составляющих выпуклую оболочку
	projection - проекция
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