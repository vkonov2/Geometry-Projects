#include "Clusterization.h"
#include <string>

// #define MaxDiameter 0.01

double current_time(void)
{
  struct timeval tv2;
  struct timezone tz;
  gettimeofday(&tv2, &tz);
  return tv2.tv_sec+tv2.tv_usec/1000000.0;
}

// Method which depends on histogramm 
double FindBoundHistMethod(int n, double* DistMin)
{
	int count, id_last;
	int bins = n; // int(n / 2);
	int* val = new int[bins];
	double right_bound = DistMin[n - 1], left_bound = DistMin[0];
	double step = (right_bound - left_bound) / bins;
	double beg, end;

	for (int i = 0; i < bins; i++)
	{
		beg = left_bound + step * i;
		end = beg + step;
		count = 0;

		for (int i = 0; i < n; i++)
			if (!(DistMin[i] > end) && !(DistMin[i] < beg))
				count++;

		val[i] = count;
	}

	for (int i = 0; i < bins; i++)
	{
		if (val[i] == 0)
		{
			id_last = i - 1;
			break;
		}
	}

	/*printf("n = %d\n", n);
	printf("id_last = %d, bins = %d\n", id_last, bins);
	printf("left_bound = %lf, step = %lf, right_bound = %lf\n", left_bound, step, right_bound);*/
		
	delete[] val;

	return left_bound + step * id_last;
}

int HierarchicalClusteringHistMethod(int n, double bound, vector<vector<double> >& DistMatr, vector<int>& Rundist)
{
	int flag = 0; // flag == 1, åñëè äëÿ i ñóùåñòâóåò òî÷êà, ðàññòîÿíèå äî êîòîðîé <= bound, èíà÷å flag == 0
	int count;

	for (int i = 0; i < n; i++)
	{
		flag = 0;
		
		for (int j = 0; j < n - i - 1; j++)
		{
			if (!(DistMatr[i][j] > bound))
			{
				Rundist.push_back(j + i + 1);
				flag = 1;
			}
		}

		if (flag == 1)
		{
			Rundist.push_back(i);
		}
	}

	set <int> UniqRundist(Rundist.begin(), Rundist.end());
	Rundist.clear();

	for (int id : UniqRundist)
		Rundist.push_back(id);

	count = Rundist.size();

	UniqRundist.clear();
	return count;
}

void FindClusterHistMethod(vector<VECTOR> Pts, vector<int>& FinalCluster)
{
	int ns = Pts.size();
	double Dist, Bound;
	double MinDist = 0.0;
	vector<double> VertDistVec;
	vector<vector<double> > DistMatr;
	VECTOR DistVec, CurrentVertex;
	double* DistMin = new double[ns];

	FILE* f = fopen("DstMin.txt", "w");

	for (int i = 0; i < ns - 1; i++)
	{
		CurrentVertex = Pts[i];
		VertDistVec.clear();
		MinDist = (CurrentVertex - Pts[i + 1]).abs();

		for (int j = i + 1; j < ns; j++)
		{
			DistVec = CurrentVertex - Pts[j];
			Dist = DistVec.abs();
			VertDistVec.push_back(Dist);

			if (MinDist > Dist)
				MinDist = Dist;
		}

		DistMin[i] = MinDist;
		fprintf(f, "%lf\n", MinDist);
		DistMatr.push_back(VertDistVec);
	}

	fclose(f);

	std::sort(DistMin, DistMin + ns - 1);

	Bound = FindBoundHistMethod(ns - 1, DistMin);

	// printf()

	HierarchicalClusteringHistMethod(ns - 1, Bound, DistMatr, FinalCluster);

	for(auto i: DistMatr)
		i.clear();
	DistMatr.clear();

	VertDistVec.clear();
	delete [] DistMin;
}

