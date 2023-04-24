#define _CRT_SECURE_NO_WARNINGS

#include "Axes.h"
#include <iostream>

void MultMatr(double** A, int n, double** eigen_vectors, int id);
void MultMatr(double** A, int n, double** eigen_vectors, int id)
{
    double sum[3];

    for (int i = 0; i < 3; i++)
    {
        sum[i] = 0.0;

        for (int j = 0; j < 3; j++)
        {
            sum[i] += A[i][j] * eigen_vectors[id][j];
        }
    }
}

void PrintMatrix(double** A);
void PrintMatrix(double** A)
{
    printf("Matrix A:\n");
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("%.5lf, ", A[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

//void FindEigenValues(double** A, double* eigen_values)

int Axis(POLYHEDRON* Poly, double** eigen_vectors, double* eigen_values, PlaneInfo* Info)
{
    double J_x = 0.0, J_y = 0.0, J_z = 0.0, J_xy = 0.0, J_xz = 0.0, J_yz = 0.0;
    double y_i = 0.0, x_i = 0.0, z_i = 0.0, mass_i = 0.0;
    double mass_sum = 0.0;

    for (int i = 0; i < Poly->nsides; i++)
    {
        x_i = Info[i].solid_angle_x;
        y_i = Info[i].solid_angle_y;
        z_i = Info[i].solid_angle_z;
        mass_i = Info[i].solid_angle_mass;
        mass_sum += mass_i;
        J_x += mass_i * (y_i * y_i + z_i * z_i);
        J_y += mass_i * (x_i * x_i + z_i * z_i);
        J_z += mass_i * (y_i * y_i + x_i * x_i);
        J_xy += mass_i * (y_i * x_i);
        J_xz += mass_i * (z_i * x_i);
        J_yz += mass_i * (y_i * z_i);
    }

    //printf("mass_sum = %lf\n", mass_sum);

    /*for (int i = 0; i < Poly->nverts; i++)
    {
        x_i = Poly->vertex[i].x;
        y_i = Poly->vertex[i].y;
        z_i = Poly->vertex[i].z;
        mass_i = 1.0;
        J_x += mass_i * (y_i * y_i + z_i * z_i);
        J_y += mass_i * (x_i * x_i + z_i * z_i);
        J_z += mass_i * (y_i * y_i + x_i * x_i);
        J_xy += mass_i * (y_i * x_i);
        J_xz += mass_i * (z_i * x_i);
        J_yz += mass_i * (y_i * z_i);
    }*/

    double** A = new double* [3];
    
    if (!A)
    {
        printf("Can't alocate memory for A in function Axis(POLYHEDRON* Poly, double** eigen_vectors, double* eigen_values)\n");
 
        return -9;
    }

    for (int i = 0; i < 3; i++)
    {
        A[i] = new double[3];

        if (!A[i])
        {
            printf("Can't alocate memory for A[%d] in function Axis(POLYHEDRON* Poly, double** eigen_vectors, double* eigen_values)\n", i);

            for (int k = 0; k < i; k++)
            {
                delete[] A[k];
            }

            delete[] A;

            return -10;
        }
    }

    A[0][0] = J_x;
    A[0][1] = -J_xy;
    A[0][2] = -J_xz;
    A[1][0] = -J_xy;
    A[1][1] = J_y;
    A[1][2] = -J_yz;
    A[2][0] = -J_xz;
    A[2][1] = -J_yz;
    A[2][2] = J_z;

    //PrintMatrix(A);

    Jacobi(3, A, eigen_values, eigen_vectors);

    double norm = 0.0;

    for (int i = 0; i < 3; i++)
    {
        norm = sqrt(eigen_vectors[i][0] * eigen_vectors[i][0] + eigen_vectors[i][1] * eigen_vectors[i][1] +
            eigen_vectors[i][2] * eigen_vectors[i][2]);
        eigen_vectors[i][0] /= norm;
        eigen_vectors[i][1] /= norm;
        eigen_vectors[i][2] /= norm;
    }

    for (int i = 0; i < 3; i++)
    {
        delete[] A[i];
    }

    delete[] A;
  
    return 0;
}