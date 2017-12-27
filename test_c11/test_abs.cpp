/**
* @file   test_abs.cpp
* @author wanguanglu(mail@wanguanglu.com)
* @date   2017/05/25 12:12:37
* @brief 
*  
**/

#include <stdlib.h>

#include <cmath>
#include <iostream>

int main() {
  double i = -0.1;
  std::cout << "Integer version: " << abs(i) << ";" << std::endl;
  std::cout << "Auto-detect type version:" << std::abs(i) << ";" << std::endl;
  std::cout << "Not equal." << std::endl;
  return 0;
}
