#pragma once

#include "Clusterization.h"

#include <iostream>
#include <vector>

using namespace std;

bool isIncluded(const int i, const int j, vector<vector<int>>& ExcludedPairs);
double bruteForce(vector <Cluster> P, const int n, tuple<int, int, double>& tGlobal, vector<vector<int>>& ExcludedPairs, const int MaxN);
double min(const double x, const double y);
double stripClosest(Cluster strip, const int size, const double d, tuple<int, int, double>& tGlobal, vector<vector<int>>& ExcludedPairs, const int MaxN);
double closestUtil(vector <Cluster> P, const int n, tuple<int, int, double>& tGlobal, vector<vector<int>>& ExcludedPairs, const int MaxN);
double closest(vector <Cluster> P, const int n, tuple<int, int, double>& tGlobal, vector<vector<int>>& ExcludedPairs);
