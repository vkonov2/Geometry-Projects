#pragma once

#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include <map>
#include "../Geometry/Polyhedr.h"
#include "../Geometry/AllGeometry.h"
#include "../PolyUtils/Structs.h"
#include "../PolyUtils/Transform.h"
#include "../../Draw/Draw.h"
#include <stdlib.h>
#include "../Polar/Polar.h"
#include <experimental/filesystem>
#include "../PolyUtils/PolyUtilsFunctions.h"
#include "../../Val_0/Net_func/Net_func.h"
#include "../../Val_0/Net_func/Utils.h"
#include "../PolyUtils/Transform.h"
#include "../PlaneGeometry/PlaneGeometryFunctions.h"

void ComparissonTest03(POLYHEDRON* PatternPoly, POLYHEDRON* RealPoly);