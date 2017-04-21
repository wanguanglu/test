/**
* @file   test_smart_ptr.cpp
* @author wanguanglu(wanguanglu@qq.com)
* @date   2017/04/11 15:50:37
* @brief 
*  
**/

#include <iostream>
#include <memory>

int main(int argc, char* argv[]) {

  std::unique_ptr<int> a = std::unique_ptr<int>(new int(5));
  std::cout<<"a = "<<*a<<std::endl;

  std::unique_ptr<int> b = std::move(a);
  std::cout<<"b = "<<*b<<std::endl;
  std::cout<<"a = "<<*a<<std::endl;

  return 0;
}
