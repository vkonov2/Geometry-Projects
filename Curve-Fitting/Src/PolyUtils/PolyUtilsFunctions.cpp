#include "PolyUtilsFunctions.h"
#include "../ConvexUtils/ConvexHull.h"
#include <iostream>
#define eps 1e-15
#define accuracy 8192

void AddOnReper(VECTOR& e1, VECTOR& e2, const VECTOR e3)
{
    if (fabs(e3.x) > eps && fabs(e3.y) > eps)
    {
        e2.x = e3.y;
        e2.y = -e3.x;
        e2.z = 0.0;
        e2 /= e2.abs();

        e1 = e2 & e3;
        e1 /= e1.abs();

       /* printf("(%lf, %lf, %lf)\n", e1.x, e1.y, e1.z);
        printf("(%lf, %lf, %lf)\n", e2.x, e2.y, e2.z);
        printf("(%lf, %lf, %lf)\n", e3.x, e3.y, e3.z);*/

        return;
    }

    if (fabs(e3.x) > eps && fabs(e3.z) > eps)
    {
        e2.x = e3.z;
        e2.z = -e3.x;
        e2.y = 0.0;
        e2 /= e2.abs();

        e1 = e2 & e3;
        e1 /= e1.abs();

        /*printf("(%lf, %lf, %lf)\n", e1.x, e1.y, e1.z);
        printf("(%lf, %lf, %lf)\n", e2.x, e2.y, e2.z);
        printf("(%lf, %lf, %lf)\n", e3.x, e3.y, e3.z);*/

        return;
    }

    if (fabs(e3.y) > eps && fabs(e3.z) > eps)
    {
        e2.y = e3.z;
        e2.z = -e3.y;
        e2.x = 0.0;
        e2 /= e2.abs();

        e1 = e2 & e3;
        e1 /= e1.abs();

       /* printf("(%lf, %lf, %lf)\n", e1.x, e1.y, e1.z);
        printf("(%lf, %lf, %lf)\n", e2.x, e2.y, e2.z);
        printf("(%lf, %lf, %lf)\n", e3.x, e3.y, e3.z);*/

        return;
    }

    if (fabs(e3.x)> eps)
    {
        e2.x = 0;
        e2.y = 1;
        e2.z = 0.0;
        e2 /= e2.abs();

        e1 = e2 & e3;
        e1 /= e1.abs();

       /* printf("(%lf, %lf, %lf)\n", e1.x, e1.y, e1.z);
        printf("(%lf, %lf, %lf)\n", e2.x, e2.y, e2.z);
        printf("(%lf, %lf, %lf)\n", e3.x, e3.y, e3.z);*/

        return;
    }

    if (fabs(e3.y) > eps)
    {
        e2.x = 1;
        e2.y = 0;
        e2.z = 0.0;
        e2 /= e2.abs();

        e1 = e2 & e3;
        e1 /= e1.abs();

        /*printf("(%lf, %lf, %lf)\n", e1.x, e1.y, e1.z);
        printf("(%lf, %lf, %lf)\n", e2.x, e2.y, e2.z);
        printf("(%lf, %lf, %lf)\n", e3.x, e3.y, e3.z);*/

        return;
    }

    if (fabs(e3.z) > eps)
    {
        e2.x = 0;
        e2.y = 1;
        e2.z = 0.0;
        e2 /= e2.abs();

        e1 = e2 & e3;
        e1 /= e1.abs();

        /*printf("(%lf, %lf, %lf)\n", e1.x, e1.y, e1.z);
        printf("(%lf, %lf, %lf)\n", e2.x, e2.y, e2.z);
        printf("(%lf, %lf, %lf)\n", e3.x, e3.y, e3.z);*/

        return;
    }

    //printf("(%lf, %lf, %lf)\n", e3.x, e3.y, e3.z);
}

