#include "stdafx.h"

#include <math.h>
#include <stdio.h>
#include "Jacobi.h"
#include "linsyst.h"
#include "../Gauss.h"
#include "../WException.h"

#include "leastsqr.h"

#include "../WLogger.h"
extern WLogger logger;
extern WLogger corrEdgeLogger;

inline void Normalize ( double *a )
{ double d=sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]); a[0]/=d; a[1]/=d; a[2]/=d; }

inline void Copy4 (double *a, double *b )
{ a[0]=b[0]; a[1]=b[1]; a[2]=b[2]; a[3]=b[3]; }

inline void swap(double &a, double &b ) {double c=a; a=b; b=c; }



/*--------------------------------------------------------------------

  Тестирование аппроксимирующей плоскости  
  для заданного множества точек трехмерного пространства.

plane   - искомая плоскость с единичным вектором нормали  
          X*plane[0] + Y*plane[1] + Z*plane[2] + plane[3] = 0
n       - количество аппроксимируемых точек
points  - массив координат точек (тройки (x,y,z)), длина массива 3*n
weights - массив весовых коэффициентов точек, для weights=0 все веса 1

return  - достигнутая погрешность аппроксимации

--------------------------------------------------------------------*/
double  TestPlane ( double *plane, int n, double *points, double *weights )
{
    double e0=0, e;
    double a=plane[0],b=plane[1],c=plane[2],d=plane[3];
    if ( weights ) {
        for (int i=0; i<n; i++) {
            e = a*points[3*i] + b*points[3*i+1] + c*points[3*i+2] + d;
//fprintf(logger.GetLog(),"%d %le\n",i,e);
            e *= e;
            e *= weights[i];
            e0 += e;
        }
    } else {
        for (int i=0; i<n; i++) {
            e = a*points[3*i] + b*points[3*i+1] + c*points[3*i+2] + d;
            e *= e;
            e0 += e;
        }
    }
    e0 /= n;
    e0 = sqrt(e0);
//fprintf(logger.GetLog(),"  err %le\n",e0);
    return e0;
}




/*--------------------------------------------------------------------

  Построение плоскости наилучшего среднеквадратичного приближения 
  для заданного множества точек трехмерного пространства.

plane   - искомая плоскость  
          X*plane[0] + Y*plane[1] + Z*plane[2] + plane[3] = 0
n       - количество аппроксимируемых точек
points  - массив координат точек (тройки (x,y,z)), длина массива 3*n
weights - массив весовых коэффициентов точек, для weights=0 все веса 1

return  - достигнутая погрешность аппроксимации

--------------------------------------------------------------------*/

double  ApproxByPlane0 ( double *plane, int n, double *points, double *weights )
{
    
    // формируем систему
    double a11=0,a12=0,a13=0,a14=0;
    double       a22=0,a23=0,a24=0;
    double             a33=0,a34=0;
    double                   a44=n;
    double  x,y,z,w;    
    if ( weights ) {
        a44 = 0;
        for (int i=0; i<n; i++ ) {
            x = points[3*i];
            y = points[3*i+1];
            z = points[3*i+2];
            w = weights[i];
            a11 += w*x*x; 
            a12 += w*x*y;
            a13 += w*x*z;
            a22 += w*y*y;
            a23 += w*y*z;
            a33 += w*z*z;
            a14 += w*x;
            a24 += w*y; 
            a34 += w*z;
            a44 += w;
        }
    } else {
        for (int i=0; i<n; i++ ) {
            x = points[3*i];
            y = points[3*i+1];
            z = points[3*i+2];
            a11 += x*x; 
            a12 += x*y;
            a13 += x*z;
            a22 += y*y;
            a23 += y*z;
            a33 += z*z;
            a14 += x;
            a24 += y; 
            a34 += z;
        }
    }

//printf("%10.5lf %10.5lf %10.5lf\n",a11,a12,a13);
//printf("%10.5lf %10.5lf %10.5lf\n",a22,a23,a33);
//printf("%10.5lf %10.5lf %10.5lf %10.5lf\n",a14,a24,a34,a44);


    // формируем задачу на собственные значения
    double *matr[3], lamb[3], *vect[3];
    double m[9], v[9]; 
    m[0] = a11 - a14*a14/a44; m[1] = a12 - a14*a24/a44; m[2] = a13 - a14*a34/a44;
    m[3] = m[1];              m[4] = a22 - a24*a24/a44; m[5] = a23 - a24*a34/a44;
    m[6] = m[2];              m[7] = m[5];              m[8] = a33 - a34*a34/a44;
    matr[0] = m;  matr[1] = m+3;  matr[2] = m+6;
    vect[0] = v;  vect[1] = v+3;  vect[2] = v+6;

//printf("%10.5lf %10.5lf %10.5lf\n",m[0],m[1],m[2]);
//printf("%10.5lf %10.5lf %10.5lf\n",m[3],m[4],m[5]);
//printf("%10.5lf %10.5lf %10.5lf\n",m[6],m[7],m[8]);

    // решаем задачу на собственные значения
    Jacobi( 3, matr, lamb, vect );

    // создаем возможные плоскости из собственных векторов
    double p1[4]={v[0],v[3],v[6],0};     
    double p2[4]={v[1],v[4],v[7],0};     
    double p3[4]={v[2],v[5],v[8],0};     
    Normalize(p1);
    Normalize(p2);
    Normalize(p3);
    p1[3] = - ( p1[0]*a14 + p1[1]*a24 + p1[2]*a34 )/a44;
    p2[3] = - ( p2[0]*a14 + p2[1]*a24 + p2[2]*a34 )/a44;
    p3[3] = - ( p3[0]*a14 + p3[1]*a24 + p3[2]*a34 )/a44;

    // находим минимальное решение
    double e0,e1,e2,e3;
    e1 = TestPlane( p1, n, points, weights ); 
    e2 = TestPlane( p2, n, points, weights ); 
    e3 = TestPlane( p3, n, points, weights ); 
        
//printf("e %le %le %le\n",e1,e2,e3);
  
    if ( e1<=e2 ) {
        if ( e1<=e3 ) { Copy4( plane, p1 ); e0 = e1; }
        else          { Copy4( plane, p3 ); e0 = e3; }
    } else {
        if ( e2<=e3 ) { Copy4( plane, p2 ); e0 = e2; }
        else          { Copy4( plane, p3 ); e0 = e3; }
    }
    return e0;
}

