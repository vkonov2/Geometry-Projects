#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

double time_now(void);
double func (double x);
double integral(double x0, double x, double I0);
int positive (double a);
double solve (double a, double eps);

/*
    The program allows you to calculate the integrals
*/

double time_now(void)
{
    struct timeval tv2;
    struct timezone tz;
    gettimeofday(&tv2, &tz);
    return tv2.tv_sec+tv2.tv_usec/1000000.0;
}

double func (double x)
{
    double res;
    res = x + 1/(pow(x,2) + 1);
    if (x > 0)
    {
        res = sqrt(res);
        return res;
    }
    return res;
}

double integral(double x0, double x, double I0)
{
    double I1, delta, s1, s2, s3, I;
    
    I1 = 0;
    delta = x - x0;
    
    while (fabs (x0 - x) > 1e-14)
    {
        s1 = func(x0 + delta/2);
        s2 = (func(x0 + delta / 4) + func(x0 + 3*delta/4)) / 2;
        s3 = fabs(s1 - s2);
        
        if (s3 < 1e-6)
        {
            I1 += s1 * delta;
            x0 += delta;
            
            if (20 * s3 < fabs(1e-6 * delta))
                delta = 1.87 * delta;
            
            if (fabs(x0 + delta) > x)
                delta = x - x0;
        }
        else
            delta /= 2;
    }
    I = I0 + I1;
    
    return I;
}

int positive (double a)
{
    int x0;
    double x = 1e-14;
    double f0, I0;
    
    I0 = integral (1, x, 0);
    f0 = I0 - a * x;
    
    while (f0 < 0)
    {
        x++;
        f0 = integral (x-1, x, I0) - a * x;
        I0 = integral (x-1, x, I0);
    }
    x0 = x + 1;
    
    return x0;
}

double solve (double a, double eps)
{
    double x0, left0, left, right0, right;
    double f0, f1, I0, I00, I1, I11, I3, I4;
    double mid0, mid1;
    double mid;
    
    x0 = 1e-14;
    I0 = integral (1, x0, 0);
    f0 = I0 - a * x0;
    
    left = 0;
    right = positive (a);
    
    mid0 = left + 1e-6;
    mid1 = (left + right) / 2;
    
    I1 = integral (x0, mid1, I0);
    f1 = I1 - a * mid1;
    
    while (fabs (f1) > eps)
    {
        I3 = integral (1, left, 0);
        I4 = integral (1, right, 0);
        
        if ((I3 - a * left) * f1 < 0)
        {
            mid0 = mid1;
            right = mid1;
        }
        else if ((I4 - a * right) * f1 < 0)
        {
            mid0 = mid1;
            left = mid1;
        }
        else if (fabs (f1) < 1e-14)
            return mid1;
        
        mid1 = (left + right) / 2;
        f0 = f1;
        f1 = integral (1, mid1, 0) - a * mid1;
        I3 = integral (left0, left, I3);
        I4 = integral (right0, right, I4);
    }
    
    return mid1;
}

int main (void)
{
    double a;
    double start;
    
    printf ("Введите a:");
    if (scanf("%lf", &a) != 1)
        return -1;
    
    start = time_now();
    printf ("root = %.18lf\n", solve(a, 1e-14));
    
    /*printf ("root = %.18lf\n", solve(a, 1e-1));
    printf ("root = %.18lf\n", solve(a, 1e-2));
    printf ("root = %.18lf\n", solve(a, 1e-3));
    printf ("root = %.18lf\n", solve(a, 1e-4));
    printf ("root = %.18lf\n", solve(a, 1e-5));
    printf ("root = %.18lf\n", solve(a, 1e-6));
    printf ("root = %.18lf\n", solve(a, 1e-7));
    printf ("root = %.18lf\n", solve(a, 1e-8));
    printf ("root = %.18lf\n", solve(a, 1e-9));
    printf ("root = %.18lf\n", solve(a, 1e-10));
    printf ("root = %.18lf\n", solve(a, 1e-11));
    printf ("root = %.18lf\n", solve(a, 1e-12));
    printf ("root = %.18lf\n", solve(a, 1e-13));
    printf ("root = %.18lf\n", solve(a, 1e-14));*/
    
    printf("METHOD: %lf secs\n", time_now()-start);
    
    return 0;
}
