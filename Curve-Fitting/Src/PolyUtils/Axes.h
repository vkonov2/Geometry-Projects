#pragma once

#include "../Geometry/AllGeometry.h"
#include "PolyUtilsFunctions.h"
#include "../Geometry/Polyhedr.h"
#include "../Mathem/Jacobi.h"
#include "PolyUtilsFunctions.h"

using namespace std;

/*------------------------------------------------------------------------------------------------------
���������� ������� ���� �������.
Poly - ������������, ����� ���� �������� ��������� � ����� (0, 0, 0) [������ ���� ��������� ��� �������� � ������]
eigen_vectors - ��������� ��� ����  [�� ������ ���� ��������� ��� �������� � ������]
eigen_values - ��������� ��� ����������� ��������, ��������������� eigen_vectors [�� ������ ���� ��������� ��� �������� � ������]
Info - ������ ��������, i-�� ������� �������� �������� ����: [�� ������ ���� ��������� ��� �������� � ������]
    area - ������� i-�� �����
    x = (1 \ n) * sum_{i = 0}^{n - 1} x_i ���������� ������ i-�� �����
    y = (1 \ n) * sum_{i = 0}^{n - 1} y_i ���������� ������ i-�� �����
    z = (1 \ n) * sum_{i = 0}^{n - 1} z_i ���������� ������ i-�� �����
    solid_angle_x, solid_angle_y, solid_angle_z - ���������� ������ i-�� ��������, ��������� ������� - i-�� �����
    solid_angle_mass - ����� i-�� ����� � �������� �������� �����
    volume - �������� �����, ��������������� i-�� �����

�� ��������� ������� � eigen_vectors � eigen_values ����� ����������� �������� � ����������� �������� ��������������.
------------------------------------------------------------------------------------------------------*/

int Axis(POLYHEDRON* Poly, double** eigen_vectors, double* eigen_values, PlaneInfo* Info);