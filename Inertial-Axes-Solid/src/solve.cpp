#include <iostream>
#include <string>
#include <cmath>

#include "functions.h"
#include "point.h"
using namespace std;

int solve (Points* points, Planes* planes, Bones* bones, int nv, int np, int nb)
{
	area (points, planes, bones, nv, np, nb);

    return 1;
}
