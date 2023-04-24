
#include "Distribution.h"

// template<class T>
void UniformDistribution(vector<double> &obj, double min, double max)
{
	random_device rand_dev;
	mt19937 generator(rand_dev());

	uniform_real_distribution<double> distribution(min, max);

	int size = (int)(obj.size());

	for (int i = 0; i < size; i++)
		obj[i] += distribution(generator);

	return;
}

// template<class T>
void NormalDistribution(vector<double> &obj, double mean, double stddev)
{
	random_device rand_dev;
	mt19937 generator(rand_dev());

	normal_distribution<double> distribution(mean, stddev);

	int size = (int)(obj.size());

	for (int i = 0; i < size; i++)
		obj[i] += distribution(generator);

	return;
}