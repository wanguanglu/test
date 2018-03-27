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

// primary test of unique ptr
void test1() {
  std::unique_ptr<int> a = std::unique_ptr<int>(new int(5));
  std::cout << "a = " << *a <<std::endl;

  std::unique_ptr<int> b = std::move(a);
  std::cout << "b = " << *b << std::endl;
  std::cout << "a = " << *a << std::endl;  // It will core dump here.
}

// test push unique ptr to vector
void test2() {
  std::unique_ptr<int> a = std::unique_ptr<int>(new int(1));
  std::unique_ptr<int> b = std::unique_ptr<int>(new int(2));
  std::unique_ptr<int> c = std::unique_ptr<int>(new int(3));

  std::vector<std::unique_ptr<int>> vec;
  vec.push_back(std::move(a));
  vec.push_back(std::move(b));
  vec.push_back(std::move(c));

  for (const auto& ptr : vec) {
    std::cout << "current = " << *ptr << std::endl;
  }
}

std::unique_ptr<int> ret_ptr() {
  return std::unique_ptr<int>(new int(1));
}

void test3() {
  std::unique_ptr<int> a = ret_ptr();
  std::unique_ptr<int> b = ret_ptr();
  std::unique_ptr<int> c = ret_ptr();

  std::vector<std::unique_ptr<int>> vec;
  vec.push_back(std::move(a));  // It's OK.
  vec.push_back(std::move(a));  // It's OK.
  vec.push_back(std::move(a));  // It's OK.

  for (const auto& ptr : vec) {
    std::cout << "current = " << *ptr << std::endl;
  }
}

void test4() {
  std::vector<std::unique_ptr<int>> vec;
  vec.push_back(ret_ptr());  // It's OK.
  vec.push_back(ret_ptr());  // It's OK.
  vec.push_back(ret_ptr());  // It's OK.

  for (const auto& ptr : vec) {
    std::cout << "current = " << *ptr << std::endl;
  }
}

void test5() {
  std::vector<std::unique_ptr<int>> vec;
  vec.push_back(std::move(ret_ptr()));  // Warning
  vec.push_back(std::move(ret_ptr()));
  vec.push_back(std::move(ret_ptr()));

  for (const auto& ptr : vec) {
    std::cout << "current = " << *ptr << std::endl;
  }
}

void test6() {
  std::unique_ptr<int> uni_ptr(new int(1));
  std::cout << uni_ptr.get() << std::endl;

  // Failed
  // std::shared_ptr<int> sh_ptr = std::make_shared<int>(*uni_ptr);
  // std::cout<<sh_ptr.get()<<std::endl;
}

void test7() {
  std::unique_ptr<int> uni_ptr(new int(1));

  const std::unique_ptr<int>& uni_ptr_copy = uni_ptr;
}

int main(int argc, char* argv[]) {
  test6();
  return 0;
}
