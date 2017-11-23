/**
* @file   test_reference.cpp
* @author wanguanglu(mail@wanguanglu.com)
* @date   2017/06/06 21:16:16
* @brief 
*  
**/

#include <iostream>

class A {
public:
  A() {
    std::cout << "contruct: " << std::endl;
  }

  ~A() {
    std::cout << "destruct: " << std::endl;
  }
};

A Get() {
  return A();
}

void test() {
  A&& a = Get();
  const A& b =  A();
  A c;
  // b = c;
}

void basic_test() {
  int a = 0;
  const int& b = a;
  a = 1;

  std::cout << "b = " << b << std::endl;
}

int main(int argc, char* argv[]) {
  basic_test();
}
