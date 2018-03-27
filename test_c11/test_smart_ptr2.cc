/**
* @file   test_smart_ptr.cpp
* @author wanguanglu(wanguanglu@qq.com)
* @date   2017/04/11 15:50:37
* @brief 
*  
**/

#include <iostream>
#include <memory>
#include <vector>

struct B {
  int x;
};

struct A {
  std::unique_ptr<B> ptr_;
};

// primary test of unique ptr
void test1() {
  std::unique_ptr<B> ptr = std::unique_ptr<B>(new B {1});
  A a = { std::move(ptr) };

  // Error
  // std::shared_ptr<A> a1 = std::make_shared<A>(a);
}

int main(int argc, char* argv[]) {
  test1();
  return 0;
}
