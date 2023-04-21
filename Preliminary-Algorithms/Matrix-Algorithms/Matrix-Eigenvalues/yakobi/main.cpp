#include <iostream>
#include <string>
#include <cmath>

#include "functions.h"
using namespace std;

int main(int argc, char const *argv[])
{
    const char* filename;
    int n, m, k, err;
    double eps;

    if (!((argc == 5 || argc == 6)
        && sscanf (argv[1], "%d", &n) == 1 && n > 0
        && sscanf (argv[2], "%d", &m) == 1 && m > 0 && m <= n
        && sscanf (argv[3], "%lf", &eps) == 1 && fabs(eps) > 1e-16 && fabs(eps) <= 1
        && sscanf (argv[4], "%d", &k) == 1 && k >= 0 && k <= 4))
    {
        printf ("\n>>>usage: %s n m eps k [filename]\n\n", argv[0]);
        return 1;
    }

    if (argc == 6 && k == 0)
        filename = argv[5];

    err = solution_alloc (n, m, eps, k, filename);
    if (err < 0)
    {
        printf(">>>error in func -> solution_alloc\n");
        return err;
    }

    return 0;
}