//-------------------------------------------------------------------------------
/*--------------------------------------------------------------------

  Построение плоскости наилучшего среднеквадратичного приближения 
  для заданного множества отрезков трехмерного пространства.

plane   - искомая плоскость  
          X*plane[0] + Y*plane[1] + Z*plane[2] + plane[3] = 0
nedges  - количество аппроксимируемых отрезков
points  - массив координат концов отрезков точек (по две тройки (x,y,z)), длина массива 6*edges
weights - массив весовых коэффициентов отрезков, для weights=0 все веса 1

return  - достигнутая погрешность аппроксимации

--------------------------------------------------------------------*/

double  ApproxByPlane2 ( double *plane, int nedges, double *points, double *weights )
{
    
    // формируем систему
    double a11=0,a12=0,a13=0,a14=0;
    double       a22=0,a23=0,a24=0;
    double             a33=0,a34=0;
    double                   a44=0;
    double  x1,x2,y1,y2,z1,z2,w;    
    double *wt;
    if ( weights == 0) {
        wt = new double [nedges];
        for (int i=0; i<nedges; i++) wt[i] = 1;
    } else {
        wt = weights;
    }
    for (int i=0; i<nedges; i++ ) {
        x1 = points[6*i];
        y1 = points[6*i+1];
        z1 = points[6*i+2];
        x2 = points[6*i+3];
        y2 = points[6*i+4];
        z2 = points[6*i+5];
        w = wt[i];
        
        a11 += ( 2*x1*x1 + 2*x2*x2 + x1*x2 + x2*x1 )*w; 
        a12 += ( 2*y1*x1 + 2*y2*x2 + y1*x2 + y2*x1 )*w;
        a13 += ( 2*z1*x1 + 2*z2*x2 + z1*x2 + z2*x1 )*w;
        a22 += ( 2*y1*y1 + 2*y2*y2 + y1*y2 + y2*y1 )*w;
        a23 += ( 2*z1*y1 + 2*z2*y2 + z1*y2 + z2*y1 )*w;
        a33 += ( 2*z1*z1 + 2*z2*z2 + z1*z2 + z2*z1 )*w;
        a14 += ( 3*x1 + 3*x2 )*w;
        a24 += ( 3*y1 + 3*y2 )*w; 
        a34 += ( 3*z1 + 3*z2 )*w;
        a44 += 6*w;
    
        //logger.Print(LM_DEBUG," %12.8lf %12.8lf %12.8lf\n",x1,y1,z1);   
        //logger.Print(LM_DEBUG," %12.8lf %12.8lf %12.8lf\n     \n      \n",x2,y2,z2);   
    
    }



    // формируем задачу на собственные значения
    double *matr[3], lamb[3], *vect[3];
    double m[9], v[9]; 
    m[0] = a11 - a14*a14/a44; m[1] = a12 - a14*a24/a44; m[2] = a13 - a14*a34/a44;
    m[3] = m[1];              m[4] = a22 - a24*a24/a44; m[5] = a23 - a24*a34/a44;
    m[6] = m[2];              m[7] = m[5];              m[8] = a33 - a34*a34/a44;
    matr[0] = m;  matr[1] = m+3;  matr[2] = m+6;
    vect[0] = v;  vect[1] = v+3;  vect[2] = v+6;

    // решаем задачу на собственные значения
    Jacobi( 3, matr, lamb, vect );

    // создаем возможные плоскости из собственных векторов
    double p1[4]={v[0],v[3],v[6],0};     
    double p2[4]={v[1],v[4],v[7],0};     
    double p3[4]={v[2],v[5],v[8],0};     
    Normalize(p1);
    Normalize(p2);
    Normalize(p3);
    p1[3] = - ( p1[0]*a14 + p1[1]*a24 + p1[2]*a34 )/a44;
    p2[3] = - ( p2[0]*a14 + p2[1]*a24 + p2[2]*a34 )/a44;
    p3[3] = - ( p3[0]*a14 + p3[1]*a24 + p3[2]*a34 )/a44;

    // находим минимальное решение
    double e0,e1,e2,e3;
    e1 = ApproxError ( p1, nedges, points, weights ); 
    e2 = ApproxError ( p2, nedges, points, weights ); 
    e3 = ApproxError ( p3, nedges, points, weights ); 
        
    if ( e1<=e2 ) {
        if ( e1<=e3 ) { Copy4( plane, p1 ); e0 = e1; }
        else          { Copy4( plane, p3 ); e0 = e3; }
    } else {
        if ( e2<=e3 ) { Copy4( plane, p2 ); e0 = e2; }
        else          { Copy4( plane, p3 ); e0 = e3; }
    }

    if ( weights == 0) delete [] wt;
    return e0;
}


