#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include "../Geometry/Polyhedr.h"
#include "../Geometry/AllGeometry.h"
#include "../Geometry/Vector.h"
#include "../PolyUtils/Structs.h"
#include "../PolyUtils/PolyUtilsFunctions.h"

using namespace std;

double FindBound(int n, double* DistMin);
int HierarchicalClustering(int n, double bound, vector<vector<double> >& DistMatr, vector<int>& Rundist);
int RecognizeRundist(POLYHEDRON* Poly, std::vector<int> &Rundist);