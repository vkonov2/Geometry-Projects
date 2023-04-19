#include <iostream>
#include <string>
#include <cmath>

#include "functions.h"
#include "point.h"
using namespace std;

int main (void)
{
    printf ("\n>>>enter count of points: ");

    double err = check_input ();
    
    if (fabs(err-0.1) <  1e-16)
        return -1;
    else
    {
        int n = (int) err;

        Point* points = new Point[n];
    
        FILE* f = fopen("points.txt", "w");

        if (!f) 
        {
           printf (">>>can't open file -> points.txt\n");
           delete [] points;
           return -2;
        }

        get_data(points, n, f);

        fclose (f);

        double start = time_now ();

        err = solve (points, n);
        if (err < 0)
        {
            printf(">>>error in function -> solve\n");
            delete [] points;
            return err;
        }

        printf("\nused time: %lf secs\n\n", time_now()-start);

        delete [] points;
    }

    return 0;
}