double RetMatchingError(POLYHEDRON* Pattern, POLYHEDRON* Real, VECTOR Norm)
{
    VECTOR2* RealProjection = new VECTOR2[Real->nverts];
    VECTOR2* PatternProjection = new VECTOR2[Pattern->nverts];

    int RealConvHullCount, PatternConvHullCount, id;
    int* RealHullInds = new int[Real->nverts];
    int* PatternHullInds = new int[Pattern->nverts];

    double err;

    net_func RealF(accuracy), PatternF(accuracy), Error(accuracy);

    RealConvHullCount = ConvexHullOfProjection(Real, Norm, RealHullInds, RealProjection);
    PatternConvHullCount = ConvexHullOfProjection(Pattern, Norm, PatternHullInds, PatternProjection);

    /*PrintProjection(RealConvHullCount, RealHullInds, RealProjection, "RealProj", ID);
    PrintProjection(PatternConvHullCount, PatternHullInds, PatternProjection, "PatternProj", ID);*/

    VECTOR2* RealPts = new VECTOR2[RealConvHullCount];
    VECTOR2* PatternPts = new VECTOR2[PatternConvHullCount];

    for (int i = 0; i < RealConvHullCount; i++)
    {
        id = RealHullInds[i];

        RealPts[i] = RealProjection[id];
    }

    for (int i = 0; i < PatternConvHullCount; i++)
    {
        id = PatternHullInds[i];

        PatternPts[i] = PatternProjection[id];
    }

    Set(RealF, RealConvHullCount, RealPts);
    Set(PatternF, PatternConvHullCount, PatternPts);

    /*RealF.fprint("D:\\YandexDisk\\Лиза\\Octonus\\RealF" + to_string(ID) + ".txt");
    PatternF.fprint("D:\\YandexDisk\\Лиза\\Octonus\\PatternF" + to_string(ID) + ".txt");
    ID++;*/
    L2_error(Error.f, PatternF.f, RealF.f, accuracy);

    err = Error.f[0];

   /* Polar Real_p_s(100, Real->nverts, RealProjection, RealConvHullCount, RealHullInds);
    Polar Pattern_p_s(100, Pattern->nverts, PatternProjection, PatternConvHullCount, PatternHullInds);*/

    delete[] RealProjection;
    delete[] PatternProjection;
    delete[] RealHullInds;
    delete[] PatternHullInds;

    /*printf("In function error via L2diff is %lf\n", error);
    printf("In function error via L2error is %lf\n", err);*/

    return err;
}

void RightOrientation(POLYHEDRON* Poly, VECTOR V[3]) // N for Rundist || OZ, Poly.Peak.z >= 0
{
    VECTOR x(1, 0, 0), y(0, 1, 0), z(0, 0, 1), VNull(0, 0, 0);
    SPIN SpinForward;

    EulerTransform(V[0], V[1], V[2], x, y, z, SpinForward);

    CONFORM C(SpinForward, VNull, 1);
    SIMILARITY S(C);

    TransformPolyhedron(Poly, S);

    V[0] = S(V[0]);
    V[1] = S(V[1]);
    V[2] = S(V[2]);
}

