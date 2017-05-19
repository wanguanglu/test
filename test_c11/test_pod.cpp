/**
* @file   test_pod.cpp
* @author wanguanglu(mail@wanguanglu.com)
* @date   2017/05/20 04:22:04
* @brief 
*  
**/

#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
  std::cout<<"is string POD type "<<std::is_pod<std::string>::value<<std::endl; 
  std::cout<<"is in POD type "<<std::is_pod<int>::value<<std::endl;
}