/*--------------------------------------------------------------------

  Вычисляет погрешность асспроксимации наилучшего среднеквадратичного приближения 
  для заданного множества отрезков трехмерного пространства.

plane   - тестируемая плоскость  
          X*plane[0] + Y*plane[1] + Z*plane[2] + plane[3] = 0
nedges  - количество аппроксимируемых отрезков
points  - массив координат концов отрезков точек (по две тройки (x,y,z)), длина массива 6*edges
weights - массив весовых коэффициентов отрезков, для weights=0 все веса 1

return  - достигнутая погрешность аппроксимации

--------------------------------------------------------------------*/

double ApproxError ( double *plane, int nedges, double *points, double *weights )
{
    double e0, d1, d2;
    double *wt;
    double x1,x2,y1,y2,z1,z2,w;

    if ( weights == 0) {
        wt = new double [nedges];
        for (int i=0; i<nedges; i++) wt[i] = 1;
    } else {
        wt = weights;
    }
    e0 = 0;
 //   logger.Print(LM_DEBUG,"      ApproxError\n");
    for (int i=0; i<nedges; i++ ) {
        x1 = points[6*i];
        y1 = points[6*i+1];
        z1 = points[6*i+2];
        x2 = points[6*i+3];
        y2 = points[6*i+4];
        z2 = points[6*i+5];
        w = wt[i];
        d1 = plane[0]*x1 + plane[1]*y1 + plane[2]*z1 + plane[3];
        d2 = plane[0]*x2 + plane[1]*y2 + plane[2]*z2 + plane[3];
// logger.Print(LM_DEBUG,"      %12.3le %12.3le\n",d1,d2);
//logger.PrintLocal("w[%3d] = %lf\n",i,w);
        e0 += ( d1*d1 + d2*d2 + (d1*d2) )*w/3.0;
    }
    e0 /= nedges;
    e0 = sqrt(e0);

    if ( weights == 0) delete [] wt;
    return e0;
}





/*--------------------------------------------------------------------

  Построение точки, лежащей в заданной плоскости и имеющую наименьшее
  среднеквадратичным удаление от заданного множества плоскостей.

point      - искомая точка
baseplane  - коэффициенты плоскости, в которой должна лежать искомая точка  
n          - количество исходных плоскостей
planes     - массив коэффициентов исходных плоскостей 
             (четверки (a,b,c,d), длина массива 4*n
weights    - массив весовых коэффициентов для плоскостей, для weights=0 
             все веса = 1
return     - достигнутая погрешность аппроксимации

--------------------------------------------------------------------*/

