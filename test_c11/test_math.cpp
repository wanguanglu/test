/**
* @file   test_math.cpp
* @author wanguanglu(mail@wanguanglu.com)
* @date   2017/05/18 23:15:27
* @brief 
*  
**/

#include <cmath>
#include <iostream>

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

void test_2() {
  // std::sqrt(x*x + y*y)
  std::cout << std::hypot(3.0, 4.0) << std::endl;
}

int main(int argc, char* argv[]) {
  test_2();
  return 0;
}
