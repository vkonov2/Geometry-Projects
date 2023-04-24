#include "FunctionsCT02.h"

using namespace std;

double FindBound(int n, double* DistMin)
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

int HierarchicalClustering(int n, double bound, vector<vector<double> >& DistMatr, vector<int>& Rundist)
{
	int flag = 0; // flag == 1, если для i существует точка, расстояние до которой <= bound, иначе flag == 0
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

int RecognizeRundist(POLYHEDRON* Poly, std::vector<int> &Rundist)
{
	int ns = Poly->nsides;
	int count;
	double Dist, bound;
	double MinDist = 0.0;
	double* DistMin = new double[ns];
	vector<vector<double> > DistMatr; // матрица дистанций
	vector<double> VertDistVec;
	VECTOR DistVec, CurrentVertex;
	VECTOR m_c(0, 0, 0);
	VECTOR n;
	VECTOR s_c;
	VECTOR n_g;
	VECTOR* Norms = new VECTOR[ns];
	PlaneInfo* Info = new PlaneInfo[Poly->nsides];
	FindSolidMassCenter(Poly, Info, m_c);

	/*if (!DistMin)
	{
		printf("Can't alloc memory for DistMin\n");
		return;
	}*/

	for (int i = 0; i < ns; i++)
	{
		n = Poly->side[i].plane.norm;
		n /= n.abs();
		s_c.x = Info[i].x;
		s_c.y = Info[i].y;
		s_c.z = Info[i].z;
		// n_g = s_c + n;
		n_g = n;
		Norms[i] = n_g;

		/*if(i < 10)
			printf("(%lf, %lf, %lf)\n", n_g.x, n_g.y, n_g.z);*/
	}
	// printf("\n\n");

	// FILE* g = fopen("/home/elizabeth/Octonus/modelassociation/LinuxVersion_1.0/DistMin.txt", "w");
	FILE* g = fopen("DistMin.txt", "w");

	for (int i = 0; i < ns - 1; i++)
	{
		CurrentVertex = Norms[i];
		VertDistVec.clear();
		MinDist = (CurrentVertex - Norms[i + 1]).abs();

		for (int j = i + 1; j < ns; j++)
		{
			DistVec = CurrentVertex - Norms[j];
			Dist = DistVec.abs();
			VertDistVec.push_back(Dist);

			if (MinDist > Dist)
				MinDist = Dist;
		}

		DistMin[i] = MinDist;
		fprintf(g, "%lf ", MinDist);
		DistMatr.push_back(VertDistVec);
	}

	fclose(g);

	/*for (int i = 0; i < ns; i++)
	{
		for (int j = 0; j < ns - i - 1; j++)
			cout << DistMatr[i][j] << " ";
		
		cout << "\n";
	}*/

	std::sort(DistMin, DistMin + ns - 1);

	/*for (int i = 0; i < ns; i++)
		printf("%.17lf ", DistMin[i]);*/

	bound = FindBound(ns - 1, DistMin);

	//cout << "Bound = " << bound << endl;

	count = HierarchicalClustering(ns - 1, bound, DistMatr, Rundist);

	//cout << "Count = " << count << endl;

	VertDistVec.clear();
	DistMatr.clear();
	delete[] DistMin;
	delete[] Norms;
	delete[] Info;

	return count;
}