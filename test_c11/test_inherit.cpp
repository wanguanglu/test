#include <iostream>

template <typename DataType>
class A {
public:
  A() : x(100), y(200) {}
protected:
  double x;
  double y;
};

template <typename DataType>
class B : public A<DataType> {
public:
  using A<DataType>::x;
  using A<DataType>::y;

  B() : A<DataType>() {}

  void Func() { 
    std::cout << x << std::endl;
  }
};

template <typename DataType>
class C : public A<DataType> {
public:
  C() : A<DataType>() {}

  void Func() { 
    std::cout << this->x << std::endl;
  }
};

template <typename DataType>
class D : public A<DataType> {
public:
  D() : A<DataType>() {}

  void Func() { 
    std::cout << A<DataType>::x << std::endl;
  }
};


int main(int argc, char* argv[]) {
  B<int> b;
  b.Func();

  C<int> c;
  c.Func();
}
