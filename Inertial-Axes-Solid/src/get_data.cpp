#include <iostream>
#include <string>
#include <cmath>

#include "functions.h"
#include "point.h"
using namespace std;

int get_data (Points* points, Planes* planes, Bones* bones, int nv, int np, int nb)
{
	FILE* f;
    int i, j;

    points[0].v = 1;
    points[0].x = 0;
    points[0].y = 0;
    points[0].z = 0;
    points[0].b1 = 1;
    points[0].b2 = 4;
    points[0].b3 = 6;

    points[1].v = 2;
    points[1].x = 1;
    points[1].y = 0;
    points[1].z = 0;
    points[1].b1 = 1;
    points[1].b2 = 2;
    points[1].b3 = 5;

    points[2].v = 3;
    points[2].x = 1;
    points[2].y = 1;
    points[2].z = 0;
    points[2].b1 = 2;
    points[2].b2 = 3;
    points[2].b3 = 8;

    points[3].v = 4;
    points[3].x = 0;
    points[3].y = 1;
    points[3].z = 0;
    points[3].b1 = 3;
    points[3].b2 = 4;
    points[3].b3 = 7;

    points[4].v = 5;
    points[4].x = 0;
    points[4].y = 0;
    points[4].z = 1;
    points[4].b1 = 6;
    points[4].b2 = 9;
    points[4].b3 = 12;

    points[5].v = 6;
    points[5].x = 1;
    points[5].y = 0;
    points[5].z = 1;
    points[5].b1 = 5;
    points[5].b2 = 9;
    points[5].b3 = 10;

    points[6].v = 7;
    points[6].x = 1;
    points[6].y = 1;
    points[6].z = 1;
    points[6].b1 = 8;
    points[6].b2 = 10;
    points[6].b3 = 11;

    points[7].v = 8;
    points[7].x = 0;
    points[7].y = 1;
    points[7].z = 1;
    points[7].b1 = 7;
    points[7].b2 = 11;
    points[7].b3 = 12;



    planes[0].p = 1;
    planes[0].v1 = 1;
    planes[0].v2 = 2;
    planes[0].v3 = 3;
    planes[0].v4 = 4;

    planes[1].p = 2;
    planes[1].v1 = 1;
    planes[1].v2 = 2;
    planes[1].v3 = 5;
    planes[1].v4 = 6;

    planes[2].p = 3;
    planes[2].v1 = 1;
    planes[2].v2 = 4;
    planes[2].v3 = 5;
    planes[2].v4 = 8;

    planes[3].p = 4;
    planes[3].v1 = 3;
    planes[3].v2 = 4;
    planes[3].v3 = 7;
    planes[3].v4 = 8;

    planes[4].p = 5;
    planes[4].v1 = 2;
    planes[4].v2 = 3;
    planes[4].v3 = 6;
    planes[4].v4 = 7;

    planes[5].p = 6;
    planes[5].v1 = 5;
    planes[5].v2 = 6;
    planes[5].v3 = 7;
    planes[5].v4 = 8;



    bones[0].b = 1;
    bones[0].p1 = 1;
    bones[0].p2 = 2;
    bones[0].v1 = 1;
    bones[0].v2 = 2;

    bones[1].b = 2;
    bones[1].p1 = 1;
    bones[1].p2 = 5;
    bones[1].v1 = 2;
    bones[1].v2 = 3;

    bones[2].b = 3;
    bones[2].p1 = 1;
    bones[2].p2 = 4;
    bones[2].v1 = 3;
    bones[2].v2 = 4;

    bones[3].b = 4;
    bones[3].p1 = 1;
    bones[3].p2 = 3;
    bones[3].v1 = 1;
    bones[3].v2 = 4;

    bones[4].b = 5;
    bones[4].p1 = 2;
    bones[4].p2 = 5;
    bones[4].v1 = 2;
    bones[4].v2 = 6;

    bones[5].b = 6;
    bones[5].p1 = 2;
    bones[5].p2 = 3;
    bones[5].v1 = 1;
    bones[5].v2 = 5;

    bones[6].b = 7;
    bones[6].p1 = 3;
    bones[6].p2 = 4;
    bones[6].v1 = 4;
    bones[6].v2 = 8;

    bones[7].b = 8;
    bones[7].p1 = 4;
    bones[7].p2 = 5;
    bones[7].v1 = 3;
    bones[7].v2 = 7;

    bones[8].b = 9;
    bones[8].p1 = 2;
    bones[8].p2 = 6;
    bones[8].v1 = 5;
    bones[8].v2 = 6;

    bones[9].b = 10;
    bones[9].p1 = 5;
    bones[9].p2 = 6;
    bones[9].v1 = 6;
    bones[9].v2 = 7;

    bones[10].b = 11;
    bones[10].p1 = 4;
    bones[10].p2 = 6;
    bones[10].v1 = 7;
    bones[10].v2 = 8;

    bones[11].b = 12;
    bones[11].p1 = 3;
    bones[11].p2 = 6;
    bones[11].v1 = 5;
    bones[11].v2 = 8;

    f = fopen("input.txt", "w");
    if (!f) 
    {
        printf (">>>can't open file -> points.txt\n");
        return -1;
    }

    fprintf(f, "%d %d %d\n", nv, np, nb);

    fprintf(f, "\n");

    for(i = 0; i < nv; i++)
        fprintf(f, "%d %lf %lf %lf %d %d %d\n", points[i].v, points[i].x, points[i].y, points[i].z, points[i].b1, points[i].b2, points[i].b3);

    fprintf(f, "\n");

    for(i = 0; i < np; i++)
        fprintf(f, "%d %d %d %d %d\n", planes[i].p, planes[i].v1, planes[i].v2, planes[i].v3, planes[i].v4);

    fprintf(f, "\n");

    for(i = 0; i < nb; i++)
        fprintf(f, "%d %d %d %d %d\n", bones[i].b, bones[i].p1, bones[i].p2, bones[i].v1, bones[i].v2);

    fclose(f);

    return 1;
}
