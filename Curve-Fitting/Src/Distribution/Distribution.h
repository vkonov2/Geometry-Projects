#ifndef DISTRIBUTION_H_
#define DISTRIBUTION_H_

#include <iostream>
#include <random>
#include <vector>

#include "../Geometry/Vector2.h"

using namespace std;

// Generating uniformly distributed data
// |
// V
// template<class T>
void UniformDistribution(vector<double> &obj, double min, double max);

// Generating normally distributed data
// |
// V
// template<class T>
void NormalDistribution(vector<double> &obj, double mean, double stddev);

#endif