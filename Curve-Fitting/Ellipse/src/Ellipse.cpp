// Ellipse.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "hdr/Ellipse.h"

#include "../../Src/Geometry/Vector2.h"
#include "../../Src/CurveGenerator/CurveGenerator.h"
#include "../../Src/CurveGenerator/EllipseFunctions.h"
#include "../../Src/Draw/DrawCurve.h"
#include "../../Src/utils.h"

int main(void)
{
    string GNUPLOTPATH = "gnudat/";
    const int DataFlag = 1;

    switch(DataFlag)
    {
        case 0:
        {
            vector<double> COEFFS(6);

            COEFFS[0] = 1.0;
            COEFFS[1] = 1.0;
            COEFFS[2] = -1.0;
            COEFFS[3] = 0.0;
            COEFFS[4] = 0.0;
            COEFFS[5] = -0.75;

            Normalize(COEFFS);

            VectorXd MAF(5);
            Coeffs2MAF(COEFFS, MAF);

            COEFFS.clear();

            break;
        }
        case 1:
        {
            const int PointsCount = (int)(1e+2);

            double Xc = 0.0;
            double Yc = 0.0;
            double A = 3.0;
            double B = 1.0;
            double Angle = 0.0;

            double beginAngle = 0.0;
            double endAngle = 360.0;

            int DistribCase = 0;

            int FlagDistribAngle = 1;
            int FlagDistribRadius = 1;

            int FlagPlot = 1;

            vector<VECTOR2> POINTS((int)(1e+6));

            double dstrAngleP1, dstrRadiusP1;
            double dstrAngleP2, dstrRadiusP2;
            switch(FlagDistribAngle)
            {
                case 0:
                {
                    dstrAngleP1 = 0.0;
                    dstrAngleP2 = 0.0;
                    break;
                }
                case 1:
                {
                    dstrAngleP1 = 0.0;
                    dstrAngleP2 = 0.02;
                    break;
                }
                case 2:
                {
                    dstrAngleP1 = -0.02;
                    dstrAngleP2 = 0.02;
                    break;
                }
                case 3:
                {
                    dstrAngleP1 = 0.0;
                    dstrAngleP2 = 0.002;
                    break;
                }
                case 4:
                {
                    dstrAngleP1 = 0.2;
                    dstrAngleP2 = 0.2;
                    break;
                }
                case 5:
                {
                    dstrAngleP1 = 10.0;
                    dstrAngleP2 = 0.0;
                    break;
                }
                case 6:
                {
                    dstrAngleP1 = 0.5;
                    dstrAngleP2 = 0.0;
                    break;
                }
                case 7:
                {
                    dstrAngleP1 = 1e+8;
                    dstrAngleP2 = 0.0;
                    break;
                }
                default:
                {
                    dstrAngleP1 = 0.0;
                    dstrAngleP2 = 0.0;
                    break;
                }
            }
            switch(FlagDistribRadius)
            {
                case 0:
                {
                    dstrRadiusP1 = 0.0;
                    dstrRadiusP2 = 0.0;
                }
                case 1:
                {
                    dstrRadiusP1 = 0.0;
                    dstrRadiusP2 = 0.05;
                    break;
                }
                case 2:
                {
                    dstrRadiusP1 = -0.02;
                    dstrRadiusP2 = 0.02;
                    break;
                }
                case 3:
                {
                    dstrRadiusP1 = 0.0;
                    dstrRadiusP2 = 0.002;
                    break;
                }
                case 4:
                {
                    dstrRadiusP1 = 0.2;
                    dstrRadiusP2 = 0.2;
                    break;
                }
                case 5:
                {
                    dstrRadiusP1 = 10.0;
                    dstrRadiusP2 = 0.0;
                    break;
                }
                case 6:
                {
                    dstrRadiusP1 = 0.5;
                    dstrRadiusP2 = 0.0;
                    break;
                }
                case 7:
                {
                    dstrRadiusP1 = 1e+8;
                    dstrRadiusP2 = 0.0;
                    break;
                }
                default:
                {
                    dstrRadiusP1 = 0.0;
                    dstrRadiusP2 = 0.0;
                    break;
                }
            }

            EllipseParams ellP = {Xc, Yc, A, B, Angle, beginAngle, endAngle, DistribCase, FlagDistribAngle, FlagDistribRadius, dstrAngleP1, dstrAngleP2, dstrRadiusP1, dstrRadiusP2, GNUPLOTPATH};

            EllipseGenerator (POINTS, PointsCount, &ellP);

            switch(FlagPlot)
            {
                case 0:
                {
                    PlotOriginEllipseData(Xc, Yc, A, B, GNUPLOTPATH);
                    break;
                }
                case 1:
                {
                    PlotAllPossibleGeneratedData(PointsCount, &ellP);
                    break;
                }
                case 2:
                {
                    PlotOriginEllipseDatawDistrib(&ellP);
                    break;
                }
                default:
                    break;
            }

            POINTS.clear();

            break;
        }
        default:
            break;
    }    

    return 0;
}
