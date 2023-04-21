#ifndef __CLASS_POINT_H__
#define __CLASS_POINT_H__
using namespace std;

class Point{
	public:
		double x,y;
		Point();
		void equaling(Point & a);
		Point(double x, double y);
		double return_x();
		double return_y();
		void keyboard_put();
		void print();
		Point& operator =(const Point &a);

};

#endif 
