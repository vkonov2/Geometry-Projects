#include "ReadFunctions.h"
#include <math.h>
#include <stdio.h>

#include "Src/Mathem/Jacobi.h"
#include "Src/Mathem/Jacobi.cpp"

// void define_plane(double* arr_x, double* arr_y, double* arr_z)
void define_plane(POLYHEDRON* Poly, int side_index)
{
	int i, j, index;

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

	int len = length(Poly->side[side_index].vlist) - 1;
	//int len = 6;

	for (i = 0; i < len; ++i)
	{
		index = Poly->side[side_index].vlist[i];

		x = Poly->vertex[index].x;
		y = Poly->vertex[index].y;
		z = Poly->vertex[index].z;

		x_sum += x;
		y_sum += y;
		z_sum += z;

		x2_sum += x*x;
		y2_sum += y*y;
		z2_sum += z*z;

		xy_sum += x*y;
		xz_sum += x*z;
		yz_sum += y*z;
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

	double** A = new double*[9];
	if (!A)
	{
		printf(">>> allocation error --> array A in define_plane\n");
		return;
	}

	for (i = 0; i < 3; ++i)
	{
		A[i] = new double [3];

		if (!A[i])
		{
			printf(">>> allocation error --> array A[%d] in define_plane\n", i);
			if (i > 0)
				for (j = 0; j < i; ++j)
					delete [] A[j];
		}
	}

	A[0][0] = x2_sum - x_sum*x_sum/len;
	A[0][1] = xy_sum - x_sum*y_sum/len;
	A[0][2] = xz_sum - x_sum*z_sum/len;
	A[1][0] = xy_sum - x_sum*y_sum/len;
	A[1][1] = y2_sum - y_sum*y_sum/len;
	A[1][2] = yz_sum - y_sum*z_sum/len;
	A[2][0] = xz_sum - x_sum*z_sum/len;
	A[2][1] = yz_sum - y_sum*z_sum/len;
	A[2][2] = z2_sum - z_sum*z_sum/len;

	// printf ("\n||\t%.3lf-lam\t%.3lf\t\t%.3lf\t\t||\n", A[0][0], A[0][1], A[0][2]);
	// printf ("||\t%.3lf\t\t%.3lf-lam\t%.3lf\t\t||\n", A[1][0], A[1][1], A[1][2]);
	// printf ("||\t%.3lf\t\t%.3lf\t\t%.3lf-lam\t||\n\n", A[2][0], A[2][1], A[2][2]);

	double* eigen_values = new double[3];
	if (!eigen_values)
	{
		printf(">>> allocation error --> array eigen_values in define_plane\n");
		delete [] A;
		return;
	}

	double** eigen_vectors = new double*[3];
	for (i = 0; i < 3; ++i)
	{
		eigen_vectors[i] = new double [3];
		if (!eigen_vectors[i])
		{
			printf(">>> allocation error --> array eigen_vectors[%d] in define_plane\n", i);
			for (j = 0; j < 3; ++j)
				delete [] A[j];
			delete [] eigen_values;
			if (i > 0)
				for (j = 0; j < i; ++j)
					delete [] eigen_vectors[j];
			return;
		}
	}

	for (i = 0; i < 3; ++i)
	{
		eigen_values[i] = 0.0;
		for (j = 0; j < 3; ++j)
			eigen_vectors[i][j] = 0.0;
	}

	Jacobi(3, A, eigen_values, eigen_vectors);

	// for (i = 0; i < 3; ++i)
	// 	printf("eigen_values[%d] = %.3lf\n", i, eigen_values[i]);
	// printf("\n");
	// for (i = 0; i < 3; ++i)
	// 	printf("eigen_vectors[%d] = (%.3lf, %.3lf, %.3lf)\n", i, eigen_vectors[i][0], eigen_vectors[i][1], eigen_vectors[i][2]);
	// printf("\n");
	// printf("norm check:\n");
	for (i = 0; i < 3; ++i)
	{
		val_a = eigen_vectors[i][0];
		val_b = eigen_vectors[i][1];
		val_c = eigen_vectors[i][2];
		//printf("norm[%d] = %.3lf\n", i, val_a*val_a+val_b*val_b+val_c*val_c);
	}
	//printf("\n");

	double* a = new double[3];
	if (!a)
	{
		printf(">>> allocation error --> array a in define_plane\n");
		for (i = 0; i < 3; ++i)
			delete [] A[i];
		delete [] eigen_values;
		for (i = 0; i < 3; ++i)
			delete [] eigen_vectors[i];
		return;
	}

	double* b = new double[3];
	if (!b)
	{
		printf(">>> allocation error --> array b in define_plane\n");
		for (i = 0; i < 3; ++i)
			delete [] A[i];
		delete [] eigen_values;
		for (i = 0; i < 3; ++i)
			delete [] eigen_vectors[i];
		delete [] a;
		return;
	}

	double* c = new double[3];
	if (!c)
	{
		printf(">>> allocation error --> array c in define_plane\n");
		for (i = 0; i < 3; ++i)
			delete [] A[i];
		delete [] eigen_values;
		for (i = 0; i < 3; ++i)
			delete [] eigen_vectors[i];
		delete [] a;
		delete [] b;
		return;
	}

	double* d = new double[3];
	if (!d)
	{
		printf(">>> allocation error --> array d in define_plane\n");
		for (i = 0; i < 3; ++i)
			delete [] A[i];
		delete [] eigen_values;
		for (i = 0; i < 3; ++i)
			delete [] eigen_vectors[i];
		delete [] a;
		delete [] b;
		delete [] c;
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
			delete [] A[i];
		delete [] eigen_values;
		for (i = 0; i < 3; ++i)
			delete [] eigen_vectors[i];
		delete [] a;
		delete [] b;
		delete [] c;
		delete [] d;
		return;
	}

	for (i = 0; i < 3; ++i)
		L[i] = 0.0;

	for (i = 0; i < 3; ++i)
	{
		val_a = eigen_vectors[i][0];
		val_b = eigen_vectors[i][1];
		val_c = eigen_vectors[i][2];

		a[i] = val_a;
		b[i] = val_b;
		c[i] = val_c;
		d[i] = - (x_sum*val_a + y_sum*val_b + z_sum*val_c)/len;
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
			index = Poly->side[side_index].vlist[j];

			x = Poly->vertex[index].x;
			y = Poly->vertex[index].y;
			z = Poly->vertex[index].z;

			sum += (val_a*x + val_b*y + val_c*z + val_d)*(val_a*x + val_b*y + val_c*z + val_d);
		}

		L[i] = sum;
	}

	// for (i = 0; i < 3; ++i)
	// 	printf("L[%d] = %.3lf\n", i, L[i]);
	// printf("\n");

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

	Poly->side[side_index].plane.norm.x = a[flag];
	Poly->side[side_index].plane.norm.y = b[flag];
	Poly->side[side_index].plane.norm.z = c[flag];
	Poly->side[side_index].plane.dist = d[flag];

	// printf("\n");
	// printf("found plane:\n");
	// printf("%.3lf x + %.3lf y + %.3lf z + %.3lf = 0\n", a[flag], b[flag], c[flag], d[flag]);
	// printf("\n");

	for (i = 0; i < 3; ++i)
		delete [] A[i];
	delete [] eigen_values;
	for (i = 0; i < 3; ++i)
		delete [] eigen_vectors[i];
	delete [] a;
	delete [] b;
	delete [] c;
	delete [] d;
	delete [] L;
	
	return;	
}

int main (void)
{
	double* x = new double[6];
	double* y = new double[6];
	double* z = new double[6];

	x[0] = 1;
	y[0] = 1;
	z[0] = 1;

	x[1] = 1;
	y[1] = -1;
	z[1] = 1;

	x[2] = 2;
	y[2] = 2;
	z[2] = 2;

	x[3] = 2;
	y[3] = -2;
	z[3] = 2;

	x[4] = 3;
	y[4] = 3;
	z[4] = 3;

	x[5] = 3;
	y[5] = -3;
	z[5] = 3;

	define_plane (x, y, z);

	delete [] x;
	delete [] y;
	delete [] z;

	return 1;
}