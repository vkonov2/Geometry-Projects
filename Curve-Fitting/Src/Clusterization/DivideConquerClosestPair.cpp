#include "DivideConquerClosestPair.h"
#include <cfloat>

// #include <bits/stdc++.h>

struct compX {
  bool operator() (const Cluster p1, const Cluster p2) { return (p1.Center.x < p2.Center.x);}
} compareX;

struct compY {
  bool operator() (const Cluster p1, const Cluster p2) { return (p1.Center.y < p2.Center.y);}
} compareY;

struct compZ {
  bool operator() (const Cluster p1, const Cluster p2) { return (p1.Center.z < p2.Center.z);}
} compareZ;

bool isIncluded(const int i, const int j, vector<vector<int>>& ExcludedPairs)
{
    for(auto k: ExcludedPairs)
        if((k[0] == i && k[1] == j) || (k[0] == j && k[1] == i))
            return true;

    return false;
}

double bruteForce(vector <Cluster> P, const int n, tuple<int, int, double>& tGlobal, vector<vector<int>>& ExcludedPairs, const int MaxN)
{
    int iBest = MaxN, jBest = MaxN;
    double min = FLT_MAX, dst = 0.0;
    for (int i = 0; i < n; ++i)
        for (int j = i+1; j < n; ++j)
            {
            	dst = (P[i].Center - P[j].Center).abs();

            	if (dst < min && dst != 0.0 && isIncluded(P[i].OriginId, P[j].OriginId, ExcludedPairs) == false)
                { 
                	// printf("min = %f\n", min);
                	min = dst;
                	iBest = P[i].OriginId;
                	jBest = P[j].OriginId;
                }
            }

    get<0>(tGlobal) = iBest;
    get<1>(tGlobal) = jBest;
    get<2>(tGlobal) = min;

    return min;
}

double min(const double x, const double y)
{
    return (x < y)? x : y;
}

double stripClosest(vector <Cluster> strip, const int size, const double d, tuple<int, int, double>& tGlobal, vector<vector<int>>& ExcludedPairs, const int MaxN)
{
    double minY = d, minZ = d, dst = 0.0; // Initialize the minimum distance as d
    int iBestY = MaxN, jBestY = MaxN;
    int iBestZ = MaxN, jBestZ = MaxN;
    int iBest, jBest;
    double min;
    bool flag = false;

    sort(strip.begin(), strip.end(), compareY);

    for (int i = 0; i < size; ++i)
        for (int j = i+1; j < size && (strip[j].Center.y - strip[i].Center.y) < minY; ++j)
        {
            dst = (strip[i].Center - strip[j].Center).abs();

        	if (dst < minY && dst != 0.0 && isIncluded(strip[i].OriginId, strip[j].OriginId, ExcludedPairs) == false)
            {
                // printf("min = %f\n", min);

                minY = dst;
                iBestY = strip[i].OriginId;
                jBestY = strip[j].OriginId;
                flag = true;
            }
        }

    if(flag == false)
    {
        iBestY = MaxN, jBestY = MaxN;
        minY = FLT_MAX;
    }
    
    flag = false;
    sort(strip.begin(), strip.end(), compareZ);

    for (int i = 0; i < size; ++i)
        for (int j = i+1; j < size && (strip[j].Center.z - strip[i].Center.z) < minZ; ++j)
        {
            dst = (strip[i].Center - strip[j].Center).abs();
            
        	if (dst < minZ && dst != 0.0 && isIncluded(strip[i].OriginId, strip[j].OriginId, ExcludedPairs) == false)
            {
                // printf("min = %f\n", min);

                minZ = dst;
                iBestZ = strip[i].OriginId;
                jBestZ = strip[j].OriginId;
                flag = true;
            }
        }

    if(flag == false)
    {
        iBestZ = MaxN, jBestZ = MaxN;
        minZ = FLT_MAX;
    }

    if(minY < minZ)
    {
    	iBest = iBestY; 
    	jBest = jBestY;
    	min = minY;
    }
    else
    {
    	iBest = iBestZ; 
    	jBest = jBestZ;
    	min = minZ;
    }

    get<0>(tGlobal) = iBest;
    get<1>(tGlobal) = jBest;
    get<2>(tGlobal) = min;
 
    return min;
}

double closestUtil(vector <Cluster> P, const int n, tuple<int, int, double>& tGlobal, vector<vector<int>>& ExcludedPairs, const int MaxN)
{
    // printf("n = %d\n", n);
    if (n <= 3)
        return bruteForce(P, n, tGlobal, ExcludedPairs, MaxN);
 
    // Find the middle Cluster
    int mid = n/2;
    Cluster midCluster = P[mid];
 
    // Consider the vertical line passing
    // through the middle Cluster calculate
    // the smallest distance dl on left
    // of middle Cluster and dr on right side
    tuple<int, int, double> tl;
    tuple<int, int, double> tr;
    tuple<int, int, double> tStrip;
    tuple<int, int, double>* t;
    vector<Cluster> midP;

    for (int i = mid; i < n; ++i)
    	midP.push_back(P[i]);

    double dl = closestUtil(P, mid, tl, ExcludedPairs, MaxN);
    double dr = closestUtil(midP, n - mid, tr, ExcludedPairs, MaxN);

    midP.clear();
 
    // Find the smaller of two distances
    double d = min(dl, dr);
    t = (dl < dr)? &tl : &tr;
    d = get<2>(*t); 
 
    // Build an array strip[] that contains
    // Clusters close (closer than d)
    // to the line passing through the middle Cluster
    vector<Cluster> strip;
    int j = 0;
    for (int i = 0; i < n; i++)
        if (abs(P[i].Center.x - midCluster.Center.x) < d)
            strip.push_back(P[i]), j++;
 
    // Find the closest Clusters in strip.
    // Return the minimum of d and closest
    // distance is strip[]
    double minStrip = stripClosest(strip, j, d, tStrip, ExcludedPairs, MaxN);
    double minGlobal;
    if(d < minStrip)
    {
    	minGlobal = d;
    	get<0>(tGlobal) = get<0>(*t);
    	get<1>(tGlobal) = get<1>(*t);
    	get<2>(tGlobal) = get<2>(*t);
    } 
    else
    {
    	minGlobal = minStrip;
    	get<0>(tGlobal) = get<0>(tStrip);
    	get<1>(tGlobal) = get<1>(tStrip);
    	get<2>(tGlobal) = get<2>(tStrip);
    }

    // printf("minGlobal = %f\n", minGlobal);

    strip.clear();

    return minGlobal;
}

double closest(vector <Cluster> P, const int n, tuple<int, int, double>& tGlobal, vector<vector<int>>& ExcludedPairs)
{
    sort(P.begin(), P.end(), compareX);
    
    return closestUtil(P, n, tGlobal, ExcludedPairs, n + 1);
}