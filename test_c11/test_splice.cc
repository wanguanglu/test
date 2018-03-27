/**
* @file   test_splice.cpp
* @author wanguanglu(mail@wanguanglu.com)
* @date   2018/03/02 17:57:01
* @brief 
*  
**/

#include <algorithm>
#include <iostream>
#include <list>
#include <string>

void Print(const std::list<int>& int_list) {
  std::cout << "================" << std::endl;
  for (const int item : int_list) {
    std::cout << item << std::endl;
  }
  std::cout << "- - - - - - - - " << std::endl;
}

void Print(const std::list<std::string>& int_list) {
  std::cout << "================" << std::endl;
  for (const auto& item : int_list) {
    std::cout << item << std::endl;
  }
  std::cout << "- - - - - - - - " << std::endl;
}

int main () {
  std::list<int> mylist1, mylist2;
  std::list<int>::iterator it;
  
  // set some initial values:
  for (int i = 1; i <= 4; i++) {
    mylist1.push_back(i);      // mylist1: 1 2 3 4
  }

  for (int i=1; i<=3; i++) {
    mylist2.push_back(i*10);   // mylist2: 10 20 30
  }
  std::cout << "mylist1:" << std::endl;
  Print(mylist1);

  std::cout << "mylist2:" << std::endl;
  Print(mylist2);

  it = mylist1.begin();
  ++it;                         // points to 2
  mylist1.splice(it, mylist2);  // mylist1: 1 10 20 30 2 3 4
                                // mylist2 (empty)
                                // "it" still points to 2 (the 5th element)                                     
  std::cout << "mylist1:" << std::endl;
  Print(mylist1);
  std::cout << "mylist2:" << std::endl;
  Print(mylist2);

  mylist2.splice(mylist2.begin(), mylist1, it);
                                // mylist1: 1 10 20 30 3 4
                                // mylist2: 2
                                // "it" is now invalid.
  std::cout << "mylist1:" << std::endl;
  Print(mylist1);
  std::cout << "mylist2:" << std::endl;
  Print(mylist2);

  it = mylist1.begin();
  std::advance(it,3);                // "it" points now to 30
  mylist1.splice(mylist1.begin(), mylist1, it, mylist1.end());
                                    // mylist1: 30 3 4 1 10 20
  std::cout << "mylist1:" << std::endl;
  Print(mylist1);
  std::cout << "mylist2:" << std::endl;
  Print(mylist2);
  
  std::list<std::string> dictionary, bword;
  dictionary.push_back("any");
  dictionary.push_back("angle");
  dictionary.push_back("ajust");
  dictionary.push_back("common");
  dictionary.push_back("cannon");
  dictionary.push_back("company");
  
  bword.push_back("blue");
  bword.push_back("banana");
  bword.push_back("break");

  std::list<std::string>::iterator its = dictionary.begin();
  for (int i = 0; i < 3; i++) {
    its++;
  }
  dictionary.splice(its, bword);

  std::cout << "dictionary:" << std::endl;
  Print(dictionary);
  std::cout << "bword:" << std::endl;
  Print(bword);
  
  return 0;
}
 
