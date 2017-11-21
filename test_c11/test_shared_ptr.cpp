/**
* @file   test_shared_ptr.cpp
* @author wanguanglu(mail@wanguanglu.com)
* @date   2017/10/30 18:29:27
* @brief 
*  
**/

#include <iostream>
#include <memory>

// Basic test.
void test1() {
  std::shared_ptr<int> a = std::make_shared<int>(1);
  std::cout << "a = " << *a << std::endl;
  std::shared_ptr<int> b = a;
  std::cout << "use count of a " << a.use_count() << std::endl;
  std::cout << "a.get() " << a.get() << std::endl;
  std::cout << "b.get() " << b.get() << std::endl;

  std::shared_ptr<int> c = std::make_shared<int>(*a);
  std::cout << "use count of a " << a.use_count() << std::endl;
}

void test2() {
  std::shared_ptr<int> a = std::make_shared<int>(1);
  const std::shared_ptr<int>& b = a;
  std::cout << "use count of a " << a.use_count() << std::endl;
  *b = 0;
  std::cout << "a = " << *a << std::endl;
  std::shared_ptr<int> c = std::make_shared<int>(1);
  // b = c; // Error

  std::shared_ptr<const int> d = a;
  std::cout << "use count of a " << a.use_count() << std::endl;
  // *d = 2;  // Error;
  d = c;
}

void use(std::shared_ptr<int>& ptr) {
  auto ptr_2 = ptr;
  std::cout << "use count of ptr " << ptr.use_count() << std::endl;
  std::cout << "use count of ptr_2 " << ptr_2.use_count() << std::endl;
}

void test3() {
  std::shared_ptr<int> ptr = std::make_shared<int>(1);
  use(ptr);
  std::cout << "use count of ptr " << ptr.use_count() << std::endl;
}

int main(int argc, char* argv[]) {
  test3();
}
