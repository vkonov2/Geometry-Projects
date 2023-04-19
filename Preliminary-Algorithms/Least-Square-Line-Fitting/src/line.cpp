#include <iostream>
#include <math.h>
#include "class_point.h"
#include "class_point.cpp"

using namespace std;

double find_b (Point * points, int n);
double find_d (Point * points, double b, double a, int n);


double find_b (Point * points, int n)
{
	double sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0, sum_total1 = 0, sum_total2 = 0;
	
	for(int i = 0; i < n; i ++)
	{
		sum1 += ( points[i].x ) * ( points[i].x );
		sum2 += ( points[i].y );
		sum3 += ( points[i].x ) * ( points[i].y );
		sum4 += ( points[i].x );
	}
	
	sum_total1 += sum1 * sum2 - sum3 * sum4;
	sum1 = 0;
	sum2 = 0;
	sum3 = 0;
	sum4 = 0;
	
	for(int i = 0; i < n; i ++)
	{
		sum1 += ( points[i].y ) * ( points[i].y );
		sum2 += ( points[i].x );
		sum3 += ( points[i].x ) * ( points[i].y );
		sum4 += ( points[i].y );
	}
	
	sum_total2 += sum1 * sum2 - sum3 * sum4;
	
	return ( sum_total1 ) / ( sum_total2 );
}


double find_d (Point * points, double b, double a, int n)
{
	double sum_a = 0, sum_b = 0;
	
	for(int i = 0; i < n; i ++)
	{
		sum_a += points[i].x;
		sum_b += points[i].y;
	}
	
	return -( a * sum_a + b * sum_b ) / n;	
}




int main(void)
{
	double err, start;
	int n;

	printf (">>>enter count of points:\n");

	start = time_now ();

	err = check_input ();
	if (err == -0.1)
		return -1;
	else
	{
		n = (int) err;
	}
	

	int n;
	double a = 1, b = 0, d = 0;
	
	FILE * f = fopen("points.txt", "w");

	if (!f) 
    {
       printf (">>>can't open file\n");
       return -1;
    }
    
	
	
	if (scanf("%d", &n)!=1)
	{
		printf(">>>entered wrong initial data\n");
	}
	
	Point * points = new Point[n];
	
	getData(points, n, f);
    
    /*for (int i = 0; i < n; ++i) 
	{
    	points[i].keyboard_put_without_weight();
    	
    	fprintf( f, "%1.2lf %1.2lf ", points[i].x, points[i].y );
    }*/
    
    fclose(f);
    
    f = fopen("line.txt", "w");

	if (f == NULL) 
    {
       	printf ("Can't open file\n");
       	exit(1);
    }
    
    try
	{
    	int count_x = 0, count_y = 0;
    	double eps = 1.0;
    	
		while(1.0 + eps > 1.0)
		{
			eps = eps / 2.0;
		}
    	
    	for(int i = 0; i < n; i++) 
    	{
    		if( points[i].x > eps ) count_x ++;
    		if( points[i].y > eps ) count_y ++;
		}
		if(count_x == 0 && count_y == 0) throw 0; // it's a point
    	if(count_x == 0) throw 1;	// a == 0
    	if(count_y == 0) throw 2;	// b == 0
    	throw 3;
    }
    
    catch (int flag) 
	{
  		if( flag == 0)
  		{
    		fprintf( f, "%1.2lf %1.2lf %1.2lf", 0.0, 0.0, 0.0 );
    		fclose(f);
    		std::cout<<"a, b, d = "<<0.0<<" "<<0.0<<" "<<0.0<<" "<<endl;
    		
    		delete [] points;
    		
    		return 0;
		}
		if( flag == 1) 
		{
			b = 0;
			d = 0;
			a = 1;
			
    		fprintf( f, "%1.2lf %1.2lf %1.2lf", a, b, d );
    		fclose(f);
    		std::cout<<"a, b, d = "<<a<<" "<<b<<" "<<d<<" "<<endl;
    		
    		delete [] points;
    		
    		return 0;
		}
		if( flag == 2) 
		{
			b = 1;
			a = 0;
			d = 0;

    		fprintf( f, "%1.2lf %1.2lf %1.2lf", a, b, d );
    		fclose(f);
    		
    		std::cout<<"a, b, d = "<<a<<" "<<b<<" "<<d<<" "<<endl;
    		
    		delete [] points;
    		
    		return 0;
		}
		if( flag == 3) 
		{
			b = find_b (points, n);
		}
		
	}
    
    /*if( a*a + b*b != 1)
    {
    	a = a / sqrt(a*a + b*b);
    	b = b / sqrt(a*a + b*b);
	}*/
	
    d = find_d (points, b, a, n);
    
    std::cout<<"a, b, d = "<<a<<" "<<b<<" "<<d<<" "<<endl;
    
    f = fopen("line_points.txt", "w");

	if (f == NULL) 
    {
       printf ("Can't open file\n");
       exit(1);
    }
    
    fprintf( f, "%lf %lf %lf %lf", 1.0, (d-a)/b, 2.0, (d-2*a)/b );
    
    fclose(f);

    f = fopen("line_coefs.txt", "w");

	if (f == NULL) 
    {
       printf ("Can't open file\n");
       exit(1);
    }
    
    fprintf( f, "%lf %lf %lf", a, b, d);
    
    fclose(f);
    
    delete points;
    
    return 0;
}
