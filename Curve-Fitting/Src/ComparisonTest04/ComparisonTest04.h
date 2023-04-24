#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include "../../../Val_0/Net_func/Net_func.h"
#include "../../../Val_0/Net_func/Utils.h"
#include "../Draw/Draw.h" 
#include "../Geometry/AllGeometry.h" // included to makefile
#include "../Geometry/Polyhedr.h" // included to makefile
#include "../PolyUtils/PolyUtilsFunctions.h" // included to makefile
#include "../PolyUtils/Structs.h" // included to makefile
#include "../PolyUtils/Transform.h"
#include "../ComparisonTest02/FunctionsCT02.h"
#include "../PlaneGeometry/PlaneGeometryFunctions.h" // included to makefile

void FindPlaneViaRundistSides(POLYHEDRON* Poly, vector<int> Rundist, PLANE& P);
void ComparissonTest04(POLYHEDRON* Poly1, POLYHEDRON* Poly2);