
#include "EllipseFunctions.h"

void Coeffs2MAF (const vector<double>& COEFFS, VectorXd& MAF)
{
	double eps = 1.0;
	while (eps + 1.0 > 1.0)
		eps /= 2.0;

	vector<double> coeffs(COEFFS);
	Normalize(coeffs);

	double A = coeffs[0];
	double B = coeffs[2]/2.0;
	double C = coeffs[1];
	double D = coeffs[3]/2.0;
	double E = coeffs[4]/2.0;
	double F = coeffs[5];

	coeffs.clear();

	MatrixXd I2(2, 2), I3(3, 3);
	VectorXd V(2);

	I2(0, 0) = A, I2(0, 1) = B;
	I2(1, 0) = B, I2(1, 1) = C;

	I3(0, 0) = A; I3(0, 1) = B; I3(0, 2) = D;
	I3(1, 0) = B; I3(1, 1) = C; I3(1, 2) = E;
	I3(2, 0) = D; I3(2, 1) = E; I3(2, 2) = F;

	V(0) = -D; V(1) = -E;

	auto pinvI2 = pInv(I2);

	auto center = pinvI2 * V;

	SelfAdjointEigenSolver<MatrixXd> eigensolver(I2);
	VectorXcd eigVals = eigensolver.eigenvalues();
	VectorXd eigVec0 = eigensolver.eigenvectors().col(0);

	double eig0 = eigVals[0].real();
	double eig1 = eigVals[1].real();

	double detI2 = I2.determinant();
	double detI3 = I3.determinant();

	double denom0 = detI2 * eig0;
	double denom1 = detI2 * eig1;

	if (!(fabs(denom0) > eps) || !(fabs(denom1) > eps))
		return;

	MAF(0) = center(0);
	MAF(1) = center(1);
	MAF(2) = sqrt(-detI3 / denom0);
	MAF(3) = sqrt(-detI3 / denom1);
	MAF(4) = acos(eigVec0(0) / eigVec0.norm());

	if (MAF(4) > M_PI / 2.0)
		MAF(4) = M_PI - MAF(4);

	return;
}

void MAF2Coeffs (const VectorXd& MAF, vector<double>& COEFFS)
{
	double X0 = MAF(0);
	double Y0 = MAF(1);
	double A = MAF(2);
	double B = MAF(3);
	double sinAngle = sin(MAF(4));
	double cosAngle = cos(MAF(4));

	COEFFS[0] = cosAngle * cosAngle / (A * A) + sinAngle * sinAngle / (B * B);
	COEFFS[1] = sinAngle * sinAngle / (A * A) + cosAngle * cosAngle / (B * B);
	COEFFS[2] = 2*(1 / (A * A) - 1 / (B * B)) * sinAngle * cosAngle;
	COEFFS[3] = -2*cosAngle*cosAngle*X0/(A*A) - 2*sinAngle*cosAngle*Y0/(A*A)-2*sinAngle*sinAngle*X0/(B*B) + 2*sinAngle*cosAngle*Y0/(B*B);
	COEFFS[4] = -2 * sinAngle * cosAngle * X0 / (A * A) - 2 * sinAngle * sinAngle * Y0 / (A * A) + 2 * sinAngle * cosAngle * X0 / (B * B) - 2 * cosAngle * cosAngle * Y0 / (B * B);
	COEFFS[5] = (cosAngle* cosAngle*X0*X0 + 2*sinAngle* cosAngle*X0*Y0+ sinAngle* sinAngle*Y0*Y0)/(A*A) + (sinAngle * sinAngle * X0 * X0 - 2 * sinAngle * cosAngle * X0 * Y0 + cosAngle * cosAngle * Y0 * Y0) / (B * B)-1;

	double val, normCOEFFS = 0.0;
    for (int i = 0; i < 6; i++)
    {
        val = COEFFS[i];
        normCOEFFS += val*val;
    }
    normCOEFFS = sqrt(normCOEFFS);
    for (int i = 0; i < 6; i++)
        COEFFS[i] /= normCOEFFS;

	return;
}

double fArc (double PHI, void* P)
{
	struct rhoParams* PARAMS = (struct rhoParams*)P;

    double Xc = (PARAMS->Xc);
    double Yc = (PARAMS->Yc);
    double A = (PARAMS->A);
    double B = (PARAMS->B);
    double Angle = (PARAMS->Angle);

	double AngleRadian = Angle * M_PI / 180.0;
	double phiRadian = PHI * M_PI / 180.0;

	double cosA = cos(AngleRadian);
	double sinA = sin(AngleRadian);

	double minAxis = (A > B) ? B : A;
	double maxAxis = (A > B) ? A : B;

	double e = sqrt(1 - minAxis*minAxis/(maxAxis*maxAxis));

	double cosPHI = cos(phiRadian);
	double sinPHI = sin(phiRadian);

	double tmp = sqrt(1 - e*e*cosPHI*cosPHI);
	double r = minAxis / tmp;

	double dr = -e*e*minAxis*sinPHI*cosPHI/(tmp*tmp*tmp);

	double res = sqrt(r*r+dr*dr);

	return res;
}


// rhoParams PARAMS = {Xc, Yc, A, B, Angle};
// CalcEllipseArcLenght (beginAngle, endAngle, &PARAMS);

double CalcEllipseArcLenght (double beginAngle, double endAngle, void* P, bool flag)
{
	// struct rhoParams* PARAMS = (struct rhoParams*)P;

	double eps = 1.0, delta;
	while (eps + 1.0 > 1.0)
		eps /= 2.0;

    if (fabs(beginAngle - endAngle) < eps)
	{
		cout << "\n>>> error in CalcEllipseArcLenght --> wrong input data\n\n";
		return -1.0; 
	}

	gsl_integration_workspace * w = gsl_integration_workspace_alloc (1000);

    double result, error;

  	gsl_function F;
  	
  	F.function = &fArc;
  	F.params = P;

  	double beginAngleRadian = beginAngle * M_PI / 180.0;
  	double endAngleRadian = endAngle * M_PI / 180.0;

  	gsl_integration_qags (&F, beginAngleRadian, endAngleRadian, 0, 1e-8, 1000, w, &result, &error);

  	gsl_integration_workspace_free (w);

  	// printf("result = %lf\n\n", result);

    return result;
}

double CalcEllipsePerimeter (double A, double B, double beginAngle, double endAngle, int flag)
{
	double res;
	double beginAngleRadian = beginAngle * M_PI / 180.0;
	double endAngleRadian = endAngle * M_PI / 180.0;
	double minAxis = (A > B) ? B : A;
	double maxAxis = (A > B) ? A : B;
	double e = sqrt(1 - minAxis*minAxis/(maxAxis*maxAxis));
	double M, N;

	switch(flag)
	{
		case(0):
		{	
			gsl_mode_t mode = (unsigned int)(1e+10);
			// res = 4*maxAxis*std::ellint_2(1, e);
			res = maxAxis*(gsl_sf_ellint_E(endAngleRadian, e, mode)-gsl_sf_ellint_E(beginAngleRadian, e, mode));
			// printf("\n\nres = %lf\n\n", res);
			return res;
		}
		case (1):
		{
			// M = boost::math::tools::agm(1-e*e, 1);

			break;
		}
		case (2):
		{
			
			
			break;
		}
		default:
		{
			cout << "\n>>> error in CalcEllipsePerimeter --> wrong input\n\n";
			return -1.0;
		}
	}

	return 1.0;
}