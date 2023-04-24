
#include "EigenFunctions.h"

MatrixXd pInv(const MatrixXd& OBJ)
{
	auto CONJ = OBJ.transpose();
	auto TEMP = (CONJ * OBJ);
	auto PINV = TEMP.inverse() * CONJ;

	return PINV;
}