#include "class_point.h"
#include <math.h>
#include <iostream>
using namespace std;

Point::Point(){
	this->x = 0;
	this->y = 0;
	
}

void Point::equaling(Point & a)   //copy-constructor
		{
			this->x = a.x;
			this->y = a.y;
		}



Point::Point(double x, double y)
{
	this->x = x;
	this->y = y;
}

double Point::return_x()
{
	return this->x;
}


double Point::return_y()
{
	return this->y;
}

void Point::keyboard_put()
{
	std::cout<<"give me x mthfck ";
    std::cin>> x;
    std::cout<<"and y mthfck ";
    std::cin>> y;
}

void Point::print()
{
	std::cout<<"p("<<this->x<<", "<<this->y<<")  ";
}


Point& Point::operator =(const Point &a)  // equaling
		{
			this->x = a.x;
			this->y = a.y;
			return *this;
		}














