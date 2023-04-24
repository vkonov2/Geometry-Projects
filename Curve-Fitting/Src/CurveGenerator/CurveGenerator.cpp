
#include "CurveGenerator.h"
#include "EllipseFunctions.h"

double RHO (double PHI, void* P)
{
	struct rhoParams* PARAMS = (struct rhoParams*)P;

    double A = (PARAMS->A);
    double B = (PARAMS->B);

	double phiRadian = PHI * M_PI / 180.0;

	double minAxis = (A > B) ? B : A;
	double maxAxis = (A > B) ? A : B;

	double e = sqrt(1 - minAxis*minAxis/(maxAxis*maxAxis));
	double cosPHI = cos(phiRadian);
	double tmp = sqrt(1 - e*e*cosPHI*cosPHI);
	double r = minAxis / tmp;

	return r;
}

void EllipseGenerator (vector<VECTOR2>& POINTS, const int PointsCount, void* P)
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

	string command;
	command = "rm -rf " + PATH + "*.txt";
	system(command.c_str());

	string originalFILENAME, movedFILENAME;
    string FileDistribAngle, FileDistribRadius;

    originalFILENAME = "EllipseOriginal.txt";
    movedFILENAME = "EllipseMoved.txt";

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

	double i, t;
	double cosT, sinT, tmp, tmpX, tmpY, tmpL;
	double eps = 1.0, delta, deltaDraw, err, x, y, tg;
	double minAxis, maxAxis, e, phi, r, dr, rnorm, ptmpX, ptmpY;
	double cosPHI, sinPHI;
	double cosAmod, sinAmod;
	double EllipseLength, deltaLen, deltaLenDraw;
	
	while (eps + 1.0 > 1.0)
		eps /= 2.0;

	if (fabs(beginAngle - endAngle) < eps || PointsCount <= 0)
	{
		cout << "\n>>> error in EllipseGenerator --> wrong input data\n\n";
		return ; 
	}

	double AngleRadian = Angle * M_PI / 180.0;
	double beginAngleRadian = beginAngle * M_PI / 180.0;
	double endAngleRadian = endAngle * M_PI / 180.0; // basic defines

	int DrawCount = (int)(PointsCount);

	double cosA = cos(AngleRadian);
	double sinA = sin(AngleRadian);

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

	delta = fabs(endAngleRadian - beginAngleRadian) / ((double)(PointsCount));
	deltaDraw = fabs(endAngleRadian - beginAngleRadian) / ((double)(DrawCount));

	EllipseLength = CalcEllipsePerimeter (A, B, beginAngle, endAngle, 0);

	deltaLenDraw = fabs(EllipseLength) / ((double)(DrawCount));
	deltaLen = fabs(EllipseLength) / ((double)(PointsCount));

	struct rhoParams PARAMS = {Xc, Yc, A, B, Angle};

	// printf("TEST = %lf\n\n", RHO(0.0, &PARAMS));

	random_device rand_dev;
	mt19937 generator(rand_dev());

	ofstream strm;
	ofstream strmAngle;
	ofstream strmRadius;

	normal_distribution<double> distributionNormAngle(dstrAngleP1, dstrAngleP2);
	normal_distribution<double> distributionNormRadius(dstrRadiusP1, dstrRadiusP2);

	uniform_real_distribution<double> distributionUniformAngle(dstrAngleP1, dstrAngleP2);
	uniform_real_distribution<double> distributionUniformRadius(dstrRadiusP1, dstrRadiusP2);

	cauchy_distribution<double> distributionCauchyAngle(dstrAngleP1, dstrAngleP2);
	cauchy_distribution<double> distributionCauchyRadius(dstrRadiusP1, dstrRadiusP2);

	gamma_distribution<double> distributionGammaAngle(dstrAngleP1, dstrAngleP2);
	gamma_distribution<double> distributionGammaRadius(dstrRadiusP1, dstrRadiusP2);

	exponential_distribution<double> distributionExponAngle(dstrAngleP1);
	exponential_distribution<double> distributionExponRadius(dstrRadiusP1);

	chi_squared_distribution<double> distributionChiAngle(dstrAngleP1);
	chi_squared_distribution<double> distributionChiRadius(dstrRadiusP1);

	student_t_distribution<double> distributionStudentAngle(dstrAngleP1);
	student_t_distribution<double> distributionStudentRadius(dstrRadiusP1); // define distributions

	switch(DistribCase)
	{
		case(0): // angle 
		{
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

			PrintToFile (POINTS, DrawCount, originalFILENAME, PATH);

			POINTS.clear();
			POINTS.resize(PointsCount);
			
			strmAngle.open((PATH+FileDistribAngle).c_str());
		    if (!strmAngle.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't open file --> " << PATH+FileDistribAngle << "\n\n";
		        return;
		    }

		    strmRadius.open((PATH+FileDistribRadius).c_str());
		    if (!strmRadius.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't open file --> " << PATH+FileDistribRadius << "\n\n";
		        return;
		    }

			for (i = 0; i < PointsCount; i++)
			{
				if (FlagDistribAngle == 0)
					err = 0.0;
				else if (FlagDistribAngle == 1)
					err = distributionNormAngle(generator);
				else if (FlagDistribAngle == 2)
					err = distributionUniformAngle(generator);
				else if (FlagDistribAngle == 3)
					err = distributionCauchyAngle(generator);
				else if (FlagDistribAngle == 4)
					err = distributionGammaAngle(generator);
				else if (FlagDistribAngle == 5)
					err = distributionExponAngle(generator);
				else if (FlagDistribAngle == 6)
					err = distributionChiAngle(generator);
				else if (FlagDistribAngle == 7)
					err = distributionStudentAngle(generator);
				else
				{
					cout << "\n>>> error in EllipseGenerator --> wrong input\n\n";
					return;
				} // defines distribution

				if (i == PointsCount-1)
					strmAngle << i << " " << err;
				else
					strmAngle << i << " " << err << "\n";

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

			strmAngle.close();
		    if (strmAngle.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't close file --> " << PATH+FileDistribAngle << "\n\n";
		        return ;
		    }

		    strmRadius.close();
		    if (strmRadius.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't close file --> " << PATH+FileDistribRadius << "\n\n";
		        return ;
		    }

			PrintToFile (POINTS, PointsCount, movedFILENAME, PATH);

			break;
		}
		case(1): // gradient
		{
			for (i = 0; i < DrawCount; i++)
			{
				phi = beginAngleRadian + i*deltaDraw;
				phi = fmod(phi, 2*M_PI);
				cosPHI = cos(phi);
				sinPHI = sin(phi);

				r = minAxis / sqrt(1 - e*e*cosPHI*cosPHI);

				POINTS[i].x = Xc + cosAmod * r * cosPHI + sinAmod * r * sinPHI;
				POINTS[i].y = Yc - sinAmod * r * cosPHI + cosAmod * r * sinPHI;
			}

			PrintToFile (POINTS, DrawCount, originalFILENAME, PATH);

			POINTS.clear();
			POINTS.resize(PointsCount);
			
			strmAngle.open((PATH+FileDistribAngle).c_str());
		    if (!strmAngle.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't open file --> " << PATH+FileDistribAngle << "\n\n";
		        return;
		    }

		    strmRadius.open((PATH+FileDistribRadius).c_str());
		    if (!strmRadius.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't open file --> " << PATH+FileDistribRadius << "\n\n";
		        return;
		    }

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
					err = distributionNormRadius(generator);
				else if (FlagDistribRadius == 2)
					err = distributionUniformRadius(generator);
				else if (FlagDistribRadius == 3)
					err = distributionCauchyRadius(generator);
				else if (FlagDistribRadius == 4)
					err = distributionGammaRadius(generator);
				else if (FlagDistribRadius == 5)
					err = distributionExponRadius(generator);
				else if (FlagDistribRadius == 6)
					err = distributionChiRadius(generator);
				else if (FlagDistribRadius == 7)
					err = distributionStudentRadius(generator);
				else
				{
					cout << "\n>>> error in EllipseGenerator --> wrong input\n\n";
					return;
				}
				if (i == PointsCount-1)
					strmRadius << i << " " << err;
				else
					strmRadius << i << " " << err << "\n";

				POINTS[i].x = x + err * tmpX;
				POINTS[i].y = y + err * tmpY;
			}

			strmAngle.close();
		    if (strmAngle.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't close file --> " << PATH+FileDistribAngle << "\n\n";
		        return ;
		    }

		    strmRadius.close();
		    if (strmRadius.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't close file --> " << PATH+FileDistribRadius << "\n\n";
		        return ;
		    }

			PrintToFile (POINTS, PointsCount, movedFILENAME, PATH);

			break;
		}
		case(2): // angle + gradient
		{
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

			PrintToFile (POINTS, DrawCount, originalFILENAME, PATH);

			POINTS.clear();
			POINTS.resize(PointsCount);
			
			strmAngle.open((PATH+FileDistribAngle).c_str());
		    if (!strmAngle.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't open file --> " << PATH+FileDistribAngle << "\n\n";
		        return;
		    }

		    strmRadius.open((PATH+FileDistribRadius).c_str());
		    if (!strmRadius.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't open file --> " << PATH+FileDistribRadius << "\n\n";
		        return;
		    }

			for (i = 0; i < PointsCount; i++)
			{
				if (FlagDistribAngle == 0)
					err = 0.0;
				else if (FlagDistribAngle == 1)
					err = distributionNormAngle(generator);
				else if (FlagDistribAngle == 2)
					err = distributionUniformAngle(generator);
				else if (FlagDistribAngle == 3)
					err = distributionCauchyAngle(generator);
				else if (FlagDistribAngle == 4)
					err = distributionGammaAngle(generator);
				else if (FlagDistribAngle == 5)
					err = distributionExponAngle(generator);
				else if (FlagDistribAngle == 6)
					err = distributionChiAngle(generator);
				else if (FlagDistribAngle == 7)
					err = distributionStudentAngle(generator);
				else
				{
					cout << "\n>>> error in EllipseGenerator --> wrong input\n\n";
					return;
				}

				if (i == PointsCount-1)
					strmAngle << i << " " << err;
				else
					strmAngle << i << " " << err << "\n";

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

				dr = -e*e*minAxis*sinPHI*cosPHI/(tmp*tmp*tmp);

		        tg = (dr*sinPHI+r*cosPHI)/(dr*cosPHI-r*sinPHI);
		        
		        tmpX = -tg/sqrt(1+tg*tg);
		        tmpY = 1/sqrt(1+tg*tg);

		        //(-tg, 1)

				if (FlagDistribRadius == 0)
					err = 0.0;
				else if (FlagDistribRadius == 1)
					err = distributionNormRadius(generator);
				else if (FlagDistribRadius == 2)
					err = distributionUniformRadius(generator);
				else if (FlagDistribRadius == 3)
					err = distributionCauchyRadius(generator);
				else if (FlagDistribRadius == 4)
					err = distributionGammaRadius(generator);
				else if (FlagDistribRadius == 5)
					err = distributionExponRadius(generator);
				else if (FlagDistribRadius == 6)
					err = distributionChiRadius(generator);
				else if (FlagDistribRadius == 7)
					err = distributionStudentRadius(generator);
				else
				{
					cout << "\n>>> error in EllipseGenerator --> wrong input\n\n";
					return;
				}
				if (i == PointsCount-1)
					strmRadius << i << " " << err;
				else
					strmRadius << i << " " << err << "\n";

				POINTS[i].x = x + err * tmpX;
				POINTS[i].y = y + err * tmpY;
			}

			strmAngle.close();
		    if (strmAngle.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't close file --> " << PATH+FileDistribAngle << "\n\n";
		        return ;
		    }

		    strmRadius.close();
		    if (strmRadius.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't close file --> " << PATH+FileDistribRadius << "\n\n";
		        return ;
		    }

			PrintToFile (POINTS, PointsCount, movedFILENAME, PATH);

			break;
		}
		case(3): // parameter
		{
			for (i = 0; i < DrawCount; i++)
			{
				t = beginAngleRadian + i*deltaDraw;

				t = fmod(t, 2*M_PI);

				cosT = cos(t);
				sinT = sin(t);

				POINTS[i].x = Xc + A * cosT * cosA - B * sinT * sinA;
				POINTS[i].y = Yc + A * cosT * sinA + B * sinT * cosA;
			}

			PrintToFile (POINTS, DrawCount, originalFILENAME, PATH);

			POINTS.clear();
			POINTS.resize(PointsCount);

			strmAngle.open((PATH+FileDistribAngle).c_str());
		    if (!strmAngle.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't open file --> " << PATH+FileDistribAngle << "\n\n";
		        return;
		    }

		    strmRadius.open((PATH+FileDistribRadius).c_str());
		    if (!strmRadius.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't open file --> " << PATH+FileDistribRadius << "\n\n";
		        return;
		    }

			for (i = 0; i < PointsCount; i++)
			{
				if (FlagDistribAngle == 0)
					err = 0.0;
				else if (FlagDistribAngle == 1)
					err = distributionNormAngle(generator);
				else if (FlagDistribAngle == 2)
					err = distributionUniformAngle(generator);
				else if (FlagDistribAngle == 3)
					err = distributionCauchyAngle(generator);
				else if (FlagDistribAngle == 4)
					err = distributionGammaAngle(generator);
				else if (FlagDistribAngle == 5)
					err = distributionExponAngle(generator);
				else if (FlagDistribAngle == 6)
					err = distributionChiAngle(generator);
				else if (FlagDistribAngle == 7)
					err = distributionStudentAngle(generator);
				else
				{
					cout << "\n>>> error in EllipseGenerator --> wrong input\n\n";
					return;
				}

				if (i == PointsCount-1)
					strmAngle << i << " " << err;
				else
					strmAngle << i << " " << err << "\n";

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

			strmAngle.close();
		    if (strmAngle.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't close file --> " << PATH+FileDistribAngle << "\n\n";
		        return ;
		    }

		    strmRadius.close();
		    if (strmRadius.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't close file --> " << PATH+FileDistribRadius << "\n\n";
		        return ;
		    }

			PrintToFile (POINTS, PointsCount, movedFILENAME, PATH);

			break;
		}
		case(4): // vector
		{
			for (i = 0; i < DrawCount; i++)
			{
				t = beginAngleRadian + i*deltaDraw;

				t = fmod(t, 2*M_PI);

				cosT = cos(t);
				sinT = sin(t);

				POINTS[i].x = Xc + A * cosT * cosA - B * sinT * sinA;
				POINTS[i].y = Yc + A * cosT * sinA + B * sinT * cosA;
			}

			PrintToFile (POINTS, DrawCount, originalFILENAME, PATH);

			POINTS.clear();
			POINTS.resize(PointsCount);

			strmAngle.open((PATH+FileDistribAngle).c_str());
		    if (!strmAngle.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't open file --> " << PATH+FileDistribAngle << "\n\n";
		        return;
		    }

		    strmRadius.open((PATH+FileDistribRadius).c_str());
		    if (!strmRadius.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't open file --> " << PATH+FileDistribRadius << "\n\n";
		        return;
		    }

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
					err = distributionNormRadius(generator);
				else if (FlagDistribRadius == 2)
					err = distributionUniformRadius(generator);
				else if (FlagDistribRadius == 3)
					err = distributionCauchyRadius(generator);
				else if (FlagDistribRadius == 4)
					err = distributionGammaRadius(generator);
				else if (FlagDistribRadius == 5)
					err = distributionExponRadius(generator);
				else if (FlagDistribRadius == 6)
					err = distributionChiRadius(generator);
				else if (FlagDistribRadius == 7)
					err = distributionStudentRadius(generator);
				else
				{
					cout << "\n>>> error in EllipseGenerator --> wrong input\n\n";
					return;
				}
				if (i == PointsCount-1)
					strmRadius << i << " " << err;
				else
					strmRadius << i << " " << err << "\n";

				tmpX /= tmpL;
				tmpY /= tmpL;

				tmp += err;

				tmpX *= tmp;
				tmpY *= tmp;

				POINTS[i].x = Xc + tmpX;
				POINTS[i].y = Yc + tmpY;
			}

			strmAngle.close();
		    if (strmAngle.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't close file --> " << PATH+FileDistribAngle << "\n\n";
		        return ;
		    }

		    strmRadius.close();
		    if (strmRadius.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't close file --> " << PATH+FileDistribRadius << "\n\n";
		        return ;
		    }

			PrintToFile (POINTS, PointsCount, movedFILENAME, PATH);

			break;
		}
		case(5): // parameter + vector
		{
			for (i = 0; i < DrawCount; i++)
			{
				t = beginAngleRadian + i*deltaDraw;

				t = fmod(t, 2*M_PI);

				cosT = cos(t);
				sinT = sin(t);

				POINTS[i].x = Xc + A * cosT * cosA - B * sinT * sinA;
				POINTS[i].y = Yc + A * cosT * sinA + B * sinT * cosA;
			}

			PrintToFile (POINTS, DrawCount, originalFILENAME, PATH);

			POINTS.clear();
			POINTS.resize(PointsCount);

			strmAngle.open((PATH+FileDistribAngle).c_str());
		    if (!strmAngle.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't open file --> " << PATH+FileDistribAngle << "\n\n";
		        return;
		    }

		    strmRadius.open((PATH+FileDistribRadius).c_str());
		    if (!strmRadius.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't open file --> " << PATH+FileDistribRadius << "\n\n";
		        return;
		    }

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
					err = distributionNormAngle(generator);
				else if (FlagDistribAngle == 2)
					err = distributionUniformAngle(generator);
				else if (FlagDistribAngle == 3)
					err = distributionCauchyAngle(generator);
				else if (FlagDistribAngle == 4)
					err = distributionGammaAngle(generator);
				else if (FlagDistribAngle == 5)
					err = distributionExponAngle(generator);
				else if (FlagDistribAngle == 6)
					err = distributionChiAngle(generator);
				else if (FlagDistribAngle == 7)
					err = distributionStudentAngle(generator);
				else
				{
					cout << "\n>>> error in EllipseGenerator --> wrong input\n\n";
					return;
				}

				if (i == PointsCount-1)
					strmAngle << i << " " << err;
				else
					strmAngle << i << " " << err << "\n";

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
					err = distributionNormRadius(generator);
				else if (FlagDistribRadius == 2)
					err = distributionUniformRadius(generator);
				else if (FlagDistribRadius == 3)
					err = distributionCauchyRadius(generator);
				else if (FlagDistribRadius == 4)
					err = distributionGammaRadius(generator);
				else if (FlagDistribRadius == 5)
					err = distributionExponRadius(generator);
				else if (FlagDistribRadius == 6)
					err = distributionChiRadius(generator);
				else if (FlagDistribRadius == 7)
					err = distributionStudentRadius(generator);
				else
				{
					cout << "\n>>> error in EllipseGenerator --> wrong input\n\n";
					return;
				}
				if (i == PointsCount-1)
					strmRadius << i << " " << err;
				else
					strmRadius << i << " " << err << "\n";

				tmpX /= tmpL;
				tmpY /= tmpL;

				tmp += err;

				tmpX *= tmp;
				tmpY *= tmp;

				POINTS[i].x = Xc + tmpX;
				POINTS[i].y = Yc + tmpY;
			}

			strmAngle.close();
		    if (strmAngle.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't close file --> " << PATH+FileDistribAngle << "\n\n";
		        return ;
		    }

		    strmRadius.close();
		    if (strmRadius.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't close file --> " << PATH+FileDistribRadius << "\n\n";
		        return ;
		    }

			PrintToFile (POINTS, PointsCount, movedFILENAME, PATH);

			break;
		}
		case(6): // curve length 
		{
			double mult = 1e+6;
			double epss = 1e-6;

			gsl_mode_t mode = (unsigned int)(1e+100);
			double deltaPhi = (endAngleRadian-beginAngleRadian)/((double)(PointsCount*mult));


			double beginInt, beginInt_prev, curInt, curInt_prev, exactInt, localPhi;
			double Len = CalcEllipsePerimeter (A, B, beginAngle, endAngle, 0);
			double deltaLen = Len/((double)(PointsCount));
			vector<double> Phi(PointsCount);

			beginInt_prev = gsl_sf_ellint_E(beginAngleRadian, e, mode);
			exactInt = deltaLen/maxAxis+beginInt;

			phi = beginAngleRadian;

			Phi[0] = beginAngleRadian;

			// printf("\nLen = %lf\n", Len);
			// printf("\ndeltaLen = %lf\n", deltaLen);
			string fl;
			fl = PATH+"Residual.txt";
			strm.open(fl.c_str());
		    if (!strm.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't open file --> " << fl.c_str() << "\n\n";
		        return;
		    }

			for (int i = 1; i < PointsCount; i++)
			{
				beginInt = beginInt_prev;
				exactInt = deltaLen/maxAxis+beginInt;

			    localPhi = Phi[i-1];
			    curInt = gsl_sf_ellint_E(localPhi, e, mode);

			    // printf("beginInt = %lf\n", beginInt);
				// printf("exactInt = %lf\n", exactInt);
				// printf("curInt = %lf\n", curInt);

			    while (localPhi < endAngleRadian)
			    {	
			    	localPhi += deltaPhi;
			    	curInt_prev = curInt;
			    	curInt = gsl_sf_ellint_E(localPhi, e, mode);

			    	// printf("localPhi = %e\n", localPhi);
			    	// printf("   l^d - l = %e\n", maxAxis*(curInt-beginInt)-deltaLen);

			    	if (curInt < curInt_prev)
			    		break;

			    	if(fabs(maxAxis*(curInt-beginInt)-deltaLen)<epss)
			    		break;
			    }

			    err = fabs(maxAxis*(curInt-beginInt)-deltaLen);

			    if (i == PointsCount-1)
					strm << i << " " << err;
				else
					strm << i << " " << err << "\n";

			    beginInt_prev = curInt;
			    Phi[i] = localPhi;
			    // printf("Phi[%d] = %lf\n\n", i, Phi[i]);
			}

			strm.close();
		    if (strm.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't close file --> " << fl.c_str() << "\n\n";
		        return ;
		    }

			for (int i = 0; i < PointsCount; i++)
			{
				cosPHI = cos(Phi[i]);
				sinPHI = sin(Phi[i]);

				r = minAxis / sqrt(1 - e*e*cosPHI*cosPHI);

				POINTS[i].x = Xc + cosAmod * r * cosPHI + sinAmod * r * sinPHI;;
				POINTS[i].y = Yc - sinAmod * r * cosPHI + cosAmod * r * sinPHI;
			}

        	PrintToFile (POINTS, PointsCount, originalFILENAME, PATH);

        	POINTS.clear();
			POINTS.resize(PointsCount);
			
			strmAngle.open((PATH+FileDistribAngle).c_str());
		    if (!strmAngle.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't open file --> " << PATH+FileDistribAngle << "\n\n";
		        return;
		    }

		    strmRadius.open((PATH+FileDistribRadius).c_str());
		    if (!strmRadius.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't open file --> " << PATH+FileDistribRadius << "\n\n";
		        return;
		    }

			for (i = 0; i < PointsCount; i++)
			{
				if (FlagDistribAngle == 0)
					err = 0.0;
				else if (FlagDistribAngle == 1)
					err = distributionNormAngle(generator);
				else if (FlagDistribAngle == 2)
					err = distributionUniformAngle(generator);
				else if (FlagDistribAngle == 3)
					err = distributionCauchyAngle(generator);
				else if (FlagDistribAngle == 4)
					err = distributionGammaAngle(generator);
				else if (FlagDistribAngle == 5)
					err = distributionExponAngle(generator);
				else if (FlagDistribAngle == 6)
					err = distributionChiAngle(generator);
				else if (FlagDistribAngle == 7)
					err = distributionStudentAngle(generator);
				else
				{
					cout << "\n>>> error in EllipseGenerator --> wrong input\n\n";
					return;
				} // defines distribution

				if (i == PointsCount-1)
					strmAngle << i << " " << err;
				else
					strmAngle << i << " " << err << "\n";

				phi = Phi[i];
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

			strmAngle.close();
		    if (strmAngle.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't close file --> " << PATH+FileDistribAngle << "\n\n";
		        return ;
		    }

		    strmRadius.close();
		    if (strmRadius.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't close file --> " << PATH+FileDistribRadius << "\n\n";
		        return ;
		    }

			PrintToFile (POINTS, PointsCount, movedFILENAME, PATH);

			Phi.clear();

			break;
		}
		case(7): // gradient
		{
			gsl_mode_t mode = (unsigned int)(1e+100);
			double deltaPhi = (endAngleRadian-beginAngleRadian)/((double)(PointsCount*1e+4));


			double beginInt, beginInt_prev, curInt, curInt_prev, exactInt, localPhi;
			double Len = CalcEllipsePerimeter (A, B, beginAngle, endAngle, 0);
			double deltaLen = Len/((double)(PointsCount));
			vector<double> Phi(PointsCount);



			beginInt_prev = gsl_sf_ellint_E(beginAngleRadian, e, mode);
			exactInt = deltaLen/maxAxis+beginInt;

			phi = beginAngleRadian;

			Phi[0] = beginAngleRadian;

			// printf("\nLen = %lf\n", Len);
			// printf("\ndeltaLen = %lf\n", deltaLen);

			for (int i = 1; i < PointsCount; i++)
			{
				beginInt = beginInt_prev;
				exactInt = deltaLen/maxAxis+beginInt;

			    localPhi = Phi[i-1];
			    curInt = gsl_sf_ellint_E(localPhi, e, mode);

			    // printf("beginInt = %lf\n", beginInt);
				// printf("exactInt = %lf\n", exactInt);
				// printf("curInt = %lf\n", curInt);

			    while (localPhi < endAngleRadian)
			    {	
			    	localPhi += deltaPhi;
			    	curInt_prev = curInt;
			    	curInt = gsl_sf_ellint_E(localPhi, e, mode);

			    	// printf("localPhi = %e\n", localPhi);
			    	// printf("   l^d - l = %e\n", maxAxis*(curInt-beginInt)-deltaLen);

			    	if (curInt < curInt_prev)
			    		break;

			    	if(fabs(maxAxis*(curInt-beginInt)-deltaLen)<1e-4)
			    		break;
			    }

			    beginInt_prev = curInt;
			    Phi[i] = localPhi;
			    // printf("Phi[%d] = %lf\n\n", i, Phi[i]);
			}

			for (int i = 0; i < PointsCount; i++)
			{
				cosPHI = cos(Phi[i]);
				sinPHI = sin(Phi[i]);

				r = minAxis / sqrt(1 - e*e*cosPHI*cosPHI);

				POINTS[i].x = Xc + cosAmod * r * cosPHI + sinAmod * r * sinPHI;;
				POINTS[i].y = Yc - sinAmod * r * cosPHI + cosAmod * r * sinPHI;
			}

        	PrintToFile (POINTS, PointsCount, originalFILENAME, PATH);

        	POINTS.clear();
			POINTS.resize(PointsCount);
			
			strmAngle.open((PATH+FileDistribAngle).c_str());
		    if (!strmAngle.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't open file --> " << PATH+FileDistribAngle << "\n\n";
		        return;
		    }

		    strmRadius.open((PATH+FileDistribRadius).c_str());
		    if (!strmRadius.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't open file --> " << PATH+FileDistribRadius << "\n\n";
		        return;
		    }

			for (i = 0; i < PointsCount; i++)
			{
				phi = Phi[i];
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
					err = distributionNormRadius(generator);
				else if (FlagDistribRadius == 2)
					err = distributionUniformRadius(generator);
				else if (FlagDistribRadius == 3)
					err = distributionCauchyRadius(generator);
				else if (FlagDistribRadius == 4)
					err = distributionGammaRadius(generator);
				else if (FlagDistribRadius == 5)
					err = distributionExponRadius(generator);
				else if (FlagDistribRadius == 6)
					err = distributionChiRadius(generator);
				else if (FlagDistribRadius == 7)
					err = distributionStudentRadius(generator);
				else
				{
					cout << "\n>>> error in EllipseGenerator --> wrong input\n\n";
					return;
				}
				if (i == PointsCount-1)
					strmRadius << i << " " << err;
				else
					strmRadius << i << " " << err << "\n";

				POINTS[i].x = x + err * tmpX;
				POINTS[i].y = y + err * tmpY;
			}

			strmAngle.close();
		    if (strmAngle.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't close file --> " << PATH+FileDistribAngle << "\n\n";
		        return ;
		    }

		    strmRadius.close();
		    if (strmRadius.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't close file --> " << PATH+FileDistribRadius << "\n\n";
		        return ;
		    }

			PrintToFile (POINTS, PointsCount, movedFILENAME, PATH);

			Phi.clear();

			break;
		}
		case(8): // curve length + gradient
		{
			gsl_mode_t mode = (unsigned int)(1e+100);
			double deltaPhi = (endAngleRadian-beginAngleRadian)/((double)(PointsCount*1e+4));


			double beginInt, beginInt_prev, curInt, curInt_prev, exactInt, localPhi;
			double Len = CalcEllipsePerimeter (A, B, beginAngle, endAngle, 0);
			double deltaLen = Len/((double)(PointsCount));
			vector<double> Phi(PointsCount);



			beginInt_prev = gsl_sf_ellint_E(beginAngleRadian, e, mode);
			exactInt = deltaLen/maxAxis+beginInt;

			phi = beginAngleRadian;

			Phi[0] = beginAngleRadian;

			// printf("\nLen = %lf\n", Len);
			// printf("\ndeltaLen = %lf\n", deltaLen);

			for (int i = 1; i < PointsCount; i++)
			{
				beginInt = beginInt_prev;
				exactInt = deltaLen/maxAxis+beginInt;

			    localPhi = Phi[i-1];
			    curInt = gsl_sf_ellint_E(localPhi, e, mode);

			    // printf("beginInt = %lf\n", beginInt);
				// printf("exactInt = %lf\n", exactInt);
				// printf("curInt = %lf\n", curInt);

			    while (localPhi < endAngleRadian)
			    {	
			    	localPhi += deltaPhi;
			    	curInt_prev = curInt;
			    	curInt = gsl_sf_ellint_E(localPhi, e, mode);

			    	// printf("localPhi = %e\n", localPhi);
			    	// printf("   l^d - l = %e\n", maxAxis*(curInt-beginInt)-deltaLen);

			    	if (curInt < curInt_prev)
			    		break;

			    	if(fabs(maxAxis*(curInt-beginInt)-deltaLen)<1e-4)
			    		break;
			    }

			    beginInt_prev = curInt;
			    Phi[i] = localPhi;
			    // printf("Phi[%d] = %lf\n\n", i, Phi[i]);
			}

			for (int i = 0; i < PointsCount; i++)
			{
				cosPHI = cos(Phi[i]);
				sinPHI = sin(Phi[i]);

				r = minAxis / sqrt(1 - e*e*cosPHI*cosPHI);

				POINTS[i].x = Xc + cosAmod * r * cosPHI + sinAmod * r * sinPHI;;
				POINTS[i].y = Yc - sinAmod * r * cosPHI + cosAmod * r * sinPHI;
			}

        	PrintToFile (POINTS, PointsCount, originalFILENAME, PATH);

        	POINTS.clear();
			POINTS.resize(PointsCount);
			
			strmAngle.open((PATH+FileDistribAngle).c_str());
		    if (!strmAngle.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't open file --> " << PATH+FileDistribAngle << "\n\n";
		        return;
		    }

		    strmRadius.open((PATH+FileDistribRadius).c_str());
		    if (!strmRadius.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't open file --> " << PATH+FileDistribRadius << "\n\n";
		        return;
		    }

			for (i = 0; i < PointsCount; i++)
			{
				if (FlagDistribAngle == 0)
					err = 0.0;
				else if (FlagDistribAngle == 1)
					err = distributionNormAngle(generator);
				else if (FlagDistribAngle == 2)
					err = distributionUniformAngle(generator);
				else if (FlagDistribAngle == 3)
					err = distributionCauchyAngle(generator);
				else if (FlagDistribAngle == 4)
					err = distributionGammaAngle(generator);
				else if (FlagDistribAngle == 5)
					err = distributionExponAngle(generator);
				else if (FlagDistribAngle == 6)
					err = distributionChiAngle(generator);
				else if (FlagDistribAngle == 7)
					err = distributionStudentAngle(generator);
				else
				{
					cout << "\n>>> error in EllipseGenerator --> wrong input\n\n";
					return;
				} // defines distribution

				if (i == PointsCount-1)
					strmAngle << i << " " << err;
				else
					strmAngle << i << " " << err << "\n";

				phi = Phi[i];
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

				dr = -e*e*minAxis*sinPHI*cosPHI/(tmp*tmp*tmp);

				tg = (dr*sinPHI+r*cosPHI)/(dr*cosPHI-r*sinPHI);
				
				tmpX = -tg/sqrt(1+tg*tg);
				tmpY = 1/sqrt(1+tg*tg);

				//(-tg, 1)

				if (FlagDistribRadius == 0)
					err = 0.0;
				else if (FlagDistribRadius == 1)
					err = distributionNormRadius(generator);
				else if (FlagDistribRadius == 2)
					err = distributionUniformRadius(generator);
				else if (FlagDistribRadius == 3)
					err = distributionCauchyRadius(generator);
				else if (FlagDistribRadius == 4)
					err = distributionGammaRadius(generator);
				else if (FlagDistribRadius == 5)
					err = distributionExponRadius(generator);
				else if (FlagDistribRadius == 6)
					err = distributionChiRadius(generator);
				else if (FlagDistribRadius == 7)
					err = distributionStudentRadius(generator);
				else
				{
					cout << "\n>>> error in EllipseGenerator --> wrong input\n\n";
					return;
				}
				if (i == PointsCount-1)
					strmRadius << i << " " << err;
				else
					strmRadius << i << " " << err << "\n";

				POINTS[i].x = x + err * tmpX;
				POINTS[i].y = y + err * tmpY;
			}

			strmAngle.close();
		    if (strmAngle.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't close file --> " << PATH+FileDistribAngle << "\n\n";
		        return ;
		    }

		    strmRadius.close();
		    if (strmRadius.is_open())
		    {
		        cout << "\n>>> error in EllipseGenerator --> can't close file --> " << PATH+FileDistribRadius << "\n\n";
		        return ;
		    }

			PrintToFile (POINTS, PointsCount, movedFILENAME, PATH);

			Phi.clear();

			break;
		}
		default:
			break;
	}	

	return ;
}









