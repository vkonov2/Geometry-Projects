#define _CRT_SECURE_NO_WARNINGS

#include "PlaneGeometryFunctions.h"
#include <iostream>
#include "../Mathem/Jacobi.h"

#define accuracy 2048

void MoveSideCenter(vector <VECTOR2>& CurrSidePts, vector <VECTOR2>& SidePts)
{
	int n = (int)(CurrSidePts.size());
	VECTOR2 Center(0, 0);

	for (int i = 0; i < n; i++)
	{
		Center.x += CurrSidePts[i].x;
		Center.y += CurrSidePts[i].y;
	}

	Center /= n;

	for (int i = 0; i < n; i++)
		CurrSidePts[i] -= Center;

	n = (int)(SidePts.size());

	for (int i = 0; i < n; i++)
		SidePts[i] -= Center;
}

double AngleMatchPolygons(vector <VECTOR3> PatternCrossSection, vector <VECTOR3> RealCrossSection, VECTOR Norm, vector <double>& Err)
{
	int PatternN = (int)(PatternCrossSection.size()), RealN = (int)(RealCrossSection.size());
	net_func RealF(accuracy), PatternF(accuracy), Error(accuracy);
	VECTOR2* RealPts = new VECTOR2[RealN];
	VECTOR2* PatternPts = new VECTOR2[PatternN];
	vector < VECTOR2 > PatternProjection, RealProjection;
	double Angle;
	int id;
	int* RealHullInds = new int[RealN], * PatternHullInds = new int[PatternN];

	//printf("Norm = %lf, %lf, %lf\n", Norm.x, Norm.y, Norm.z);

	FindProjection(Norm, PatternCrossSection, PatternProjection);
	FindProjection(Norm, RealCrossSection, RealProjection);

	MoveSideCenter(PatternProjection, RealProjection);

	for (int i = 0; i < RealN; i++)
		RealPts[i] = RealProjection[i];

	CreateConvexHull(RealN, &RealProjection[0], RealHullInds);
	CreateConvexHull(PatternN, &PatternProjection[0], PatternHullInds);

	/*printf("Pattern cross sec len = %d, CH = %d\n", PatternN, PatternConvHullCount);
	printf("Real cross sec len = %d, CH = %d\n", RealN, RealConvHullCount);*/

	for (int i = 0; i < RealN; i++)
	{
		id = RealHullInds[i];
		RealPts[i] = RealProjection[id];
	}

	// FILE* f;
	// f = fopen("D:\\YandexDisk\\Лиза\\Octonus\\RBP.txt", "w");

	// for (int i = 0; i < RealN - 1; i++)
	// {
	// 	id = RealHullInds[i];
	// 	id_pl = RealHullInds[i + 1];

	// 	fprintf(f, "%lf, %lf\n", RealPts[id].x, RealPts[id].y);
	// 	fprintf(f, "%lf, %lf\n\n", RealPts[id_pl].x, RealPts[id_pl].y);
	// }

	// id = RealHullInds[RealN - 1];
	// id_pl = RealHullInds[0];

	// fprintf(f, "%lf, %lf\n", RealPts[id].x, RealPts[id].y);
	// fprintf(f, "%lf, %lf\n\n", RealPts[id_pl].x, RealPts[id_pl].y);

	// fclose(f);

	for (int i = 0; i < PatternN; i++)
	{
		id = PatternHullInds[i];
		PatternPts[i] = PatternProjection[id];
	}
	
	// f = fopen("D:\\YandexDisk\\Лиза\\Octonus\\PP.txt", "w");

	// for (int i = 0; i < PatternN - 1; i++)
	// {
	// 	id = PatternHullInds[i];
	// 	id_pl = PatternHullInds[i + 1];

	// 	fprintf(f, "%lf, %lf\n", PatternPts[id].x, PatternPts[id].y);
	// 	fprintf(f, "%lf, %lf\n\n", PatternPts[id_pl].x, PatternPts[id_pl].y);
	// }

	// id = PatternHullInds[PatternN - 1];
	// id_pl = PatternHullInds[0];

	// fprintf(f, "%lf, %lf\n", PatternPts[id].x, PatternPts[id].y);
	// fprintf(f, "%lf, %lf\n\n", PatternPts[id_pl].x, PatternPts[id_pl].y);

	// fclose(f);

	Set(RealF, RealN, RealPts);
	Set(PatternF, PatternN, PatternPts);

	L2_error(Error.f, PatternF.f, RealF.f, accuracy);

	for (int i = 0; i < accuracy; i++)
	{
		Err.push_back(Error.f[i]);
		//printf("Error.f[%d]: %lf\n", i, Error.f[i]);
	}

	int MinId = find_min(Error.f, accuracy);
	//printf("Error = %lf\n", Error.f[MinId]);
	Angle = (2 * M_PI * MinId) / accuracy;

	PatternProjection.clear();
	RealProjection.clear();
	delete[] RealPts;
	delete[] PatternPts;
	delete[] RealHullInds;
	delete[] PatternHullInds;

	return Angle;
}

