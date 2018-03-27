/**
* @file   test_distance.cpp
* @author wanguanglu(mail@wanguanglu.com)
* @date   2018/03/02 18:20:31
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
  auto iter1 = vec.begin();
  auto iter2 = iter1 + 3;

  std::cout << std::distance(iter1, iter2) << std::endl;
  std::cout << std::distance(iter2, iter1) << std::endl;
}
