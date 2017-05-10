/**
* @file   hello_world.cpp
* @author wanguanglu(mail@wanguanglu.com)
* @date   2017/05/10 19:52:59
* @brief 
*  
**/

#include <iostream>

#include <gflags/gflags.h>

DEFINE_bool(bool_value, true, "Boolean value.");
DEFINE_int32(int_value, 1000, "Integer value.");
DEFINE_int64(int64_value, 10001, "Integer value.");
DEFINE_double(double_value, 1.01, "double value.");
DEFINE_string(str_value, "", "String value.");

int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  std::cout<<"bool_value: "<<FLAGS_bool_value<<std::endl;
  std::cout<<"int_value: "<<FLAGS_int_value<<std::endl;
  std::cout<<"int64_value: "<<FLAGS_int64_value<<std::endl;
  std::cout<<"double_value: "<<FLAGS_double_value<<std::endl;
  std::cout<<"str_value: "<<FLAGS_str_value<<std::endl;
  return 0;
}
