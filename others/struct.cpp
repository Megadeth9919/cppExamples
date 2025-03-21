#include <iostream>
struct A{
  int a;
  int b;
};

int main() {

  A a;
  a.a = 1;
  std::cout << a.a;
  return 0;
}