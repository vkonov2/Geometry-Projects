#define _CRT_SECURE_NO_WARNINGS

#include "Draw.h"
#include <iostream>
#include <string>
#include "../PlaneGeometry/PlaneGeometryFunctions.h"
#include "../Geometry/AllGeometry.h"
#include "../Geometry/Vector.h"
#include "../PlaneGeometry/PlaneGeometryFunctions.h"

using namespace std;

void PointsGenerator (VECTOR* pts, int N, PLANE plane, const char* Name)
{

    const string fullname_str = string(Name) + ".txt";
    const char* fullname = fullname_str.c_str();

    int i, j;
    double k_0, k_mid;
    VECTOR mid;

    mid.x = 0.0;
    mid.y = 0.0;
    mid.z = 0.0;

    for (i = 0; i < N; i++)
    {
        mid.x += pts[i].x;
        mid.y += pts[i].y;
        mid.z += pts[i].z;
    }

    mid.x /= N;
    mid.y /= N;
    mid.z /= N;

    //printf("mid = %lf %lf %lf\n", mid.x, mid.y, mid.z);

    k_0 = -(plane.norm.x*pts[0].x+plane.norm.y*pts[0].y+plane.norm.z*pts[0].z+plane.dist);
    k_mid = -(plane.norm.x*mid.x+plane.norm.y*mid.y+plane.norm.z*mid.z+plane.dist);

    VECTOR pts_0_pr = pts[0] + k_0*plane.norm;
    VECTOR pts_mid_pr = mid + k_mid*plane.norm;

    VECTOR e1, e2;

    if(fabs(k_0) < 1e-5 && fabs(k_mid) < 1e-5)
    {
    	//printf("Here\n");
    	e1 = pts[1]-pts[0];
    	e2 = e1 & plane.norm;
    }

    else
    {
    	e1 = (pts_mid_pr-pts_0_pr);
    	e2 = e1 & plane.norm;
    }

    //printf("Before e1 = %lf %lf %lf\n", e1.x, e1.y, e1.z);

    e1 /= e1.abs();
    e2 /= e2.abs();
    double k = 1.3;
    e1 /= (double (2 * k));
    e2 /= (double (k));

    // printf("k_0 = %1.20lf\n", k_0);
    // printf("k_mid = %lf\n", k_mid);

    // printf("pts_0_pr = %lf %lf %lf\n", pts_0_pr.x, pts_0_pr.y, pts_0_pr.z);
    // printf("e1 = %lf %lf %lf\n", e1.x, e1.y, e1.z);
    // printf("e2 = %lf %lf %lf\n", e2.x, e2.y, e2.z);

    FILE* f = fopen (fullname, "w");
    if (!f)
    {
        printf(">>> can't open file --> plane.txt\n");
        return ;
    }

    k *= 4;
    
    for (i = -k; i < k; i++)
    {
        for (j = -k; j < k; j++)
        {
            VECTOR e11 = i*e1 + j*e2 + pts_mid_pr;
            VECTOR e22 = j*e1 + i*e2 + pts_mid_pr;
            fprintf(f, "%lf %lf %lf\n", e11.x, e11.y, e11.z);
            fprintf(f, "%lf %lf %lf\n", e22.x, e22.y, e22.z);
        }
    }
    fclose(f);



    f = fopen("GnuplotData/OriPts.txt", "w");
    for (int r = 0; r < N; ++r)
    {
        VECTOR n = pts[r];
        // n /= n.abs();
        fprintf(f, "%lf %lf %lf\n", n.x, n.y, n.z);
    }

    fclose(f);

    // f = fopen("norm.txt", "w");
    // fprintf(f, "%lf %lf %lf\n", pts[0].x, pts[0].y, pts[0].z);
    // fprintf(f, "%lf, %lf, %lf\n", plane.norm.x/plane.norm.abs()+ pts[0].x, plane.norm.y/plane.norm.abs()+ pts[0].y, plane.norm.z/plane.norm.abs()+ pts[0].z);
    // fclose(f);

    return ;
}

void Draw(POLYHEDRON* Real_Poly, const char* dir, const char* prefix)
{
	const string str = string(dir) + string(prefix) + ".txt";
	const char* fullname = str.c_str();
	const int N = Real_Poly->nsides;

	int i, j, nv, v1, v2;

	FILE* f = fopen(fullname, "w");
	if (!f)
	{
		printf(">>> can't open file --> %s\n", fullname);
		return;
	}

	for (i = 0; i < N; ++i)
	{
		nv = CountSideVertices (Real_Poly->side[i].vlist);

		for (j = 0; j < nv; j++)
		{
			v1 = Real_Poly->side[i].vlist[j];
			v2 = Real_Poly->side[i].vlist[j+1];
			fprintf(f, "%lf %lf %lf\n", Real_Poly->vertex[v1].x, Real_Poly->vertex[v1].y, Real_Poly->vertex[v1].z);
			fprintf(f, "%lf %lf %lf\n    \n    \n", Real_Poly->vertex[v2].x, Real_Poly->vertex[v2].y, Real_Poly->vertex[v2].z);
		}
	}

	fclose(f);

}

