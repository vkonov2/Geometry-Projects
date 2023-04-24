#define _CRT_SECURE_NO_WARNINGS

#include "ComparisonTest.h"
#include <string>
#define accuracy 100

using namespace std;


void swap(int* a, int i, int j)
{
    int s = a[i];
    a[i] = a[j];
    a[j] = s;
}

bool NextPermutation(int* a, int n)
{
    int j = n - 2;
    while (j != -1 && a[j] >= a[j + 1]) j--;
    if (j == -1)
        return false; 
    int k = n - 1;
    while (a[j] >= a[k]) k--;
    swap(a, j, k);
    int l = j + 1, r = n - 1; 
    while (l < r)
        swap(a, l++, r--);
    return true;
}

int ComparisonTest(POLYHEDRON* Pattern_Poly, POLYHEDRON* Real_Poly)
{
	int Reflection[24];

	Reflection[0] = -1;
	Reflection[1] = 1;
	Reflection[2] = 1;

	Reflection[3] = 1;
	Reflection[4] = -1;
	Reflection[5] = 1;

	Reflection[6] = 1;
	Reflection[7] = 1;
	Reflection[8] = -1;

	Reflection[9] = -1;
	Reflection[10] = -1;
	Reflection[11] = 1;

	Reflection[12] = -1;
	Reflection[13] = 1;
	Reflection[14] = -1;

	Reflection[15] = 1;
	Reflection[16] = -1;
	Reflection[17] = -1;

	Reflection[18] = -1;
	Reflection[19] = -1;
	Reflection[20] = -1;

	Reflection[21] = 1;
	Reflection[22] = 1;
	Reflection[23] = 1;

	int Pattern_conv_hull_count = 0;
	int Real_conv_hull_count = 0;
	int a1, a2, a3;
	int m0, m1, m2;
	int BestReflectionId;
	int L2_norm_id = 0, j = 0; //L2_norm_id - номер перестановки, имеющей наименьший L2diff
	int err = 0;
	double L2_diff[6][8][4];
	double L2_norm = 0.0;
	VECTOR v_null(0, 0, 0);
	SPIN spin_forward;
	SPIN spin_backward;
	PlaneInfo* Pattern_Info = new PlaneInfo[Pattern_Poly->nsides];

	WAssert(Pattern_Info, "Can't allocate memory for Pattern_Info in ComparisonTest.cpp\n");

	PlaneInfo* Real_Info = new PlaneInfo[Real_Poly->nsides];

	if (!Real_Info)
	{
		delete[] Pattern_Info;
	}

	WAssert(Real_Info, "Can't allocate memory for Real_Info in ComparisonTest.cpp\n");

	double* eigen_values = new double[3];

	if (!eigen_values)
	{
		delete[] Pattern_Info;
		delete[] Real_Info;
	}

	WAssert(eigen_values, "Can't allocate memory for eigen_values in ComparisonTest.cpp\n");

	double** eigen_vectors = new double* [3];

	if (!eigen_vectors)
	{
		delete[] eigen_values;
		delete[] Pattern_Info;
		delete[] Real_Info;
	}

	WAssert(eigen_vectors, "Can't allocate memory for eigen_vectors in ComparisonTest.cpp\n");

	for (int i = 0; i < 3; ++i)
	{
		eigen_vectors[i] = new double[3];

		if (!eigen_vectors[i])
		{
			for (int j = 0; j < i; j++)
			{
				delete[] eigen_vectors[j];
			}

			delete[] eigen_values;
			delete[] eigen_vectors;
			delete[] Pattern_Info;
			delete[] Real_Info;
		}

		WAssert(eigen_vectors[i], "Can't allocate memory for eigen_vectors[%d] in ComparisonTest.cpp\n", i);
	}

	printf("Pattern polyhedron\n");
	MassCenterZeroing(Pattern_Poly, Pattern_Info);
	err = Axis(Pattern_Poly, eigen_vectors, eigen_values, Pattern_Info);

	printf("Eigen values are %lf, %lf, %lf\n", eigen_values[0], eigen_values[1], eigen_values[2]);

	if (err < 0)
	{
		for (int j = 0; j < 3; j++)
		{
			delete[] eigen_vectors[j];
		}

		delete[] eigen_values;
		delete[] Pattern_Info;
		delete[] Real_Info;
		delete[] eigen_vectors;
	}

	WAssert(err == 0, "Problems in Axis() for Pattern_Poly in ComparisonTest.cpp\n");

	VECTOR3 Pattern_eigen[3];

	if (!Pattern_eigen)
	{
		for (int j = 0; j < 3; j++)
		{
			delete[] eigen_vectors[j];
		}

		delete[] eigen_values;
		delete[] eigen_vectors;
		delete[] Pattern_Info;
		delete[] Real_Info;
	}

	WAssert(Pattern_eigen, "Can't allocate memory for Pattern_eigen in ComparisonTest.cpp\n");

	for (int i = 0; i < 3; i++)
	{
		Pattern_eigen[i].x = eigen_vectors[0][i];
		Pattern_eigen[i].y = eigen_vectors[1][i];
		Pattern_eigen[i].z = eigen_vectors[2][i];
	}

	printf("Eigen vectors:\n");
	for (int i = 0; i < 3; ++i)
		printf("(%lf, %lf, %lf)\n", Pattern_eigen[i].x, Pattern_eigen[i].y
			, Pattern_eigen[i].z);

	printf("Real polyhedron\n");
	MassCenterZeroing(Real_Poly, Real_Info);
	err = Axis(Real_Poly, eigen_vectors, eigen_values, Real_Info);

	if (err < 0)
	{
		for (int j = 0; j < 3; j++)
		{
			delete[] eigen_vectors[j];
		}

		delete[] eigen_values;
		delete[] eigen_vectors;
		delete[] Pattern_Info;
		delete[] Real_Info;
	}

	WAssert(err == 0, "Problems in Axis() for Real_Poly in ComparisonTest.cpp\n");

	VECTOR3 Real_eigen[3];

	if (!Real_eigen)
	{
		for (int j = 0; j < 3; j++)
		{
			delete[] eigen_vectors[j];
		}

		delete[] eigen_values;
		delete[] eigen_vectors;
		delete[] Pattern_Info;
		delete[] Real_Info;
	}

	WAssert(Real_eigen, "Can't allocate memory for Real_eigen in ComparisonTest.cpp\n");

	for (int i = 0; i < 3; i++)
	{
		Real_eigen[i].x = eigen_vectors[0][i];
		Real_eigen[i].y = eigen_vectors[1][i];
		Real_eigen[i].z = eigen_vectors[2][i];
	}

	printf("Eigen vectors:\n");
	for (int i = 0; i < 3; ++i)
		printf("(%lf, %lf, %lf)\n", Real_eigen[i].x, Real_eigen[i].y
			, Real_eigen[i].z);

	int* a = new int[3];

	if (!a)
	{
		for (int j = 0; j < 3; j++)
		{
			delete[] eigen_vectors[j];
		}

		delete[] eigen_values;
		delete[] eigen_vectors;
		delete[] Pattern_Info;
		delete[] Real_Info;
	}

	WAssert(a, "Can't allocate memory for a in ComparisonTest.cpp\n");

	a[0] = 0;
	a[1] = 1;
	a[2] = 2;

	int* Real_hullInds = new int[Real_Poly->nverts];

	if (!Real_hullInds)
	{
		for (int j = 0; j < 3; j++)
		{
			delete[] eigen_vectors[j];
		}

		delete[] eigen_values;
		delete[] eigen_vectors;
		delete[] Pattern_Info;
		delete[] Real_Info;
		delete[] a;
	}

	WAssert(Real_hullInds, "Can't allocate memory for Real_hullInds in ComparisonTest.cpp\n");

	VECTOR2* Real_projection = new VECTOR2[Real_Poly->nverts];

	if (!Real_projection)
	{
		for (int j = 0; j < 3; j++)
		{
			delete[] eigen_vectors[j];
		}

		delete[] eigen_values;
		delete[] eigen_vectors;
		delete[] Pattern_Info;
		delete[] Real_Info;
		delete[] a;
		delete[] Real_hullInds;
	}

	WAssert(Real_projection, "Can't allocate memory for Real_projection in ComparisonTest.cpp\n");

	int* Pattern_hullInds = new int[Pattern_Poly->nverts];

	if (!Pattern_hullInds)
	{
		for (int j = 0; j < 3; j++)
		{
			delete[] eigen_vectors[j];
		}

		delete[] eigen_values;
		delete[] eigen_vectors;
		delete[] Pattern_Info;
		delete[] Real_Info;
		delete[] a;
		delete[] Real_hullInds;
		delete[] Real_projection;
	}

	WAssert(Pattern_hullInds, "Can't allocate memory for Pattern_hullInds in ComparisonTest.cpp\n");

	VECTOR2* Pattern_projection = new VECTOR2[Pattern_Poly->nverts];

	if (!Pattern_projection)
	{
		for (int j = 0; j < 3; j++)
		{
			delete[] eigen_vectors[j];
		}

		delete[] eigen_values;
		delete[] eigen_vectors;
		delete[] Pattern_Info;
		delete[] Real_Info;
		delete[] a;
		delete[] Real_hullInds;
		delete[] Real_projection;
		delete[] Pattern_hullInds;
	}

	WAssert(Pattern_projection, "Can't allocate memory for Pattern_projection in ComparisonTest.cpp\n");

	for (int i = 0; i < Pattern_Poly->nverts; i++)
	{
		Pattern_projection[i].x = v_null.x;
		Pattern_projection[i].y = v_null.y;
	}

	for (int i = 0; i < Real_Poly->nverts; i++)
	{
		Real_projection[i].x = v_null.x;
		Real_projection[i].y = v_null.y;
	}

	for (int k = 0; k < 8; k++)
	{
		m0 = Reflection[k * 3];
		m1 = Reflection[k * 3 + 1];
		m2 = Reflection[k * 3 + 2];

		EulerTransform(m0 * Real_eigen[0], m1 * Real_eigen[1], m2 * Real_eigen[2],
			Pattern_eigen[0], Pattern_eigen[1], Pattern_eigen[2], spin_forward);
		spin_backward = ~spin_forward;

		CONFORM con_forward(spin_forward, v_null, 1);
		SIMILARITY sim_forward(con_forward);

		TransformPolyhedron(Real_Poly, sim_forward);

		for (int i = 0; i < 3; ++i)
		{
			Real_conv_hull_count = ConvexHullOfProjection(Real_Poly, Pattern_eigen[i], Real_hullInds, Real_projection);
			Pattern_conv_hull_count = ConvexHullOfProjection(Pattern_Poly, Pattern_eigen[i], Pattern_hullInds, Pattern_projection);

			Polar Real_p_s(accuracy, Real_Poly->nverts, Real_projection, Real_conv_hull_count, Real_hullInds);
			Polar Pattern_p_s(accuracy, Pattern_Poly->nverts, Pattern_projection, Pattern_conv_hull_count, Pattern_hullInds);

			L2_diff[j][k][i] = Pattern_p_s.L2Diff(Real_p_s);
		}

		L2_diff[j][k][3] = sqrt(L2_diff[j][k][0] * L2_diff[j][k][0] + L2_diff[j][k][1] * L2_diff[j][k][1] + L2_diff[j][k][2] * L2_diff[j][k][2]);

		if (j + k == 0)
		{
			L2_norm = L2_diff[j][k][3];
			BestReflectionId = k;
			L2_norm_id = j;
		}

		else if (L2_norm > L2_diff[j][k][3])
		{
			L2_norm = L2_diff[j][k][3];
			BestReflectionId = k;
			L2_norm_id = j;
		}

		CONFORM con_back(spin_backward, v_null, 1);
		SIMILARITY sim_back(con_back);

		TransformPolyhedron(Real_Poly, sim_back);

		con_back.~CONFORM();
		sim_back.~SIMILARITY();
		con_forward.~CONFORM();
		sim_forward.~SIMILARITY();
	}

	j++;

	while (NextPermutation(a, 3))
	{
		a1 = a[0];
		a2 = a[1];
		a3 = a[2];

		for (int k = 0; k < 8; k++)
		{
			m0 = Reflection[k * 3];
			m1 = Reflection[k * 3 + 1];
			m2 = Reflection[k * 3 + 2];

			EulerTransform(m0 * Real_eigen[a1], m1 * Real_eigen[a2], m2 * Real_eigen[a3],
				Pattern_eigen[0], Pattern_eigen[1], Pattern_eigen[2], spin_forward);

			spin_backward = ~spin_forward;

			CONFORM con_forward(spin_forward, v_null, 1);
			SIMILARITY sim_forward(con_forward);

			TransformPolyhedron(Real_Poly, sim_forward);

			for (int i = 0; i < 3; ++i)
			{
				Real_conv_hull_count = ConvexHullOfProjection(Real_Poly, Pattern_eigen[i], Real_hullInds, Real_projection);
				Pattern_conv_hull_count = ConvexHullOfProjection(Pattern_Poly, Pattern_eigen[i], Pattern_hullInds, Pattern_projection);

				Polar Real_p_s(accuracy, Real_Poly->nverts, Real_projection, Real_conv_hull_count, Real_hullInds);
				Polar Pattern_p_s(accuracy, Pattern_Poly->nverts, Pattern_projection, Pattern_conv_hull_count, Pattern_hullInds);

				L2_diff[j][k][i] = Pattern_p_s.L2Diff(Real_p_s);
			}

			L2_diff[j][k][3] = sqrt(L2_diff[j][k][0] * L2_diff[j][k][0] + L2_diff[j][k][1] * L2_diff[j][k][1] + L2_diff[j][k][2] * L2_diff[j][k][2]);

			if (L2_norm > L2_diff[j][k][3])
			{
				L2_norm = L2_diff[j][k][3];
				BestReflectionId = k;
				L2_norm_id = j;
			}

			CONFORM con_back(spin_backward, v_null, 1);
			SIMILARITY sim_back(con_back);

			TransformPolyhedron(Real_Poly, sim_back);

			con_back.~CONFORM();
			sim_back.~SIMILARITY();
			con_forward.~CONFORM();
			sim_forward.~SIMILARITY();

		}

		j++;
	}

	j = 0;
	a[0] = 0;
	a[1] = 1;
	a[2] = 2;

	int k = BestReflectionId;

	m0 = Reflection[k * 3];
	m1 = Reflection[k * 3 + 1];
	m2 = Reflection[k * 3 + 2];

	if (j == L2_norm_id)
	{
		a1 = a[0];
		a2 = a[1];
		a3 = a[2];
	}
	else
	{
		while (NextPermutation(a, 3))
		{
			j++;
			if (j == L2_norm_id)
			{
				a1 = a[0];
				a2 = a[1];
				a3 = a[2];
				break;
			}
		}
	}

	EulerTransform(m0 * Real_eigen[a1], m1 * Real_eigen[a2], m2 * Real_eigen[a3],
		Pattern_eigen[0], Pattern_eigen[1], Pattern_eigen[2], spin_forward);

	spin_backward = ~spin_forward;

	CONFORM cf(spin_forward, v_null, 1);
	SIMILARITY sf(cf);

	TransformPolyhedron(Real_Poly, sf);

	for (int j = 0; j < 3; j++)
	{
		delete[] eigen_vectors[j];
	}

	delete[] a;
	delete[] eigen_vectors;
	delete[] eigen_values;
	delete[] Real_hullInds;
	delete[] Real_projection;
	delete[] Pattern_hullInds;
	delete[] Pattern_projection;
	delete[] Pattern_Info;
	delete[] Real_Info;

	return 0;
}