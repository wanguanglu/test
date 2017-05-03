/**
* @file   error.cc
* @author wanguanglu(mail@wanguanglu.com)
* @date   2017/02/08 11:00:11
* @brief 
*  
**/
#include <string>
#include <stdexcept>
#include <iostream>

class Error : public std::runtime_error {
public:
  explicit Error(const std::string &s) : std::runtime_error(s){}
};

void test() {
  std::string error_info = "Just for test";
  throw Error(error_info);
}

void test2() {
  abort();
}

int main(int argc, char* argv[]) {
  try {
    test();
  } catch (Error e) {
    std::cout<<"Error: "<<e.what()<<std::endl;
  }

  //test();
  test2();
  return 0;
}


