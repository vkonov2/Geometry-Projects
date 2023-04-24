
#include "DrawCurve.h"

#include "Gnuplot.h"

void PlotOriginEllipseData(double Xc, double Yc, double A, double B, string& GNUPLOTPATH)
{
	Gnuplot plot;

    string command, min, max;
    double MinRange, MaxRange;

    MaxRange = (A > B) ? (Xc + A + 1.0) : (Yc + B + 1.0);
    MinRange = -MaxRange;

    min = to_string(MinRange);
    max = to_string(MaxRange);

    string originalFILENAME, movedFILENAME;

    originalFILENAME = "EllipseOriginal.txt";
    movedFILENAME = "EllipseMoved.txt";

    string line;
    // line = "' w l lw 3 ";
    line = "' ";

    command = "reset\nset terminal qt 1 size 700,700 font 'Geneva,12'\nset title 'Ellipse Generator'\nset xrange [" + min + ":" + max + "]\nset yrange [" + min + ":" + max + "]\nset xlabel 'X-axis'\nset ylabel 'Y-axis'\nset key spacing 1.3\nplot '" + GNUPLOTPATH + movedFILENAME + "' lt rgb 'blue' ti 'EllipseMoved', '"+ GNUPLOTPATH + originalFILENAME + line + "lt rgb 'red' ti 'EllipseOriginal'";

    plot (command.c_str());

	return;
}

