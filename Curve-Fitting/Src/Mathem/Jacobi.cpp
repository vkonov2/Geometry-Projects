// #include "stdafx.h"

#include <math.h>
#include <stdio.h>


//***************************************************************************//
//
//  The Jacobi Method for Finding the Eigenvalue and Eigenvectors
//                      of Symmetric Matrices.
// 
//  Метод Якоби нахождения собственных значений и собственных векторов
//                       для симметричных матриц
//    Входные данные:
//        n - размерность матрицы
//        a - исходная матрица. В процессе работы наддиагональные элементы
//            будут изменены, но их легко восстановить по поддиагональным
//    Выходные данные:
//        d - массив собственных значений
//        v - массив собственных векторов
//
//***************************************************************************//

void Jacobi ( const unsigned int n, double * const * a, double * d, double * const * v )
{
    if ( n == 0 ) return;
    double * b = new double[n+n];
    double * z = b + n;
    unsigned int i, j;
    for ( i = 0; i < n; ++i )
    {
        z[i] = 0.;
        b[i] = d[i] = a[i][i];
        for ( j = 0; j < n; ++j )
        {
            v[i][j] = i == j ? 1. : 0.;
        }
    }

    for ( i = 0; i < 50; ++i )
    {
        double sm = 0.;
        unsigned int p, q;
        for ( p = 0; p < n - 1; ++p )
        {
            for ( q = p + 1; q < n; ++q )
            {
                sm += fabs ( a[p][q] );
            }
        }
        //if ( sm == 0 ) break;
        if ( sm < 1.e-15 ) break;
        const double tresh = i < 3 ? 0.2 * sm / ( n*n ) : 0.;
        for ( p = 0; p < n - 1; ++p )
        {
            for ( q = p + 1; q < n; ++q )
            {
                const double g = 1e12 * fabs ( a[p][q] );
                if ( i >= 3 && fabs ( d[p] ) > g && fabs ( d[q] ) > g ) a[p][q] = 0.;
                else
                if ( fabs ( a[p][q] ) > tresh )
                {
                    const double theta = 0.5 * ( d[q] - d[p] ) / a[p][q];
                    double t = 1. / ( fabs(theta) + sqrt(1.+theta*theta) );
                    if ( theta < 0 ) t = - t;
                    const double c = 1. / sqrt ( 1. + t*t );
                    const double s = t * c;
                    const double tau = s / ( 1. + c );
                    const double h = t * a[p][q];
                    z[p] -= h;
                    z[q] += h;
                    d[p] -= h;
                    d[q] += h;
                    a[p][q] = 0.;
                    for ( j = 0; j < p; ++j )
                    {
                        const double g = a[j][p];
                        const double h = a[j][q];
                        a[j][p] = g - s * ( h + g * tau );
                        a[j][q] = h + s * ( g - h * tau );
                    }
                    for ( j = p+1; j < q; ++j )
                    {
                        const double g = a[p][j];
                        const double h = a[j][q];
                        a[p][j] = g - s * ( h + g * tau );
                        a[j][q] = h + s * ( g - h * tau );
                    }
                    for ( j = q+1; j < n; ++j )
                    {
                        const double g = a[p][j];
                        const double h = a[q][j];
                        a[p][j] = g - s * ( h + g * tau );
                        a[q][j] = h + s * ( g - h * tau );
                    }
                    for ( j = 0; j < n; ++j )
                    {
                        const double g = v[j][p];
                        const double h = v[j][q];
                        v[j][p] = g - s * ( h + g * tau );
                        v[j][q] = h + s * ( g - h * tau );
                    }
                }
            }
        }
        for ( p = 0; p < n; ++p )
        {
            d[p] = ( b[p] += z[p] );
            z[p] = 0.;
        }
    }
    delete[] b;
}


