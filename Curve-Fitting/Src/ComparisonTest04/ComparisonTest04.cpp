#include "ComparisonTest04.h"

const char* PathToWrite4 = "/home/elizabeth/Octonus/modelassociation/LinuxVersion_1.0/";

void FindPlaneViaRundistSides(POLYHEDRON* Poly, vector<int> Rundist, PLANE& P)
{
	vector<int> AlreadyIncludedPts;
	int N = Rundist.size(), Count = 0, Flag = 0;
	int Len;
	VECTOR* V;

	for (int i = 0; i < N; i++)
	{
		Len = length(Poly->side[Rundist[i]].vlist) - 1;

		for (int j = 0; j < Len; j++)
		{
			Flag = 0;

			for (int k = 0; k < Count; k++)
			{
				if (Poly->side[Rundist[i]].vlist[j] == AlreadyIncludedPts[k])
				{
					Flag = 1;
					break;
				}
			}

			if (Flag == 0)
			{
				AlreadyIncludedPts.push_back(Poly->side[Rundist[i]].vlist[j]);
				Count++;
			}
		}
	}

	V = new VECTOR[Count];

	for (int i = 0; i < Count; i++)
	{
		V[i] = Poly->vertex[AlreadyIncludedPts[i]];
	}

	DefinePlaneCoeff(Count, V, P);

	AlreadyIncludedPts.clear();
	delete[] V;
}

void ComparissonTest04(POLYHEDRON* Pattern, POLYHEDRON* Real)
{
	// system("del /q D:\\YandexDisk\\Ëèçà\\Octonus\\*.*");

	vector <double> Err;
	vector<int> RealRundist, PatternRundist;
	vector <VECTOR3> PatternCrossSection, RealCrossSection;
	PLANE P, CrossSectionPlane;
	double MaxZ = 0.0, Angle;
	int sign;

	// DrawAxPolyTogether(Real, "D:\\YandexDisk\\Ëèçà\\Octonus\\", "OriReal", "No");
	// DrawAxPolyTogether(Pattern, "D:\\YandexDisk\\Ëèçà\\Octonus\\", "OriPattern", "No");

	MatchPolyMainSymAx(Pattern, Real, RealRundist, PatternRundist);

	// DrawAxPolyTogether(Real, "D:\\YandexDisk\\Ëèçà\\Octonus\\", "RealMatchedAx", "No");
	// DrawAxPolyTogether(Pattern, "D:\\YandexDisk\\Ëèçà\\Octonus\\", "PatternMatchedAx", "No");

	FindPlaneViaRundistSides(Pattern, PatternRundist, P);

	for (int i = 0; i < Pattern->nverts; i++)
		if (fabs(Pattern->vertex[i].z) > MaxZ)
		{
			MaxZ = fabs(Pattern->vertex[i].z);

			if (Pattern->vertex[i].z < 0)
				sign = 1;
			else
				sign = -1;
		}

	MaxZ *= sign;

	CrossSectionPlane = P;
	CrossSectionPlane.dist += (MaxZ - P.dist) / 2.0;

	FindCrossSection(Pattern, CrossSectionPlane, PatternCrossSection);
	FindCrossSection(Real, CrossSectionPlane, RealCrossSection);

	Angle = AngleMatchPolygons(PatternCrossSection, RealCrossSection, VECTOR(0, 0, 1), Err);

	printf("Angle = %lf\n", Angle);

	VECTOR Vnull(0, 0, 0);
	SPIN Sp(VECTOR(0, 0, 1), Angle);
	CONFORM C(Sp, Vnull, 1);
	SIMILARITY S(C);

	TransformPolyhedron(Real, S);

	// DrawAxPolyTogether (Pattern, "D:\\YandexDisk\\Ëèçà\\Octonus\\", "ResPattern", "No");
	// DrawAxPolyTogether (Real, "D:\\YandexDisk\\Ëèçà\\Octonus\\", "ResReal", "No");

	// f = fopen("D:\\YandexDisk\\Ëèçà\\Octonus\\OriRealCrossSection.txt", "w");

	// for (int i = 0; i < (int)(RealCrossSection.size()); i++)
	// 	fprintf(f, "%lf, %lf, %lf\n", RealCrossSection[i].x, RealCrossSection[i].y, RealCrossSection[i].z);

	// fclose(f);

	Err.clear();
	PatternCrossSection.clear();
	RealCrossSection.clear();
	FindCrossSection(Pattern, CrossSectionPlane, PatternCrossSection);
	FindCrossSection(Real, CrossSectionPlane, RealCrossSection);

	printf("PatternCrossSection %d == %d RealCrossSection\n", ((int)(PatternCrossSection.size())), ((int)(RealCrossSection.size())));


	// f = fopen("D:\\YandexDisk\\Ëèçà\\Octonus\\PatternCrossSection.txt", "w");

	// for (int i = 0; i < (int)(PatternCrossSection.size()); i++)
	// 	fprintf(f, "%lf, %lf, %lf\n", PatternCrossSection[i].x, PatternCrossSection[i].y, PatternCrossSection[i].z);

	// fclose(f);

	// f = fopen("D:\\YandexDisk\\Ëèçà\\Octonus\\RealCrossSection.txt", "w");

	// for (int i = 0; i < (int)(RealCrossSection.size()); i++)
	// 	fprintf(f, "%lf, %lf, %lf\n", RealCrossSection[i].x, RealCrossSection[i].y, RealCrossSection[i].z);

	// fclose(f);

	PatternCrossSection.clear();
	RealCrossSection.clear();
	RealRundist.clear(); 
	PatternRundist.clear();
	RealRundist.clear(); 
	PatternRundist.clear();

}