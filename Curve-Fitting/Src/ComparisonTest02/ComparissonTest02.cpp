#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

#include "ComparissonTest02.h"
#define eps 1e-15
#define accuracy 4096
const char* PathToWrite0 = "D:\\YandexDisk\\Лиза\\Octonus\\";

using namespace std;

//void AddOnReper(VECTOR& e1, VECTOR& e2, const VECTOR e3)
//{
//    if (fabs(e3.x) > eps && fabs(e3.y) > eps)
//    {
//        e2.x = e3.y;
//        e2.y = -e3.x;
//        e2.z = 0.0;
//        e2 /= e2.abs();
//
//        e1 = e2 & e3;
//        e1 /= e1.abs();
//
//        return;
//    }
//
//    if (fabs(e3.x) > eps && fabs(e3.z) > eps)
//    {
//        e2.x = e3.z;
//        e2.z = -e3.x;
//        e2.y = 0.0;
//        e2 /= e2.abs();
//
//        e1 = e2 & e3;
//        e1 /= e1.abs();
//
//        return;
//    }
//
//    if (fabs(e3.y) > eps && fabs(e3.z) > eps)
//    {
//        e2.y = e3.z;
//        e2.z = -e3.y;
//        e2.x = 0.0;
//        e2 /= e2.abs();
//
//        e1 = e2 & e3;
//        e1 /= e1.abs();
//
//        return;
//    }
//}

void PrintProjection0(int ConvHullCount, int* PatternHullInds, VECTOR2* Projection, string id1, int i);
void PrintProjection0(int ConvHullCount, int* PatternHullInds, VECTOR2* Projection, string id1, int i)
{
    const string fullDirname__string = string(PathToWrite0) + id1 + string(to_string(i)) + string(".txt");
    const char* FullDirName = fullDirname__string.c_str();

    FILE* f = fopen(FullDirName, "w");
    int id_pl, id;

    for (int i = 0; i < ConvHullCount - 1; i++)
    {
        id = PatternHullInds[i];
        id_pl = PatternHullInds[i + 1];

        fprintf(f, "%lf, %lf\n", Projection[id].x, Projection[id].y);
        fprintf(f, "%lf, %lf\n\n", Projection[id_pl].x, Projection[id_pl].y);
    }

    id = PatternHullInds[ConvHullCount - 1];
    id_pl = PatternHullInds[0];

    fprintf(f, "%lf, %lf\n", Projection[id].x, Projection[id].y);
    fprintf(f, "%lf, %lf\n\n", Projection[id_pl].x, Projection[id_pl].y);

    fclose(f);
}

