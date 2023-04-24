#pragma once

#include <sys/time.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
//#include "../Geometry/Polyhedr.h"
#include "../Geometry/AllGeometry.h"
#include "../Geometry/Vector.h"
//#include "../PolyUtils/Structs.h"
//#include "../PolyUtils/PolyUtilsFunctions.h"
#include "../Draw/Draw.h"
// #include "DivideConquerClosestPair.h"

using namespace std;

double current_time(void);

class Cluster
{
public:
	VECTOR Center; // Center of a cluster
	int PtsNum;
	double Variance;
	double Diameter;
	VECTOR CurrentSum;
	vector <VECTOR> IncludedPts;
	vector <int> Ids;
	int OriginId;

	Cluster()
	{
		this->Center = VECTOR(0.0, 0.0, 0.0);
		this->PtsNum = 0;
		this->CurrentSum = Center;
		this->Variance = 0.0;
		this->Diameter = 0.0;
		this->OriginId = 0;
	}

	Cluster& operator=(const Cluster& other) // weak equaling: only Center and OriginId
    {
        if (this == &other)
            return *this;

        this->IncludedPts.clear();
        this->Ids.clear();

        this->Center = other.Center;
		this->PtsNum = other.PtsNum;
		this->CurrentSum = other.Center;
		this->Variance = other.Variance;
		this->Diameter = other.Diameter;
		this->OriginId = other.OriginId;

		for(auto i: other.IncludedPts)
			this->IncludedPts.push_back(i);

		for(auto i: other.Ids)
			this->Ids.push_back(i);


        return *this;
    }

    bool CanAdd(Cluster P)
    {
    	VECTOR FutureCenter = (this->CurrentSum + P.CurrentSum) / (this->PtsNum + P.PtsNum);

    	for(auto i: this->IncludedPts)
    		if( (i - FutureCenter).abs() >=  this->Variance)
    			return false;

    	for(auto i: P.IncludedPts)
    		if( (i - FutureCenter).abs() >=  this->Variance)
    			return false;

    	return true;
    }

	double MinCosineDist(Cluster P)
	{
		double Cos = (this->IncludedPts[0] | P.IncludedPts[0]) / (this->IncludedPts[0].abs() * P.IncludedPts[0].abs());
		double Dist = 1 - Cos;
		double CurDist = 0.0;

		for(auto i: this->IncludedPts)
		{
			for(auto j: P.IncludedPts)
			{	
				Cos = (i | j) / (i.abs() * j.abs());
				CurDist = 1 - Cos;
			
				if(CurDist > Dist)
					Dist = CurDist;
			}
		}

		return Dist; 
	}

	double MaxEuclidianDist(Cluster P)
	{
		// printf("Here in MaxEuclidianDist\n");
		double Dist = (this->IncludedPts[0] - P.IncludedPts[0]).abs();
		double CurDist = 0.0;

		for(auto i: this->IncludedPts)
		{
			for(auto j: P.IncludedPts)
			{	
				CurDist = (i - j).abs();
			
				if(CurDist > Dist)
					Dist = CurDist;
			}
		}

		return Dist; 
	}

	double MinEuclidianDist(Cluster P)
	{
		// printf("Here in MaxEuclidianDist\n");
		double Dist = (this->IncludedPts[0] - P.IncludedPts[0]).abs();
		double CurDist = 0.0;

		for(auto i: this->IncludedPts)
		{
			for(auto j: P.IncludedPts)
			{	
				CurDist = (i - j).abs();
			
				if(CurDist < Dist)
					Dist = CurDist;
			}
		}

		return Dist; 
	}

	void CalcDiameter()
	{
		this->Diameter = this->MaxEuclidianDist(*this);
	}

	double CalcDiameterAfterAdd(Cluster P)
	{
		// double DistBetweenClusters = MaxEuclidianDist(P);
		// double Dist;
		//double start = current_time();
		// initializer_list<double> dsts ({DistBetweenClusters, P.Diameter, this->Diameter});

		// Dist = max(dsts);
		//printf(">>> Calculation of cluster diameter after point addition time: %lf secs\n", current_time()-start);

		// return Dist; 
		return 0.0;
	}

	double CalcCenterDistance(Cluster P)
	{
		return (this->Center - P.Center).abs();
	}

	void AddPt(VECTOR P, int Id)
	{
		this->IncludedPts.push_back(P);
		this->Ids.push_back(Id);
		this->PtsNum += 1;
		this->CurrentSum += P;
		this->Center = this->CurrentSum / (double)(this->PtsNum);
		this->Center /= this->Center.abs();
	}

	void PrintInfo()
	{
		VECTOR V = this->Center;

		printf("Center is (%lf, %lf, %lf), variance is %lf, points number is %d\n", V.x, V.y, V.z, this->Variance, this->PtsNum);
	}

	void ClusterToFile(const string Name, int Id)
	{
		const string fullname_str = "GnuplotData//" + string(Name) + to_string(Id) + ".txt";
    	const char* fullname = fullname_str.c_str();

    	FILE* f = fopen(fullname, "w");

    	for (auto j: this->IncludedPts)
	    {
	        fprintf(f, "%lf %lf %lf\n", j.x, j.y, j.z);
	    }

	    fclose(f);
	}

	~Cluster()
	{
		IncludedPts.clear();
		Ids.clear();
	}
};

void DeletePair(vector<vector<int>> ExcludedPairs, const int i, const int j);
bool compare (const int i, const int j);
void MergeEqualClusters(vector <Cluster>& Pts);
void HierarchicalClustering(vector<VECTOR> Pts, vector<int>& FinalClusters, vector <Cluster>& Clusters, const int ClusterNumber, const double MaxDiameter);
void HierarchicalClustering2(vector<VECTOR> Pts, const double h);