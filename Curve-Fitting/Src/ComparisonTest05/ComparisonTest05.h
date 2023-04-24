#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <algorithm>
#include "../../Val_0/Net_func/Net_func.h"
#include "../../Val_0/Net_func/Utils.h"
#include "../../Draw/Draw.h"
#include "../Geometry/AllGeometry.h"
#include "../Geometry/Polyhedr.h"
#include "../PolyUtils/PolyUtilsFunctions.h"
#include "../PolyUtils/Structs.h"
#include "../PolyUtils/Transform.h"
#include "../ComparisonTest02/FunctionsCT02.h"
#include "../PolyUtils/PolyUtilsFunctions.h"
#include "../PlaneGeometry/PlaneGeometryFunctions.h"

//void MoveSideCenter(vector <VECTOR3>& CurrSidePts, vector <VECTOR3>& SidePts);
void DrawSideProj(VECTOR3 N, vector <VECTOR3> Pts, int id);
void DrawSideCenters(POLYHEDRON* Poly, int id0, int id1);
void ComparissonTest05(POLYHEDRON* Poly1, POLYHEDRON* Poly2);