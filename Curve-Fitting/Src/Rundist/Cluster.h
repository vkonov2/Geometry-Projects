#pragma once

#include <vector>
using namespace std;


// size_t getClusters(const arma::mat& points, const double epsilon, vector<vector<arma::mat>>& CLUSTERS);
// void cart2pol(arma::mat& pt);
// void pol2cart(arma::mat& pt);
// double getQuarterDataAnglle(vector<arma::mat> quarterPoints, const double e);
double GirthC(const vector<vector<double>>& projection, const double epsilon);
int norm_clusters(const vector<vector<double>>& norms, const double epsilon, vector<int>& CLUSTERS);