void PlotAllPossibleGeneratedData(const int PointsCount, void* P)
{
    struct EllipseParams* ellP = (struct EllipseParams*)P;

    double Xc = (ellP->Xc);
    double Yc = (ellP->Yc);
    double A = (ellP->A);
    double B = (ellP->B);
    double Angle = (ellP->Angle);
    double beginAngle = (ellP->beginAngle);
    double endAngle = (ellP->endAngle);
    int DistribCase = (ellP->DistribCase);
    int FlagDistribAngle = (ellP->FlagDistribAngle);
    int FlagDistribRadius = (ellP->FlagDistribRadius);
    double dstrAngleP1 = (ellP->dstrAngleP1);
    double dstrAngleP2 = (ellP->dstrAngleP2);
    double dstrRadiusP1 = (ellP->dstrRadiusP1);
    double dstrRadiusP2 = (ellP->dstrRadiusP2);
    string& PATH = (ellP->PATH);
    
    int i;

    vector<string> originalFILENAME(6);
    vector<string> movedFILENAME(6);

    for (i = 0; i < 6; i++)
    {
        originalFILENAME[i] = "EllipseOriginal" + to_string(i) + ".txt";
        movedFILENAME[i] = "EllipseMoved" + to_string(i) + ".txt";
    } 

    double eps = 1.0;
    while (eps + 1.0 > 1.0)
        eps /= 2.0; 

    double t, cosT, sinT, tmp, tmpX, tmpY, tmpL;
    double delta, deltaDraw, err, x, y, tg;
    double minAxis, maxAxis, e, phi, r, dr, rnorm, ptmpX, ptmpY;

    if (fabs(beginAngle - endAngle) < eps || PointsCount <= 0)
    {
        cout << "\n>>> error in EllipseGenerator --> wrong input data\n\n";
        return ; 
    }

    double AngleRadian = Angle * M_PI / 180.0;
    double beginAngleRadian = beginAngle * M_PI / 180.0;
    double endAngleRadian = endAngle * M_PI / 180.0;

    double cosPHI, sinPHI; // basic defines

    int DrawCount = (int)(PointsCount);

    double cosA = cos(AngleRadian);
    double sinA = sin(AngleRadian);

    double cosAmod, sinAmod;

    if (A < B)
    {
        cosAmod = cos(M_PI/2.0-AngleRadian);
        sinAmod = sin(M_PI/2.0-AngleRadian);
    }
    else
    {
        cosAmod = cos(M_PI-AngleRadian);
        sinAmod = sin(M_PI-AngleRadian);
    }

    minAxis = (A > B) ? B : A;
    maxAxis = (A > B) ? A : B;

    e = sqrt(1 - minAxis*minAxis/(maxAxis*maxAxis));

    deltaDraw = fabs(endAngleRadian - beginAngleRadian) / ((double)(DrawCount));

    delta = fabs(endAngleRadian - beginAngleRadian) / ((double)(PointsCount));

    random_device rand_dev;
    mt19937 generator(rand_dev());

    normal_distribution<double> distributionNormAngle0(dstrAngleP1, dstrAngleP2);
    normal_distribution<double> distributionNormRadius0(dstrRadiusP1, dstrRadiusP2);

    normal_distribution<double> distributionNormAngle1(dstrAngleP1, dstrAngleP2);
    normal_distribution<double> distributionNormRadius1(dstrRadiusP1, dstrRadiusP2);

    normal_distribution<double> distributionNormAngle2(dstrAngleP1, dstrAngleP2);
    normal_distribution<double> distributionNormRadius2(dstrRadiusP1, dstrRadiusP2);

    normal_distribution<double> distributionNormAngle3(dstrAngleP1, dstrAngleP2);
    normal_distribution<double> distributionNormRadius3(dstrRadiusP1, dstrRadiusP2);

    normal_distribution<double> distributionNormAngle4(dstrAngleP1, dstrAngleP2);
    normal_distribution<double> distributionNormRadius4(dstrRadiusP1, dstrRadiusP2);

    normal_distribution<double> distributionNormAngle5(dstrAngleP1, dstrAngleP2);
    normal_distribution<double> distributionNormRadius5(dstrRadiusP1, dstrRadiusP2);

    uniform_real_distribution<double> distributionUniformAngle0(dstrAngleP1, dstrAngleP2);
    uniform_real_distribution<double> distributionUniformRadius0(dstrRadiusP1, dstrRadiusP2);

    uniform_real_distribution<double> distributionUniformAngle1(dstrAngleP1, dstrAngleP2);
    uniform_real_distribution<double> distributionUniformRadius1(dstrRadiusP1, dstrRadiusP2);

    uniform_real_distribution<double> distributionUniformAngle2(dstrAngleP1, dstrAngleP2);
    uniform_real_distribution<double> distributionUniformRadius2(dstrRadiusP1, dstrRadiusP2);

    uniform_real_distribution<double> distributionUniformAngle3(dstrAngleP1, dstrAngleP2);
    uniform_real_distribution<double> distributionUniformRadius3(dstrRadiusP1, dstrRadiusP2);

    uniform_real_distribution<double> distributionUniformAngle4(dstrAngleP1, dstrAngleP2);
    uniform_real_distribution<double> distributionUniformRadius4(dstrRadiusP1, dstrRadiusP2);

    uniform_real_distribution<double> distributionUniformAngle5(dstrAngleP1, dstrAngleP2);
    uniform_real_distribution<double> distributionUniformRadius5(dstrRadiusP1, dstrRadiusP2);

    cauchy_distribution<double> distributionCauchyAngle0(dstrAngleP1, dstrAngleP2);
    cauchy_distribution<double> distributionCauchyRadius0(dstrRadiusP1, dstrRadiusP2);

    cauchy_distribution<double> distributionCauchyAngle1(dstrAngleP1, dstrAngleP2);
    cauchy_distribution<double> distributionCauchyRadius1(dstrRadiusP1, dstrRadiusP2);

    cauchy_distribution<double> distributionCauchyAngle2(dstrAngleP1, dstrAngleP2);
    cauchy_distribution<double> distributionCauchyRadius2(dstrRadiusP1, dstrRadiusP2);

    cauchy_distribution<double> distributionCauchyAngle3(dstrAngleP1, dstrAngleP2);
    cauchy_distribution<double> distributionCauchyRadius3(dstrRadiusP1, dstrRadiusP2);

    cauchy_distribution<double> distributionCauchyAngle4(dstrAngleP1, dstrAngleP2);
    cauchy_distribution<double> distributionCauchyRadius4(dstrRadiusP1, dstrRadiusP2);

    cauchy_distribution<double> distributionCauchyAngle5(dstrAngleP1, dstrAngleP2);
    cauchy_distribution<double> distributionCauchyRadius5(dstrRadiusP1, dstrRadiusP2);

    gamma_distribution<double> distributionGammaAngle0(dstrAngleP1, dstrAngleP2);
    gamma_distribution<double> distributionGammaRadius0(dstrRadiusP1, dstrRadiusP2);

    gamma_distribution<double> distributionGammaAngle1(dstrAngleP1, dstrAngleP2);
    gamma_distribution<double> distributionGammaRadius1(dstrRadiusP1, dstrRadiusP2);

    gamma_distribution<double> distributionGammaAngle2(dstrAngleP1, dstrAngleP2);
    gamma_distribution<double> distributionGammaRadius2(dstrRadiusP1, dstrRadiusP2);

    gamma_distribution<double> distributionGammaAngle3(dstrAngleP1, dstrAngleP2);
    gamma_distribution<double> distributionGammaRadius3(dstrRadiusP1, dstrRadiusP2);

    gamma_distribution<double> distributionGammaAngle4(dstrAngleP1, dstrAngleP2);
    gamma_distribution<double> distributionGammaRadius4(dstrRadiusP1, dstrRadiusP2);

    gamma_distribution<double> distributionGammaAngle5(dstrAngleP1, dstrAngleP2);
    gamma_distribution<double> distributionGammaRadius5(dstrRadiusP1, dstrRadiusP2);

    exponential_distribution<double> distributionExponAngle0(dstrAngleP1);
    exponential_distribution<double> distributionExponRadius0(dstrRadiusP1);

    exponential_distribution<double> distributionExponAngle1(dstrAngleP1);
    exponential_distribution<double> distributionExponRadius1(dstrRadiusP1);

    exponential_distribution<double> distributionExponAngle2(dstrAngleP1);
    exponential_distribution<double> distributionExponRadius2(dstrRadiusP1);

    exponential_distribution<double> distributionExponAngle3(dstrAngleP1);
    exponential_distribution<double> distributionExponRadius3(dstrRadiusP1);

    exponential_distribution<double> distributionExponAngle4(dstrAngleP1);
    exponential_distribution<double> distributionExponRadius4(dstrRadiusP1);

    exponential_distribution<double> distributionExponAngle5(dstrAngleP1);
    exponential_distribution<double> distributionExponRadius5(dstrRadiusP1);

    chi_squared_distribution<double> distributionChiAngle0(dstrAngleP1);
    chi_squared_distribution<double> distributionChiRadius0(dstrRadiusP1);

    chi_squared_distribution<double> distributionChiAngle1(dstrAngleP1);
    chi_squared_distribution<double> distributionChiRadius1(dstrRadiusP1);

    chi_squared_distribution<double> distributionChiAngle2(dstrAngleP1);
    chi_squared_distribution<double> distributionChiRadius2(dstrRadiusP1);

    chi_squared_distribution<double> distributionChiAngle3(dstrAngleP1);
    chi_squared_distribution<double> distributionChiRadius3(dstrRadiusP1);

    chi_squared_distribution<double> distributionChiAngle4(dstrAngleP1);
    chi_squared_distribution<double> distributionChiRadius4(dstrRadiusP1);

    chi_squared_distribution<double> distributionChiAngle5(dstrAngleP1);
    chi_squared_distribution<double> distributionChiRadius5(dstrRadiusP1);

    student_t_distribution<double> distributionStudentAngle0(dstrAngleP1);
    student_t_distribution<double> distributionStudentRadius0(dstrRadiusP1);

    student_t_distribution<double> distributionStudentAngle1(dstrAngleP1);
    student_t_distribution<double> distributionStudentRadius1(dstrRadiusP1);

    student_t_distribution<double> distributionStudentAngle2(dstrAngleP1);
    student_t_distribution<double> distributionStudentRadius2(dstrRadiusP1);

    student_t_distribution<double> distributionStudentAngle3(dstrAngleP1);
    student_t_distribution<double> distributionStudentRadius3(dstrRadiusP1);

    student_t_distribution<double> distributionStudentAngle4(dstrAngleP1);
    student_t_distribution<double> distributionStudentRadius4(dstrRadiusP1);

    student_t_distribution<double> distributionStudentAngle5(dstrAngleP1);
    student_t_distribution<double> distributionStudentRadius5(dstrRadiusP1); // define distributions 

    // ------------------------ (0) --- angle
    // ------------------------ (0) --- angle
    // ------------------------ (0) --- angle
    // ------------------------ (0) --- angle
    // ------------------------ (0) --- angle
    // ------------------------ (0) --- angle

    vector<VECTOR2> POINTS((int)(1e+6));

    for (i = 0; i < DrawCount; i++)
    {
        phi = beginAngleRadian + i*deltaDraw;
        phi = fmod(phi, 2*M_PI);
        cosPHI = cos(phi);
        sinPHI = sin(phi);

        r = minAxis / sqrt(1 - e*e*cosPHI*cosPHI);

        POINTS[i].x = Xc + cosAmod * r * cosPHI + sinAmod * r * sinPHI;;
        POINTS[i].y = Yc - sinAmod * r * cosPHI + cosAmod * r * sinPHI;
    }

    PrintToFile (POINTS, DrawCount, originalFILENAME[0], PATH);

    for (i = 0; i < PointsCount; i++)
    {
        if (FlagDistribAngle == 0)
            err = 0.0;
        else if (FlagDistribAngle == 1)
            err = distributionNormAngle0(generator);
        else if (FlagDistribAngle == 2)
            err = distributionUniformAngle0(generator);
        else if (FlagDistribAngle == 3)
            err = distributionCauchyAngle0(generator);
        else if (FlagDistribAngle == 4)
            err = distributionGammaAngle0(generator);
        else if (FlagDistribAngle == 5)
            err = distributionExponAngle0(generator);
        else if (FlagDistribAngle == 6)
            err = distributionChiAngle0(generator);
        else if (FlagDistribAngle == 7)
            err = distributionStudentAngle0(generator);
        else
        {
            cout << "\n>>> error in EllipseGenerator --> wrong input\n\n";
            return;
        } // defines generator

        phi = beginAngleRadian + i*delta;
        phi = fmod(phi, 2*M_PI);

        phi += err;
        cosPHI = cos(phi);
        sinPHI = sin(phi);

        tmp = sqrt(1 - e*e*cosPHI*cosPHI);
        r = minAxis / tmp;

        ptmpX = cosAmod * r * cosPHI + sinAmod * r * sinPHI;
        ptmpY = -sinAmod * r * cosPHI + cosAmod * r * sinPHI;

        POINTS[i].x = Xc + ptmpX;
        POINTS[i].y = Yc + ptmpY;
    }

    PrintToFile (POINTS, PointsCount, movedFILENAME[0], PATH);

    POINTS.clear();
    POINTS.resize((int)(1e+6));

    // ------------------------ (1) --- gradient
    // ------------------------ (1) --- gradient
    // ------------------------ (1) --- gradient
    // ------------------------ (1) --- gradient
    // ------------------------ (1) --- gradient
    // ------------------------ (1) --- gradient

    for (i = 0; i < DrawCount; i++)
    {
        phi = beginAngleRadian + i*deltaDraw;
        phi = fmod(phi, 2*M_PI);
        cosPHI = cos(phi);
        sinPHI = sin(phi);

        r = minAxis / sqrt(1 - e*e*cosPHI*cosPHI);

        POINTS[i].x = Xc + cosAmod * r * cosPHI + sinAmod * r * sinPHI;;
        POINTS[i].y = Yc - sinAmod * r * cosPHI + cosAmod * r * sinPHI;
    }

    PrintToFile (POINTS, DrawCount, originalFILENAME[1], PATH);

    POINTS.clear();
    POINTS.resize((int)(1e+6));

    for (i = 0; i < PointsCount; i++)
    {
        phi = beginAngleRadian + i*delta;
        phi = fmod(phi, 2*M_PI);
        cosPHI = cos(phi);
        sinPHI = sin(phi);

        tmp = sqrt(1 - e*e*cosPHI*cosPHI);
        r = minAxis / tmp;

        ptmpX = cosAmod * r * cosPHI + sinAmod * r * sinPHI;
        ptmpY = -sinAmod * r * cosPHI + cosAmod * r * sinPHI;

        x = Xc + ptmpX;
        y = Yc + ptmpY;

        dr = -e*e*minAxis*sinPHI*cosPHI/(tmp*tmp*tmp);

        tg = (dr*sinPHI+r*cosPHI)/(dr*cosPHI-r*sinPHI);
        
        tmpX = -tg/sqrt(1+tg*tg);
        tmpY = 1/sqrt(1+tg*tg);

        //(-tg, 1)

        if (FlagDistribRadius == 0)
            err = 0.0;
        else if (FlagDistribRadius == 1)
            err = distributionNormRadius1(generator);
        else if (FlagDistribRadius == 2)
            err = distributionUniformRadius1(generator);
        else if (FlagDistribRadius == 3)
            err = distributionCauchyRadius1(generator);
        else if (FlagDistribRadius == 4)
            err = distributionGammaRadius1(generator);
        else if (FlagDistribRadius == 5)
            err = distributionExponRadius1(generator);
        else if (FlagDistribRadius == 6)
            err = distributionChiRadius1(generator);
        else if (FlagDistribRadius == 7)
            err = distributionStudentRadius1(generator);
        else
        {
            cout << "\n>>> error in EllipseGenerator --> wrong input\n\n";
            return;
        } // defines generator

        POINTS[i].x = x + err * tmpX;
        POINTS[i].y = y + err * tmpY;
    }

    PrintToFile (POINTS, PointsCount, movedFILENAME[1], PATH);

    POINTS.clear();
    POINTS.resize((int)(1e+6));

    // ------------------------ (2) --- angle + gradient
    // ------------------------ (2) --- angle + gradient
    // ------------------------ (2) --- angle + gradient
    // ------------------------ (2) --- angle + gradient
    // ------------------------ (2) --- angle + gradient
    // ------------------------ (2) --- angle + gradient

    for (i = 0; i < DrawCount; i++)
    {
        phi = beginAngleRadian + i*deltaDraw;
        phi = fmod(phi, 2*M_PI);
        cosPHI = cos(phi);
        sinPHI = sin(phi);

        r = minAxis / sqrt(1 - e*e*cosPHI*cosPHI);

        POINTS[i].x = Xc + cosAmod * r * cosPHI + sinAmod * r * sinPHI;;
        POINTS[i].y = Yc - sinAmod * r * cosPHI + cosAmod * r * sinPHI;
    }

    PrintToFile (POINTS, DrawCount, originalFILENAME[2], PATH);

    POINTS.clear();
    POINTS.resize((int)(1e+6));

    for (i = 0; i < PointsCount; i++)
    {
        if (FlagDistribAngle == 0)
            err = 0.0;
        else if (FlagDistribAngle == 1)
            err = distributionNormAngle2(generator);
        else if (FlagDistribAngle == 2)
            err = distributionUniformAngle2(generator);
        else if (FlagDistribAngle == 3)
            err = distributionCauchyAngle2(generator);
        else if (FlagDistribAngle == 4)
            err = distributionGammaAngle2(generator);
        else if (FlagDistribAngle == 5)
            err = distributionExponAngle2(generator);
        else if (FlagDistribAngle == 6)
            err = distributionChiAngle2(generator);
        else if (FlagDistribAngle == 7)
            err = distributionStudentAngle2(generator);
        else
        {
            cout << "\n>>> error in EllipseGenerator --> wrong input\n\n";
            return;
        } // defines generator

        phi = beginAngleRadian + i*delta;
        phi = fmod(phi, 2*M_PI);

        phi += err;
        cosPHI = cos(phi);
        sinPHI = sin(phi);

        tmp = sqrt(1 - e*e*cosPHI*cosPHI);
        r = minAxis / tmp;

        ptmpX = cosAmod * r * cosPHI + sinAmod * r * sinPHI;
        ptmpY = -sinAmod * r * cosPHI + cosAmod * r * sinPHI;

        x = Xc + ptmpX;
        y = Yc + ptmpY;

        dr = e*e*minAxis*sinPHI*cosPHI/(tmp*tmp*tmp);
        // for real derivative r|phi you need to add minus (-) before all statement 

        rnorm = sqrt(r*r+dr*dr);
        r /= rnorm;
        dr /= rnorm;

        if (FlagDistribRadius == 0)
            err = 0.0;
        else if (FlagDistribRadius == 1)
            err = distributionNormRadius2(generator);
        else if (FlagDistribRadius == 2)
            err = distributionUniformRadius2(generator);
        else if (FlagDistribRadius == 3)
            err = distributionCauchyRadius2(generator);
        else if (FlagDistribRadius == 4)
            err = distributionGammaRadius2(generator);
        else if (FlagDistribRadius == 5)
            err = distributionExponRadius2(generator);
        else if (FlagDistribRadius == 6)
            err = distributionChiRadius2(generator);
        else if (FlagDistribRadius == 7)
            err = distributionStudentRadius2(generator);
        else
        {
            cout << "\n>>> error in EllipseGenerator --> wrong input\n\n";
            return;
        } // defines generator

        POINTS[i].x = x + err * r;
        POINTS[i].y = y + err * dr;
    }

    PrintToFile (POINTS, PointsCount, movedFILENAME[2], PATH);

    POINTS.clear();
    POINTS.resize((int)(1e+6));

    // ------------------------ (3) --- parameter
    // ------------------------ (3) --- parameter
    // ------------------------ (3) --- parameter
    // ------------------------ (3) --- parameter
    // ------------------------ (3) --- parameter
    // ------------------------ (3) --- parameter

    for (i = 0; i < DrawCount; i++)
    {
        t = beginAngleRadian + i*deltaDraw;

        t = fmod(t, 2*M_PI);

        cosT = cos(t);
        sinT = sin(t);

        POINTS[i].x = Xc + A * cosT * cosA - B * sinT * sinA;
        POINTS[i].y = Yc + A * cosT * sinA + B * sinT * cosA;
    }

    PrintToFile (POINTS, DrawCount, originalFILENAME[3], PATH);

    POINTS.clear();
    POINTS.resize((int)(1e+6));

    for (i = 0; i < PointsCount; i++)
    {
        if (FlagDistribAngle == 0)
            err = 0.0;
        else if (FlagDistribAngle == 1)
            err = distributionNormAngle3(generator);
        else if (FlagDistribAngle == 2)
            err = distributionUniformAngle3(generator);
        else if (FlagDistribAngle == 3)
            err = distributionCauchyAngle3(generator);
        else if (FlagDistribAngle == 4)
            err = distributionGammaAngle3(generator);
        else if (FlagDistribAngle == 5)
            err = distributionExponAngle3(generator);
        else if (FlagDistribAngle == 6)
            err = distributionChiAngle3(generator);
        else if (FlagDistribAngle == 7)
            err = distributionStudentAngle3(generator);
        else
        {
            cout << "\n>>> error in EllipseGenerator --> wrong input\n\n";
            return;
        } // defines generator

        t = beginAngleRadian + i*delta;
        t += err;
        t = fmod(t, 2*M_PI);

        cosT = cos(t);
        sinT = sin(t);

        tmpX = A * cosT * cosA - B * sinT * sinA;
        tmpY = A * cosT * sinA + B * sinT * cosA;

        POINTS[i].x = Xc + tmpX;
        POINTS[i].y = Yc + tmpY;
    }

    PrintToFile (POINTS, PointsCount, movedFILENAME[3], PATH);

    POINTS.clear();
    POINTS.resize((int)(1e+6));

    // ------------------------ (4) --- radius
    // ------------------------ (4) --- radius
    // ------------------------ (4) --- radius
    // ------------------------ (4) --- radius
    // ------------------------ (4) --- radius
    // ------------------------ (4) --- radius

    for (i = 0; i < DrawCount; i++)
    {
        t = beginAngleRadian + i*deltaDraw;

        t = fmod(t, 2*M_PI);

        cosT = cos(t);
        sinT = sin(t);

        POINTS[i].x = Xc + A * cosT * cosA - B * sinT * sinA;
        POINTS[i].y = Yc + A * cosT * sinA + B * sinT * cosA;
    }

    PrintToFile (POINTS, DrawCount, originalFILENAME[4], PATH);

    POINTS.clear();
    POINTS.resize((int)(1e+6));

    for (i = 0; i < PointsCount; i++)
    {
        t = beginAngleRadian + i*delta;
        t = fmod(t, 2*M_PI);

        cosT = cos(t);
        sinT = sin(t);

        tmpX = A * cosT * cosA - B * sinT * sinA;
        tmpY = A * cosT * sinA + B * sinT * cosA;

        tmpL = sqrt(tmpX*tmpX + tmpY*tmpY);
        tmp = tmpL;

        if (FlagDistribRadius == 0)
            err = 0.0;
        else if (FlagDistribRadius == 1)
            err = distributionNormRadius4(generator);
        else if (FlagDistribRadius == 2)
            err = distributionUniformRadius4(generator);
        else if (FlagDistribRadius == 3)
            err = distributionCauchyRadius4(generator);
        else if (FlagDistribRadius == 4)
            err = distributionGammaRadius4(generator);
        else if (FlagDistribRadius == 5)
            err = distributionExponRadius4(generator);
        else if (FlagDistribRadius == 6)
            err = distributionChiRadius4(generator);
        else if (FlagDistribRadius == 7)
            err = distributionStudentRadius4(generator);
        else
        {
            cout << "\n>>> error in EllipseGenerator --> wrong input\n\n";
            return;
        } // defines generator

        tmpX /= tmpL;
        tmpY /= tmpL;

        tmp += err;

        tmpX *= tmp;
        tmpY *= tmp;

        POINTS[i].x = Xc + tmpX;
        POINTS[i].y = Yc + tmpY;
    }

    PrintToFile (POINTS, PointsCount, movedFILENAME[4], PATH);

    POINTS.clear();
    POINTS.resize((int)(1e+6));

    // ------------------------ (5) --- parameter + radius
    // ------------------------ (5) --- parameter + radius
    // ------------------------ (5) --- parameter + radius
    // ------------------------ (5) --- parameter + radius
    // ------------------------ (5) --- parameter + radius
    // ------------------------ (5) --- parameter + radius

    for (i = 0; i < DrawCount; i++)
    {
        t = beginAngleRadian + i*deltaDraw;

        t = fmod(t, 2*M_PI);

        cosT = cos(t);
        sinT = sin(t);

        POINTS[i].x = Xc + A * cosT * cosA - B * sinT * sinA;
        POINTS[i].y = Yc + A * cosT * sinA + B * sinT * cosA;
    }

    PrintToFile (POINTS, DrawCount, originalFILENAME[5], PATH);

    POINTS.clear();
    POINTS.resize((int)(1e+6));

    for (i = 0; i < PointsCount; i++)
    {
        t = beginAngleRadian + i*delta;
        t = fmod(t, 2*M_PI);

        cosT = cos(t);
        sinT = sin(t);

        tmpX = A * cosT * cosA - B * sinT * sinA;
        tmpY = A * cosT * sinA + B * sinT * cosA;

        tmpL = sqrt(tmpX*tmpX + tmpY*tmpY);
        tmp = tmpL;

        if (FlagDistribAngle == 0)
            err = 0.0;
        else if (FlagDistribAngle == 1)
            err = distributionNormAngle5(generator);
        else if (FlagDistribAngle == 2)
            err = distributionUniformAngle5(generator);
        else if (FlagDistribAngle == 3)
            err = distributionCauchyAngle5(generator);
        else if (FlagDistribAngle == 4)
            err = distributionGammaAngle5(generator);
        else if (FlagDistribAngle == 5)
            err = distributionExponAngle5(generator);
        else if (FlagDistribAngle == 6)
            err = distributionChiAngle5(generator);
        else if (FlagDistribAngle == 7)
            err = distributionStudentAngle5(generator);
        else
        {
            cout << "\n>>> error in EllipseGenerator --> wrong input\n\n";
            return;
        } // defines generator

        t = beginAngleRadian + i*delta;
        t += err;
        t = fmod(t, 2*M_PI);

        cosT = cos(t);
        sinT = sin(t);

        tmpX = A * cosT * cosA - B * sinT * sinA;
        tmpY = A * cosT * sinA + B * sinT * cosA;

        tmpL = sqrt(tmpX*tmpX + tmpY*tmpY);

        if (FlagDistribRadius == 0)
            err = 0.0;
        else if (FlagDistribRadius == 1)
            err = distributionNormRadius5(generator);
        else if (FlagDistribRadius == 2)
            err = distributionUniformRadius5(generator);
        else if (FlagDistribRadius == 3)
            err = distributionCauchyRadius5(generator);
        else if (FlagDistribRadius == 4)
            err = distributionGammaRadius5(generator);
        else if (FlagDistribRadius == 5)
            err = distributionExponRadius5(generator);
        else if (FlagDistribRadius == 6)
            err = distributionChiRadius5(generator);
        else if (FlagDistribRadius == 7)
            err = distributionStudentRadius5(generator);
        else
        {
            cout << "\n>>> error in EllipseGenerator --> wrong input\n\n";
            return;
        } // defines generator

        tmpX /= tmpL;
        tmpY /= tmpL;

        tmp += err;

        tmpX *= tmp;
        tmpY *= tmp;

        POINTS[i].x = Xc + tmpX;
        POINTS[i].y = Yc + tmpY;
    }

    PrintToFile (POINTS, PointsCount, movedFILENAME[5], PATH);

    POINTS.clear();

    // ------------------------ (end)
    // ------------------------ (end)
    // ------------------------ (end)
    // ------------------------ (end)
    // ------------------------ (end)
    // ------------------------ (end)

    Gnuplot plot;

    string lines, terminal;
    // lines = "w l lw 3";
    lines = "";
    terminal = "\nset terminal qt size 1700,1000 font 'Geneva,12'";
    // terminal = "";

    // string refresh;
    // refresh = "\n\nwhile (1) {\nreplot\npause 1\n}";
    // refresh = "\n\npause 1\nreread";

    string commandMulti;
    vector<string> commandEllipse(6);

    string min, max, sizeX, sizeY;
    double MinRange, MaxRange, dsizeX, dsizeY;

    double dorigX1, dorigX2, dorigX3;
    double dorigY1, dorigY2;

    string origX1, origX2, origX3;
    string origY1, origY2;

    MaxRange = (A > B) ? (Xc + A + 1.0) : (Yc + B + 1.0);
    MinRange = -MaxRange;

    dsizeX = 0.32;
    dsizeY = 0.5;    

    dorigX1 = 0.0;
    dorigX2 = 0.35;
    dorigX3 = 0.7;

    dorigY1 = 0.0;
    dorigY2 = 0.49;

    min = to_string(MinRange);
    max = to_string(MaxRange);

    sizeX = to_string(dsizeX);
    sizeY = to_string(dsizeY);

    origX1 = to_string(dorigX1);
    origX2 = to_string(dorigX2);
    origX3 = to_string(dorigX3);
    origY1 = to_string(dorigY1);
    origY2 = to_string(dorigY2);

    for (i = 0; i < 6; i++)
        commandEllipse[i] = "plot '" + PATH + movedFILENAME[i] + "' lt rgb 'blue' ti 'EllipseMoved', '"+ PATH + originalFILENAME[i] + "' " + lines + "lt rgb 'red' ti 'EllipseOriginal'";

    commandMulti = "reset" + terminal + "\nset size 3,2\nset multiplot\nset size " + sizeX + ", " + sizeY + "\nset origin " + origX1 + ", " + origY2 + "\nset title 'ANGLE VARIATION' font 'Geneva,18' tc rgb 'black'\nset xrange [" + min + ":" + max + "]\nset yrange [" + min + ":" + max + "]\nset xlabel 'X-axis'\nset ylabel 'Y-axis'\nset key spacing 1.3\n" + commandEllipse[0] + "\nset size " + sizeX + ", " + sizeY + "\nset origin " + origX2 + ", " + origY2 + "\nset title 'GRADIENT VARIATION'\nset xrange [" + min + ":" + max + "]\nset yrange [" + min + ":" + max + "]\nset xlabel 'X-axis'\nset ylabel 'Y-axis'\nset key spacing 1.3\n" + commandEllipse[1] + "\nset size " + sizeX + ", " + sizeY + "\nset origin " + origX3 + ", " + origY2 + "\nset title 'ANGLE AND GRADIENT VARIATION'\nset xrange [" + min + ":" + max + "]\nset yrange [" + min + ":" + max + "]\nset xlabel 'X-axis'\nset ylabel 'Y-axis'\nset key spacing 1.3\n" + commandEllipse[2] + "\nset size " + sizeX + ", " + sizeY + "\nset origin " + origX1 + ", " + origY1 + "\nset title 'PARAMETER VARIATION'\nset xrange [" + min + ":" + max + "]\nset yrange [" + min + ":" + max + "]\nset xlabel 'X-axis'\nset ylabel 'Y-axis'\nset key spacing 1.3\n" + commandEllipse[3] + "\nset size " + sizeX + ", " + sizeY + "\nset origin " + origX2 + ", " + origY1 + "\nset title 'RADIUS VARIATION'\nset xrange [" + min + ":" + max + "]\nset yrange [" + min + ":" + max + "]\nset xlabel 'X-axis'\nset ylabel 'Y-axis'\nset key spacing 1.3\n" + commandEllipse[4] + "\nset size " + sizeX + ", " + sizeY + "\nset origin " + origX3 + ", " + origY1 + "\nset title 'PARAMETER AND RADIUS VARIATION'\nset xrange [" + min + ":" + max + "]\nset yrange [" + min + ":" + max + "]\nset xlabel 'X-axis'\nset ylabel 'Y-axis'\nset key spacing 1.3\n" + commandEllipse[5] + "\nunset multiplot";

    plot(commandMulti.c_str());

    originalFILENAME.clear();
    movedFILENAME.clear();
    commandEllipse.clear();

    return;
}