double  NearestPointToPlanes ( 
    double *point, double *baseplane, int n, double *planes, double *weights )
{

    corrEdgeLogger.PrintLocal("nearest point to planes\n");
    corrEdgeLogger.PrintLocal("  base plane %12.8f %12.8f %12.8f %12.8f\n", baseplane[0],baseplane[1],baseplane[2],baseplane[3]);
    for (int i=0; i<n; i++) {
        corrEdgeLogger.PrintLocal("       plane %12.8f %12.8f %12.8f %12.8f  %10.6f\n", 
            planes[4*i], planes[4*i+1], planes[4*i+2], planes[4*i+3], weights[i]);
    }


    double syst[6];
    int    systres;
    double & A11=syst[0];
    double & A12=syst[1];
    double & A21=syst[2];
    double & A22=syst[3];
    double & F1 =syst[4];
    double & F2 =syst[5];
    double m, a, b, c, d, e, ai, bi, ci, di, wi;
    int    i, j, cnum;
    A11 = A12 = A21 = A22 = F1 = F2 = 0;

    // выбираем наибольшую компоненту нормали базовой плоскости
    if ( fabs(baseplane[0]) > fabs(baseplane[1]) )  { cnum = 0; m = fabs(baseplane[0]); } 
    else { cnum = 1; m = fabs(baseplane[1]); } 
    if ( m < fabs(baseplane[2]) ) { cnum = 2; }

    // рассмотриваем три отдельных случая
    switch ( cnum ) {
    case 0:  // x = d + by + cz
    logger.PrintLocal("case 0\n");
        d = -baseplane[3]/baseplane[0];
        b = -baseplane[1]/baseplane[0];
        c = -baseplane[2]/baseplane[0];
        for (i=0; i<n; i++) {
            j = 4*i;
            wi = (weights) ? weights[i] : 1.0;
            bi = planes[j+1] + planes[j]*b;
            ci = planes[j+2] + planes[j]*c;
            di = planes[j+3] + planes[j]*d;
            A11 += wi*bi*bi;
            A12 += wi*bi*ci;
            A22 += wi*ci*ci;
            F1  -= wi*bi*di;
            F2  -= wi*ci*di; 
        }
        A21 = A12;
        systres = LinSystem2x2 ( syst, point+1, point + 2 );
        point[0] = d + b*point[1] + c*point[2];
        break;
    case 1:  // y = d + ax + cz 
    logger.PrintLocal("case 1\n");
        d = -baseplane[3]/baseplane[1];
        a = -baseplane[0]/baseplane[1];
        c = -baseplane[2]/baseplane[1];
        for (i=0; i<n; i++) {
            j = 4*i;
            wi = (weights) ? weights[i] : 1.0;
            ai = planes[j  ] + planes[j+1]*a;
            ci = planes[j+2] + planes[j+1]*c;
            di = planes[j+3] + planes[j+1]*d;
            A11 += wi*ai*ai;
            A12 += wi*ai*ci;
            A22 += wi*ci*ci;
            F1  -= wi*ai*di;
            F2  -= wi*ci*di; 
        }
        A21 = A12;
        systres = LinSystem2x2 ( syst, point, point + 2 );
        point[1] = d + a*point[0] + c*point[2];
        break;
    case 2:  // z = d + ax + by 
    logger.PrintLocal("case 2\n");
        d = -baseplane[3]/baseplane[2];
        a = -baseplane[0]/baseplane[2];
        b = -baseplane[1]/baseplane[2];
        for (i=0; i<n; i++) {
            j = 4*i;
            wi = (weights) ? weights[i] : 1.0;
            ai = planes[j  ] + planes[j+2]*a;
            bi = planes[j+1] + planes[j+2]*b;
            di = planes[j+3] + planes[j+2]*d;
            A11 += wi*ai*ai;
            A12 += wi*ai*bi;
            A22 += wi*bi*bi;
            F1  -= wi*ai*di;
            F2  -= wi*bi*di; 
        }
        A21 = A12;
        LinSystem2x2 ( syst, point, point + 1 );
        point[2] = d + a*point[0] + b*point[1];
        break;

    }

    // вычисление погрешности приближения
    e = 0;
    for (i=0; i<n; i++) {
        j = 4*i;
        wi = (weights) ? weights[i] : 1.0;
        d = point[0]*planes[j] + point[1]*planes[j+1] + point[2]*planes[j+2] + planes[j+3];
        corrEdgeLogger.PrintLocal(" %d:%e  %e\n",i,d,wi);
        e += wi*d*d;
       
    }
    e = sqrt(e);

    corrEdgeLogger.PrintLocal("\n  point %10.6f %10.6f %10.6f  err %e\n",point[0],point[1],point[2],e);

    return e;
}



