#include <cassert>
#include <iostream>

class A {
public:
  A() {
    std::cout << "construct A\n";
  }

  ~A() { std::cout << "destruct A\n"; }
};

class B {
public:
  B(A &&a) {
    std::cout << "move construct B\n";
  }

  B(A &a) {
    std::cout << "construct B\n";
  }

  B& operator=(const B& other) {
    printf("=\n");
    return *this;
  }
  
  ~B() { std::cout << "destruct B\n"; }
};

void dummy(const B &b) {

}

int main() {
  A a;
  dummy(std::move(a));
  printf("dummy end\n");
  return 0;
}