double PlaneVectorCos(const PHSIDE side, const VECTOR n)
{
    return fabs(n | side.plane.norm) / (sqrt(side.plane.norm | side.plane.norm) *
        sqrt(n | n));
}

double Plane3DArea(const PHSIDE side, POINT3* vertex)
{
	double scalar_prod_xy = 0.0, scalar_prod_yz = 0.0, scalar_prod_xz = 0.0;
    double cos = 0.0, max = 0.0, area_sum = 0.0;
    int index, index_pl_1, flag;
    VECTOR n_xy(0, 0, 1);
    VECTOR n_yz(1, 0, 0);
    VECTOR n_xz(0, 1, 0);

    scalar_prod_xy = fabs(n_xy | side.plane.norm);
    scalar_prod_yz = fabs(n_yz | side.plane.norm);
    scalar_prod_xz = fabs(n_xz | side.plane.norm);

    int len = length(side.vlist) - 1;

    if (scalar_prod_xy > scalar_prod_yz)
    {
        max = scalar_prod_xy;
        flag = 0;
    }
    else
    {
        max = scalar_prod_yz;
        flag = 2;
    }

    if (scalar_prod_xz > max)
    {
        max = scalar_prod_xz;
        flag = 1;
    }

    if (flag == 0)
    {
        cos = PlaneVectorCos(side, n_xy);

        for (int j = 0; j < len; j++)
        {
            index = side.vlist[j];
            index_pl_1 = side.vlist[j + 1];

            area_sum += vertex[index].x * vertex[index_pl_1].y - vertex[index_pl_1].x * vertex[index].y;
        }
    }

    if (flag == 2)
    {
        cos = PlaneVectorCos(side, n_yz);

        for (int j = 0; j < len; j++)
        {
            index = side.vlist[j];
            index_pl_1 = side.vlist[j + 1];

            area_sum += vertex[index].y * vertex[index_pl_1].z - vertex[index_pl_1].y * vertex[index].z;
        }
    }

    if (flag == 1)
    {
        cos = PlaneVectorCos(side, n_xz);

        for (int j = 0; j < len; j++)
        {
            index = side.vlist[j];
            index_pl_1 = side.vlist[j + 1];

            area_sum += vertex[index].x * vertex[index_pl_1].z - vertex[index_pl_1].x * vertex[index].z;
        }
    }

    area_sum /= cos;
    area_sum /= 2.0;

	//double xs = 0.0, ys = 0.0, zs = 0.0;
	//int len = length(side.vlist) - 1;

	//for (int i = 0; i < len; i++)
	//{
	//	index = side.vlist[i];

	//	xs += vertex[index].x;
	//	ys += vertex[index].y;
	//	zs += vertex[index].z;
	//}

	//xs /= len;
	//ys /= len;
	//zs /= len;

	//POINT3 Mass(xs, ys, zs);
	//double area;

	//for (int j = 0; j < len; j++)
	//{
	//	index = side.vlist[j];
	//	index_pl_1 = side.vlist[j + 1];

	//	area = ((Mass - vertex[index]) & (Mass - vertex[index_pl_1])).abs();
	//	area /= 2.0;

	//	area_sum += area;
	//}

    return fabs(area_sum);
}