double TestPointToPlanes (double *point, int n, double *planes, double *weights )
{
    double err = 0;
    double d, w = 0;
//    logger.Print(LM_DEBUG, "    TestPointToPlanes\n" );
    for (int i=0; i<n; i++) {
        d = planes[4*i]*point[0] + planes[4*i+1]*point[1] + planes[4*i+2]*point[2] + planes[4*i+3]; 
//    logger.Print(LM_DEBUG, "      w %10.5lf  e %12.2le\n",weights[i], d );
        if ( weights[i] < 1.e-10 ) continue;
        err += d*d*weights[i];
        w += weights[i];
    }
    err = sqrt(err/w);
//   logger.Print(LM_DEBUG, "      err %12.2le\n",err );
    return err;    
}



double NearestPointToPlanes2 ( double *point, double *baseplane, int n, double *planes, double *weights )
{
    double err;          
//    double * matr[4];
    double elem[16]={0.,0.,0.,0., 0.,0.,0.,0., 0.,0.,0.,0., 0.,0.,0.,0.};
    double f[4]={0.,0.,0.,0.};
#define m(i,j) elem[4*(i)+(j)]
//    matr[0] = elem;
//    matr[1] = elem+4;
//    matr[2] = elem+8;
//    matr[3] = elem+12;

    double a,b,c,d,w;

    for (int i=0; i<n; i++ ) {
        a = planes[4*i];
        b = planes[4*i+1];
        c = planes[4*i+2];
        d = planes[4*i+3];
        w = weights[i];
    
        m(0,0) += 2*a*a*w;
        m(0,1) += 2*b*a*w;
        m(0,2) += 2*c*a*w;
        
        m(1,1) += 2*b*b*w;
        m(1,2) += 2*c*b*w;

        m(2,2) += 2*c*c*w;
        
        f[0] -= 2*d*a*w;
        f[1] -= 2*d*b*w;
        f[2] -= 2*d*c*w;
    }
    m(1,0) = m(0,1);
    m(2,0) = m(0,2);
    m(2,1) = m(1,2);
   
    m(3,0) = m(0,3) = baseplane[0];
    m(3,1) = m(1,3) = baseplane[1];
    m(3,2) = m(2,3) = baseplane[2];
    m(3,3) = 0;
    f[3] = -baseplane[3];

    bool res = true;
    double x[4];
    res = SimpleGauss( 4, elem, f, x );
    point[0] = x[0];
    point[1] = x[1];
    point[2] = x[2];


    //WAssert ( res, "NearestPointToPlanes2; degenerate system" );
    err = TestPointToPlanes ( point, n, planes, weights ); 

    double wht = 1;
    double err0 = TestPointToPlanes ( point, 1, baseplane, &wht );
   // WAssert ( fabs(err0) < 1e-12, "NearestPointToPlanes2; out of plane" );

   
    return err;
#undef m
}



void TestNearestPointToPlanes2 ()
{
    double point[3]={1.,2.,3.};
    double baseplane[4]={1.,0.,0.,0.} ;
    int n = 2;
    double *planes = new double[4*n];
    double *weights = new double[n];

    for (int i=0; i<n; i++) weights[i] = 1;
    planes[0] = 0; planes[1] = 1; planes[2] = 0; planes[3] = 0;
    planes[4] = 0; planes[5] = 0; planes[6] = 1; planes[7] = 0;
    

    NearestPointToPlanes2 ( point, baseplane, n, planes, weights );
    logger.Print(LM_DEBUG, "TestNearestPointToPlanes2\n");
    logger.Print(LM_DEBUG, "point %lf  %lf  %lf\n", point[0],point[1],point[2]);

    delete [] weights;
    delete [] planes;
}

/////////////////////

double ErrAproxByLine ( double * line, int n, double *points, double *weights )
{
    double e, err=0;
    if ( weights ) {
        for (int i=0; i<n; i++) {
            e = weights[i]*( line[0]*points[2*i] + line[1]*points[2*i+1] + line[2] );
            err += e*e;
        }
    } else {
        for (int i=0; i<n; i++) {
            e = ( line[0]*points[2*i] + line[1]*points[2*i+1] + line[2] );
            err += e*e;
        }
    }
    err /= n;
    err= sqrt(err);
    return err;
}

/*--------------------------------------------------------------------

  Построение прямой наилучшего среднеквадратичного приближения 
  для заданного множества точек двумерного пространства.

plane   - искомая плоскость  
          X*line[0] + Y*line[1] + line[2] = 0
n       - количество аппроксимируемых точек
points  - массив координат точек (пары (x,y), длина массива 2*n)
weights - массив весовых коэффициентов точек, для weights=0 все веса 1

return  - достигнутая погрешность аппроксимации

--------------------------------------------------------------------*/

