#include "stdafx.h"
#include <math.h>

#include "../WLogger.h"
#include "../Utils.h"
extern WLogger logger;


//**********************************************************
//           Linear System Solvers
//
//**********************************************************



//
//  Решение системы линейных уравнений 2х2
//
#define SLE2_EPS  1.e-16

int SLE2Solve (double * a, double *f, double *x)
{
    double c,d;
    int imax = AbsMaxInd( 4, a );
    if ( fabs(a[imax]) < SLE2_EPS ) return -1;
    switch( imax ) {
        case 0:
            c = a[3] - a[1]*a[2]/a[0];
            if ( fabs(c) < SLE2_EPS ) return -2;
            d = f[1] - f[0]*a[2]/a[0];
            x[1] = d/c;
            x[0] = (f[0] - a[1]*x[1])/a[0]; 
            break;
        case 1:
            c = (a[2] - a[0]*a[3]/a[1]);
            if ( fabs(c)< SLE2_EPS ) return -2;
            d = (f[1] - f[0]*a[3]/a[1]);
            x[0] = d/c;
            x[1] = (f[0] - a[0]*x[0])/a[1]; 
            break;
        case 2:
            c = a[1] - a[0]*a[3]/a[2];
            if ( fabs(c)< SLE2_EPS ) return -2;
            d = f[0] - f[1]*a[0]/a[2];
            x[1] = d/c;
            x[0] = (f[1] - a[3]*x[1])/a[2]; 
            break;
        case 3:
            c = (a[0] - a[1]*a[2]/a[3]);
            if ( fabs(c)< SLE2_EPS ) return -2;
            d = (f[0] - f[1]*a[1]/a[3]);
            x[0] = d/c;
            x[1] = (f[1] - a[2]*x[0])/a[3]; 
            break;
    }
    return 0;
}

//
//  Решение системы линейных уравнений 3х3
//
int SLE3Solve (double * a, double *f, double *x)
{
    int ret;
    double aa[4], ff[2], xx[2];
    int imax = AbsMaxInd( 9, a );
    if ( fabs(a[imax]) < SLE2_EPS ) return -1;
    switch( imax ) {
        case 3:  
            swap(a[0],a[3]); swap(a[1],a[4]); swap(a[2],a[5]); swap(f[0],f[1]); break;
        case 6:  
            swap(a[0],a[3]); swap(a[1],a[4]); swap(a[2],a[5]); swap(f[0],f[1]); break;
        

    }
    switch( imax ) {
        case 0:
            aa[0] = a[4] - a[1]*a[3]/a[0];
            aa[1] = a[5] - a[2]*a[3]/a[0];
            aa[2] = a[7] - a[1]*a[6]/a[0];
            aa[3] = a[8] - a[2]*a[3]/a[0];
            ff[0] = f[1] - f[0]*a[3]/a[0];
            ff[1] = f[2] - f[0]*a[6]/a[0];
            ret = SLE2Solve ( aa, ff, xx );
            if (ret < 0 ) return ret;
            x[1] = xx[0];
            x[2] = xx[1];
            x[0] = (f[0] - x[1]*a[1] - x[2]*a[2])/a[0];
    }
    return 0;
}






//---------------------------------------------------------
//  Solution of 2x2 linear system by Cramer's method
//
//   a11 x + a12 y = f1;
//   a21 x + a22 y = f2;
// 
// system   - double[6] - {a11,a12,a21,a22,f1,f2}
// return   - <0 inconsisted, =0 inf many solutions, >0 unique solution
//

#define  REL_2x2_EPS  1e-13;

int  LinSystem2x2 ( double *system, double *x, double *y )
{
    int res = 1;
    double &a11=system[0];
    double &a12=system[1];
    double &a21=system[2];
    double &a22=system[3];
    double &f1 =system[4];
    double &f2 =system[5];
    
    double det, detx, dety, thr;
    thr = fabs(system[0]) + fabs(system[1]) + fabs(system[2]) + fabs(system[3]);
    thr *= 0.25*REL_2x2_EPS;

    det  = a11*a22 - a12*a21;
    detx = f1*a22 - f2*a12;
    dety = f2*a11 - f1*a21;

    if ( fabs(det)<thr ) {
        if ( fabs(detx)<thr && fabs(dety)<thr ) res = 0;
        else return -1;
    }
    if ( res==0 ) {
        if ( fabs(a11) > fabs(a12) ) {
            *x = f1/a11;  
            *y = 0;
        } else {
            *x = 0;
            *y = f1/a12;
        }
    } else {
        *x = detx/det;
        *y = dety/det;
    }
    return res;   
}


// простейший метод Гаусса с выбором главного элемента по столбцу


