#include <list>
#include <iostream>

struct A
{
  int a;
  int b;
};


int main() {

  std::list<A> l{{.a=1, .b=2}, {.a=3, .b=4}};
  for (auto it = l.begin(); it != l.end(); ++it) {
    std::cout << it->a << " " << it->b << std::endl;
  }
  l.emplace()
  l.swap(list<A> &x)
  return 0;
}