void MatchPolyMainSymAx(POLYHEDRON* Pattern, POLYHEDRON* Real, vector<int>& RealRundist, vector<int>& PatternRundist)
{
    net_func RealFPlus(accuracy), PatternFPlus(accuracy), errorPlus(accuracy);

    int PatternN, RealN;
    int id;
    double Err[2];

    PatternN = RecognizeRundist(Pattern, PatternRundist);
    RealN = RecognizeRundist(Real, RealRundist);

    PlaneInfo* PatternInfo = new PlaneInfo[Pattern->nsides];
    MassCenterZeroing(Pattern, PatternInfo);

    PlaneInfo* RealInfo = new PlaneInfo[Real->nsides];
    MassCenterZeroing(Real, RealInfo);

    SPIN SpinForward, SpinBackward;
    CONFORM ConForward[3], ConBackward[3];
    SIMILARITY SimForward[3], SimBackward[3];
    VECTOR PatternMainN, FinalNorm, Pattern_e1, Pattern_e2, Real_e1, Real_e2, n, s_c, n_g, VNull(0, 0, 0), RealSignNorms[2], V[3], * PatternNorms = new VECTOR[PatternN], * RealNorms = new VECTOR[RealN];
    PLANE PatternPlane, RealPlane;

    for (int i = 0; i < PatternN; i++)
    {
        id = PatternRundist[i];

        n = Pattern->side[id].plane.norm;
        n /= n.abs();
        s_c.x = PatternInfo[id].x;
        s_c.y = PatternInfo[id].y;
        s_c.z = PatternInfo[id].z;
        n_g = s_c + n;

        PatternNorms[i] = n_g;
    }

    for (int i = 0; i < RealN; i++)
    {
        id = RealRundist[i];

        n = Real->side[id].plane.norm;
        n /= n.abs();
        s_c.x = RealInfo[id].x;
        s_c.y = RealInfo[id].y;
        s_c.z = RealInfo[id].z;
        n_g = s_c + n;

        RealNorms[i] = n_g;
    }

    DefinePlaneCoeff(RealN, RealNorms, RealPlane);
    DefinePlaneCoeff(PatternN, PatternNorms, PatternPlane);

    PatternMainN = PatternPlane.norm;
    RealSignNorms[0] = RealPlane.norm;
    RealSignNorms[1] = -RealPlane.norm;

    AddOnReper(Pattern_e1, Pattern_e2, PatternMainN);
    AddOnReper(Real_e1, Real_e2, RealSignNorms[0]);

    V[0] = Pattern_e1;
    V[1] = Pattern_e2;
    V[2] = PatternMainN;

    RightOrientation(Pattern, V);

    PatternMainN = V[2];

    printf("Pattern V[2] = (%lf, %lf, %lf)\n", V[2].x, V[2].y, V[2].z);

    for (int i = 0; i < 2; i++)
    {
        EulerTransform(Real_e1, Real_e2, RealSignNorms[i],
            Pattern_e1, Pattern_e2, PatternMainN, SpinForward);
        SpinBackward = ~SpinForward;

        ConForward[i] = CONFORM(SpinForward, VNull, 1);
        SimForward[i] = SIMILARITY(ConForward[i]);
        ConBackward[i] = CONFORM(SpinBackward, VNull, 1);
        SimBackward[i] = SIMILARITY(ConBackward[i]);

        TransformPolyhedron(Real, SimForward[i]);

        Err[i] = RetMatchingError(Pattern, Real, Pattern_e1);

        TransformPolyhedron(Real, SimBackward[i]);
    }

    if (Err[0] < Err[1])
    {
        FinalNorm = RealSignNorms[0];
    }

    else
    {
        FinalNorm = RealSignNorms[1];
    }

    EulerTransform(Real_e1, Real_e2, FinalNorm,
        Pattern_e1, Pattern_e2, PatternMainN, SpinForward);
    SpinBackward = ~SpinForward;

    ConForward[2] = CONFORM(SpinForward, VNull, 1);
    SimForward[2] = SIMILARITY(ConForward[2]);

    TransformPolyhedron(Real, SimForward[2]);
    printf("After match (%lf, %lf, %lf)\n", SimForward[2](FinalNorm).x, SimForward[2](FinalNorm).y, SimForward[2](FinalNorm).z);
    printf("PatternMainN (%lf, %lf, %lf)\n", PatternMainN.x, PatternMainN.y, PatternMainN.z);

    delete[] PatternNorms;
    delete[] RealNorms;
    delete[] PatternInfo;
    delete[] RealInfo;
}

void FindCrossSection(const POLYHEDRON* Poly, PLANE P, vector <VECTOR3>& CrossSection)
{
    VECTOR3 BonePt, P1, P2;
    PHINDEX i1, i2;
    int N = Poly->nbones;
    double alpha;
    double A = P.norm.x, B = P.norm.y, C = P.norm.z, D = P.dist;

    for (int i = 0; i < N; i++)
    {
        i1 = Poly->bone[i].v0;
        i2 = Poly->bone[i].v1;
        P1 = Poly->vertex[i1];
        P2 = Poly->vertex[i2];
        alpha = -(D + A * P2.x + B * P2.y + C * P2.z) / (A * (P1.x - P2.x) + B * (P1.y - P2.y) + C * (P1.z - P2.z));

        if ((!(alpha > 1)) && (!(alpha < 0)))
            CrossSection.push_back(P1 * alpha + (1 - alpha) * P2);
    }
}

