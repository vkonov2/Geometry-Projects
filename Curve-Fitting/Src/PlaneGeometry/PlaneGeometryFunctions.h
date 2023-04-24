#pragma once

#include <iostream>
#include <vector>
#include "../Geometry/Similart.h"
#include "../Geometry/AllGeometry.h"
#include "../Geometry/Vector.h"
#include "../Geometry/Vector2.h"
#include "../Geometry/_defs.h"
#include "../Geometry/Plane.h"
#include "../Geometry/Polyhedr.h"
#include "../Geometry/Vector.h"
#include "../../../Val_0/Net_func/Net_func.h"
#include "../../../Val_0/Net_func/FFTSource.h"
#include "../../../Val_0/Net_func/Utils.h"
#include "../PolyUtils/PolyUtilsFunctions.h"
#include "../ConvexUtils/ConvexHull.h"

using namespace std;
/*------------------------------------------------------------------------------------------------------
double PlaneVectorCos(const PHSIDE side, const VECTOR n);
	функция находит косинус угла между плоскостью и вектором

	side - плоскость
	n - вектор

double Plane3DArea(const PHSIDE side);
	функция находит площадь многоугольника в пространстве

void DefinePlaneCoeff(PHSIDE* side, POINT3* vertex);
	функция находит уравнение плоскости для точек vertex in side 
------------------------------------------------------------------------------------------------------*/

void MoveSideCenter(vector <VECTOR2>& CurrSidePts, vector <VECTOR2>& SidePts);
double AngleMatchPolygons(vector <VECTOR3> PatternCrossSection, vector <VECTOR3> RealCrossSection, VECTOR Norm, vector <double>& Err);
double PlaneVectorCos(const PHSIDE side, const VECTOR n);
double Plane3DArea(const PHSIDE side, POINT3* vertex);
//void DefinePlaneCoeff(POLYHEDRON* Poly, int side_index);
void DefinePlaneCoeff(int N, VECTOR* pts, PLANE& Res);