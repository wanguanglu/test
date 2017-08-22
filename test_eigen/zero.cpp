/**
* @file   zero.cc
* @author wanguanglu(mail@wanguanglu.com)
* @date   2016/12/20 05:01:18
* @brief 
*  
**/

#include <iostream>
#include <Eigen/Dense>

using Eigen::MatrixXd;

int main() {
  MatrixXd m(2, 2);
  std::cout << m << std::endl;

  MatrixXd n = MatrixXd::Random(2, 2);
  std::cout << n << std::endl;

  n.setZero(2, 3);
  std::cout << n << std::endl;
}
