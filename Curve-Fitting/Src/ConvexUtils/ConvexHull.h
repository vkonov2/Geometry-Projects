#ifndef _CONVEX_HULL_H_
#define _CONVEX_HULL_H_

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
/*------------------------------------------------------------------------------------------------------

���������� �������� �������� ��������� ����� �� ���������.
������������ �������� � ����������� �� ���� ������������ ������� �����
� ����������� ������� � ��������� �� ������ ���������� ������������
���� ���������������� ����� �������� ��������.

�������� ����� ��������� �������������, ���� ���������� ����� ���� ���� (distEps).
����� ��� ������ ����� �� ���� ����������� ����� �������������� ������ (angleEps).

���������� �������������� ��� ������������ ������ ����� PType, �������� ���� x,y.
������� ������ ������� ��� ���������� � ������ PType �� ��������������.

������� ���������� ��������:

int  CreateConvexHull ( int nPoints, const PType * points, int * hullInds );

nPoints    - ���������� �����, ����������� � ���������� �������� ��������
points     - ������ ����� ���� PType ����� nPoints
hullInds   - ������� �������� ����� ������� points, ������������ �������� ��������
return     - ���������� ����� � �������� ��������

int  CreateConvexHull ( int nPoints, const int * pointInds, const PType * points,  int * hullInds );

nPoints    - ���������� �����, ����������� � ���������� �������� ��������
pointInds  - ������ �������� ����� � ������� points, ����������� � ���������� �������� �������� (����� nPoints)
points     - ������ ����� ���� PType, ���������� � ��� ����� �����, ��������� � ������� pointInds 
hullInds   - ������� �������� ����� ������� points, ������������ �������� ��������
return     - ���������� ����� � �������� ��������


2014-11-03  �.�.�����������
------------------------------------------------------------------------------------------------------*/

// ����������� �������������� ����� � ����������
static const double angleEps = 1.e-12;
static const double distEps = 1.e-10;

// ��������� ��� ������ �� ����� �������� ��������
struct IndAng {
    double angle, dist;
    int ind;
};

// ������� ��������� �����
inline int CmpAngle( double a, double b ) {
    if ( a < b - angleEps ) return -1;  
    if ( a > b + angleEps ) return  1;
    return 0;
}

// ������� ��������� ��� ���������� �����
int CmpIndAng( const void *a, const void *b);
// int CmpIndAng( const void *a, const void *b)
// {
//     IndAng *aa = (IndAng*)a;
//     IndAng *bb = (IndAng*)b;
//     int ret = CmpAngle ( aa->angle, bb->angle ); 
//     if ( ret ) return ret;
//     if ( aa->dist > bb->dist ) return -1;
//     if ( aa->dist < bb->dist ) return  1;
//     return 0;
// }

//
//  ��������� ������� ��� ��������� ��������
//
inline double Distance(double x1, double y1, double x2, double y2 ) {
    double dx = x1 - x2;
    double dy = y1 - y2;
    return sqrt(dx*dx+dy*dy);
}

template <class PType> 
inline double Distance ( const PType &a, const PType &b ) { 
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx*dx+dy*dy);
}



//------------------------------------------------------------------------------------------------------

template <class PType>
void PrintPoints ( const char * fname, int n, IndAng * ia, const PType * points)
{
    FILE *f=fopen(fname, "w");
    for (int i=0; i<n; i++) {
        fprintf(f,"  %lf %lf\n", points[ia[i].ind].x, points[ia[i].ind].y );
    }
    fclose(f);
}




//------------------------------------------------------------------------------------------------------
// ���������� ��������� �������� �������� ��������� �����
//
// nPoints    - ���������� �����, ����������� � ���������� �������� ��������
// points     - ������ ����� ���� PType ����� nPoints
// hullInds   - ������� �������� ����� ������� points, ������������ �������� ��������
// return     - ���������� ����� � �������� ��������
//
template <class PType>
int  CreateConvexHull ( int nPoints, const PType * points, int * hullInds )
{
    int * pointInds  = new int[nPoints];
    for (int i=0; i<nPoints; i++) pointInds[i] = i;
    nPoints = CreateConvexHull ( nPoints, pointInds, points, hullInds );
    delete [] pointInds;
    return nPoints;
}

