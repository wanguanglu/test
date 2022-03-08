/**
 * @file   hello.cc
 * @author wanguanglu(mail@wanguanglu.com)
 * @date   2016/12/20 05:01:18
 * @brief
 *
 **/

#include <Eigen/Dense>
#include <iostream>

using Eigen::MatrixXd;

int main() {
  MatrixXd m(2, 2);
  std::cout << m << std::endl;
  m(0, 0) = 3;
  m(1, 0) = 2.5;
  m(0, 1) = -1;
  m(1, 1) = m(1, 0) + m(0, 1);
  std::cout << m << std::endl;
}
