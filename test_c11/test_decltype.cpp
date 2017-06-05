/**
* @file   test_decltype.cpp
* @author wanguanglu(mail@wanguanglu.com)
* @date   2017/06/05 20:20:23
* @brief 
*  
**/

void basic_test() {
  int x = 0;
  decltype(x) y = 0;  // type of y is int
  decltype(x + y) z = 1.0;  // type of z is int

  const int& i = x;
  decltype(i) j = y;    // type of j is const int&

  const decltype(z) * p = &z;   // *p is const int, p is const int*
  decltype(z)* pi = &z;  // *pi is int, pi is int *
//  decltype(pi)* pp = *pi;  // *pp is int*, pp is int**
}

class Foo {
public:
  static const int number = 0;
  int x;
};

void variable_test() {
  int n = 0;
  volatile const int & x = n;

  decltype(n) a = n;  // a -> int
  decltype(x) b = n;  // b -> const volatile int &

  decltype(Foo::number) c = 0;  // c -> const int

  Foo foo;
  decltype(foo.x) d = 0;  // d -> int
}

int main(int argc, char* argv[]) {
  return 0;
}
