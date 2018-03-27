/**
* @file   test_tuple.cpp
* @author wanguanglu(mail@wanguanglu.com)
* @date   2017/04/26 20:42:12
* @brief 
*  
**/

#include <tuple>
#include <iostream>

int main(int argc, char* argv[]) {
  int x = 0;
  int y = 1;
  std::tuple<int, int> tp = std::make_tuple(x, y);
  std::cout<<"the firt value: "<<std::get<0>(tp)<<"\n";
  std::cout<<"the second value: "<<std::get<1>(tp)<<"\n";

  tp = std::make_tuple(x, y);
  std::cout<<"the firt value: "<<std::get<0>(tp)<<"\n";
  std::cout<<"the second value: "<<std::get<1>(tp)<<"\n";

  tp = std::tie(x, y);
  std::cout<<"the firt value: "<<std::get<0>(tp)<<"\n";
  std::cout<<"the second value: "<<std::get<1>(tp)<<"\n";

  int a, b;
  std::tie(a, b) = tp;
  std::cout<<"a: "<<a<<"\n";
  std::cout<<"b: "<<b<<"\n";
}