void FindSubPlaneInfo(PlaneInfo* Info, POLYHEDRON* Poly, int SideId)
{
    //printf("True area = %lf\n", Info->area);
    int index, index_pl;
    int len = length(Poly->side[SideId].vlist);
    double area;
    double x_sum = 0.0, y_sum = 0.0, z_sum = 0.0, global_subarea = 0.0;
    POINT3 Mass(Info[SideId].x, Info[SideId].y, Info[SideId].z);
    POINT3 M;
    POINT3 SubTriangleCenter;

    for (int i = 0; i < len - 1; i++)
    {
        SubPlaneInfo SubInfo;

        index = Poly->side[SideId].vlist[i];
        index_pl = Poly->side[SideId].vlist[i + 1];

        /*val0 = (Mass - Poly->vertex[index]).abs();
        val1 = (Mass - Poly->vertex[index_pl]).abs();
        val2 = (Poly->vertex[index] - Poly->vertex[index_pl]).abs();

        p = (val0 + val1 + val2) / 2.0;
        area = p * (p - val0) * (p - val1) * (p - val2);*/
        area = ((Mass - Poly->vertex[index]) & (Mass - Poly->vertex[index_pl])).abs();
        area /= 2.0;

        SubInfo.area = area;

        /*printf("area = %lf, SubInfo.area = %lf\n", area, SubInfo.area);

        printf("%lf + %lf = %lf\n", global_subarea, area, global_subarea + area);*/

        global_subarea += area;

        M = (Poly->vertex[index] + Poly->vertex[index_pl]) / 2.0;

        SubTriangleCenter = M + (1.0 / 3.0) * (Mass - M);

        SubInfo.x = SubTriangleCenter.x;
        SubInfo.y = SubTriangleCenter.y;
        SubInfo.z = SubTriangleCenter.z;

        Info->SubInfo[SideId].push_back(SubInfo);

        //if (SideId == 1)
        //{
        //    printf("Mass = (%lf, %lf, %lf)\n", Mass.x, Mass.y, Mass.z);
        //    printf("Poly->vertex[1] = (%lf, %lf, %lf)\n", Poly->vertex[index].x, Poly->vertex[index].y, Poly->vertex[index].z);
        //    printf("Poly->vertex[2] = (%lf, %lf, %lf)\n", Poly->vertex[index_pl].x, Poly->vertex[index_pl].y, Poly->vertex[index_pl].z);
        //    //printf("mp1 = %lf\n", val0);
        //    //printf("mp2 = %lf\n", val1);
        //    //printf("p1p2 = %lf\n", val2);
        //    printf("area = %lf\n\n", area);
        //}
    }

    for (int i = 0; i < len - 1; i++)
    {
        //printf("%d Subinfo area = %lf\n", i, Info->SubInfo[SideId][i].area);
  
        Info->SubInfo[SideId][i].mass = Info->SubInfo[SideId][i].area / global_subarea;
        x_sum += Info->SubInfo[SideId][i].mass * Info->SubInfo[SideId][i].x;
        y_sum += Info->SubInfo[SideId][i].mass * Info->SubInfo[SideId][i].y;
        z_sum += Info->SubInfo[SideId][i].mass * Info->SubInfo[SideId][i].z;
    }

    /*printf("True area = %lf\n", Info[SideId].area);
    printf("global_area = %lf\n\n", global_subarea);*/

    Info[SideId].x_area = x_sum;
    Info[SideId].y_area = y_sum;
    Info[SideId].z_area = z_sum;
}

