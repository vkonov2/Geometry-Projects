
#ifndef DRAW_POLYHEDRON_H_
#define DRAW_POLYHEDRON_H_

#include <iostream>
#include "../Geometry/Polyhedr.h"
#include "../Geometry/Similart.h"
#include "../PolyUtils/Transform.h"
#include "../PolyUtils/Axes.h"
//#include "../Clusterization/Clusterization.h"

using namespace std;

void PointsGenerator (VECTOR* pts, int N, PLANE plane, const char* Name);
void Draw(POLYHEDRON* P, const char* dir, const char* filename);
void DrawSelected (POLYHEDRON* Real_Poly, int N, vector<int>& Sides, const char* dir, const char* prefix);
void DrawSelected2 (POLYHEDRON* Real_Poly, int N, vector<int>& Sides, const char* dir, const char* prefix);
void DrawAxes(VECTOR* ax, const char* fullpath, VECTOR mass_center);
void DrawAxPolyTogether(POLYHEDRON* Poly, const char* dir, const char* prefix, const char* DrawAxYesNo);
void DrawPointedSide(POLYHEDRON* Poly, int SideId, const char* dir);
void DrawUnionOfChains(vector <int> Ids, POLYHEDRON* Poly, const string Name, int Id);
void DrawApproximationPlaneOfChain(std::vector<int> Ids, const POLYHEDRON* Poly, const char* Name);
void DrawNormPlaneOfChain(std::vector<int> Ids, const POLYHEDRON* Poly, const char* Name, const PLANE Plane);
void DrawChain(POLYHEDRON* Poly, vector<int> Chain, int Id);
void DrawNormApproximationPlaneOfChain(std::vector<int> Ids, POLYHEDRON* Poly, const char* Name);
void DrawNormApproximationPlaneOfChainWithNull(std::vector<int> Ids, POLYHEDRON* Poly, const char* Name);

#endif // DRAW_POLYHEDRON_H_