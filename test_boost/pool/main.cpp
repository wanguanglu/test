/**
* @file   main.cpp
* @author wanguanglu(mail@wanguanglu.com)
* @date   2017/08/06 18:45:02
* @brief 
*  
**/

#include <iostream>

#include <boost/pool/object_pool.hpp>

struct Student{
  Student() {
    std::cout << "Construct ..." << std::endl;
  }

  ~Student() {
    std::cout << "Destruct ..." << std::endl;
  }

  int id_;
  int age_;
};

int main(int argc, char* argv[]) {
  boost::object_pool<Student> alloctor;
  Student* a = alloctor.construct();
  Student* b = alloctor.construct();
  alloctor.destroy(a);
  alloctor.destroy(b);

  std::cout << "--------------" << std::endl;
  Student* c = alloctor.malloc();
  alloctor.free(c);
  return 0;
}

