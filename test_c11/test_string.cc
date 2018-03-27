/**
* @file   test_string.cpp
* @author wanguanglu(mail@wanguanglu.com)
* @date   2017/05/17 11:36:13
* @brief 
*  
**/

#include <algorithm>
#include <functional>
#include <iostream>
#include <string>

// Trim from start (in place)
void TrimLeft(std::string* str) {
  /*
  const auto& end = std::find_if(str->begin(), str->end(),
      std::not1(std::ptr_fun<int, int>(std::isspace)));
  */
  const auto& end = std::find_if_not(str->begin(), str->end(), isspace);

  str->erase(str->begin(), end);
}

// Trim from end (in place)
void TrimRight(std::string* str) {
  const auto& start = std::find_if_not(str->rbegin(), str->rend(),
    isspace).base();

  str->erase(start, str->end());
}

// Trim from both ends (in place)
void Trim(std::string* s) {
  TrimLeft(s);
  TrimRight(s);
}

int main(int argc, char* argv[]) {
  std::string a = "    abcde\t\t";

  std::cout<<a<<"length: "<<a.length()<<std::endl;
  Trim(&a);
  std::cout<<a<<"length: "<<a.length()<<std::endl;

  return 0;
}
