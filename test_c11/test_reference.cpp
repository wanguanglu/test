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

int main(int argc, char* argv[]) {
  A&& a = Get();
}