//void DefinePlaneCoeff(POLYHEDRON* Poly, int side_index)
//{
//	int i, j, index;
//
//	double x, y, z;
//	double x_sum, y_sum, z_sum;
//	double x2_sum, y2_sum, z2_sum;
//	double xy_sum, xz_sum, yz_sum;
//	double val_a, val_b, val_c, val_d, sum;
//
//	x_sum = 0;
//	y_sum = 0;
//	z_sum = 0;
//	x2_sum = 0;
//	y2_sum = 0;
//	z2_sum = 0;
//	xy_sum = 0;
//	xz_sum = 0;
//	yz_sum = 0;
//
//	int len = length(Poly->side[side_index].vlist) - 1;
//
//	for (i = 0; i < len; ++i)
//	{
//		index = Poly->side[side_index].vlist[i];
//
//		x = Poly->vertex[index].x;
//		y = Poly->vertex[index].y;
//		z = Poly->vertex[index].z;
//
//		x_sum += x;
//		y_sum += y;
//		z_sum += z;
//
//		x2_sum += x * x;
//		y2_sum += y * y;
//		z2_sum += z * z;
//
//		xy_sum += x * y;
//		xz_sum += x * z;
//		yz_sum += y * z;
//	}
//
//	 for (i = 0; i < 6; ++i)
//	 {
//	 	x = arr_x[i];
//	 	y = arr_y[i];
//	 	z = arr_z[i];
//
//	 	x_sum += x;
//	 	y_sum += y;
//	 	z_sum += z;
//
//	 	x2_sum += x*x;
//	 	y2_sum += y*y;
//	 	z2_sum += z*z;
//
//	 	xy_sum += x*y;
//	 	xz_sum += x*z;
//	 	yz_sum += y*z;
//	 }
//
//	 printf ("x_sum = %.3lf\n", x_sum);
//	 printf ("y_sum = %.3lf\n", y_sum);
//	 printf ("z_sum = %.3lf\n", z_sum);
//
//	 printf ("x2_sum = %.3lf\n", x2_sum);
//	 printf ("y2_sum = %.3lf\n", y2_sum);
//	 printf ("z2_sum = %.3lf\n", z2_sum);
//
//	 printf ("xy_sum = %.3lf\n", xy_sum);
//	 printf ("xz_sum = %.3lf\n", xz_sum);
//	 printf ("yz_sum = %.3lf\n", yz_sum);
//
//	double** A = new double* [9];
//	if (!A)
//	{
//		printf(">>> allocation error --> array A in define_plane\n");
//		return;
//	}
//
//	for (i = 0; i < 3; ++i)
//	{
//		A[i] = new double[3];
//
//		if (!A[i])
//		{
//			printf(">>> allocation error --> array A[%d] in define_plane\n", i);
//			if (i > 0)
//				for (j = 0; j < i; ++j)
//					delete[] A[j];
//		}
//	}
//
//	A[0][0] = x2_sum - x_sum * x_sum / len;
//	A[0][1] = xy_sum - x_sum * y_sum / len;
//	A[0][2] = xz_sum - x_sum * z_sum / len;
//	A[1][0] = xy_sum - x_sum * y_sum / len;
//	A[1][1] = y2_sum - y_sum * y_sum / len;
//	A[1][2] = yz_sum - y_sum * z_sum / len;
//	A[2][0] = xz_sum - x_sum * z_sum / len;
//	A[2][1] = yz_sum - y_sum * z_sum / len;
//	A[2][2] = z2_sum - z_sum * z_sum / len;
//
//	 printf ("\n||\t%.3lf-lam\t%.3lf\t\t%.3lf\t\t||\n", A[0][0], A[0][1], A[0][2]);
//	 printf ("||\t%.3lf\t\t%.3lf-lam\t%.3lf\t\t||\n", A[1][0], A[1][1], A[1][2]);
//	 printf ("||\t%.3lf\t\t%.3lf\t\t%.3lf-lam\t||\n\n", A[2][0], A[2][1], A[2][2]);
//
//	double* eigen_values = new double[3];
//	if (!eigen_values)
//	{
//		printf(">>> allocation error --> array eigen_values in define_plane\n");
//		delete[] A;
//		return;
//	}
//
//	double** eigen_vectors = new double* [3];
//	for (i = 0; i < 3; ++i)
//	{
//		eigen_vectors[i] = new double[3];
//		if (!eigen_vectors[i])
//		{
//			printf(">>> allocation error --> array eigen_vectors[%d] in define_plane\n", i);
//			for (j = 0; j < 3; ++j)
//				delete[] A[j];
//			delete[] eigen_values;
//			if (i > 0)
//				for (j = 0; j < i; ++j)
//					delete[] eigen_vectors[j];
//			return;
//		}
//	}
//
//	for (i = 0; i < 3; ++i)
//	{
//		eigen_values[i] = 0.0;
//		for (j = 0; j < 3; ++j)
//			eigen_vectors[i][j] = 0.0;
//	}
//
//	Jacobi(3, A, eigen_values, eigen_vectors);
//
//	 for (i = 0; i < 3; ++i)
//	 	printf("eigen_values[%d] = %.3lf\n", i, eigen_values[i]);
//	 printf("\n");
//	 for (i = 0; i < 3; ++i)
//	 	printf("eigen_vectors[%d] = (%.3lf, %.3lf, %.3lf)\n", i, eigen_vectors[i][0], eigen_vectors[i][1], eigen_vectors[i][2]);
//	 printf("\n");
//	 printf("norm check:\n");
//	for (i = 0; i < 3; ++i)
//	{
//		val_a = eigen_vectors[i][0];
//		val_b = eigen_vectors[i][1];
//		val_c = eigen_vectors[i][2];
//		printf("norm[%d] = %.3lf\n", i, val_a*val_a+val_b*val_b+val_c*val_c);
//	}
//	printf("\n");
//
//	double* a = new double[3];
//	if (!a)
//	{
//		printf(">>> allocation error --> array a in define_plane\n");
//		for (i = 0; i < 3; ++i)
//			delete[] A[i];
//		delete[] eigen_values;
//		for (i = 0; i < 3; ++i)
//			delete[] eigen_vectors[i];
//		return;
//	}
//
//	double* b = new double[3];
//	if (!b)
//	{
//		printf(">>> allocation error --> array b in define_plane\n");
//		for (i = 0; i < 3; ++i)
//			delete[] A[i];
//		delete[] eigen_values;
//		for (i = 0; i < 3; ++i)
//			delete[] eigen_vectors[i];
//		delete[] a;
//		return;
//	}
//
//	double* c = new double[3];
//	if (!c)
//	{
//		printf(">>> allocation error --> array c in define_plane\n");
//		for (i = 0; i < 3; ++i)
//			delete[] A[i];
//		delete[] eigen_values;
//		for (i = 0; i < 3; ++i)
//			delete[] eigen_vectors[i];
//		delete[] a;
//		delete[] b;
//		return;
//	}
//
//	double* d = new double[3];
//	if (!d)
//	{
//		printf(">>> allocation error --> array d in define_plane\n");
//		for (i = 0; i < 3; ++i)
//			delete[] A[i];
//		delete[] eigen_values;
//		for (i = 0; i < 3; ++i)
//			delete[] eigen_vectors[i];
//		delete[] a;
//		delete[] b;
//		delete[] c;
//		return;
//	}
//
//	for (i = 0; i < 3; ++i)
//	{
//		a[i] = 0.0;
//		b[i] = 0.0;
//		c[i] = 0.0;
//		d[i] = 0.0;
//	}
//
//	double* L = new double[3];
//	if (!L)
//	{
//		printf(">>> allocation error --> array d in define_plane\n");
//		for (i = 0; i < 3; ++i)
//			delete[] A[i];
//		delete[] eigen_values;
//		for (i = 0; i < 3; ++i)
//			delete[] eigen_vectors[i];
//		delete[] a;
//		delete[] b;
//		delete[] c;
//		delete[] d;
//		return;
//	}
//
//	for (i = 0; i < 3; ++i)
//		L[i] = 0.0;
//
//	for (i = 0; i < 3; ++i)
//	{
//		val_a = eigen_vectors[i][0];
//		val_b = eigen_vectors[i][1];
//		val_c = eigen_vectors[i][2];
//
//		a[i] = val_a;
//		b[i] = val_b;
//		c[i] = val_c;
//		d[i] = -(x_sum * val_a + y_sum * val_b + z_sum * val_c) / len;
//	}
//
//	for (i = 0; i < 3; ++i)
//	{
//		val_a = a[i];
//		val_b = b[i];
//		val_c = c[i];
//		val_d = d[i];
//		sum = 0.0;
//
//		 for (j = 0; j < len; ++j)
//		 	sum += (val_a*arr_x[j] + val_b*arr_y[j] + val_c*arr_z[j] + val_d)*(val_a*arr_x[j] + val_b*arr_y[j] + val_c*arr_z[j] + val_d);
//
//		for (j = 0; j < len; ++j)
//		{
//			index = Poly->side[side_index].vlist[j];
//
//			x = Poly->vertex[index].x;
//			y = Poly->vertex[index].y;
//			z = Poly->vertex[index].z;
//
//			sum += (val_a * x + val_b * y + val_c * z + val_d) * (val_a * x + val_b * y + val_c * z + val_d);
//		}
//
//		L[i] = sum;
//	}
//
//	 for (i = 0; i < 3; ++i)
//	 	printf("L[%d] = %.3lf\n", i, L[i]);
//	 printf("\n");
//
//	int flag = 0;
//
//	if (L[0] <= L[1])
//	{
//		if (L[0] <= L[2])
//			flag = 0;
//		else
//			flag = 2;
//	}
//	else
//	{
//		 L[0] > L[1]
//		if (L[1] <= L[2])
//			flag = 1;
//		else
//			flag = 2;
//	}
//
//	Poly->side[side_index].plane.norm.x = a[flag];
//	Poly->side[side_index].plane.norm.y = b[flag];
//	Poly->side[side_index].plane.norm.z = c[flag];
//	Poly->side[side_index].plane.dist = d[flag];
//
//	 printf("\n");
//	 printf("found plane:\n");
//	 printf("%.3lf x + %.3lf y + %.3lf z + %.3lf = 0\n", a[flag], b[flag], c[flag], d[flag]);
//	 printf("\n");
//
//	for (i = 0; i < 3; ++i)
//		delete[] A[i];
//	delete[] eigen_values;
//	for (i = 0; i < 3; ++i)
//		delete[] eigen_vectors[i];
//	delete[] a;
//	delete[] b;
//	delete[] c;
//	delete[] d;
//	delete[] L;
//
//	return;
//}

