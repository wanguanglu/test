/**
* @file   pool_alloc.cpp
* @author wanguanglu(mail@wanguanglu.com)
* @date   2017/08/06 19:44:27
* @brief 
*  
**/

#include <iostream>

#include <boost/pool/pool_alloc.hpp>

struct Student{
  Student() {
    std::cout << "Construct ..." << std::endl;
  }

  ~Student() {
    std::cout << "Destruct ..." << std::endl;
  }

  int id;
  int age;
};

int main(int argc, char* argv[]) {
  boost::pool_allocator<Student, boost::default_user_allocator_new_delete, 
                        boost::details::pool::default_mutex, 
                        32, 3> allocator;
  std::cout << allocator.max_size() << std::endl;
  Student* a = allocator.allocate(1);
  Student* b = allocator.allocate(1);
  std::cout << a << std::endl;
  std::cout << b << std::endl;
  Student* c = allocator.allocate(1);
  std::cout << c << std::endl;
  Student* d = allocator.allocate(1);
  std::cout << d << std::endl;

  std::cout << allocator.max_size() << std::endl;
  return 0;
}