double ApproxByLine ( double * line, int n, double *points, double *weights )
{
    double a11=0, a12=0, a22=0, a13=0, a23=0, a33=0, a133, a233;
    double w, x, y, p, q, lam1, lam2, a, b, c;
    double b11, b12, b22, e1, e2;
    double line1[3], line2[3];
    if ( weights ) {
        for (int i=0; i<n; i++) {
            w = weights[i];
            x = points[2*i];
            y = points[2*i+1];
            a11 += w*x*x;
            a12 += w*x*y;
            a22 += w*y*y;
            a13 += w*x;
            a23 += w*y;
            a33 += w;
        }
    } else { 
        for (int i=0; i<n; i++) {
            x = points[2*i];
            y = points[2*i+1];
//printf("x %lf y %lf\n",x,y);
            a11 += x*x;
            a12 += x*y;
            a22 += y*y;
            a13 += x;
            a23 += y;
        }
        a33 = n;
    }
//printf("%lf %lf %lf %lf %lf %lf\n",a11,a12,a22,a13,a23,a33);
    a133 = a13/a33;
    a233 = a23/a33;
//printf("a123 %lf %lf\n",a133,a233);
    a11 -= a13*a13/a33;
    a12 -= a13*a23/a33;
    a22 -= a23*a23/a33;
//printf("matr %lf  %lf\n",a11,a12);
//printf("matr %lf  %lf\n",a12,a22);
    p = (a11 + a22)/2;
    q = a11*a22 - a12*a12;
    lam1 = -p - sqrt( p*p - q );
    lam2 = -p + sqrt( p*p - q );
//printf("lam %lf %lf\n",lam1,lam2);
    
    // test lam1
    b11 = a11 + lam1;
    b12 = a12;
    b22 = a22 + lam1;
    if ( fabs(b11) > fabs(b22) ) {
        b = 1; a = -b12/b11;
    } else {
        a = 1; b = -b12/b22;
    }
    c = sqrt(a*a + b*b);
    line1[0] = a/c;
    line1[1] = b/c;
    line1[2] = -a133*line1[0] - a233*line1[1];
//printf("abc1  %lf %lf %lf\n",a,b,c);
//printf("line  %lf %lf %lf\n",line1[0],line1[1],line1[2]);
    e1 = ErrAproxByLine ( line1, n, points, weights );

    // test lam2
    b11 = a11 + lam2;
    b12 = a12;
    b22 = a22 + lam2;
    if ( fabs(b11) > fabs(b22) ) {
        b = 1; a = -b12/b11;
    } else {
        a = 1; b = -b12/b22;
    }
    c = sqrt(a*a + b*b);
    line2[0] = a/c;
    line2[1] = b/c;
    line2[2] = -a133*line2[0] - a233*line2[1];
//printf("abc2  %lf %lf %lf\n",a,b,c);
//printf("line  %lf %lf %lf\n",line2[0],line2[1],line2[2]);
    e2 = ErrAproxByLine ( line2, n, points, weights );

    if ( fabs(e1) <fabs(e2) ) {
        line[0] = line1[0];
        line[1] = line1[1];
        line[2] = line1[2];
        return e1;
    } else {
        line[0] = line2[0];
        line[1] = line2[1];
        line[2] = line2[2];
        return e2;
    }
}


/*--------------------------------------------------------------------
   тест аппроксимации для ApproxByLine3D
--------------------------------------------------------------------*/
double ErrApproxByLine3D ( double * a, double *b, int n, double *points, double *weights )
{
    int i,j;
    double qx, qy, qz, err, qb;

    err=0;
    if ( weights ) {
        for ( i=0; i<n; i++ ) {
            j = 3*i;
            qx = points[j] - a[0];
            qy = points[j+1] - a[1];
            qz = points[j+2] - a[2];
            qb = qx*b[0] + qy*b[1] + qz*b[2];
            err += weights[i]*( qx*qx + qy*qy + qz*qz - qb*qb );
        }
    } else {
        for ( i=0; i<n; i++ ) {
            j = 3*i;
            qx = points[j] - a[0];
            qy = points[j+1] - a[1];
            qz = points[j+2] - a[2];
            qb = qx*b[0] + qy*b[1] + qz*b[2];
            err += ( qx*qx + qy*qy + qz*qz - qb*qb );
           // logger.Print(LM_DEBUG,"%d err+ %le\n",i,err);
        }
    }
    if ( err < 0 ) err = -err;
    return sqrt(err);
}
double DistToLine3D ( double x, double y , double z, double * line ) 
{
    double qx, qy, qz, qb, d;
    qx = x - line[0];
    qy = y - line[1];
    qz = z - line[2];
    qb = qx*line[3] + qy*line[4] + qz*line[5];
    d = qx*qx + qy*qy + qz*qz - qb*qb;
    return sqrt(fabs(d));
}



