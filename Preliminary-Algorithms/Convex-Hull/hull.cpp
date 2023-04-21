#include<iostream>
#include <math.h>
#include "class_point.h"
#include "class_point.cpp"
//#include "quick_sort.cpp"
#include <list>
using namespace std;
bool right_turn(std::list<Point>& L);
int partition_x (Point * points, int low, int high);
void quick_Sort_by_x(Point * points, int low, int high);
int partition_y (Point * points, int low, int high);
void quick_Sort_by_y(Point * points, int low, int high);





bool right_turn(std::list<Point>& L)
{
	//std::cout<<"FUCK"<<endl;
	Point * p = new Point[3];  // points_for_determinant                                            
	list <Point> :: iterator t = L.end();
	advance(t, -1);
	p[2] = *t;
	advance(t, -2);
	p[0] = *t;
	advance(t, 1);
	p[1] = (*t);
	//p[0].print();
	//p[1].print();
	//p[2].print();
	double det = p[1].x * p[2].y - p[1].y * p[2].x - p[0].x * p[2].y + p[0].y * p[2].x + p[0].x * p[1].y - p[0].y * p[1].x;
	delete [] p;
	//std::cout<<"det = "<<det<<endl;
	std::cout<<"det / fabs(det) = "<<det / fabs(det)<<endl;
	//std::cout<<p[1].x<<"*"<<p[2].y<<" - "<<p[1].y<<"*"<<p[2].x<<" - "<<p[0].x<<"*"<<p[2].y<<" + "<<p[0].y<<"*"<<p[2].x<<" + "<<p[0].x<<"*"<<p[1].y<<" - "<<p[0].y<<"*"<<p[1].x<<endl;
	if(   det / fabs(det) == -1   ) return 1;
	else return 0;
}


int partition_x (Point * points, int low, int high)
{
    Point * points_reserve = new Point[2];
    points_reserve[0] = points[high];
 
    int i = (low - 1);  

    for (int j = low; j <= high - 1; j++)
    {
        if (points[j].x < points_reserve[0].x)
        {
            i++;   
            points_reserve[1] = points[i];
            points[i] = points[j];
            points[j] = points_reserve[1];
        }
    }
    points_reserve[1] = points[i + 1];
    points[i + 1] = points[high];
    points[high] = points_reserve[1];
    delete [] points_reserve;
    return (i + 1);
}


void quick_Sort_by_x(Point * points, int low, int high)
{
    if (low < high)
    {
        int pi = partition_x(points, low, high);

        quick_Sort_by_x(points, low, pi - 1);  
        quick_Sort_by_x(points, pi + 1, high); 
    }
} 


int partition_y (Point * points, int low, int high)
{
    Point * points_reserve = new Point[2];
    points_reserve[0] = points[high];
 
    int i = (low - 1);  

    for (int j = low; j <= high - 1; j++)
    {
        if (points[j].y < points_reserve[0].y)
        {
            i++;   
            points_reserve[1] = points[i];
            points[i] = points[j];
            points[j] = points_reserve[1];
        }
    }
    points_reserve[1] = points[i + 1];
    points[i + 1] = points[high];
    points[high] = points_reserve[1];
    delete [] points_reserve;
    return (i + 1);
}


void quick_Sort_by_y(Point * points, int low, int high)
{
    if (low < high)
    {
        int pi = partition_y(points, low, high);

        quick_Sort_by_y(points, low, pi - 1);  
        quick_Sort_by_y(points, pi + 1, high); 
    }
}



int main(void){
	int n;
	std::cout<<"enter point count :3 ";
	std::cin>>n;
	Point * points = new Point[n];
    for (int i = 0; i < n; ++i) 
	{
    	points[i].keyboard_put();
    	//std::cout<<"Point number "<<i<<" has x = "<<points[i].x<<" and y = "<<points[i].y<<endl;
    }
    quick_Sort_by_x(points, 0, n-1);	
    std::cout<<"After X_sort "<<endl;
    for (int i = 0; i < n; ++i) 
	{
    	points[i].print();
    }
    std::cout<<endl;
    
    int count = 0;
    for (int i = 1; i < n; ++i) 
	{
    	if( points[i-1].x == points[i].x ) 
    	{
    		count++;
    		std::cout<<"Yessssssss "<<endl;
		}
    	else
    	{
    		std::cout<<"Y_sort "<<i-count<<"and "<<i<<endl;
    		quick_Sort_by_y(points, i-count, i);
    		count = 0;
		}
		if(count == n-1 ) quick_Sort_by_y(points, 0, n-1);
    }
    
    std::cout<<"After Y_sort "<<endl;
    for (int i = 0; i < n; ++i) 
	{
    	points[i].print();
    }
    std::cout<<endl;
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    list < Point > L_upper;
    L_upper.push_front(points[0]);
    L_upper.push_back(points[1]);
    list <Point> :: iterator t = L_upper.end();
    //std::cout<<"upper list after 0 and 1:"<<endl;
	//for (t = L_upper.begin(); t != L_upper.end(); t++) (*t).print();
	//std::cout<<endl;
    for(int i = 2; i < n; i++)
    {
    	L_upper.push_back(points[i]);
    	//std::cout<<"upper list after i:"<<endl;
 	    //for (t = L_upper.begin(); t != L_upper.end(); t++) (*t).print();
 	    //std::cout<<endl;
    	while ( L_upper.size() > 2 && right_turn(L_upper) != 1 )
    	{
    		t = L_upper.end();
    		advance(t, -2);
    		L_upper.erase(t);
		}
		//std::cout<<"upper list after del:"<<endl;
		//for (t = L_upper.begin(); t != L_upper.end(); t++) (*t).print();
		//std::cout<<endl;
		
	}
	//std::cout<<endl;
	
	list < Point > L_lower;
    L_lower.push_front(points[n-1]);
    L_lower.push_back(points[n-2]);
    //std::cout<<"lower list after n-1 and n-2:"<<endl;
	//for (t = L_lower.begin(); t != L_lower.end(); t++) (*t).print();
	//std::cout<<endl;
    t = L_lower.end();
    for(int i = n-3; i >= 0; i--)
    {
    	L_lower.push_back(points[i]);
    	while ( L_lower.size() > 2 && right_turn(L_lower) != 1 )
    	{
    		t = L_lower.end();
    		advance(t, -2);
    		L_lower.erase(t);
		}
		
	}
	if( L_lower.size() != 0)
	{
		t = L_lower.begin();
        L_lower.erase(t);	
	}
	if( L_lower.size() != 0)
	{
		t = L_lower.end();
		advance(t, -1);
        L_lower.erase(t);
	}
	
	
	/*std::cout<<"upper list:"<<endl;
	for (t = L_upper.begin(); t != L_upper.end(); t++) (*t).print();
	std::cout<<"lower list:"<<endl;
	for (t = L_lower.begin(); t != L_lower.end(); t++) (*t).print();
	std::cout<<endl;*/
	
	
	
	
	
	
	
	
	
	
	std::cout<<"Final list:"<<endl;
	for (t = L_upper.begin(); t != L_upper.end(); t++) (*t).print();
	if( L_lower.size() == 0) return 0;
    for (t = L_lower.begin(); t != L_lower.end(); ++t) 
    {
    	L_upper.push_back(points[1]);
    	(*t).print();
	}
    L_lower.clear();
    delete [] points;
    
   
    return 0;
}