void Area(const POLYHEDRON* Poly, int side_index, double& x_sum, double& y_sum, double& z_sum, double& area_sum, PlaneInfo* Info)
{
    int len = length(Poly->side[side_index].vlist) - 1;
    int index;

    x_sum = 0;
    y_sum = 0;
    z_sum = 0;

    for (int j = 0; j < len; j++)
    {
        index = Poly->side[side_index].vlist[j];

        x_sum += Poly->vertex[index].x;
        y_sum += Poly->vertex[index].y;
        z_sum += Poly->vertex[index].z;
    }

    area_sum = Plane3DArea(Poly->side[side_index], Poly->vertex);

    x_sum /= len;
    y_sum /= len;
    z_sum /= len;

    Info[side_index].x = x_sum;
    Info[side_index].y = y_sum;
    Info[side_index].z = z_sum;
    Info[side_index].area = area_sum;
    //printf("area_sum = %lf\n", area_sum);

    //start mass center via areas------------------------
    /*PLANE p = Poly->side[side_index].plane;

    SubPlaneInfo* SubInfo = new SubPlaneInfo[len];

    for (int j = 0; j < len; j++)
    {
        index = Poly->side[side_index].vlist[j];

        x_sum += Poly->vertex[index].x;
        y_sum += Poly->vertex[index].y;
        z_sum += Poly->vertex[index].z;
    }*/
}

void Volume(const POLYHEDRON* Poly, int side_index, double& volume, PlaneInfo* Info, const VECTOR mass_center)
{
    VECTOR v_center(mass_center.x - Info[side_index].x, mass_center.y - Info[side_index].y, mass_center.z - Info[side_index].z);
    Info[side_index].solid_angle_x = mass_center.x - v_center.x * (3.0 / 4.0);
    Info[side_index].solid_angle_y = mass_center.y - v_center.y * (3.0 / 4.0);
    Info[side_index].solid_angle_z = mass_center.z - v_center.z * (3.0 / 4.0);

    double area = Info[side_index].area;
    double h = fabs(Poly->side[side_index].plane.norm.x * mass_center.x +
        Poly->side[side_index].plane.norm.y * mass_center.y +
        Poly->side[side_index].plane.norm.z * mass_center.z + Poly->side[side_index].plane.dist) / (sqrt(Poly->side[side_index].plane.norm | Poly->side[side_index].plane.norm));

    Info[side_index].volume = (h * area) / 3.0;
    volume += Info[side_index].volume;
}

void FindSolidMassCenter(POLYHEDRON* Poly, PlaneInfo* Info, VECTOR& mass_center)
{
    double x_sum = 0.0, y_sum = 0.0, z_sum = 0.0, area_sum = 0.0, whole_area = 0.0;
    double volume_x_sum = 0.0, volume_y_sum = 0.0, volume_z_sum = 0.0, whole_volume = 0.0;

    for (int i = 0; i < Poly->nsides; i++)
    {
        x_sum = 0.0;
        y_sum = 0.0;
        z_sum = 0.0;
        area_sum = 0.0;

        Area(Poly, i, x_sum, y_sum, z_sum, area_sum, Info);

        whole_area += fabs(area_sum);
    }

    x_sum = 0.0;
    y_sum = 0.0;
    z_sum = 0.0;

    for (int i = 0; i < Poly->nsides; i++)
    {
        Info[i].mass = Info[i].area / whole_area;
        x_sum += Info[i].mass * Info[i].x;
        y_sum += Info[i].mass * Info[i].y;
        z_sum += Info[i].mass * Info[i].z;
    }

    VECTOR mass_center_local(x_sum, y_sum, z_sum);

    for (int i = 0; i < Poly->nsides; i++)
    {
        Volume(Poly, i, whole_volume, Info, mass_center_local);
    }

    for (int i = 0; i < Poly->nsides; i++)
    {
        Info[i].solid_angle_mass = Info[i].volume / whole_volume;
        volume_x_sum += Info[i].solid_angle_mass * Info[i].solid_angle_x;
        volume_y_sum += Info[i].solid_angle_mass * Info[i].solid_angle_y;
        volume_z_sum += Info[i].solid_angle_mass * Info[i].solid_angle_z;
    }

    mass_center.x = volume_x_sum;
    mass_center.y = volume_y_sum;
    mass_center.z = volume_z_sum;

    for (int i = 0; i < Poly->nsides; i++)
    {
        Info[i].x -= mass_center.x;
        Info[i].y -= mass_center.y;
        Info[i].z -= mass_center.z;

        Info[i].solid_angle_x -= mass_center.x;
        Info[i].solid_angle_y -= mass_center.y;
        Info[i].solid_angle_z -= mass_center.z;
    }

    //printf("Mass center via pyramids is (%lf, %lf, %lf)\n", mass_center.x, mass_center.y, mass_center.z);
}

