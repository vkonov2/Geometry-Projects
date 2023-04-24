#pragma once

#include "../PolyUtils/Axes.h"
#include "../WException.h"
#include "../Polar/Polar.h"
#include "../PolyUtils/Transform.h"
#include "../../Draw/Draw.h"
#include <iostream>

/*------------------------------------------------------------------
void swap(int* a, int i, int j);      функция для генерации перестановок
bool NextPermutation(int* a, int n);  функция для генерации перестановок

int ComparisonTest(POLYHEDRON* Pattern_Poly, POLYHEDRON* Real_Poly);
	функция сопоставляет два многогранника: с помощью углов Эйлера Real_Poly -> Pattern_Poly
------------------------------------------------------------------*/

void swap(int* a, int i, int j);
bool NextPermutation(int* a, int n);
int ComparisonTest(POLYHEDRON*, POLYHEDRON*);