//------------------------------------------------------------------------------------------------------
// ���������� ��������� �������� �������� ��������� �����
//
// nPoints    - ���������� �����, ����������� � ���������� �������� ��������
// pointInds  - ������ �������� ����� � ������� points, ����������� � ���������� �������� �������� (����� nPoints)
// points     - ������ ����� ���� PType, ���������� � ��� ����� �����, ��������� � ������� pointInds 
// hullInds   - ������� �������� ����� ������� points, ������������ �������� ��������
// return     - ���������� ����� � �������� ��������
//
template <class PType>
int  CreateConvexHull ( int nPoints, const int * pointInds, const PType * points,  int * hullInds )
{
    int i, j;
    int nConvex = 0;
    IndAng * ia = new IndAng [nPoints];  // ������ ��������

    switch ( nPoints ) {
        case 2:  hullInds[1] = pointInds[1];
        case 1:  hullInds[0] = pointInds[0];
        case 0: 
            return nPoints;
    }

    // ����� � ��������� ������� ����� (������� ����� ������)
    PType p0 = points[ pointInds[0] ];
    int   i0 = 0;
    for ( i=0; i<nPoints; i++ ) {
        j = pointInds[i];
        ia[i].ind = j;
        if ( p0.x < points[j].x ) continue;
        if ( p0.x > points[j].x ) {
            p0 = points[j]; i0 = i;         
        } else {
            if ( p0.y > points[j].y ) {
                p0 = points[j]; i0 = i;         
            }
        }
    }
    int itmp = ia[i0].ind;  ia[i0].ind = ia[0].ind; ia[0].ind = itmp;
    ia[0].angle = -3;
    ia[0].dist = 0;
    
    // ���������� ������� ��������
    for ( i=1; i<nPoints; i++ ) {
        j = ia[i].ind;
        ia[i].angle = atan2 ( points[j].y - p0.y, points[j].x - p0.x );
        ia[i].dist = Distance ( points[j], p0 ); 
    }
//PrintPoints ( "D:\\tmp\\p0", nPoints, ia, points );

    // ���������� ����� �� ����
    qsort( ia, nPoints, sizeof(IndAng), CmpIndAng );
//PrintPoints ( "D:\\tmp\\p1", nPoints, ia, points );

    // ������ �� ������������� �����
    for ( j=1, i=1; i<nPoints; i++ ) {
        if ( CmpAngle( ia[i].angle, ia[j-1].angle ) != 0 ) ia[j++] = ia[i];            
    } 
    nPoints = j;
//PrintPoints ( "D:\\tmp\\p2", nPoints, ia, points );

    // ������ ��� �����
    hullInds[0] = ia[0].ind;
    for ( i=1; i<nPoints; i++ ) {
        if ( Distance<PType>( points[ia[i].ind], points[ia[0].ind] ) > distEps ) {
            hullInds[1] = ia[i].ind;
            break;
        }
    }
    if (  i == nPoints ) return 1; 
    nConvex = 2;

    // ������ ��������� �����
    double x1,y1,x2,y2,vp;
    int j1, j2, j3;
    for ( i = nConvex; i<nPoints; i++ ) {
        nConvex++;
        do {
            nConvex--;
            j1 = hullInds[ nConvex-2 ];
            j2 = hullInds[ nConvex-1 ];
            j3 = ia[i].ind;
            x1 = points[j2].x - points[j1].x;
            y1 = points[j2].y - points[j1].y;
            x2 = points[j3].x - points[j2].x;
            y2 = points[j3].y - points[j2].y;
            //vp = VectProd(x1,y1,x2,y2);
            vp = x1 * y2 - x2 * y1;
        } while ( vp <= 0 && nConvex > 2 );
        hullInds[ nConvex++ ] = ia[i].ind;
    } 
    
    delete [] ia;
    return nConvex;
}





#endif  // _CONVEX_HULL_H_
