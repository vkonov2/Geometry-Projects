#define _CRT_SECURE_NO_WARNINGS

#include "Transform.h"
#include <iostream>

void MassCenterZeroing(POLYHEDRON* Poly, PlaneInfo* Info)
{
    VECTOR mass_center(0, 0, 0);
    FindSolidMassCenter(Poly, Info, mass_center);
    //printf("Pyramid mass center is (%lf, %lf, %lf)\n", mass_center.x, mass_center.y, mass_center.z);
    //FindCentroid(Poly, Info, mass_center);

    VECTOR v(1, 0, 0);
    VECTOR v_move(-mass_center.x, -mass_center.y, -mass_center.z);
    SPIN sp(v, 0);
    CONFORM con(sp, v_move, 1.0);
    SIMILARITY sim(con);

    TransformPolyhedron(Poly, sim);

    FindSolidMassCenter(Poly, Info, mass_center);
    //FindCentroid(Poly, Info, mass_center);
    //printf("Second mass center is (%lf, %lf, %lf)\n", mass_center.x, mass_center.y, mass_center.z);
}

void TransformPolyhedron(POLYHEDRON* pPoly, const SIMILARITY& sim)
{
    for (int i = 0; i < pPoly->nverts; i++) {
        pPoly->vertex[i] = sim(pPoly->vertex[i]);
    }
    for (int i = 0; i < pPoly->nsides; i++) 
    {
        pPoly->side[i].plane = TransformBy(sim, pPoly->side[i].plane);
    }
}

PLANE TransformBy(const SIMILARITY& sim, const PLANE& pln)
{
    PLANE pnew;
    VECTOR3 v0, v1, v2;
    pnew.norm.x = sim.rot.x[0] * pln.norm[0] + sim.rot.x[1] * pln.norm[1] + sim.rot.x[2] * pln.norm[2];
    pnew.norm.y = sim.rot.y[0] * pln.norm[0] + sim.rot.y[1] * pln.norm[1] + sim.rot.y[2] * pln.norm[2];
    pnew.norm.z = sim.rot.z[0] * pln.norm[0] + sim.rot.z[1] * pln.norm[1] + sim.rot.z[2] * pln.norm[2];
    v0 = -pln.dist * pln.norm;
    v1 = sim(v0);
    pnew.dist = -(pnew.norm[0] * v1[0] + pnew.norm[1] * v1[1] + pnew.norm[2] * v1[2]);

    //logger.Print(LM_DEBUG, "%e %e\n", pln % v0, pnew % v1);

    return pnew;
}

void EulerTransform(VECTOR e_1, VECTOR e_2, VECTOR e_3,
    const VECTOR e_1_constant, const VECTOR e_2_constant, const VECTOR e_3_constant, SPIN& spin_forward)
{
    double phi, theta, ksi;
    double e = 1.0;
    double cos_ = 0.0;
    double sin_ = 0.0;
    double sin_abs_val = 0.0;
    int sign = 0;
    VECTOR f = e_3 & e_3_constant;
    VECTOR v_null(0, 0, 0);
    VECTOR sin_v;

    /*printf("(%lf, %lf, %lf)\n", e_1.x, e_1.y, e_1.z);
    printf("(%lf, %lf, %lf)\n", e_2.x, e_2.y, e_2.z);
    printf("(%lf, %lf, %lf)\n", e_3.x, e_3.y, e_3.z);*/

    while (e + 1.0 > 1.0)
        e /= 2.0;

    if (sqrt(f | f) > e)
        f /= sqrt(f | f);
    else
    {
        f.x = 0;
        f.y = -1;
        f.z = 0;
    }

    //phi begin
    sin_v = e_1 & f;

    if ((sin_v | e_3) < 0)
        sign = -1;
    else
        sign = 1;

    sin_abs_val = sin_v.abs();
    sin_abs_val /= e_1.abs();
    sin_abs_val /= f.abs();
    sin_ = sin_abs_val; 
    cos_ = e_1 | f;
    cos_ /= e_1.abs();
    cos_ /= f.abs();

    if (fabs(cos_) > e)
    {
        phi = atan(sin_ / cos_);//phi in [0, 2pi)
    }
    else
    {
        if (sign < 0)
        {
            phi = -90 * (M_PI / 180);
        }
        else
        {
            phi = 90 * (M_PI / 180);
        }
    }

    if (cos_ > 0)
    {
        if (sign < 0)
            phi *= -1;
    }

    if (cos_ < 0)
    {
        phi = M_PI + phi;

        if (sign < 0)
            phi *= -1;
    }
    //phi end

    //theta begin
    cos_ = e_3_constant | e_3;
    cos_ /= e_3_constant.abs();
    cos_ /= e_3.abs();
    theta = acos(cos_);//theta in [0, pi]
    //theta end

    //ksi begin
    sin_v = f & e_1_constant;

    if ((sin_v | e_3_constant) < 0)
        sign = -1;
    else
        sign = 1;

    sin_abs_val = sin_v.abs();
    sin_abs_val /= e_1_constant.abs();
    sin_abs_val /= f.abs();
    sin_ = sin_abs_val;
    cos_ = e_1_constant | f;
    cos_ /= e_1_constant.abs();
    cos_ /= f.abs();

    if (fabs(cos_) > e)
    {
        ksi = atan(sin_ / cos_);//phi in [0, 2pi)
    }
    else
    {
        if (sign < 0)
        {
            ksi = -90 * (M_PI / 180);
        }
        else
        {
            ksi = 90 * (M_PI / 180);
        }
    }

    if (cos_ > 0 && fabs(cos_) > e)
    {
        if (sign < 0)
            ksi *= -1;
    }

    if (cos_ < 0 && fabs(cos_) > e)
    {
        ksi = M_PI + ksi;

        if (sign < 0)
            ksi *= -1;
    }
    //ksi end

    SPIN spin0(e_3, phi);
    CONFORM con0(spin0, v_null, 1);
    SIMILARITY sim0(con0);
    e_1 = sim0(e_1);
    e_2 = sim0(e_2);
    e_3 = sim0(e_3);

    SPIN spin1(e_1, theta);
    CONFORM con1(spin1, v_null, 1); 
    SIMILARITY sim1(con1);
    e_1 = sim1(e_1);
    e_2 = sim1(e_2);
    e_3 = sim1(e_3);

    SPIN spin2(e_3, ksi);
    CONFORM con2(spin2, v_null, 1);
    SIMILARITY sim2(con2);
    e_1 = sim2(e_1);
    e_2 = sim2(e_2);
    e_3 = sim2(e_3);

    spin_forward = spin2 * spin1 * spin0;
}