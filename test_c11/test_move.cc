/**
* @file   test_move.cpp
* @author wanguanglu(mail@wanguanglu.com)
* @date   2017/05/17 09:51:41
* @brief 
*  
**/

#include <iostream>

class Test {
public:
  Test() {
    std::cout<<"basic construct."<<std::endl;
  }

  Test(const Test& x) {
    std::cout<<"copy construct."<<std::endl;
  }

  Test(const Test&& x) {
    std::cout<<"right value copy construct."<<std::endl;
  }

  Test& operator=(const Test&& x) {
    std::cout<<"= assign of right value."<<std::endl;
    return *this;
  }

  Test& operator=(const Test& x) {
    std::cout<<"= assign of left value."<<std::endl;
    return *this;
  }
};

Test func1() {
  Test x = Test();
  return x;
}

int main(int argc, char* argv[]) {
  Test a;
  Test b = a;
  Test c = std::move(a);

  std::cout<<std::endl;
  c = b;

  std::cout<<std::endl;
  c = std::move(b);

  // d = func1() is the same as
  // d = std::move(func1())
  std::cout<<std::endl;
  Test d;
  d = func1();

  std::cout<<std::endl;
  d = std::move(func1());
  return 0;
}
