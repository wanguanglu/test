/**
* @file   test_map.cpp
* @author wanguanglu(wanguanglu@qq.com)
* @date   2017/04/21 09:24:03
* @brief 
*  
**/

#include <iostream>
#include <map>

template<typename Map>
void print_map(Map& m) {
  std::cout << '{';
  for(auto& p: m) {
    std::cout << p.first << ':' << p.second << ' ';
  }
  std::cout << "}\n";
}

void test_out_of_range(const std::map<std::string, int>& m) {
  try {
    m.at("out");
  } catch (std::out_of_range& e) {
    std::cout<<"out of range"<<std::endl;
    std::cout<<e.what()<<std::endl;
  }
}

int main(int argc, char* argv[]) {
  // (1) Default constructor
  std::map<std::string, int> map1;
  map1["something"] = 69;
  map1["anything"] = 199;
  map1["that thing"] = 50;
  std::cout << "map1 = ";
  print_map(map1);

  // (2) Range constructor
  std::map<std::string, int> iter(map1.find("something"), map1.end());
  std::cout << "\niter = "; print_map(iter);
  std::cout << "map1 = "; print_map(map1);

  // (3) Copy constructor
  std::map<std::string, int> copied(map1);
  std::cout << "\ncopied = "; print_map(copied);
  std::cout << "map1 = "; print_map(map1);

  // (4) Move constructor
  std::map<std::string, int> moved(std::move(map1));
  std::cout << "\nmoved = "; print_map(moved);
  std::cout << "map1 = "; print_map(map1);

  // (5) Initializer list constructor
  const std::map<std::string, int> init {
      {"this", 100},
      {"can", 100},
      {"be", 100},
      {"const", 100},
  };
  std::cout << "\ninit = "; print_map(init);

  test_out_of_range(init);
}
