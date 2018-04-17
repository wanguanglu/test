/**
* @file   test_vector.cpp
* @author wanguanglu(wanguanglu@qq.com)
* @date   2017/04/21 09:24:03
* @brief 
*  
**/

#include <iostream>
#include <vector>
#include <set>

int main(int argc, char* argv[]) {
  std::vector<int> v = {0, 1, 2, 3, 4, 5};
  
  for (const int& i : v) { // access by const reference
    std::cout << i << ' ';
  }
  std::cout << '\n';
                     
  for (auto i : v) { // access by value, the type of i is int
    std::cout << i << ' ';
  }
  std::cout << '\n';
                                        
  for (auto&& i : v) { // access by reference, the type of i is int&
    std::cout << i << ' ';
  }
  std::cout << '\n';
                                                           
  for (int n : {0, 1, 2, 3, 4, 5}) { // the initializer may be a braced-init-list
    std::cout << n << ' ';
  }
  std::cout << '\n';
                                                                              
  int a[] = {0, 1, 2, 3, 4, 5};
  for (int n : a) { // the initializer may be an array
    std::cout << n << ' ';
  }
  std::cout << '\n';
                                                                                                     
  for (int n : a) {
    std::cout << 1 << ' '; // the loop variable need not be used
  }
  std::cout << '\n';

  std::set<int> test_set;
  test_set.insert(1);
  test_set.insert(2);
  test_set.insert(3);
  test_set.insert(4);

  std::vector<int> test_vec = {test_set.begin(), test_set.end()};
  for (int n : test_vec) {
    std::cout << n << " ";
  }
  std::cout << '\n';

  auto iter = test_vec.end();
  test_vec.insert(iter, 100);
  for (int n : test_vec) {
    std::cout << n << " ";
  }
  std::cout << '\n';
}
