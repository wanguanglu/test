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
  boost::object_pool<Student> alloctor(1, 2);
  std::cout << alloctor.get_next_size() << std::endl;
  Student* a = alloctor.construct();
  std::cout << a << "\t" << alloctor.get_next_size() << std::endl;
  Student* b = alloctor.construct();
  alloctor.destroy(a);
  alloctor.destroy(b);

  std::cout << b << "\t" << alloctor.get_next_size() << std::endl;

  std::cout << "--------------" << std::endl;
  Student* c = alloctor.malloc();
  std::cout << c << "\t" << alloctor.get_next_size() << std::endl;
  alloctor.free(c);
  Student* d = alloctor.construct();
  std::cout << d << "\t" << alloctor.get_next_size() << std::endl;
  alloctor.destroy(d);

  return 0;
}

