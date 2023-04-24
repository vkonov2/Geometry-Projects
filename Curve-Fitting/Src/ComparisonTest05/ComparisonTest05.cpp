#define _CRT_SECURE_NO_WARNINGS

#include "ComparisonTest05.h"

void DrawSideCenters(POLYHEDRON* Poly, int id0, int id1)
{
	PlaneInfo* Info = new PlaneInfo[Poly->nsides];
	MassCenterZeroing(Poly, Info);

	const string fullDirname__string = "D:\\YandexDisk\\Ëèçà\\Octonus\\SymSides" + string(to_string(id0)) + "_" + string(to_string(id1)) + string(".txt");
	const char* FullDirName = fullDirname__string.c_str();

	FILE* f = fopen(FullDirName, "w");

	fprintf(f, "%lf, %lf, %lf\n", Info[id0].x, Info[id0].y, Info[id0].z);
	fprintf(f, "%lf, %lf, %lf", Info[id1].x, Info[id1].y, Info[id1].z);

	fclose(f);

	delete[] Info;
}

void DrawSideProj(VECTOR3 N, vector <VECTOR3> Pts, int id)
{
	vector < VECTOR2 > Projection;

	FindProjection(N, Pts, Projection);

	const string fullDirname__string = "D:\\YandexDisk\\Ëèçà\\Octonus\\Projection" + string(to_string(id)) + string(".txt");
	const char* FullDirName = fullDirname__string.c_str();

	FILE* f = fopen(FullDirName, "w");

	for (int i = 0; i < (int)(Projection.size()); i++)
		fprintf(f, "%lf, %lf\n", Projection[i].x, Projection[i].y);

	fprintf(f, "%lf, %lf\n", Projection[0].x, Projection[0].y);

	fclose(f);

	Projection.clear();
}

