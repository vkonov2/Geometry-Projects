#include "Clusterization.h"
#include "DivideConquerClosestPair.h"
#include <string>
#include <tuple>

double current_time(void)
{
  struct timeval tv2;
  struct timezone tz;
  gettimeofday(&tv2, &tz);
  return tv2.tv_sec+tv2.tv_usec/1000000.0;
}


void HierarchicalClustering(vector<VECTOR> Pts, vector<int>& FinalClusters, vector <Cluster>& Clusters, const int ClusterNumber, const double MaxDiameter)
{
	int N = Pts.size();

	for (int i = 0; i < N; ++i)
	{
		Cluster C;

		C.AddPt(Pts[i], i);

		C.OriginId = i;

		Clusters.push_back(C);
	}

    return;
}

double FindClosestPair(vector <Cluster> C, int& BestI, int& BestJ, double h)
{
	int N = C.size(), isNothingToMerge = 1;
	// int flag;
	double Dst = 0.0, MinDst = 1e+20;

	BestI = 0;
	BestJ = 1;

	for (int i = 0; i < N - 1; ++i)
	{
		for (int j = i + 1; j < N; ++j)
		{
			Dst = C[i].MinEuclidianDist(C[j]);

			if((MinDst > Dst) && (fabs(Dst - h) < 1e-14))
			{
				MinDst = Dst;
				BestI = i;
				BestJ = j;
				isNothingToMerge = 0;
			}
		}
	}

	if(isNothingToMerge == 1)
		MinDst = -100.0;

	return MinDst;
}

void HierarchicalClustering2(vector<VECTOR> Pts, const double h)
{
	int N = Pts.size();
	int BestI, BestJ; //, offset;
	double MinDst = 0.0;
	tuple<int, int, double> t;
    vector <Cluster> Clusters;

	for (int i = 0; i < N; ++i)
	{
		Cluster C;

		C.AddPt(Pts[i], i);

		C.OriginId = i;

		Clusters.push_back(C);
	}

	bool Sig = true;

	while(Sig)
	{
		MinDst = FindClosestPair(Clusters, BestI, BestJ, h);

        if(MinDst < 0)
		{
			Sig = false;
			break;
		}

		int count = 0;
		for(auto j: Clusters[BestJ].IncludedPts)
		{
			Clusters[BestI].AddPt(j, Clusters[BestJ].Ids[count]);
			count += 1;
		}

		Clusters.erase(Clusters.begin() + (BestJ));

		N = Clusters.size();
	}

	printf("Clusters len is %d\n", (int)(Clusters.size()));

    Clusters.clear();

}

