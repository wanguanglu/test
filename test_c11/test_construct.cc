/**
* @file   test_construct.cpp
* @author wanguanglu(mail@wanguanglu.com)
* @date   2017/10/30 21:49:30
* @brief 
*  
**/

#include <iostream>

class B {
public:
  virtual ~B() = default;
};

class A : public B {
public:
  explicit A(int i) : x_(i) {}

  void Print() {
    std::cout << "x = " << x_ << std::endl;
  }

private:
  const int x_;
};

int main(int argc, char* argv[]) {
  A a(1);
  A b(2);
  // b = a;  // compile error.
  b.Print();
  return 0;
}