// Hierarchical clustering
double FindClosestPair(vector <Cluster> C, int& BestI, int& BestJ, double MaxDiameter)
{
	int N = C.size(), isNothingToMerge = 1;
	// int sgn1, sgn2;
	int flag;
	double Dst = 0.0, MinDst = 100.0;

	// printf("In begin MinDst = %lf, Dst = %lf\n", MinDst, Dst);

	BestI = 0;
	BestJ = 1;

	// for(auto [a, b]: DoNotMerge)
	// 	printf("(%d, %d) ", a, b);
	// printf("\n");

	for (int i = 0; i < N - 1; ++i)
	{
		for (int j = i + 1; j < N; ++j)
		{
			// Dst = C[i].MinEuclidianDist(C[j]);
			Dst = C[i].CalcCenterDistance(C[j]);

			// if(find(begin(DoNotMerge), end(DoNotMerge), pair<int, int>(i, j)) != end(DoNotMerge))
			// 	sgn1 = 0;
			// else
			// 	sgn1 = 1;
			// if(find(begin(DoNotMerge), end(DoNotMerge), pair<int, int>(j, i)) != end(DoNotMerge))
			// 	sgn2 = 0;
			// else
			// 	sgn2 = 1;
			if( C[i].CalcDiameterAfterAdd(C[j]) <= MaxDiameter )
				flag = 1;
			else
				flag = 0;

			// printf("sgn1 = %d, sgn2 = %d\n", sgn1, sgn2);
			// if(MinDst > 0.0)
			// printf("MinDst = %.40lf, Dst = %lf\n", MinDst, Dst);

			// if(sgn2 == 1 && sgn1 == 1 && flag == 1)	
			if(MinDst > Dst && i != j && Dst > 0.0 && flag == 1)
			{
				MinDst = Dst;
				BestI = i;
				BestJ = j;
				isNothingToMerge = 0;
			}
		}
	}

	if(isNothingToMerge == 1)
		MinDst = -100;

	return MinDst;
}

bool compare (int i,int j);
bool compare (int i,int j) { return (i>j); }

void MergeEqualClusters(vector <Cluster>& Pts);
void MergeEqualClusters(vector <Cluster>& Pts)
{
    bool merge = true;
    int size;
    int count = 0;
    vector <int> PointsToMerge;
    double eps = 1.0;

    while(eps + 1.0 > 1.0)
    	eps /= 2.0;

    while(merge)
    {
        size = Pts.size();
        count = 0;

        for (int i = 0; i < size; ++i)
        {
            PointsToMerge.push_back(i);
            for (int j = i + 1; j < size; ++j)
            {
                if(Pts[j].IncludedPts[0].x == Pts[i].IncludedPts[0].x && Pts[j].IncludedPts[0].y == Pts[i].IncludedPts[0].y && Pts[j].IncludedPts[0].z == Pts[i].IncludedPts[0].z)
                if(!((Pts[j].IncludedPts[0] - Pts[i].IncludedPts[0]).abs() > eps))
                {
                    count++;
                    // printf("%lf %lf %lf\n", Pts[j].IncludedPts[0].x, Pts[j].IncludedPts[0].y, Pts[j].IncludedPts[0].z);
                    PointsToMerge.push_back(j);
                }
            }

            if(count != 0)
            {
            	// printf("\n");
            	break;
            }
            else
            	PointsToMerge.clear();
        }

        if(count == 0)
        {
            merge = false;
            continue;
        }

        sort(begin(PointsToMerge), end(PointsToMerge), compare);

        // printf("Pts len before merge: %d, count = %d\n", (int)(Pts.size()), count);

		int LastPt = PointsToMerge[PointsToMerge.size() - 1];
		int idMerge;

		// printf("LastPt = %d\n", LastPt);
		// printf("PointsToMerge size is %d\n", (int)(PointsToMerge.size()));

        for (int i = 0; i < PointsToMerge.size() - 1; ++i)
        {
        	idMerge = PointsToMerge[i];
        	// printf("idMerge = %d\n", idMerge);
        	Pts[LastPt].Ids.push_back(Pts[idMerge].Ids[0]);
            Pts.erase(Pts.begin() + idMerge);
        }

        printf("Clusters len after merge: %d\n", (int)(Pts.size()));

        PointsToMerge.clear();
        // printf("PointsToMerge size is %d, but:\n", (int)(PointsToMerge.size()));

        // for (auto i: PointsToMerge)
        // 	printf("%d ", i);

        // printf("\n");
    }

    PointsToMerge.clear();
}

