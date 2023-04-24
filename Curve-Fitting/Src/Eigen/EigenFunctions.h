#ifndef EIGEN_FUNCTIONS_H_
#define EIGEN_FUNCTIONS_H_

#include <iostream>
#include <string>

#include <eigen3/Eigen/Dense>

using Eigen::MatrixXd;

MatrixXd pInv(const MatrixXd& T);

#endif // EIGEN_FUNCTIONS_H_