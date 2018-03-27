/**
* @file   test_advance.cpp
* @author wanguanglu(mail@wanguanglu.com)
* @date   2018/03/02 18:15:48
* @brief 
*  
**/

#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
  std::vector<int> vec;
  for (int i = 0; i < 5; i++) {
    vec.push_back(i);
  }
  auto iter = vec.begin();

  std::advance(iter, 3);
  std::cout << *iter << std::endl;

  // Invalid iterator.
  std::advance(iter, 5);
  std::cout << *iter << std::endl;
}
