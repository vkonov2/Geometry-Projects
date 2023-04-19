#ifndef _LEASTSQRT_H_
#define _LEASTSQRT_H_


double  TestPlane ( double *plane, int n, double *points, double *weights=0 );
double  ApproxByPlane0 ( double *plane, int n, double *points, double *weights=0 );
double  NearestPointToPlanes ( double *point, double *baseplane, int n, double *planes, double *weights=0 );
double  NearestPointToPlanes2 ( double *point, double *baseplane, int n, double *planes, double *weights=0 );
void TestNearestPointToPlanes2 ();


// аппроксимаци€ по отрезкам (интегральна€)
double ApproxByPlane2 ( double *plane, int n, double *points, double *weights=0 );
double ApproxError ( double *plane, int n, double *points, double *weights=0 );



double ErrAproxByLine ( double * line, int n, double *points, double *weights=0 );

/*--------------------------------------------------------------------

  ѕостроение пр€мой наилучшего среднеквадратичного приближени€ 
  дл€ заданного множества точек двумерного пространства.

plane   - искома€ плоскость  
          X*plane[0] + Y*plane[1] + plane[2] = 0
n       - количество аппроксимируемых точек
points  - массив координат точек (пары (x,y), длина массива 2*n)
weights - массив весовых коэффициентов точек, дл€ weights=0 все веса 1

return  - достигнута€ погрешность аппроксимации

--------------------------------------------------------------------*/

double ApproxByLine ( double * line, int n, double *points, double *weights =0  );




/*--------------------------------------------------------------------

  ѕостроение пр€мой наилучшего среднеквадратичного приближени€ 
  дл€ заданного множества точек трехмерного пространства.

line    - искома€ пр€ма€, задаетс€ в виде точки (x,y,x) - line[0],line[1],line[2]
          и единичного вектора направлени€ (dx,dy,dz) - line[3],line[4],line[5]
n       - количество аппроксимируемых точек
points  - массив координат точек (тройки (x,y,z), длина массива 3*n)
weights - массив весовых коэффициентов точек, дл€ weights=0 все веса 1

return  - достигнута€ погрешность аппроксимации

--------------------------------------------------------------------*/
double ApproxByLine3D ( double * line, int n, double *points, double *weights=0 );
int TestApproxByLine3D ();

#endif // _LEASTSQRT_H_

