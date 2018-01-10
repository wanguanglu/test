/**
* @file   test_construct.cpp
* @author wanguanglu(mail@wanguanglu.com)
* @date   2018/01/10 14:22:53
* @brief 
*  
**/

#include <iostream>

class A {
public:
    A(int x, int y) {
        std::cout << "In constructor." << std::endl;
    }

    A operator= (const A& a) {
        std::cout << "In copy." << std::endl;
    }
};

int main(int argc, char* argv[]) {
    A a = A(1, 2);

    A b = A(2, 3);
    a = b;

    return 0;
}
