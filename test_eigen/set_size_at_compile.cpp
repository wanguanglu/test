/**
 * @file   set_size_at_compile.cpp
 * @author wanguanglu(mail@wanguanglu.com)
 * @date   2017/05/25 17:04:34
 * @brief
 *
 **/

#include <iostream>

#include <Eigen/Dense>

using namespace Eigen;
using namespace std;

int main() {
  Matrix3d m = Matrix3d::Random();
  m = (m + Matrix3d::Constant(1.2)) * 50;
  cout << "m =" << endl << m << endl;
  Vector3d v(1, 2, 3);

  cout << "m * v =" << endl << m * v << endl;
}