bool SimpleGauss ( int n, double *matr, double *f, double *x )
{
#define a(i,j) matr[ (i)*n + (j) ]
    const double eps = 1.e-12; // HACK        
    double amax, aabs, atmp, alp; 
    int imax;

    // приведение к треугольному виду
    // цикл по столбцам
    for (int j=0; j<n-1; j++ ) {
        // выбор ведущего элемента
        amax = fabs(a(j,j));
        imax = j;
        for (int i=j+1; i<n; i++ ) {
            aabs = fabs(a(i,j));
            if ( amax < aabs ) { amax = aabs; imax = i; }
        }
        // перестановка строк
        if ( imax != j ) {
            for (int k=j; k<n; k++) {
                atmp = a(j,k); a(j,k) = a(imax,k); a(imax,k) = atmp; 
            }
            atmp = f[j]; f[j] = f[imax]; f[imax] = atmp;
        }
        // вычисление линейной комбинации
        for (int i=j+1; i<n; i++) {
            alp = a(i,j)/a(j,j); 
            for (int k=j; k<n; k++) {
                a(i,k) -= alp*a(j,k);
            }
            f[i] -= alp*f[j];
        }
    }
    // проверка вырожденности
    for (int i=0; i<n; i++) {
        if ( fabs(a(i,i))<eps ) return false;
    }
    // обратная подстановка
    for (int i=n-1; i>=0; i--) {
        x[i] = f[i];
        for (int j=i+1; j<n; j++) {
            x[i] -= a(i,j)*x[j];
        }
        x[i] /= a(i,i);
    }
    return true;
#undef a
}

void TestSimpleGauss()
{
    int n= 4;
    double x[4]={0,0,0,0};
    double matr[16]={ 3., -7.,  2.,  0.,
                     -4., 12., -8.,  3., 
                    -14.,  2.,  6.,  4.,
                      5., -7.,  0., 11. };
    double f[4] = {-2., 3., -2., 9. };
    bool res = SimpleGauss(4,matr,f,x);
    logger.Print(LM_DEBUG,"TestSimpleGauss  %lf %lf %lf %;f\n",x[0],x[1],x[2],x[3]);
    

}

void PrintMatrices(const char *text, const double *r0, const double *r1, const double *r2, const double *v0=0, const double *v1=0, const double *v2=0)
{
	logger.Print(LM_DEBUG,"%s\n",text);
	logger.Print(LM_DEBUG, " %10.5f %10.5f %10.5f ", r0[0], r0[1], r0[2]);
    if (v0) logger.Print(LM_DEBUG, " %10.5f %10.5f %10.5f ", v0[0], v0[1], v0[2]);
	logger.Print(LM_DEBUG, "\n %10.5f %10.5f %10.5f ", r1[0], r1[1], r1[2]);
	if (v1) logger.Print(LM_DEBUG, " %10.5f %10.5f %10.5f ", v1[0], v1[1], v1[2]);
	logger.Print(LM_DEBUG, "\n %10.5f %10.5f %10.5f ", r2[0], r2[1], r2[2]);
	if (v2) logger.Print(LM_DEBUG, " %10.5f %10.5f %10.5f ", v2[0], v2[1], v2[2]);
	logger.Print(LM_DEBUG, "\n");
}

void TestMatrices(const double *matr, const double *inverse)
{
	double inv[9];
	inv[0] = matr[0] * inverse[0] + matr[1] * inverse[3] + matr[2] * inverse[6];
	inv[1] = matr[0] * inverse[1] + matr[1] * inverse[4] + matr[2] * inverse[7];
	inv[2] = matr[0] * inverse[2] + matr[1] * inverse[5] + matr[2] * inverse[8];

	inv[3] = matr[3] * inverse[0] + matr[4] * inverse[3] + matr[5] * inverse[6];
	inv[4] = matr[3] * inverse[1] + matr[4] * inverse[4] + matr[5] * inverse[7];
	inv[5] = matr[3] * inverse[2] + matr[4] * inverse[5] + matr[5] * inverse[8];

	inv[6] = matr[6] * inverse[0] + matr[7] * inverse[3] + matr[8] * inverse[6];
	inv[7] = matr[6] * inverse[1] + matr[7] * inverse[4] + matr[8] * inverse[7];
	inv[8] = matr[6] * inverse[2] + matr[7] * inverse[5] + matr[8] * inverse[8];

	MaxAccum acc, acc1;
	acc.Add(fabs(inv[1]));
	acc.Add(fabs(inv[2]));
	acc.Add(fabs(inv[3]));
	acc.Add(fabs(inv[5]));
	acc.Add(fabs(inv[6]));
	acc.Add(fabs(inv[7]));
	acc1.Add(fabs(inv[0] - 1.0));
	acc1.Add(fabs(inv[4] - 1.0));
	acc1.Add(fabs(inv[8] - 1.0));
	logger.Print(LM_DEBUG, " %10.4e %10.4e\n", acc.Max(), acc1.Max());
	if (acc.Max() > 1e-10 || acc1.Max() > 1.e-10) 	logger.Print(LM_DEBUG, " --- ERROR\n");
	//PrintMatrices("test", matr, matr+3, matr+6, inverse, inverse+3, inverse + 6);
	//PrintMatrices("test", inv, inv + 3, inv + 6);

}