void PlotOriginEllipseDatawDistrib(void* P)
{
    struct EllipseParams* ellP = (struct EllipseParams*)P;

    double Xc = (ellP->Xc);
    double Yc = (ellP->Yc);
    double A = (ellP->A);
    double B = (ellP->B);
    double Angle = (ellP->Angle);
    double beginAngle = (ellP->beginAngle);
    double endAngle = (ellP->endAngle);
    int DistribCase = (ellP->DistribCase);
    int FlagDistribAngle = (ellP->FlagDistribAngle);
    int FlagDistribRadius = (ellP->FlagDistribRadius);
    double dstrAngleP1 = (ellP->dstrAngleP1);
    double dstrAngleP2 = (ellP->dstrAngleP2);
    double dstrRadiusP1 = (ellP->dstrRadiusP1);
    double dstrRadiusP2 = (ellP->dstrRadiusP2);
    string& GNUPLOTPATH = (ellP->PATH);

    double AngleRadian = Angle * M_PI / 180.0;

    double cosA = cos(AngleRadian);
    double sinA = sin(AngleRadian);

    string originalFILENAME, movedFILENAME;
    string FileDistribAngle, FileDistribRadius;

    originalFILENAME = "EllipseOriginal.txt";
    movedFILENAME = "EllipseMoved.txt";
    // FileDistribAngle = "";
    // FileDistribRadius = "";

    string line;
    // line = "' w l lw 3 ";
    line = "' ";

    switch(FlagDistribAngle)
    {
        case 0:
        {
            FileDistribAngle = "NoDistribAngle.txt";
            break;
        }
        case 1:
        {
            FileDistribAngle = "NormDistribAngle.txt";
            break;
        }
        case 2:
        {
            FileDistribAngle = "UniformDistribAngle.txt";
            break;
        }
        case 3:
        {
            FileDistribAngle = "CauchyDistribRadius.txt";
            break;
        }
        case 4:
        {
            FileDistribAngle = "GammaDistribRadius.txt";
            break;
        }
        case 5:
        {
            FileDistribAngle = "ExponDistribRadius.txt";
            break;
        }
        case 6:
        {
            FileDistribAngle = "ChiDistribAngle.txt";
            break;
        }
        case 7:
        {
            FileDistribAngle = "StudentDistribAngle.txt";
            break;
        }
        default:
        {
            FileDistribAngle = "";
            break;
        }
    }

    switch(FlagDistribRadius)
    {
        case 0:
        {
            FileDistribRadius = "NoDistribRadius.txt";
        }
        case 1:
        {
            FileDistribRadius = "NormDistribRadius.txt";
            break;
        }
        case 2:
        {
            FileDistribRadius = "UniformDistribRadius.txt";
            break;
        }
        case 3:
        {
            FileDistribRadius = "CauchyDistribRadius.txt";
            break;
        }
        case 4:
        {
            FileDistribRadius = "GammaDistribRadius.txt";
            break;
        }
        case 5:
        {
            FileDistribRadius = "ExponDistribRadius.txt";
            break;
        }
        case 6:
        {
            FileDistribRadius = "ChiDistribRadius.txt";
            break;
        }
        case 7:
        {
            FileDistribRadius = "StudentDistribRadius.txt";
            break;
        }
        default:
        {
            FileDistribRadius = "";
            break;
        }
    }

	Gnuplot plot;

	string commandEllipseOriginal, commandEllipseMoved, commandDistrib, commandDistribAngle, commandDistribRadius, commandBinsAngle, commandBinsRadius, Bins, widthAngle, widthRadius, distribAngleName, distribRadiusName, commandMulti;

    commandEllipseOriginal = "plot '" + GNUPLOTPATH + originalFILENAME + "' lt rgb 'red' ti ''";

    commandEllipseMoved = "plot '" + GNUPLOTPATH + movedFILENAME + "' lt rgb 'blue' ti ''";

    commandDistrib = "plot '" + GNUPLOTPATH + "Residual.txt" + "' w l lw 3 lt rgb 'red' ti ''";

    commandDistribAngle = "plot '" + GNUPLOTPATH + FileDistribAngle + "' ti ''";

    commandDistribRadius = "plot '"+ GNUPLOTPATH + FileDistribRadius + "' ti ''";

    Bins = "u (bin($2,width)):(1.0) s f w boxes fs solid 0.5";

    if (FlagDistribAngle == 3)
        widthAngle = "0.0002";
    else if (FlagDistribAngle == 6)
        widthAngle = "0.5";
    else
        widthAngle = "0.008";

    if (FlagDistribAngle == 0)
        distribAngleName = "NO";
    else if (FlagDistribAngle == 1)
        distribAngleName = "NORMAL";
    else if (FlagDistribAngle == 2)
        distribAngleName = "UNIFORM";
    else if (FlagDistribAngle == 3)
        distribAngleName = "CAUCHY";
    else if (FlagDistribAngle == 4)
        distribAngleName = "GAMMA";
    else if (FlagDistribAngle == 5)
        distribAngleName = "EXPON";
    else if (FlagDistribAngle == 6)
        distribAngleName = "CHI";
    else if (FlagDistribAngle == 7)
        distribAngleName = "STUDENT";
    else
    {
        distribAngleName = "";
        cout << "\n>>>error in PlotOriginEllipseDatawDistrib --> wrong input\n\n";
        return;
    }

    if (FlagDistribRadius == 0)
        distribRadiusName = "NO";
    else if (FlagDistribRadius == 1)
        distribRadiusName = "NORMAL";
    else if (FlagDistribRadius == 2)
        distribRadiusName = "UNIFORM";
    else if (FlagDistribRadius == 3)
        distribRadiusName = "CAUCHY";
    else if (FlagDistribRadius == 4)
        distribRadiusName = "GAMMA";
    else if (FlagDistribRadius == 5)
        distribRadiusName = "EXPON";
    else if (FlagDistribRadius == 6)
        distribRadiusName = "CHI";
    else if (FlagDistribRadius == 7)
        distribRadiusName = "STUDENT";
    else
    {
        distribRadiusName = "";
        cout << "\n>>>error in PlotOriginEllipseDatawDistrib --> wrong input\n\n";
        return;
    }

    if (FlagDistribRadius == 3)
        widthRadius = "0.0002";
    else if (FlagDistribRadius == 6)
        widthRadius = "0.5";
    else
        widthRadius = "0.008";

    commandBinsAngle = "width=" + widthAngle + "\nbin(x, s) = s*int(x/s) + width/2\nset boxwidth width\nplot '" + GNUPLOTPATH + FileDistribAngle + "' " + Bins + " ti 'Angle error'";

    commandBinsRadius = "width=" + widthRadius + "\nbin(x, s) = s*int(x/s) + width/2\nset boxwidth width\nplot '" + GNUPLOTPATH + FileDistribRadius + "' " + Bins + " ti 'Radius error'";

    double MinRange, MaxRange;

    double dorigX1, dorigX2, dorigX3, dorigX4;
    double dorigY1, dorigY2;
    double ratio = ((double)(A))/((double)(B));

    string sizeXell, sizeYell;
    string sizeXdistrib, sizeYdistrib;
    string sizeXbin, sizeYbin;

    string origX1, origX2, origX3, origX4;
    string origY1, origY2;

    string min, max, ssize, srange, equal;

    double step;
    double dxRangeMax, dyRangeMax, dxRangeMin, dyRangeMin;
    string minX, minY, maxX, maxY;

    // step = 0.0;
    step = 0.3;
    // step = 0.5;
    // step = 1.0;

    // MaxRange = (A > B) ? (Xc + A + step) : (Yc + B + step);
    // MinRange = -MaxRange; 

    dxRangeMax = Xc + (A + step)*cosA;
    dyRangeMax = Yc + (B + step)*sinA;
    dxRangeMin = Xc - (A + step)*cosA;
    dyRangeMin = Yc - (B + step)*sinA;

    minX = to_string(dxRangeMin);
    maxX = to_string(dxRangeMax);
    minY = to_string(dyRangeMin);
    maxY = to_string(dyRangeMax);

    sizeXell = "0.5";
    sizeYell = "0.5";
    if (fabs(ratio-4) < 1e-14)
    {
        if (fabs(Angle) < 1e-14)
            sizeYell = "0.32";
        else if (fabs(Angle-30)<1e-14)
            sizeYell = "0.5";
    }
    if (fabs(ratio-3) < 1e-14)
    {
        if (fabs(Angle) < 1e-14)
            sizeYell = "0.32";
        else if (fabs(Angle-30)<1e-14)
            sizeYell = "0.6";
    }
    if (fabs(ratio-2) < 1e-14)
    {
        if (fabs(Angle) < 1e-14)
            sizeYell = "0.4";
        else if (fabs(Angle-30)<1e-14)
            sizeYell = "0.65";
    }

    ssize = "set size " + sizeXell + ", " + sizeYell;
    // ssize = "";

    // srange = "set xrange [" + minX + ":" + maxX + "]\nset yrange [" + minY + ":" + maxY + "]\n";
    srange = "";

    // equal = "set view equal xy";
    // equal = "set size ratio " + to_string(ratio);
    equal = "";

    // if (A > B)
    //     equal = "set size ratio " + to_string(ratio) + "#" + to_string(ratio) + "r1#1=-1";
    // else
    //     equal = "set size ratio 1#1r" + to_string(1/ratio) + "#" + to_string(1/ratio) + "=-1";

    sizeXdistrib = "0.25";
    sizeYdistrib = "0.35";

    sizeXbin = "0.25";
    sizeYbin = "0.35";

    dorigX1 = 0.0;
    dorigX2 = 0.25;
    dorigX3 = 0.5;
    dorigX4 = 0.75;

    dorigY1 = 0.0;
    dorigY2 = 0.4;

    origX1 = to_string(dorigX1);
    origX2 = to_string(dorigX2);
    origX3 = to_string(dorigX3);
    origX4 = to_string(dorigX4);

    origY1 = to_string(dorigY1);
    origY2 = to_string(dorigY2);

    string name1, name2;

    if (DistribCase > -1 && DistribCase < 3)
    {
        name1 = "ANGLE";
        name2 = "GRADIENT";
    }
    else if (DistribCase > 2 && DistribCase < 6)
    {
        name1 = "PARAMETER";
        name2 = "RADIUS";
    }
    else if (DistribCase > 5 && DistribCase < 9)
    {
        name1 = "LENGTH";
        name2 = "GRADIENT";
    }
    else
    {
        name1 = "";
        name2 = "";
    }

    string c0, c00, c1, c2, c3, c4, c5, c6;

    c0 = "reset\nset terminal qt 1 size 1700,1000 font 'Geneva,12'\nset size 4,2\nset multiplot\n";

    c00 = "set size 0.5, 0.35\nset origin " + origX3 + ", " + origY1 + "\nset title '" + name1 + " RESIDUAL LENGTH DISTRIBUTION' font 'Geneva,20' tc rgb 'black'\nset xlabel 'Number of point'\nset ylabel 'Value of residual'\nset key spacing 1.3\n" + commandDistrib + "\n";

    c1 = "set size " + sizeXdistrib + ", " + sizeYdistrib + "\nset origin " + origX1 + ", " + origY1 + "\nset title '" + name1 + " ERROR " + distribAngleName + " DISTRIBUTION' font 'Geneva,20' tc rgb 'black'\nset xlabel 'Number of point'\nset ylabel 'Value of error'\nset key spacing 1.3\n" + commandDistribAngle + "\n";

    c2 = "set size " + sizeXdistrib + ", " + sizeYdistrib + "\nset origin " + origX3 + ", " + origY1 + "\nset title '" + name2 + " ERROR " + distribRadiusName + " DISTRIBUTION'\nset xlabel 'Number of point'\nset ylabel 'Value of error'\nset key spacing 1.3\n" + commandDistribRadius + "\n";

    c3 = "set size " + sizeXbin + ", " + sizeYbin + "\nset origin " + origX2 + ", " + origY1 + "\nset title '" + name1 + " ERROR " + distribAngleName + " BAR CHART'\nset xlabel 'Angle error'\nset ylabel 'Count of points'\nset key spacing 1.3\n" + commandBinsAngle + "\n";

    c4 = "set size " + sizeXbin + ", " + sizeYbin + "\nset origin " + origX4 + ", " + origY1 + "\nset title '" + name2 + " ERROR " + distribRadiusName + " BAR CHART'\nset xlabel 'Radius error'\nset ylabel 'Count of points'\nset key spacing 1.3\n" + commandBinsRadius + "\n";

    c5 = ssize + "\nset origin " + origX1 + ", " + origY2 + "\n" + srange + equal + "\nset title 'ELLIPSE ORIGINAL DATA'\n" + "set xlabel 'X-axis'\nset ylabel 'Y-axis'\nset key spacing 1.3\n" + commandEllipseOriginal + "\n";

    c6 = ssize + "\nset origin " + origX3 + ", " + origY2 + "\n" + srange + equal + "\nset title 'ELLIPSE MOVED DATA'\nset xlabel 'X-axis'\n" + "set ylabel 'Y-axis'\nset key spacing 1.3\n" + commandEllipseMoved + "\nunset multiplot";



    if (DistribCase%3==0)
        commandMulti = c0+c00+c1+c3+c5+c6;
    else if(DistribCase%3==1)
        commandMulti = c0+c2+c4+c5+c6;
    else
        commandMulti = c0+c1+c2+c3+c4+c5+c6;

    plot(commandMulti);

	return;
}

