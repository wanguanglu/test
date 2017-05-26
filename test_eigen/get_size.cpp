/**
* @file   set_size_at_run.cpp
* @author wanguanglu(mail@wanguanglu.com)
* @date   2017/05/25 17:05:26
* @brief 
*  
**/

#include <iostream>
#include <Eigen/Dense>
using namespace Eigen;
using namespace std;

int main() {
  MatrixXd m = MatrixXd::Random(3,3);
  m = (m + MatrixXd::Constant(3, 3, 1.2)) * 50;

  VectorXd v(3);

  cout <<"m.size: " << m.size() << endl;
  cout <<"m.rows: " << m.rows() << endl;
  cout <<"m.cols: " << m.cols() << endl;

  cout <<"v.size: " << v.size() << endl;
  cout <<"v.rows: " << v.rows() << endl;
  cout <<"v.cols: " << v.cols() << endl;
}
