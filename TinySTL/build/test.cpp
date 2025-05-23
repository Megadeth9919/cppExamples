#include <cstdlib>
#include <iostream>

class A {
  ~A() { std::cout<< "not trivial" << std::endl; }
};

class B {
  A a;
};

int main() {
  std::cout << std::boolalpha;
  std::cout << std::is_trivially_destructible<A>::value << std::endl;
  std::cout << std::is_trivially_destructible<B>::value << std::endl;
  return 0;
}