/*--------------------------------------------------------------------

  Построение прямой наилучшего среднеквадратичного приближения 
  для заданного множества точек трехмерного пространства.

line    - искомая прямая, задается в виде точки (x,y,x) - line[0],line[1],line[2]
          и единичного вектора направления (dx,dy,dz) - line[3],line[4],line[5]
n       - количество аппроксимируемых точек
points  - массив координат точек (тройки (x,y,z), длина массива 3*n)
weights - массив весовых коэффициентов точек, для weights=0 все веса 1

return  - достигнутая погрешность аппроксимации

--------------------------------------------------------------------*/
double ApproxByLine3D ( double * line, int n, double *points, double *weights )
{
    int i,j;
    double w, ax, ay, az, a[3], err;

    if ( n==2 ) {
        ax = points[0] - points[3];
        ay = points[1] - points[4];
        az = points[2] - points[5];
        w = sqrt(ax*ax + ay*ay + az*az);
        line[0] = points[0];
        line[1] = points[1];
        line[2] = points[2];
        line[3] = ax/w;
        line[4] = ay/w;
        line[5] = az/w;
        return 0;
    }

    // вычисляем центр тяжести
    ax = ay = az = 0;
    w = 0;
    if ( weights ) {
        for ( i=0; i<n; i++ ) {
            j = 3*i;
            ax += weights[i] * points[j];
            ay += weights[i] * points[j+1];
            az += weights[i] * points[j+2];
            w += weights[i];
        } 
    } else {
        for ( i=0; i<n; i++ ) {
            j = 3*i;
            ax += points[j];
            ay += points[j+1];
            az += points[j+2];
        }
        w = n;
    }
    ax /= w;
    ay /= w;
    az /= w;
    a[0] = ax; a[1] = ay; a[2] = az;

    // вычисляем коэффициенты системы
    double qx, qy, qz;
    double qxx, qyy, qzz, qxy, qyz, qxz;
    qxx = qyy = qzz = qxy = qyz = qxz = 0;
    if ( weights ) {
        for ( i=0; i<n; i++ ) {
            j = 3*i;      
            qx = points[j] - ax;
            qy = points[j+1] - ay;
            qz = points[j+2] - az;
            w = weights[i];
            qxx += w * qx * qx;
            qyy += w * qy * qy;
            qzz += w * qz * qz;
            qxy += w * qx * qy;
            qxz += w * qx * qz;
            qyz += w * qy * qz;
        }
    } else {
        for ( i=0; i<n; i++ ) {
            j = 3*i;      
            qx = points[j] - ax;
            qy = points[j+1] - ay;
            qz = points[j+2] - az;
            qxx += qx * qx;
            qyy += qy * qy;
            qzz += qz * qz;
            qxy += qx * qy;
            qxz += qx * qz;
            qyz += qy * qz;
        }
    }

    // решаем задачу на собственные значения
    double *matr[3], lamb[3], *vect[3];
    double m[9], v[9]; 
    m[0] = qxx;    m[1] = qxy;   m[2] = qxz;
    m[3] = m[1];   m[4] = qyy;   m[5] = qyz;
    m[6] = m[2];   m[7] = m[5];  m[8] = qzz;
    matr[0] = m;  matr[1] = m+3;  matr[2] = m+6;
    vect[0] = v;  vect[1] = v+3;  vect[2] = v+6;
    
    //corrEdgeLogger.PrintLocal( "matr  %10.5lf %10.5lf %10.5lf\n",matr[0][0],matr[0][1],matr[0][2] );
    //corrEdgeLogger.PrintLocal( "matr  %10.5lf %10.5lf %10.5lf\n",matr[1][0],matr[1][1],matr[1][2] );
    //corrEdgeLogger.PrintLocal( "matr  %10.5lf %10.5lf %10.5lf\n",matr[2][0],matr[2][1],matr[2][2] );
    
    Jacobi( 3, matr, lamb, vect );

    // нормируем собственные векторы
    double d;
    swap (v[1],v[3]);
    swap (v[2],v[6]);
    swap (v[5],v[7]);
    d = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);  v[0] /= d; v[1] /= d; v[2] /= d;
    d = sqrt(v[3]*v[3] + v[4]*v[4] + v[5]*v[5]);  v[3] /= d; v[4] /= d; v[5] /= d;
    d = sqrt(v[6]*v[6] + v[7]*v[7] + v[8]*v[8]);  v[6] /= d; v[7] /= d; v[8] /= d;

    // выбираем минимальное значение
    double e0,e1,e2;
    int imin;
    e0 = ErrApproxByLine3D ( a, vect[0], n, points, weights );
    e1 = ErrApproxByLine3D ( a, vect[1], n, points, weights );
    e2 = ErrApproxByLine3D ( a, vect[2], n, points, weights );
    
    //logger.Print( LM_DEBUG, "val %10.5lf vect %10.5lf %10.5lf %10.5lf  %le\n", lamb[0], vect[0][0], vect[0][1], vect[0][2], e0 );
    //logger.Print( LM_DEBUG, "val %10.5lf vect %10.5lf %10.5lf %10.5lf  %le\n", lamb[1], vect[1][0], vect[1][1], vect[1][2], e1 );
    //logger.Print( LM_DEBUG, "val %10.5lf vect %10.5lf %10.5lf %10.5lf  %le\n", lamb[2], vect[2][0], vect[2][1], vect[2][2], e2 );

    if ( e0 < e1 ) {
        if ( e2 < e0 ) { imin = 2; err = e2; } 
        else           { imin = 0; err = e0; } 
    } else {
        if ( e2 < e1 ) { imin = 2; err = e2; } 
        else           { imin = 1; err = e1; } 
    }
    line[0] = ax;
    line[1] = ay;
    line[2] = az;
    line[3] = vect[imin][0];
    line[4] = vect[imin][1];
    line[5] = vect[imin][2];

    //logger.Print(LM_DEBUG,"ErrApproxByLine3D   %le %le %le    %le\n",e0, e1, e2, err);
    //corrEdgeLogger.PrintLocal("ErrApproxByLine3D   %le %le %le    %le\n",e0, e1, e2, err);
    return err;    
}

