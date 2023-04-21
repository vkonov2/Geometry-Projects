#include <iostream>
#include <string>
#include <cmath>

#include "functions.h"
using namespace std;

int main(int argc, char const *argv[])
{
    const char * filename;
    int n, m, k;

    n = stoi (argv[1]);
    if (n < 0)
    {
        printf(">>>wrong initial data\n");
        return -1;
    }

    m = stoi (argv[2]);
    if ((m < 0)||(m > n*n))
    {
        printf(">>>wrong initial data\n");
        return -1;
    }

    k = stoi (argv[3]);
    if ((k < 0)||(k > 4))
    {
        printf(">>>wrong initial data\n");
        return -1;
    }

    if (k == 0)
        filename = argv[4];

    solution (n, m, k, filename);

    return 0;
}