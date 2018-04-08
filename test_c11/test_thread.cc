/**
* @file   test_thread.cpp
* @author wanguanglu(mail@wanguanglu.com)
* @date   2017/10/11 11:18:28
* @brief 
*  
**/

#include <chrono>
#include <future>
#include <iostream>
#include <thread>

void func3() {
  std::cout << "In func3." << std::endl;
}

void func4(int y, int z) {
  std::cout << "In func4 " << "y: " << y << " z: " << z << std::endl;
}

class C {
public:
    virtual void func1() = 0;
};

class B : public C {
public:
    void func1() override {
        std::cout << "In class B func1." << std::endl;
    }

    int func2() {
        return 0;
    }
};

class A {
public:
  void TestThread() {
    std::thread t1(&A::func1, this);
    t1.join();

    std::thread t2(&A::func2, this, 2, 3);
    t2.join();

    std::thread t3(&func3);
    t3.join();

    std::thread t4(&func4, 2, 3);
    t4.join();

    B b;
    std::thread t5(&B::func1, &b);
    t5.join();

    // std::packaged_task<int()> task(&B::func2);
    // std::future<int> ret = task.get_future();
    // std::thread t6(std::move(task), &b);
    // int value = ret.get();
    // std::cout << "value: " << value << std::endl;
  }

  void func1(){
    std::cout << "In func1." << std::endl;
  }

  void func2(int y, int z) {
    std::cout << "In func2." << std::endl;
    std::cout << "x: " << x << " y: " << y << " z: " << z << std::endl;
  }

private:
  int x = 0;
};

void Test() {
  A a;
  a.TestThread();
}

int main(int argc, char* argv[]) {
  Test();
  return 0;
}