void ComparissonTest05(POLYHEDRON* Ref, POLYHEDRON* Cur)
{
	system("del /q D:\\YandexDisk\\Ëèçà\\Octonus\\*.*");

	int ns = Ref->nsides;
	double SumNorm = 0.0;
	//ns -= 230;
	ns = 4;
	int len, id;
	VECTOR CurrSideNorm, SideNorm, ProjNorm;
	vector<vector <double>> L2errors;
	vector <double> CurrSideL2errors, Err;
	vector <VECTOR3> CurrSidePts, SidePts;
	PlaneInfo* Info = new PlaneInfo[Ref->nsides];
	MassCenterZeroing(Ref, Info);
	PLANE P;
	ns = 9;

	for (int i = 0; i < ns - 1; i++)
	{
		printf("%d\\%d\n", i, ns);
		CurrSideL2errors.clear();
		CurrSidePts.clear();
		len = length(Ref->side[i].vlist) - 1;
		CurrSideNorm = Ref->side[i].plane.norm;
		CurrSideNorm /= CurrSideNorm.abs();

		for (int j = 0; j < len; j++)
		{
			id = Ref->side[i].vlist[j];
			CurrSidePts.push_back(Ref->vertex[id]);
		}

		for (int j = i + 1; j < ns; j++)
		{
			SidePts.clear();
			len = length(Ref->side[j].vlist) - 1;
			SideNorm = Ref->side[j].plane.norm;
			SideNorm /= SideNorm.abs();

			for (int k = 0; k < len; k++)
			{
				id = Ref->side[j].vlist[k];
				SidePts.push_back(Ref->vertex[id]);
			}

			ProjNorm = CurrSideNorm - SideNorm;
			ProjNorm /= ProjNorm.abs();

			Err.clear();

			AngleMatchPolygons(CurrSidePts, SidePts, ProjNorm, Err);

			CurrSideL2errors.push_back(Err[0]);
		}

		L2errors.push_back(CurrSideL2errors);

		auto it = min_element(L2errors[i].begin(), L2errors[i].end());
		printf("*it = %lf\n", *it);

		SumNorm += *it;
	}

	/*for (int i = 0; i < 1; i++)
	{
		for (int j = 0; j < ns - i - 1; j++)
			printf("%lf ", L2errors[i][j]);
		printf("\n");
	}*/

	int* SymmSideId = new int[ns];
	double* Error = new double[ns];
	//ns -= 220;
	printf("SumNorm = %lf\n", SumNorm);

	for (int i = 0; i < ns - 1; i++)
	{
		L2errors[i][0] /= SumNorm;
		Error[i] = L2errors[i][0];
		SymmSideId[i] = i + 1;

		for (int j = 0; j < ns - i - 1; j++)
		{
			L2errors[i][j] /= SumNorm;

			if (Error[i] > L2errors[i][j])
			{
				Error[i] = L2errors[i][j];
				SymmSideId[i] = j + i + 1;
			}
		}
	}

	double err = Error[0];
	id = SymmSideId[0];
	int I = 0;

	for (int i = 0; i < ns - 1; i++)
	{
		if (err > Error[i])
		{
			err = Error[i];
			id = SymmSideId[i];
			I = i;
		}
	}

	//printf("Sym %lf < %lf Best\n", L2errors[0][3], L2errors[0][SymmSideId]);
	vector <VECTOR3> CrossSection;
	FILE* f;
	printf("I = %d, id = %d\n", I, id);

	CurrSideNorm = Ref->side[I].plane.norm;
	CurrSideNorm /= CurrSideNorm.abs();

	SideNorm = Ref->side[id].plane.norm;
	SideNorm /= SideNorm.abs();

	ProjNorm = CurrSideNorm - SideNorm;
	ProjNorm /= ProjNorm.abs();

	P.dist = 0.0;
	P.norm = ProjNorm;

	FindCrossSection(Ref, P, CrossSection);
	DrawSideCenters(Ref, I, id);
	//printf("%d\n", SymmSideId[id]);

	const string fullDirname__string = "D:\\YandexDisk\\Ëèçà\\Octonus\\SymPlane" + string(to_string(I)) + string(".txt");
	const char* FullDirName = fullDirname__string.c_str();

	f = fopen(FullDirName, "w");

	for (int i = 0; i < (int)(CrossSection.size()); i++)
		fprintf(f, "%lf, %lf, %lf\n", CrossSection[i].x, CrossSection[i].y, CrossSection[i].z);

	fclose(f);

	CrossSection.clear();

	DrawAxPolyTogether(Ref, "D:\\YandexDisk\\Ëèçà\\Octonus\\", "Ref", "No");

	//---------------------------------------------------------------------------
	int i = 127, j = 207;
	CurrSideL2errors.clear();
	CurrSidePts.clear();
	len = length(Ref->side[i].vlist) - 1;
	CurrSideNorm = Ref->side[i].plane.norm;
	CurrSideNorm /= CurrSideNorm.abs();

	for (int j = 0; j < len; j++)
	{
		id = Ref->side[i].vlist[j];
		CurrSidePts.push_back(Ref->vertex[id]);
	}

	SidePts.clear();
	len = length(Ref->side[j].vlist) - 1;
	SideNorm = Ref->side[j].plane.norm;
	SideNorm /= SideNorm.abs();

	for (int k = 0; k < len; k++)
	{
		id = Ref->side[j].vlist[k];
		SidePts.push_back(Ref->vertex[id]);
	}

	ProjNorm = CurrSideNorm - SideNorm;
	ProjNorm /= ProjNorm.abs();

	Err.clear();

	AngleMatchPolygons(CurrSidePts, SidePts, ProjNorm, Err);

	CurrSideL2errors.push_back(Err[0]);
	DrawSideProj(ProjNorm, CurrSidePts, i);
	DrawSideProj(ProjNorm, SidePts, j);

	printf("127 <--> 201 error = %lf\n", Err[0]);

	i = 6, j = 7;
	CurrSideL2errors.clear();
	CurrSidePts.clear();
	len = length(Ref->side[i].vlist) - 1;
	CurrSideNorm = Ref->side[i].plane.norm;
	CurrSideNorm /= CurrSideNorm.abs();

	for (int j = 0; j < len; j++)
	{
		id = Ref->side[i].vlist[j];
		CurrSidePts.push_back(Ref->vertex[id]);
	}

	SidePts.clear();
	len = length(Ref->side[j].vlist) - 1;
	SideNorm = Ref->side[j].plane.norm;
	SideNorm /= SideNorm.abs();

	for (int k = 0; k < len; k++)
	{
		id = Ref->side[j].vlist[k];
		SidePts.push_back(Ref->vertex[id]);
	}

	ProjNorm = CurrSideNorm - SideNorm;
	ProjNorm /= ProjNorm.abs();

	Err.clear();

	AngleMatchPolygons(CurrSidePts, SidePts, ProjNorm, Err);

	DrawSideProj(ProjNorm, CurrSidePts, i);
	DrawSideProj(ProjNorm, SidePts, j);

	CurrSideL2errors.push_back(Err[0]);

	printf("6 <--> 7 error = %lf\n", Err[0]);

	CrossSection.clear();
	CurrSideL2errors.clear();
	L2errors.clear();
	CurrSidePts.clear();
	SidePts.clear();
}