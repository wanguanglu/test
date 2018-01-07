/**
* @file   test_struct.cpp
* @author wanguanglu(mail@wanguanglu.com)
* @date   2018/01/07 19:54:19
* @brief 
*  
**/

#include <iostream>

struct A {
    int x = -1;
    int y = -2;
};

int main(int argc, char* argv[]) {
    A a;
    std::cout << "x: " << a.x << "y: " << a.y << std::endl;

    a.x = 2;
    a.y = 3;
    std::cout << "x: " << a.x << "y: " << a.y << std::endl;

    a = A();
    std::cout << "x: " << a.x << "y: " << a.y << std::endl;
    
    return 0;
}