void DrawSelected (POLYHEDRON* Real_Poly, int N, vector<int>& Sides, const char* dir, const char* prefix)
{
	const string str = string(dir) + string(prefix) + ".txt";
	const char* fullname = str.c_str();

	int i, j, nv, v1, v2;

	FILE* f = fopen(fullname, "w");
	if (!f)
	{
		printf(">>> can't open file --> %s\n", fullname);
		return;
	}

	for (i = 0; i < N; ++i)
	{
		nv = CountSideVertices (Real_Poly->side[Sides[i]].vlist);

		for (j = 0; j < nv; j++)
		{
			v1 = Real_Poly->side[Sides[i]].vlist[j];
			v2 = Real_Poly->side[Sides[i]].vlist[j+1];
			fprintf(f, "%lf %lf %lf\n", Real_Poly->vertex[v1].x, Real_Poly->vertex[v1].y, Real_Poly->vertex[v1].z);
			fprintf(f, "%lf %lf %lf\n    \n    \n", Real_Poly->vertex[v2].x, Real_Poly->vertex[v2].y, Real_Poly->vertex[v2].z);
		}
	}

	fclose(f);

}

void DrawSelected2 (POLYHEDRON* Real_Poly, int N, vector<int>& Sides, const char* dir, const char* prefix)
{
	const string str = string(dir) + string(prefix) + ".txt";
	const char* fullname = str.c_str();

	int i, j, nv, v1, v2, flag;

	FILE* f = fopen(fullname, "w");
	if (!f)
	{
		printf(">>> can't open file --> %s\n", fullname);
		return;
	}

	for (i = 0; i < Real_Poly->nsides; ++i)
	{
		flag = 0;
		for (j = 0; j < N; j++)
			if (i == Sides[j])
				flag = 1;

		if (flag == 0)
		{
			nv = CountSideVertices (Real_Poly->side[i].vlist);

			for (j = 0; j < nv; j++)
			{
				v1 = Real_Poly->side[i].vlist[j];
				v2 = Real_Poly->side[i].vlist[j+1];
				fprintf(f, "%lf, %lf, %lf\n", Real_Poly->vertex[v1].x, Real_Poly->vertex[v1].y, Real_Poly->vertex[v1].z);
				fprintf(f, "%lf, %lf, %lf\n    \n    \n", Real_Poly->vertex[v2].x, Real_Poly->vertex[v2].y, Real_Poly->vertex[v2].z);
			}
		}
	}

	fclose(f);

}

void DrawAxes(VECTOR* ax, const char* fullpath, VECTOR mass_center)
{
	const string fullname_string1 = string(fullpath) + string("1.txt");
	const char* fullname1 = fullname_string1.c_str();

	const string fullname_string2 = string(fullpath) + string("2.txt");
	const char* fullname2 = fullname_string2.c_str();

	const string fullname_string3 = string(fullpath) + string("3.txt");
	const char* fullname3 = fullname_string3.c_str();

	FILE* f = fopen(fullname1, "w");
	fprintf(f, "%lf %lf %lf\n ", mass_center.x, mass_center.y, mass_center.z);
	fprintf(f, "%lf %lf %lf\n", (3 * ax[0].x + mass_center.x), (3 * ax[0].y + mass_center.y), (3 * ax[0].z + mass_center.z));
	fclose(f);

	f = fopen(fullname2, "w");
	fprintf(f, "%lf %lf %lf\n ", mass_center.x, mass_center.y, mass_center.z);
	fprintf(f, "%lf %lf %lf\n", (3 * ax[1].x + mass_center.x), (3 * ax[1].y + mass_center.y), (3 * ax[1].z + mass_center.z));
	fclose(f);

	f = fopen(fullname3, "w");
	fprintf(f, "%lf %lf %lf\n ", mass_center.x, mass_center.y, mass_center.z);
	fprintf(f, "%lf %lf %lf\n", (3 * ax[2].x + mass_center.x), (3 * ax[2].y + mass_center.y), (3 * ax[2].z + mass_center.z));
	fclose(f);
}

void DrawPointedSide(POLYHEDRON* Poly, int SideId, const char* dir)
{
	int N = length(Poly->side[SideId].vlist) - 1;
	VECTOR *pts = new VECTOR[N];
	int id;

	printf("N = %d\n", N);

	for (int i = 0; i < N; ++i)
	{
		id = Poly->side[SideId].vlist[i];
		pts[i] = Poly->vertex[id];

		// printf("%1.10lf %1.10lf %1.10lf\n", pts[i].x, pts[i].y, pts[i].z);
	}

	const string fullname_string = string(dir) + string("Plane") + to_string(SideId);
	const char* fullname = fullname_string.c_str();

	// printf("SideId = %d\n", SideId);

	PointsGenerator (pts, N, Poly->side[SideId].plane, fullname);

	delete[] pts;
}