void DrawEllipseArc(double Xc, double Yc, double A, double B, double Angle, double beginAngle, double endAngle, string& FILENAME, string& PATH)
{
    double i, t, x, y;
    double cosT, sinT;
    double eps = 1.0, delta;
    
    while (eps + 1.0 > 1.0)
        eps /= 2.0;

    int DrawCount = (int)(1e+3);

    if (fabs(beginAngle - endAngle) < eps)
    {
        cout << "\n>>> error in PlotEllipseArc --> wrong input data\n\n";
        return ; 
    }

    double AngleRadian = Angle * M_PI / 180.0;
    double beginAngleRadian = beginAngle * M_PI / 180.0;
    double endAngleRadian = endAngle * M_PI / 180.0;

    double cosA = cos(AngleRadian);
    double sinA = sin(AngleRadian);

    ofstream strm;

    strm.open((PATH+FILENAME).c_str());
    if (!strm.is_open())
    {
        cout << "\n>>> error in PlotEllipseArc --> can't open file --> " << PATH+FILENAME << "\n\n";
        return;
    }

    delta = fabs(endAngleRadian - beginAngleRadian) / ((double)(DrawCount));

    for (i = 0; i < DrawCount; i++)
    {
        t = beginAngleRadian + i*delta;

        t = fmod(t, 2*M_PI);

        cosT = cos(t);
        sinT = sin(t);

        x = Xc + A * cosT * cosA - B * sinT * sinA;
        y = Yc + A * cosT * sinA + B * sinT * cosA;

        if (i == DrawCount-1)
            strm << x << " " << y;
        else
            strm << x << " " << y << "\n";
    }

    strm.close();
    if (strm.is_open())
    {
        cout << "\n>>> error in PlotEllipseArc --> can't close file --> " << PATH+FILENAME << "\n\n";
        return ;
    }

    return;
}