void FindCentroid(POLYHEDRON* Poly, PlaneInfo* Info, VECTOR& mass_center)
{
    double mx = 0.0, my = 0.0, mz = 0.0;

    for (int i = 0; i < Poly->nverts; i++)
    {
        mx += Poly->vertex[i].x;
        my += Poly->vertex[i].y;
        mz += Poly->vertex[i].z;
    }

    mx /= Poly->nverts;
    my /= Poly->nverts;
    mz /= Poly->nverts;

    mass_center.x = mx;
    mass_center.y = my;
    mass_center.z = mz;

    //printf("Centroid %lf, %lf, %lf\n", mx, my, mz);
}

int OrtReper(VECTOR l, VECTOR& e_1, VECTOR& e_2, VECTOR& e_3)
{
    int flag = 0;
    double projection = 0.0, e = 1.0;
    POINT3 O(0, 0, 0);
    POINT3 A(l.x * 7, l.y * 3, l.z * 10); // point that doesn't belong to line O + l * t, t in R
    POINT3 C;
    VECTOR n_xy(0, 0, 1);
    VECTOR n; // perpendicular to vector l
    VECTOR v = A - O; // vector from point O to point A

    while (1.0 + e > 1.0)
        e /= 2.0;

    l /= l.abs();

    projection = v | l;
    C = projection * l;
    n = v - C;
    n /= n.abs();

    e_3 = l;
    e_1 = n;
    e_2 = e_3 & e_1;
    e_2 /= e_2.abs();

    return flag;
}

int FindOrientation(const VECTOR a, const VECTOR b, const VECTOR c)
{
    double Ax, Ay, Az;
    double res;

    Ax = b.y * c.z - b.z * c.y;
    Ay = b.x * c.z - b.z * c.x;
    Az = b.x * c.y - b.y * c.x;

    res = a.x * Ax - a.y * Ay + a.z * Az;

    if (res > 0)
        return 1;
    else
        return -1;
}