void ComparissonTest02(POLYHEDRON* PatternPoly, POLYHEDRON* RealPoly)
{
    system("del /q D:\\YandexDisk\\Лиза\\Octonus\\*.*");
	vector<int> RealRundist;
	vector<int> PatternRundist;
	int PatternN, RealN;
    int id;
    double BestPlusNorm, BestMinusNorm;
    int BestPlusId, BestMinusId;

    DrawAxPolyTogether(RealPoly, "D:\\YandexDisk\\Лиза\\Octonus\\", "OriReal_", "No");
    DrawAxPolyTogether(PatternPoly, "D:\\YandexDisk\\Лиза\\Octonus\\", "OriPattern_", "No");

	PatternN = RecognizeRundist(PatternPoly, PatternRundist);
	RealN = RecognizeRundist(RealPoly, RealRundist);

	PlaneInfo* PatternInfo = new PlaneInfo[PatternPoly->nsides];
	MassCenterZeroing(PatternPoly, PatternInfo);

	PlaneInfo* RealInfo = new PlaneInfo[RealPoly->nsides];
	MassCenterZeroing(RealPoly, RealInfo);

    /*DrawAxPolyTogether(RealPoly, "D:\\YandexDisk\\Лиза\\Octonus\\", "OriReal_", "No");*/

    VECTOR FinalNorm;
    SPIN SpinForward, SpinBackward;
    VECTOR PatternMainN, RealMainN;
    VECTOR Pattern_e1, Pattern_e2;
    VECTOR Real_e1, Real_e2;
    VECTOR n;
    VECTOR s_c;
    VECTOR n_g;
    VECTOR* PatternNorms = new VECTOR[PatternN]; //нормали рундистов
    VECTOR* RealNorms = new VECTOR[RealN];    //нормали рундистов
    PLANE PatternPlane;
    PLANE RealPlane;

    for (int i = 0; i < PatternN; i++)
    {
        id = PatternRundist[i];

        n = PatternPoly->side[id].plane.norm;
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

        n = RealPoly->side[id].plane.norm;
        n /= n.abs();
        s_c.x = RealInfo[id].x;
        s_c.y = RealInfo[id].y;
        s_c.z = RealInfo[id].z;
        n_g = s_c + n;

        RealNorms[i] = n_g;

        //printf("%lf, %lf, %lf\n", RealNorms[i].x, RealNorms[i].y, RealNorms[i].z);
    }

    DefinePlaneCoeff(RealN, RealNorms, RealPlane);
    DefinePlaneCoeff(PatternN, PatternNorms, PatternPlane);

    PatternMainN = PatternPlane.norm;
    RealMainN = RealPlane.norm;

    //Best for Plus begin-----------------------------------------------------------

    AddOnReper(Pattern_e1, Pattern_e2, PatternMainN);
    AddOnReper(Real_e1, Real_e2, RealMainN);

    VECTOR ax[3];

    ax[0] = Real_e1;
    ax[1] = Real_e2;
    ax[2] = RealMainN;

    VECTOR mass_center(0, 0, 0);
   // DrawAxes(ax, "D:\\YandexDisk\\Лиза\\Octonus\\RealAx", mass_center);
    //DrawAxPolyTogether(RealPoly, "D:\\YandexDisk\\Лиза\\Octonus\\", "OriReal_", "No");

    EulerTransform(Real_e1, Real_e2, RealMainN,
        Pattern_e1, Pattern_e2, PatternMainN, SpinForward);
    SpinBackward = ~SpinForward;

    VECTOR v_null(0, 0, 0);
    CONFORM ConForwardPlus(SpinForward, v_null, 1);
    SIMILARITY SimForwardPlus(ConForwardPlus);

    CONFORM ConBackwardPlus(SpinBackward, v_null, 1);
    SIMILARITY SimBackwardPlus(ConBackwardPlus);

    TransformPolyhedron(RealPoly, SimForwardPlus);

    //DrawAxPolyTogether(RealPoly, "D:\\YandexDisk\\Лиза\\Octonus1\\", "AxesReal_", "No");

    int* RealHullInds = new int[RealPoly->nverts];
    int* PatternHullInds = new int[PatternPoly->nverts];

    VECTOR2* RealProjection = new VECTOR2[RealPoly->nverts];
    VECTOR2* PatternProjection = new VECTOR2[PatternPoly->nverts];

    int RealConvHullCount = ConvexHullOfProjection(RealPoly, PatternMainN, RealHullInds, RealProjection);
    int PatternConvHullCount = ConvexHullOfProjection(PatternPoly, PatternMainN, PatternHullInds, PatternProjection);

    //double* error = new double[accuracy];

    net_func RealFPlus(accuracy), PatternFPlus(accuracy), errorPlus(accuracy);

    VECTOR2* RealPtsPlus = new VECTOR2[RealConvHullCount];
    VECTOR2* PatternPtsPlus = new VECTOR2[PatternConvHullCount];

    for (int i = 0; i < RealConvHullCount; i++)
    {
        id = RealHullInds[i];

        RealPtsPlus[i] = RealProjection[id];
    }

    for (int i = 0; i < PatternConvHullCount; i++)
    {
        id = PatternHullInds[i];

        PatternPtsPlus[i] = PatternProjection[id];
    }

    Set(RealFPlus, RealConvHullCount, RealPtsPlus);
    Set(PatternFPlus, PatternConvHullCount, PatternPtsPlus);

    L2_error(errorPlus.f, RealFPlus.f, PatternFPlus.f, accuracy);

    double min_value = errorPlus.f[0];
    double s, value;
    int min_id = 0;

    for (int m = 1; m < accuracy; m++)
    {


        value = errorPlus.f[m];

        //f << m << " " << value << "\n";
        if (value < min_value)
        {
            //printf("m = %d, ||.||^2=%lf, min= %lf\n", m, value, min);
            s = m;
            min_value = value;
            min_id = m;
        }
    }

    BestPlusId = min_id;
    BestPlusNorm = min_value;
    //DrawAxPolyTogether(RealPoly, "D:\\YandexDisk\\Лиза\\Octonus\\", "PlusReal_", "No");
    TransformPolyhedron(RealPoly, SimBackwardPlus);
    //DrawAxPolyTogether(RealPoly, "D:\\YandexDisk\\Лиза\\Octonus\\", "BackPlusReal_", "No");

    //Best for Minus begin-----------------------------------------------------------

    EulerTransform(Real_e1, Real_e2, -RealMainN,
        Pattern_e1, Pattern_e2, PatternMainN, SpinForward);
    SpinBackward = ~SpinForward;

    CONFORM ConForwardMinus(SpinForward, v_null, 1);
    SIMILARITY SimForwardMinus(ConForwardMinus);

    CONFORM ConBackwardMinus(SpinBackward, v_null, 1);
    SIMILARITY SimBackwardMinus(ConBackwardMinus);

    TransformPolyhedron(RealPoly, SimForwardMinus);

    RealConvHullCount = ConvexHullOfProjection(RealPoly, PatternMainN, RealHullInds, RealProjection);
    PatternConvHullCount = ConvexHullOfProjection(PatternPoly, PatternMainN, PatternHullInds, PatternProjection);

    net_func RealFMinus(accuracy), PatternFMinus(accuracy), errorMinus(accuracy);

    VECTOR2* RealPtsMinus = new VECTOR2[RealConvHullCount];
    VECTOR2* PatternPtsMinus = new VECTOR2[PatternConvHullCount];

    for (int i = 0; i < RealConvHullCount; i++)
    {
        id = RealHullInds[i];

        RealPtsMinus[i] = RealProjection[id];
    }

    for (int i = 0; i < PatternConvHullCount; i++)
    {
        id = PatternHullInds[i];

        PatternPtsMinus[i] = PatternProjection[id];
    }

    Set(RealFMinus, RealConvHullCount, RealPtsMinus);
    Set(PatternFMinus, PatternConvHullCount, PatternPtsMinus);

    L2_error(errorMinus.f, RealFMinus.f, PatternFMinus.f, accuracy);

    min_value = errorMinus.f[0];
    min_id = 0;

    for (int m = 1; m < accuracy; m++)
    {


        value = errorMinus.f[m];

        //f << m << " " << value << "\n";
        if (value < min_value)
        {
            //printf("m = %d, ||.||^2=%lf, min= %lf\n", m, value, min);
            s = m;
            min_value = value;
            min_id = m;
        }
    }

    BestMinusId = min_id;
    BestMinusNorm = min_value;
    //DrawAxPolyTogether(PatternPoly, "D:\\YandexDisk\\Лиза\\Octonus\\", "MinusReal_", "No");
    TransformPolyhedron(RealPoly, SimBackwardMinus);
    //DrawAxPolyTogether(RealPoly, "D:\\YandexDisk\\Лиза\\Octonus\\", "BackMinusReal_", "No");


    //Best BestNorm is found-------------------------------------------------

    if (BestMinusNorm < BestPlusNorm)
    {
        FinalNorm = -RealPlane.norm;
        min_id = BestMinusId;
    }

    else
    {
        FinalNorm = RealPlane.norm;
        min_id = BestPlusId;
    }

    //min_id = 3412;
    double angle = (2 * M_PI * min_id) / accuracy;

    cout << "min_id = " << min_id << endl;
    cout << (angle * 180) / (M_PI) << endl;

    EulerTransform(Real_e1, Real_e2, FinalNorm,
        Pattern_e1, Pattern_e2, PatternMainN, SpinForward);

    CONFORM ConForward(SpinForward, v_null, 1);
    SIMILARITY SimForward(ConForward);

    TransformPolyhedron(RealPoly, SimForward);

    SPIN SpinBestTurn(PatternMainN, -angle);
    CONFORM ConBestTurn(SpinBestTurn, v_null, 1);
    SIMILARITY SimBestTurn(ConBestTurn);

    TransformPolyhedron(RealPoly, SimBestTurn);

    /*VECTOR ax[3];
    VECTOR mass_center(0, 0, 0);
    DrawAxes(ax, "D:\\python\\Delete\\PatternAx.txt", mass_center);*/

    RealConvHullCount = ConvexHullOfProjection(RealPoly, PatternMainN, RealHullInds, RealProjection);
    PatternConvHullCount = ConvexHullOfProjection(PatternPoly, PatternMainN, PatternHullInds, PatternProjection);

    PrintProjection0(PatternConvHullCount, PatternHullInds, PatternProjection, "PatternProj", 0);
    PrintProjection0(RealConvHullCount, RealHullInds, RealProjection, "RealProj", 0);

    DrawAxPolyTogether(PatternPoly, "D:\\YandexDisk\\Лиза\\Octonus\\", "Pattern_", "No");
    DrawAxPolyTogether(RealPoly, "D:\\YandexDisk\\Лиза\\Octonus\\", "ResReal_", "No");

    delete[] RealPtsPlus;
    delete[] RealPtsMinus;
    delete[] RealNorms;
    delete[] PatternNorms;
    delete[] RealProjection;
    delete[] PatternProjection;
    delete[] RealHullInds;
    delete[] PatternHullInds;
    //delete[] error;
    delete[] RealInfo;
    delete[] PatternInfo;



}