void DefinePlaneCoeff(int N, VECTOR* pts, PLANE & Res)
{
	int i, j;

	double x, y, z;
	double x_sum, y_sum, z_sum;
	double x2_sum, y2_sum, z2_sum;
	double xy_sum, xz_sum, yz_sum;
	double val_a, val_b, val_c, val_d, sum;

	x_sum = 0;
	y_sum = 0;
	z_sum = 0;
	x2_sum = 0;
	y2_sum = 0;
	z2_sum = 0;
	xy_sum = 0;
	xz_sum = 0;
	yz_sum = 0;

	int len = N;

	/*for (int i = 0; i < N; i++)
	{
		printf("(%lf, %lf, %lf)\n", pts[i].x, pts[i].y, pts[i].z);
	}*/

	for (i = 0; i < len; ++i)
	{
		x = pts[i].x;
		y = pts[i].y;
		z = pts[i].z;

		x_sum += x;
		y_sum += y;
		z_sum += z;

		x2_sum += x * x;
		y2_sum += y * y;
		z2_sum += z * z;

		xy_sum += x * y;
		xz_sum += x * z;
		yz_sum += y * z;
	}

	// for (i = 0; i < 6; ++i)
	// {
	// 	x = arr_x[i];
	// 	y = arr_y[i];
	// 	z = arr_z[i];

	// 	x_sum += x;
	// 	y_sum += y;
	// 	z_sum += z;

	// 	x2_sum += x*x;
	// 	y2_sum += y*y;
	// 	z2_sum += z*z;

	// 	xy_sum += x*y;
	// 	xz_sum += x*z;
	// 	yz_sum += y*z;
	// }

	// printf ("x_sum = %.3lf\n", x_sum);
	// printf ("y_sum = %.3lf\n", y_sum);
	// printf ("z_sum = %.3lf\n", z_sum);

	// printf ("x2_sum = %.3lf\n", x2_sum);
	// printf ("y2_sum = %.3lf\n", y2_sum);
	// printf ("z2_sum = %.3lf\n", z2_sum);

	// printf ("xy_sum = %.3lf\n", xy_sum);
	// printf ("xz_sum = %.3lf\n", xz_sum);
	// printf ("yz_sum = %.3lf\n", yz_sum);

	double** A = new double* [3];
	if (A == NULL)
	{
		printf(">>> allocation error --> array A in define_plane\n");
		return;
	}

	// printf("Here in define_plane1\n");

	for (i = 0; i < 3; ++i)
	{
		A[i] = new double[3];

		if (!A[i])
		{
			printf(">>> allocation error --> array A[%d] in define_plane\n", i);
			for (j = 0; j < i; ++j)
				delete[] A[j];

			delete[] A;
			break;
		}
	}

	A[0][0] = x2_sum - x_sum * x_sum / len;
	A[0][1] = xy_sum - x_sum * y_sum / len;
	A[0][2] = xz_sum - x_sum * z_sum / len;
	A[1][0] = xy_sum - x_sum * y_sum / len;
	A[1][1] = y2_sum - y_sum * y_sum / len;
	A[1][2] = yz_sum - y_sum * z_sum / len;
	A[2][0] = xz_sum - x_sum * z_sum / len;
	A[2][1] = yz_sum - y_sum * z_sum / len;
	A[2][2] = z2_sum - z_sum * z_sum / len;

	// printf("Here in define_plane0\n");

	// printf ("\n||\t%.3lf-lam\t%.3lf\t\t%.3lf\t\t||\n", A[0][0], A[0][1], A[0][2]);
	// printf ("||\t%.3lf\t\t%.3lf-lam\t%.3lf\t\t||\n", A[1][0], A[1][1], A[1][2]);
	// printf ("||\t%.3lf\t\t%.3lf\t\t%.3lf-lam\t||\n\n", A[2][0], A[2][1], A[2][2]);

	double* eigen_values = new double[3];
	if (!eigen_values)
	{
		printf(">>> allocation error --> array eigen_values in define_plane\n");
		for (j = 0; j < 3; ++j)
			delete[] A[j];
		delete[] A;
		return;
	}

	double** eigen_vectors = new double* [3];
	for (i = 0; i < 3; ++i)
	{
		eigen_vectors[i] = new double[3];
		if (!eigen_vectors[i])
		{
			printf(">>> allocation error --> array eigen_vectors[%d] in define_plane\n", i);
			for (j = 0; j < 3; ++j)
				delete[] A[j];
			delete[] eigen_values;
			for (j = 0; j < i; ++j)
				delete[] eigen_vectors[j];

			delete[] eigen_vectors;
			delete[] A;
			return;
		}
	}

	for (i = 0; i < 3; ++i)
	{
		eigen_values[i] = 0.0;
		for (j = 0; j < 3; ++j)
			eigen_vectors[i][j] = 0.0;
	}

	// printf("Before Jacobi :3\n");

	// for (i = 0; i < 3; ++i)
	// {
	// 	for (j = 0; j < 3; ++j)
	// 		printf("%lf ", A[i][j]);
	// 	printf("\n");
	// }

	Jacobi(3, A, eigen_values, eigen_vectors);

	// for (i = 0; i < 3; ++i)
	// 	printf("eigen_values[%d] = %.3lf\n", i, eigen_values[i]);
	// printf("\n");
	// for (i = 0; i < 3; ++i)
	// 	printf("eigen_vectors[%d] = (%.3lf, %.3lf, %.3lf)\n", i, eigen_vectors[i][0], eigen_vectors[i][1], eigen_vectors[i][2]);
	// printf("\n");
	// printf("norm check:\n");
	//for (i = 0; i < 3; ++i)
	//{
	//	val_a = eigen_vectors[i][0];
	//	val_b = eigen_vectors[i][1];
	//	val_c = eigen_vectors[i][2];
	//	//printf("norm[%d] = %.3lf\n", i, val_a*val_a+val_b*val_b+val_c*val_c);
	//}
	//printf("\n");

	double* a = new double[3];
	if (!a)
	{
		printf(">>> allocation error --> array a in define_plane\n");
		for (i = 0; i < 3; ++i)
			delete[] A[i];
		delete[] eigen_values;
		for (i = 0; i < 3; ++i)
			delete[] eigen_vectors[i];
		delete[] eigen_vectors;
		delete[] A;
		return;
	}

	double* b = new double[3];
	if (!b)
	{
		printf(">>> allocation error --> array b in define_plane\n");
		for (i = 0; i < 3; ++i)
			delete[] A[i];
		delete[] eigen_values;
		for (i = 0; i < 3; ++i)
			delete[] eigen_vectors[i];
		delete[] eigen_vectors;
		delete[] A;
		delete[] a;
		return;
	}

	double* c = new double[3];
	if (!c)
	{
		printf(">>> allocation error --> array c in define_plane\n");
		for (i = 0; i < 3; ++i)
			delete[] A[i];
		delete[] eigen_values;
		for (i = 0; i < 3; ++i)
			delete[] eigen_vectors[i];
		delete[] eigen_vectors;
		delete[] A;
		delete[] a;
		delete[] b;
		return;
	}

	double* d = new double[3];
	if (!d)
	{
		printf(">>> allocation error --> array d in define_plane\n");
		for (i = 0; i < 3; ++i)
			delete[] A[i];
		delete[] eigen_values;
		for (i = 0; i < 3; ++i)
			delete[] eigen_vectors[i];
		delete[] eigen_vectors;
		delete[] A;
		delete[] a;
		delete[] b;
		delete[] c;
		return;
	}

	for (i = 0; i < 3; ++i)
	{
		a[i] = 0.0;
		b[i] = 0.0;
		c[i] = 0.0;
		d[i] = 0.0;
	}

	double* L = new double[3];
	if (!L)
	{
		printf(">>> allocation error --> array d in define_plane\n");
		for (i = 0; i < 3; ++i)
			delete[] A[i];
		delete[] eigen_values;
		for (i = 0; i < 3; ++i)
			delete[] eigen_vectors[i];
		delete[] eigen_vectors;
		delete[] A;
		delete[] a;
		delete[] b;
		delete[] c;
		delete[] d;
		return;
	}

	for (i = 0; i < 3; ++i)
		L[i] = 0.0;

	for (i = 0; i < 3; ++i)
	{
		val_a = eigen_vectors[0][i];
		val_b = eigen_vectors[1][i];
		val_c = eigen_vectors[2][i];

		a[i] = val_a;
		b[i] = val_b;
		c[i] = val_c;
		d[i] = -(x_sum * val_a + y_sum * val_b + z_sum * val_c) / len;
	}

	for (i = 0; i < 3; ++i)
	{
		val_a = a[i];
		val_b = b[i];
		val_c = c[i];
		val_d = d[i];
		sum = 0.0;

		// for (j = 0; j < len; ++j)
		// 	sum += (val_a*arr_x[j] + val_b*arr_y[j] + val_c*arr_z[j] + val_d)*(val_a*arr_x[j] + val_b*arr_y[j] + val_c*arr_z[j] + val_d);

		for (j = 0; j < len; ++j)
		{
			x = pts[j].x;
			y = pts[j].y;
			z = pts[j].z;

			sum += (val_a * x + val_b * y + val_c * z + val_d) * (val_a * x + val_b * y + val_c * z + val_d);
		}

		L[i] = sum;
	}

	/* for (i = 0; i < 3; ++i)
	 	printf("L[%d] = %.3lf\n", i, L[i]);
	 printf("\n");*/

	int flag = 0;

	if (L[0] <= L[1])
	{
		if (L[0] <= L[2])
			flag = 0;
		else
			flag = 2;
	}
	else
	{
		// L[0] > L[1]
		if (L[1] <= L[2])
			flag = 1;
		else
			flag = 2;
	}

	Res.norm.x = a[flag];
	Res.norm.y = b[flag];
	Res.norm.z = c[flag];
	Res.dist = d[flag];

	 /*printf("\n");
	 printf("found plane:\n");
	 printf("%.3lf x + %.3lf y + %.3lf z + %.3lf = 0\n", a[flag], b[flag], c[flag], d[flag]);
	 printf("\n");*/

	for (i = 0; i < 3; ++i)
		delete[] A[i];
	delete[] eigen_values;
	for (i = 0; i < 3; ++i)
		delete[] eigen_vectors[i];
	delete[] eigen_vectors;
	delete[] A;
	delete[] a;
	delete[] b;
	delete[] c;
	delete[] d;
	delete[] L;

	return;
}