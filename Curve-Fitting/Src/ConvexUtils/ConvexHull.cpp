//#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "ConvexHull.h"



int CmpIndAng( const void *a, const void *b)
{
    IndAng *aa = (IndAng*)a;
    IndAng *bb = (IndAng*)b;
    int ret = CmpAngle ( aa->angle, bb->angle ); 
    if ( ret ) return ret;
    if ( aa->dist > bb->dist ) return -1;
    if ( aa->dist < bb->dist ) return  1;
    return 0;
}

