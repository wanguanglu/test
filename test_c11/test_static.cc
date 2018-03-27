/**
* @file   test_static.cpp
* @author wanguanglu(mail@wanguanglu.com)
* @date   2017/11/06 10:35:52
* @brief 
*  
**/

#include <iostream>

class A {
public:
  A() {
    std::cout << "Constuct A" << std::endl;
  }
};

static A a;

class B {
public:
  B() {
    std::cout << "Constuct B" << std::endl;
  }
};

void func() {
  static B b;
}

int main(int argc, char* argv[]) {
  std::cout << "In main." << std::endl;
  func();
  return 0;
}