void FindProjection(const VECTOR n, const vector < VECTOR > Pts, vector < VECTOR2 >& projection)
{
    POINT3 O(0, 0, 0);
    VECTOR norm(n);
    VECTOR3 v;
    VECTOR x(1, 0, 0);
    VECTOR y(0, 1, 0);
    VECTOR z(0, 0, 1);
    VECTOR e1, e2;
    double len = sqrt(n | n), e = 1.0;
    double proj_x = 0.0, proj_y = 0.0;

    norm /= len;

    while (e + 1.0 > 1.0)
        e /= 2.0;

    if (!(fabs(len) > e))
    {
        printf("Normal vector is zero vector\n");

        return;
    }

    if (!(fabs(fabs(z | norm) - 1) > e))
    {

        e1.x = 1;
        e1.y = 0;
        e1.z = 0;

        e2.x = 0;
        e2.y = 1;
        e2.z = 0;
    }

    else if (!(fabs(fabs(x | norm) - 1) > e))
    {
        e1.x = 0;
        e1.y = 1;
        e1.z = 0;

        e2.x = 0;
        e2.y = 0;
        e2.z = 1;
    }

    else if (!(fabs(fabs(y | norm) - 1) > e))
    {
        e1.x = 0;
        e1.y = 0;
        e1.z = 1;

        e2.x = 1;
        e2.y = 0;
        e2.z = 0;
    }

    else
    {
        POINT3 O1;
        POINT3 O2;

        if (fabs(norm.x) > e && fabs(norm.y) > e)
        {
            O1.z = 0;
            O1.x = 1;
            O1.y = (-norm.x * 1) / norm.y;

            e1 = O1 - O;
            e2 = n & e1;
            e2 /= e2.abs();
        }

        else if (fabs(norm.x) > e && fabs(norm.z) > e)
        {
            O1.y = 0;
            O1.x = 3;
            O1.z = (-norm.x * 3) / norm.z;

            e1 = O1 - O;
            e2 = n & e1;
            e2 /= e2.abs();
        }

        else if (fabs(norm.z) > e && fabs(norm.y) > e)
        {
            O1.x = 0;
            O1.z = 3;
            O1.y = (-norm.z * 3) / norm.y;


            e1 = O1 - O;

            e2 = n & e1;
            e2 /= e2.abs();
        }
    }

    int nverts = (int)(Pts.size());

    len = sqrt(e1 | e1);

    if (!(fabs(len) > e))
    {
        printf("e1 vector is zero vector\n");

        return;
    }

    e1 /= len;

    len = sqrt(e2 | e2);

    if (!(fabs(len) > e))
    {
        printf("e2 vector is zero vector\n");

        return;
    }

    e2 /= len;

    for (int i = 0; i < nverts; i++)
    {
        v = Pts[i] - O;
        proj_x = v | e1;
        proj_y = v | e2;
        projection.push_back(VECTOR2(proj_x, proj_y));
    }
}

void FindProjection2(const VECTOR n, const vector < VECTOR > Pts, vector < VECTOR2 >& projection, const VECTOR e2)
{
    POINT3 O(0, 0, 0);
    VECTOR3 v;
    VECTOR e1 = e2 & n;
    e1 /= e1.abs();
    // double len = sqrt(n | n), e = 1.0;
    double proj_x = 0.0, proj_y = 0.0;
    int nverts = Pts.size();

    for (int i = 0; i < nverts; i++)
    {
        v = Pts[i] - O;
        proj_x = v | e1;
        proj_y = v | e2;
        projection.push_back(VECTOR2(proj_x, proj_y));
    }
}

void FindProjectionSinglePoint(const VECTOR n, VECTOR pt, VECTOR2& projection, const VECTOR e2)
{
    POINT3 O(0, 0, 0);
    VECTOR3 v;
    VECTOR e1 = e2 & n;
    e1 /= e1.abs();
    // double len = sqrt(n | n), e = 1.0;
    double proj_x = 0.0, proj_y = 0.0;

    v = pt - O;
    proj_x = v | e1;
    proj_y = v | e2;
    projection = VECTOR2(proj_x, proj_y);
}

