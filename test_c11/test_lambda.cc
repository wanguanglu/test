/**
* @file   test_lambda.cpp
* @author wanguanglu(mail@wanguanglu.com)
* @date   2017/10/11 10:17:30
* @brief 
*  
**/

#include <functional>
#include <iostream>

void test() {
  auto func1 = [](int i) { return i + 4; };
  std::cout << "func1(4):" << func1(4) << std::endl;

  int var = 5;
  auto func2 = [&var] {
    var += 5;
    return var + 5;
  };

  std::cout << "func2(4):" << func2() << std::endl;
  std::cout << "var: " << var << std::endl;

  // like all callable objects, closures can be captured in std::function
  // (this may incur unnecessary overhead)
  std::function<int(int)> func3 = [](int i) { return i + 4; };
  std::cout << "func3: " << func3(6) << '\n'; 
}

void test_capture() {
  int a = 0;
  int b = 1;

  auto func1 = [a]() {
    std::cout << a << std::endl;
    // a += 1; // Compile error.
    // std::cout << b << std::endl;  // Compile error.
  };
  func1();
  std::cout << "After call func1(): " << a << std::endl;

  auto func2 = [&a]() {
    std::cout << a << std::endl;
    a += 1;
  };
  func2();
  std::cout << "After call func2(): " << a << std::endl;

  auto func3 = [=] () {
    std::cout << a << std::endl;
    // a += 1; // Compile error.
    std::cout << b << std::endl;
  };
  func3();

  auto func4 = [&] () {
    std::cout << a << std::endl;
    a += 1;
    std::cout << b << std::endl;
    b += 1;
  };
  func4();
  std::cout << "After call func4(): " << a << std::endl;
  std::cout << "After call func4(): " << b << std::endl;

  int c = 0;
  auto func5 = [=, &c] {
    std::cout << a << std::endl;
    // a += 1; // Compile error.
    std::cout << b << std::endl;
    std::cout << c << std::endl;
    c += 1;
  };
  func5();

  auto func6 = [&, c] {
    std::cout << a << std::endl;
    a += 1;
    std::cout << b << std::endl;
    std::cout << c << std::endl;
    // c += 1;  // Compile error.
  };
  func6();
}

class A {
public:
  void TestLambda() {
    auto func1 = [this] {
      std::cout << x << std::endl;
      std::cout << y << std::endl;
    };
    func1();

    int z = 0;
    auto func2 = [this, z] {
      std::cout << x << std::endl;
      std::cout << y << std::endl;
      std::cout << z << std::endl;
    };
    func2();
  }
private:
  int x = 0;
  int y = 0;
};

void test_this() {
  A a;
  a.TestLambda();
}

int main(int argc, char* argv[]) {
  // test();
  // test_capture();
  test_this();
  return 0;
}
