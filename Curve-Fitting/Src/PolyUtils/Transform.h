#pragma once

#include "PolyUtilsFunctions.h"
#include "Structs.h"
#include "../Geometry/Polyhedr.h"
#include "../Geometry/Similart.h"
#include "../Draw/Draw.h"

/*------------------------------------------------------------------------------------------------------
void MassCenterZeroing(POLYHEDRON* Poly, PlaneInfo* Info);
	функция сдвигает центр масс (по телесным объемам) многогранника в (0, 0, 0)

	Poly - многогранник
	Info - массив структур, i-ый элемент которого содержит поля: [обязан быть определен при передаче в фунцию]

void TransformPolyhedron(POLYHEDRON* pPoly, const SIMILARITY& sim);
	pPoly - многогранник
	sim - преобразование, применяемое к многограннику

PLANE TransformBy(const SIMILARITY& sim, const PLANE& pln);
	pln - плоскость
	sim - преобразование, применяемое к плоскости pln

void EulerTransform(POLYHEDRON* Poly, VECTOR e_1, VECTOR e_2, VECTOR e_3, const VECTOR e_1_constant, const VECTOR e_2_constant, const VECTOR e_3_constant, SPIN& spin_forward);
	Функция переводит один репер в другой, используя углы Эйлера

	Poly - многогранник
	e_1, e_2, e_3 - тройка векторов, соответствующая Poly
	e_1_constant, e_2_constant, e_3_constant - тройка векторов, которая является образом e_1, e_2, e_3
	spin_forward - поворот на углы Эйлера: e_1-> e_1_constant, e_2-> e_2_constant, e_3-> e_3_constant

void ScaleAlongAxisRegardingSpaceCenter(POLYHEDRON* Poly, VECTOR l, double k);
	Функция растягивает многогранник вдоль координатной оси с направляющим вектором l в
	k раз относительно точки (0, 0, 0)

	Poly - многогранник
	l in {(1, 0, 0), (0, 1, 0), (0, 0, 1)}
	k - коэффициент растяжения

void ScaleAlongAxisRegardingMassCenter(POLYHEDRON* Poly, const VECTOR l, const double k);
	Функция растягивает многогранник вдоль оси с направляющим вектором l в k раз относительно
	центра масс многогранника

	Poly - многогранник
	l - направляющий вектор оси растяжения
	k - коэффициент растяжения
------------------------------------------------------------------------------------------------------*/

void MassCenterZeroing(POLYHEDRON* Poly, PlaneInfo* Info);
void TransformPolyhedron(POLYHEDRON* pPoly, const SIMILARITY& sim);
PLANE TransformBy(const SIMILARITY& sim, const PLANE& pln);
void EulerTransform(VECTOR e_1, VECTOR e_2, VECTOR e_3,
	const VECTOR e_1_constant, const VECTOR e_2_constant, const VECTOR e_3_constant, SPIN& spin_forward);
// void scale_along_defined_axis_regarding_mass_center(POLYHEDRON* Poly, VECTOR l, double k);
// int ScaleAlongAxisRegardingSpaceCenter(POLYHEDRON* Poly, VECTOR l, double k);