int ConvexHullOfProjection(const POLYHEDRON* Poly, const VECTOR n, int* hullInds, VECTOR2* projection)
{
    /*POINT3 O(0, 0, 0);
    VECTOR norm(n);
    VECTOR3 v;
    VECTOR x(1, 0, 0);
    VECTOR y(0, 1, 0);
    VECTOR z(0, 0, 1);
    VECTOR e1, e2;
    double len = sqrt(n | n), e = 1.0;
    double proj_x = 0.0, proj_y = 0.0;
    int hull_len = 0;

    norm /= len;

    while (e + 1.0 > 1.0)
        e /= 2.0;

    if (!(fabs(len) > e))
    {
        printf("Normal vector is zero vector\n");

        return -1;
    }

    if (!(fabs(fabs(z | norm) - 1) > e))
    {

        e1.x = 1;
        e1.y = 0;
        e1.z = 0;

        e2.x = 0;
        e2.y = 1;
        e2.z = 0;
    }

    else if (!(fabs(fabs(x | norm) - 1) > e))
    {
        e1.x = 0;
        e1.y = 1;
        e1.z = 0;

        e2.x = 0;
        e2.y = 0;
        e2.z = 1;
    }

    else if (!(fabs(fabs(y | norm) - 1) > e))
    {
        e1.x = 0;
        e1.y = 0;
        e1.z = 1;

        e2.x = 1;
        e2.y = 0;
        e2.z = 0;
    }

    else
    {
        POINT3 O1;
        POINT3 O2;

        if (fabs(norm.x) > e && fabs(norm.y) > e)
        {
            O1.z = 0;
            O1.x = 1;
            O1.y = (-norm.x * 1) / norm.y;

            e1 = O1 - O;
            e2 = n & e1;
            e2 /= e2.abs();
        }

        else if (fabs(norm.x) > e && fabs(norm.z) > e)
        {
            O1.y = 0;
            O1.x = 3;
            O1.z = (-norm.x * 3) / norm.z;

            e1 = O1 - O;
            e2 = n & e1;
            e2 /= e2.abs();
        }

        else if (fabs(norm.z) > e && fabs(norm.y) > e)
        {
            O1.x = 0;
            O1.z = 3;
            O1.y = (-norm.z * 3) / norm.y;


            e1 = O1 - O;

            e2 = n & e1;
            e2 /= e2.abs();
        }
    }

    int nverts = Poly->nverts;

    len = sqrt(e1 | e1);

    if (!(fabs(len) > e))
    {
        printf("e1 vector is zero vector\n");

        return -1;
    }

    e1 /= len;

    len = sqrt(e2 | e2);

    if (!(fabs(len) > e))
    {
        printf("e2 vector is zero vector\n");

        return -1;
    }

    e2 /= len;

    for (int i = 0; i < nverts; i++)
    {
        v = Poly->vertex[i] - O;
        proj_x = v | e1;
        proj_y = v | e2;
        projection[i].x = proj_x;
        projection[i].y = proj_y;
    }*/

    int nverts = Poly->nverts;
    vector < VECTOR > Pts;
    vector < VECTOR2 > VecProjection;

    for (int i = 0; i < nverts; i++)
        Pts.push_back(Poly->vertex[i]);

    FindProjection(n, Pts, VecProjection);

    for (int i = 0; i < nverts; i++)
        projection[i] = VecProjection[i];

    int hull_len = CreateConvexHull(nverts, projection, hullInds);

    Pts.clear();
    VecProjection.clear();

    return hull_len;
}

// void MovePole(vector<VECTOR>& Pts)
// {
//     bool isPole = true;
//     VECTOR Pole(0, 0, 1);
//     double eps = 1.0;
//     int step = 0;

//     VECTOR v_null(0, 0, 0);
//     VECTOR v_x(1, 0, 0);
//     VECTOR v_y(0, 1, 0);

//     while(1.0 + eps > 1.0)
//         eps /= 2.0;

//     while(isPole)
//     {
//         isPole = false;
//         for(auto i: Pts)
//             if( !( (i - Pole).abs() > eps ) )
//             {
//                 isPole = true;
//                 printf("yes\n");
//                 break;
//             }

//         if(isPole == false)
//             continue;

//         if(step % 2 ==0)
//         {
//             SPIN sp (v_x, M_PI/360.0);
//             CONFORM con(sp, v_null, 1.0);
//             SIMILARITY sim(con);

//             for(auto i: Pts)
//                 i = sim(i);
//         }
//         else
//         {
//             SPIN sp (v_y, M_PI/360.0);
//             CONFORM con(sp, v_null, 1.0);
//             SIMILARITY sim(con);

//             for(auto i: Pts)
//                 i = sim(i);
//         }
//     }
// }

// void StereographicProjection(const vector<VECTOR> Pts, vector<VECTOR2>& StereoProj)
// {
//     StereoProj.clear();
//     MovePole(Pts);
//     VECTOR2 projection;

//     for(auto i: Pts)
//     {
//         projection.x = i.x / (1 - i.z);
//         projection.y = i.y / (1 - i.z);

//         StereoProj.push_back(projection);
//     }
// }