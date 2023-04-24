#pragma once

#include "../Geometry/_defs.h"
#include <vector>
#include <map>
#include <iostream>

using namespace std;
/*-----------------------------------------------------------------------------------------------------------
Info - массив структур, i-ый элемент которого содержит поля: 
		area - площадь i-ой грани
		x = (1 \ n) * sum_{i = 0}^{n - 1} x_i координата центра i-ой грани
		y = (1 \ n) * sum_{i = 0}^{n - 1} y_i координата центра i-ой грани
		z = (1 \ n) * sum_{i = 0}^{n - 1} z_i координата центра i-ой грани
		solid_angle_x, solid_angle_y, solid_angle_z - координаты центра i-ой пирамиды, основание которой - i-ая грань
		solid_angle_mass - масса i-ой грани в терминах телесных углов
		volume - телесный объем, соответствующий i-ой грани
-----------------------------------------------------------------------------------------------------------*/

struct SubPlaneInfo
{
	double area = 0.0;
	double x = 0.0, y = 0.0, z = 0.0;
	double mass = 0.0;
};

struct PlaneInfo
{
	map <int, vector<SubPlaneInfo > > SubInfo;

	double area = 0.0;
	double x = 0.0, y = 0.0, z = 0.0;
	double x_area = 0.0, y_area = 0.0, z_area = 0.0;
	double mass = 0.0;
	double solid_angle_x = 0.0, solid_angle_y = 0.0, solid_angle_z = 0.0;
	double solid_angle_mass = 0.0;
	double volume = 0.0;
};