#include <iostream>
#include <string>
#include <cmath>

#include "point.h"
using namespace std;

Point::Point()
{
	this->x = 0;
	this->y = 0;
	this->w = 1;
}

void Point::equaling(Point & a)
{
	this->x = a.x;
	this->y = a.y;
	this->w = a.w;
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

void Point::keyboard_put_with_weight()
{
	printf(">>>print x, y, w:");
    std::cin>> x >> y >> w;
}

void Point::keyboard_put_without_weight()
{
	w = 1;
	printf(">>>print x, y:");
    std::cin>> x >> y;
}

void Point::print()
{
	std::cout<<"p("<<this->x<<", "<<this->y<<")  ";
}


Point& Point::operator =(const Point &a)
{
	this->x = a.x;
	this->y = a.y;
	this->w = a.w;
	return *this;
}
