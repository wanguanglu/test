/**
* @file   test_math.cpp
* @author wanguanglu(mail@wanguanglu.com)
* @date   2017/05/18 23:15:27
* @brief 
*  
**/

#include <cmath>
#include <iostream>
#include <limits>

void test_numlimits() {
  std::cout << std::numeric_limits<double>::max() << std::endl;
  std::cout << std::numeric_limits<double>::min() << std::endl;
  std::cout << std::numeric_limits<float>::max() << std::endl;
  std::cout << std::numeric_limits<float>::min() << std::endl;
  std::cout << std::numeric_limits<int>::max() << std::endl;
  std::cout << std::numeric_limits<int>::min() << std::endl;
  std::cout << std::numeric_limits<uint32_t>::max() << std::endl;
  std::cout << std::numeric_limits<uint32_t>::min() << std::endl;
  std::cout << std::numeric_limits<int16_t>::max() << std::endl;
  std::cout << std::numeric_limits<int16_t>::min() << std::endl;
  std::cout << std::numeric_limits<uint16_t>::max() << std::endl;
  std::cout << std::numeric_limits<uint16_t>::min() << std::endl;
  std::cout << std::numeric_limits<char>::max() << std::endl;
  std::cout << std::numeric_limits<char>::min() << std::endl;
}

// https://msdn.microsoft.com/en-us/library/4hwaceh6.aspx
void test_const() {
  std::cout << "PI " << M_PI << std::endl;
  std::cout << "PI/2 " << M_PI_2 << std::endl;
  std::cout << "E " << M_E << std::endl;
  std::cout << "Log2E " << M_LOG2E << std::endl;
  std::cout << "Log10E " << M_LOG10E << std::endl;
  std::cout << "LN2 " << M_LN2 << std::endl;
  std::cout << "LN10 " << M_LN10 << std::endl;
}

void test_hypot() {
  // std::sqrt(x*x + y*y)
  std::cout << std::hypot(3.0, 4.0) << std::endl;
}

void test_floor() {
  float a = 0.5;
  std::cout << a << "\t" << int(a) << std::endl;
  std::cout << a << "\tfloor\t" << int(floor(a)) << std::endl;

  a = 0.2;
  std::cout << a << "\t" << int(a) << std::endl;
  std::cout << a << "\tfloor\t" << int(floor(a)) << std::endl;

  a = 0.8;
  std::cout << a << "\t" << int(a) << std::endl;
  std::cout << a << "\tfloor\t" << int(floor(a)) << std::endl;

  a = -0.8;
  std::cout << a << "\t" << int(a) << std::endl;
  std::cout << a << "\tfloor\t" << int(floor(a)) << std::endl;

  a = -0.2;
  std::cout << a << "\t" << int(a) << std::endl;
  std::cout << a << "\tfloor\t" << int(floor(a)) << std::endl;

  a = -1.8;
  std::cout << a << "\t" << int(a) << std::endl;
  std::cout << a << "\tfloor\t" << int(floor(a)) << std::endl;
}

void test_max_min() {
  int x = 0;
  int y = 10;
  std::cout << "std::max(x, y): " << std::max(x, y) << std::endl;
  std::cout << "std::min(x, y): " << std::min(x, y) << std::endl;
}

void test_pow() {
  double x = 4.0;
  std::cout << "std::pow(x, 0.5): " << std::pow(x, 0.5) << std::endl;
}

void test_abs() {
  double x = -1.5;
  std::cout << "std::abs(x): " << std::abs(x) << std::endl;
  std::cout << "abs(x): " << abs(x) << std::endl;
}

int main(int argc, char* argv[]) {
  test_numlimits();
  test_abs();
  return 0;
}