void HierarchicalClustering(vector<VECTOR> Pts, vector<int>& FinalClusters, vector <Cluster>& Clusters, int ClusterNumber, double MaxDiameter)
{
	int N = Pts.size();
	// vector <pair<int, int>>::iterator it;
	// vector <pair<int,int>> DoNotMerge;
	int BestI, BestJ;
	double MinDst = 0.0;

	// DoNotMerge.push_back(pair<int, int> (-1, -1));

	for (int i = 0; i < N; ++i)
	{
		Cluster C;

		C.AddPt(Pts[i], i);

		Clusters.push_back(C);
	}

	bool Sig = true;
	double start = current_time();
	MergeEqualClusters(Clusters);
	printf(">>> Merge equal clusters time: %lf secs\n", current_time()-start);

	start = current_time();
	MergeEqualClusters(Clusters);
	printf(">>> Merge equal clusters time: %lf secs\n", current_time()-start);
	N = Clusters.size();
	printf("N = %d\n", N);

	FILE* f = fopen("GnuplotData//PlaneClusters.txt", "w");
	for (int i = 0; i < N; ++i)
	{
		// printf("Cluster[%d] len is %d:\n", i, (int)(Clusters[i].Ids.size()));
		fprintf(f, "%.40lf %.40lf %.40lf\n", Clusters[i].IncludedPts[0].x, Clusters[i].IncludedPts[0].y, Clusters[i].IncludedPts[0].z);
	}
	fclose(f);

	while(Sig)
	{
		start = current_time();
		MinDst = FindClosestPair(Clusters, BestI, BestJ, MaxDiameter);
		printf(">>> Find closest pair time: %lf secs\n\n", current_time()-start);

		if(MinDst < 0 || MinDst > MaxDiameter)
		{
			Sig = false;
			break;
		}
		printf("MinDst = %e\n", MinDst);

		int count = 0;
		for(auto j: Clusters[BestJ].IncludedPts)
		{
			Clusters[BestI].AddPt(j, Clusters[BestJ].Ids[count]);
			count += 1;
		}

		Clusters[BestI].CalcDiameter();

		Clusters.erase(Clusters.begin() + (BestJ));

		// if( Clusters[BestI].CalcDiameterAfterAdd(Clusters[BestJ]) <= MaxDiameter )
		// {	
		// 	int count = 0;
		// 	for(auto j: Clusters[BestJ].IncludedPts)
		// 	{
		// 		Clusters[BestI].AddPt(j, Clusters[BestJ].Ids[count]);
		// 		count += 1;
		// 	}

		// 	Clusters[BestI].CalcDiameter();

		// 	Clusters.erase(Clusters.begin() + (BestJ));
		// 	// printf("Clusters len is %d\n", (int)(Clusters.size()));
			
		// 	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //   		//Delete ids pairs after merge----------------------------------------------------------------------------------------
		// 	// it = DoNotMerge.begin();
		// 	// while(it != DoNotMerge.end())
		// 	// {
		// 	// 	if(it->first == BestI || it->first == BestJ || it->second == BestI || it->second == BestJ)
		// 	// 		DoNotMerge.erase(it);
		// 	// 	else
		// 	// 	{
		// 	// 		++it;
		// 	// 	}
		// 	// }

		// 	//Delete ids pairs after merge----------------------------------------------------------------------------------------
		// 	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// }
		// else
		// {
		// 	DoNotMerge.push_back(pair<int, int> (BestI, BestJ));
		// }

		printf("Clusters len is %d\n", (int)(Clusters.size()));

	}

	printf("Clusters len is %d\n", (int)(Clusters.size()));

	for (int i = 0; i < (int)(Clusters.size()); ++i)
	{
		if(Clusters[i].PtsNum > 10)
			printf("i = %d, len = %d\n", i, Clusters[i].PtsNum);
	}

	f = fopen("MainCluster.txt", "w");

	int Id = 3;

    for (auto i: Clusters[Id].IncludedPts)
    {
        VECTOR j = i;
        fprintf(f, "%lf %lf %lf\n", j.x, j.y, j.z);
    }

    fclose(f);

    int Num = 0;
    string CommandGnuplot = "splot \'PlaneClusters.txt\', ";
    string CommandGnuplotAll = "splot \'PlaneClusters.txt\', ";
    string Cl = "Cluster";
    int maxLen = 0, iMaxLen = 0;

    for (auto i: Clusters)
    {
        // if(i.PtsNum > 10)
        i.ClusterToFile("Cluster", Num);
        if(i.PtsNum > 10)
        	CommandGnuplot += "\'Cluster" + to_string(Num) + ".txt\' lw 3, ";

        CommandGnuplotAll += "\'Cluster" + to_string(Num) + ".txt\' lw 3, ";

        if(i.Ids.size() > maxLen)
        {
        	maxLen = i.Ids.size();
        	iMaxLen = Num;
        }

        Num++;

        // i.CalcDiameter();
        // if(i.PtsNum > 1)
        // {
        // 	printf("%e i.PtsNum = %d\n", i.Diameter, i.PtsNum);
        //     if(i.Diameter >= MaxDiameter)
        //     	printf("*** %e\n", i.Diameter);}
    }

    printf("maxLen = %d, iMaxLen = %d\n", maxLen, iMaxLen);

    // printf("%s\n", (CommandGnuplot).c_str());

    f = fopen("GnuplotData//Commands.txt", "w");
    fprintf(f, "%s\n\n\n", (CommandGnuplot).c_str());
    fprintf(f, "%s\n", (CommandGnuplotAll).c_str());
    fclose(f);

    for(auto i: Clusters[Id].Ids)
    	FinalClusters.push_back(i);

    // DoNotMerge.clear();
}
