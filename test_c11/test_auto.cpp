/**
* @file   test_auto.cpp
* @author wanguanglu(mail@wanguanglu.com)
* @date   2017/06/05 19:42:52
* @brief 
*  
**/

#include <iostream>
#include <map>
#include <string>

void basic_test() {
  auto x = 5;   // type of x is int
  auto pi = new auto(1);    // type of pi is int*
  const auto *v = &x, u = 6;  // type if v is const int*, type of u is const int
  static auto y = 0.0;  // type of y is static double

  // auto int r;  // error
  // auto s;  // error, 
}

void map_iterator() {
  std::map<int, int> result_map;
  for (int i = 0; i < 10; i++) {
    result_map[i] = i * i;
  }

  for (std::map<int, int>::iterator it = result_map.begin();
      it != result_map.end(); it++) {
    std::cout << it->first << "\t" << it->second << std::endl;
  }

  std::cout << "use auto." << std::endl;

  for (auto it = result_map.begin(); it != result_map.end(); it++) {
    std::cout << it->first << "\t" << it->second << std::endl;
  }
}

/*
void unordered_multimap_iterator() {
  std::unordered_multimap<int, int> result_map;

  for (int i = 0; i < 10; i++) {
    result_map[i] = i;
    result_map[i] = i + 1;
    result_map[i] = i + 2;
  }

  std::pair<std::unordered_multimap<int, int>::iterator,
    std::unordered_multimap<int, int>::iterator> range;
  range = result_map.equal_range(5);
  for (std::unordered_multimap<int, int>::iterator it = range.first;
      it != range.second; it++) {
    std::cout << it->first << "\t" << it->second << std::endl;
  }
}
*/

class A {
public:
  static int Get() {
    return 1;
  }
};

class B {
public:
  static std::string Get() {
    return "class B";
  }
};

template <typename T>
void Get() {
  auto info = T::Get();
  std::cout << info << std::endl;
}

void TestGet() {
  Get<A>();
  Get<B>();
}

int main(int argc, char* argv[]) {
  TestGet();
  return 0;
}
