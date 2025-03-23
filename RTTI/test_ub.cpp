#include <iostream>
class Base {};
class Derived : public Base {
  int x = 1;
public:
  int derivedFunction() { 
    std::cout << "hello\n";
    return x; 
  }
};

int main() {
  Base* basePtr = new Base();
  Derived* derivedPtr = static_cast<Derived*>(basePtr); // 编译通过，但运行时无效
  int x = derivedPtr->derivedFunction(); // 未定义行为：basePtr实际上并不是Derived类型
  std::cout << x;
}