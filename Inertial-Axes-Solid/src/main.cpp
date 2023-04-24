#include <iostream>
#include <string>
#include <cmath>

#include "functions.h"
#include "point.h"
using namespace std;

int main (void)
{
	int nv, np, nb;
    
	nv = 8;
	np = 6;
	nb = 12;

	Points* points = new Points[nv];
    if (!points)
    {
        printf(">>>allocation error --> points\n");
        return -1;
    }
    Planes* planes = new Planes[np]; 
    if (!planes)
    {
        printf(">>>allocation error --> planes\n");
        return -1;
    }
    Bones* bones = new Bones[nb];
    if (!bones)
    {
        printf(">>>allocation error --> bones\n");
        return -1;
    }

	get_data (points, planes, bones, nv, np, nb);

	solve (points, planes, bones, nv, np, nb);

	delete [] points;
    delete [] planes;
    delete [] bones;

    return 0;
}