// Вычисление обратной матрицы для случая 3х3
// matr [9] --- исходная матрица по строкам
// inverse [9] --- обратная матрица по строкам
// return --- успешность вычисления матрицы 

bool SimpleGaussInverse3x3(const double *matr, double *inverse)
{
	double wmatr[9], inv[9];
	memcpy(wmatr, matr, 9 * sizeof(double));
	double *row0 = wmatr, *row1 = wmatr + 3, *row2 = wmatr + 6;
	double *inv0 = inv, *inv1 = inv + 3, *inv2 = inv + 6;
	
	memset(inv, 0, 9 * sizeof(double));
	inv0[0] = inv1[1] = inv2[2] = 1.0;

	//PrintMatrices("initial", row0, row1, row2, inv0, inv1, inv2);

	int irowmax;
	double coef;

	Max3(fabs(row0[0]), fabs(row1[0]), fabs(row2[0]), irowmax);
	switch (irowmax) {
	case 1: Swap(row0, row1);  Swap(inv0, inv1); break;
	case 2: Swap(row0, row2);  Swap(inv0, inv2); break;
	}
	//PrintMatrices("row0 perm", row0, row1, row2, inv0, inv1, inv2);

	if (fabs(row0[0]) < 1.e-9) return false;
	coef = row1[0] / row0[0];
	row1[0] -= coef*row0[0]; //
	row1[1] -= coef*row0[1];
	row1[2] -= coef*row0[2];
    inv1[0] -= coef*inv0[0];
	inv1[1] -= coef*inv0[1]; 
	inv1[2] -= coef*inv0[2]; 
	//PrintMatrices("row1 minus1", row0, row1, row2, inv0, inv1, inv2);

	coef = row2[0] / row0[0];
	row2[0] -= coef*row0[0]; //
	row2[1] -= coef*row0[1];
	row2[2] -= coef*row0[2];
	inv2[0] -= coef*inv0[0];
	inv2[1] -= coef*inv0[1];
	inv2[2] -= coef*inv0[2];
	//PrintMatrices("row1 minus2", row0, row1, row2, inv0, inv1, inv2);

	Max2(fabs(row1[1]), fabs(row2[1]), irowmax);
	if (irowmax == 1) {
		Swap(row1, row2);  Swap(inv1, inv2);
	}
	//PrintMatrices("row2", row0, row1, row2, inv0, inv1, inv2);

	if (fabs(row1[1]) < 1.e-9) return false;
	coef = row2[1] / row1[1];
	row2[1] -= coef*row1[1]; //
	row2[2] -= coef*row1[2];
	inv2[0] -= coef*inv1[0];
	inv2[1] -= coef*inv1[1];
	inv2[2] -= coef*inv1[2];

	//PrintMatrices("row2 minus", row0, row1, row2, inv0, inv1, inv2);
	
	if (fabs(row2[2]) < 1.e-9) return false;
	// have got nondegenerate triangular form

	inv0[2] /= row0[0];
	inv0[1] /= row0[0];
	inv0[0] /= row0[0];
	row0[2] /= row0[0];
	row0[1] /= row0[0];
	row0[0] = 1.0;

	inv1[2] /= row1[1];
	inv1[1] /= row1[1];
	inv1[0] /= row1[1];
	row1[2] /= row1[1];
	row1[1] = 1.0;

	inv2[2] /= row2[2];
	inv2[1] /= row2[2];
	inv2[0] /= row2[2];
	row2[2] = 1.0;

	//PrintMatrices("unit diagonal", row0, row1, row2, inv0, inv1, inv2);

	coef = row1[2];
	row1[2] -= coef*row2[2];
	inv1[0] -= coef*inv2[0];
	inv1[1] -= coef*inv2[1];
	inv1[2] -= coef*inv2[2];
	coef = row0[2];
	row0[2] -= coef*row2[2];
	inv0[0] -= coef*inv2[0];
	inv0[1] -= coef*inv2[1];
	inv0[2] -= coef*inv2[2];

	//PrintMatrices("last column", row0, row1, row2, inv0, inv1, inv2);

	coef = row0[1];
	row0[1] -= coef*row1[1];
	inv0[0] -= coef*inv1[0];
	inv0[1] -= coef*inv1[1];
	inv0[2] -= coef*inv1[2];

	//PrintMatrices("final", row0, row1, row2, inv0, inv1, inv2);

	inverse[0] = inv0[0];
	inverse[1] = inv0[1];
	inverse[2] = inv0[2];
	inverse[3] = inv1[0];
	inverse[4] = inv1[1];
	inverse[5] = inv1[2];
	inverse[6] = inv2[0];
	inverse[7] = inv2[1];
	inverse[8] = inv2[2];

	return true;
}


void SimpleGaussInverse3x3_Test()
{
	double matr[9];
	double inverse[9];

	for (int i = 0; i < 200; i++) {
		for (int j=0; j<9; j++) matr[j] = rand() % 1000;
		SimpleGaussInverse3x3(matr, inverse);
		TestMatrices(matr, inverse);
	}
}