// void DrawChainIds(Cluster v, int ChainId, const char* dir)
// {
// 	const string fullname_string = string("Chain") + to_string(ChainId) + string("txt") ;
// 	const char* fullname = fullname_string.c_str();

// 	FILE* f = fopen(fullname, "w");

// 	if (!f)
// 	{
// 		printf("Cant open %s\n", fullname);
// 		return;
// 	}

// 	for(auto j: v.IncludedPts)
// 		fprintf(f, "%lf %lf %lf\n", j.x, j.y, j.z);
// 	fclose(f);
// }

void DrawUnionOfChains(vector <int> Ids, POLYHEDRON* Poly, const string Name, int Id)
{
	const string fullname_str = string(Name) + to_string(Id) + ".txt";
	const char* fullname = fullname_str.c_str();

	vector < int> UnionOfChains;

    for (auto k: Ids)
        UnionOfChains.push_back(k);

    set <int> Union(UnionOfChains.begin(), UnionOfChains.end());

    UnionOfChains.clear();

    for (int id : Union)
        UnionOfChains.push_back(id);

    // printf("%d\n", (int)(UnionOfChains.size()));

    DrawSelected (Poly, (int)(UnionOfChains.size()), UnionOfChains, "./", fullname);

    Union.clear();
    UnionOfChains.clear();
}

void DrawApproximationPlaneOfChain(std::vector<int> Ids, const POLYHEDRON* Poly, const char* Name)
{
	std::vector<int> AlreadyIncludedPts;
	int N = Ids.size();
	int n, Num, id;
	PLANE Plane;

	for (int i = 0; i < N; ++i)
	{
		id = Ids[i];
		n = length(Poly->side[id].vlist) - 1;

		for (int j = 0; j < n; ++j)
		{
			Num = Poly->side[id].vlist[j];
			if(std::find(AlreadyIncludedPts.begin(), AlreadyIncludedPts.end(), Num) == AlreadyIncludedPts.end())
				AlreadyIncludedPts.push_back(Num);
		}
	}

	N = AlreadyIncludedPts.size();
	VECTOR* pts = new VECTOR[N];

	for (int i = 0; i < N; ++i)
		pts[i] = Poly->vertex[AlreadyIncludedPts[i]];

	DefinePlaneCoeff(N, pts, Plane);
    // printf("Plane which we draw is %lf %lf %lf %lf\n", Plane.norm.x, Plane.norm.y, Plane.norm.z, Plane.dist);
	PointsGenerator (pts, N, Plane, Name);


	delete[] pts;
	AlreadyIncludedPts.clear();
}

void DrawNormPlaneOfChain(std::vector<int> Ids, const POLYHEDRON* Poly, const char* Name, const PLANE Plane)
{
	int N = Ids.size();
	VECTOR* pts = new VECTOR[N];
	VECTOR n;

	for (int i = 0; i < N; ++i)
	{
		n = Poly->side[Ids[i]].plane.norm;
		n /= n.abs();
		pts[i] = n;
	}

	PointsGenerator (pts, N, Plane, Name);

	delete[] pts;
}


void DrawChain(POLYHEDRON* Poly, vector<int> Chain, int Id)
{
    int id;
    int N = Chain.size();

    VECTOR n;

    FILE* g = fopen(("GnuplotData//SphereChain" + to_string(Id) + ".txt").c_str(), "w");

    for (int i = 0; i < N; ++i)
    {
        id = Chain[i];
        n = Poly->side[id].plane.norm;
        n /= n.abs();
        fprintf(g, "%lf %lf %lf\n", n.x, n.y, n.z);
    }

    fclose(g);
}

void DrawNormApproximationPlaneOfChain(std::vector<int> Ids, POLYHEDRON* Poly, const char* Name)
{
	int N = Ids.size();
	PLANE Plane;
	VECTOR* pts = new VECTOR[N];

	for (int i = 0; i < N; ++i)
	{
		pts[i] = Poly->side[Ids[i]].plane.norm / Poly->side[Ids[i]].plane.norm.abs();
	}

	DefinePlaneCoeff(N, pts, Plane);
    // printf("Plane which we draw is %lf %lf %lf %lf\n", Plane.norm.x, Plane.norm.y, Plane.norm.z, Plane.dist);
	PointsGenerator (pts, N, Plane, Name);

	delete[] pts;
}

void DrawNormApproximationPlaneOfChainWithNull(std::vector<int> Ids, POLYHEDRON* Poly, const char* Name)
{
	int N = Ids.size() + 1;
	PLANE Plane;
	VECTOR* pts = new VECTOR[N];

	for (int i = 0; i < N - 1; ++i)
	{
		pts[i] = Poly->side[Ids[i]].plane.norm / Poly->side[Ids[i]].plane.norm.abs();
	}

	pts[N - 1] = VECTOR(0, 0, 0);

	DefinePlaneCoeff(N, pts, Plane);
    // printf("Plane which we draw is %lf %lf %lf %lf\n", Plane.norm.x, Plane.norm.y, Plane.norm.z, Plane.dist);
	PointsGenerator (pts, N, Plane, Name);

	delete[] pts;
}