int TestApproxByLine3D ()
{
    logger.Print( LM_DEBUG, "TestApproxByLine3D\n" );
    
    
    int n, t, j;
    double p[30];
    double w[10];
    double line[6];
    double err; //, d, dx, dy, dz;

    // test 1
    // прямая  x=3+t, y=2+2t, z=1+3t с равномерным шагом
    n = 5;
    logger.Print( LM_DEBUG, "Test 1 points\n");
    for ( t=0; t<n; t++ ) {
        j = 3*t;
        p[j] = 3+t;   
        p[j+1] = 2+2*t;   
        p[j+2] = 1+3*t;   
    logger.Print( LM_DEBUG, " %10.5lf %10.5lf %10.5lf\n", p[j],p[j+1],p[j+2]);
    }
    err = ApproxByLine3D ( line, n, p );
    logger.Print( LM_DEBUG, "result a %10.5lf %10.5lf %10.5lf\n", line[0], line[1], line[2]);
    logger.Print( LM_DEBUG, "result b %10.5lf %10.5lf %10.5lf\n", line[3], line[4], line[5]);
    logger.Print( LM_DEBUG, "result err  %12.2le\n", err);
    //if ( err > 1.e-12 ) return -1;
    for ( t=0; t<n; t++ ) {
        j = 3*t;
        err = DistToLine3D ( p[j], p[j+1], p[j+2], line );
        logger.Print( LM_DEBUG, "test point %10.5lf %10.5lf %10.5lf  dist %12.2le\n", p[j],p[j+1],p[j+2], err);
        //if ( err > 1.e-12 ) return -1;
    }
 
    // test 2
    // прямая  x=3+t, y=2+2t, z=1+3t с весами
    n = 3;
    w[0] = 1;
    w[1] = 2;
    w[2] = 3;
    logger.Print( LM_DEBUG, "Test 1 points\n");
    for ( t=0; t<n; t++ ) {
        j = 3*t;
        p[j] = 3+t;   
        p[j+1] = 2+2*t;   
        p[j+2] = 1+3*t;   
    logger.Print( LM_DEBUG, " %10.5lf %10.5lf %10.5lf\n", p[j],p[j+1],p[j+2]);
    }
    err = ApproxByLine3D ( line, n, p, w );
    logger.Print( LM_DEBUG, "result a %10.5lf %10.5lf %10.5lf\n", line[0], line[1], line[2]);
    logger.Print( LM_DEBUG, "result b %10.5lf %10.5lf %10.5lf\n", line[3], line[4], line[5]);
    logger.Print( LM_DEBUG, "result err  %12.2le\n", err);
    //if ( err > 1.e-12 ) return -1;
    for ( t=0; t<n; t++ ) {
        j = 3*t;
        err = DistToLine3D ( p[j], p[j+1], p[j+2], line );
        logger.Print( LM_DEBUG, "test point %10.5lf %10.5lf %10.5lf  dist %12.2le\n", p[j],p[j+1],p[j+2], err);
        //if ( err > 1.e-12 ) return -1;
    }
 


    return 0;
}
