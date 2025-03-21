#include <iostream>

using namespace std;
class Base1{
public:
    virtual void f1(){ cout << "Base1 的 f1" << endl; }
    virtual void f2(){ cout << "Base1 的 f2" << endl; }
};
class Base2{
public:
    virtual void f3(){ cout << "Base2 的 f3" << endl; }
};

class Derived : public Base1, Base2{
public:
    void f1(){ cout << "Derived 的 f1" << endl; }
    void f2(){ cout << "Derived 的 f2" << endl; }
    // 派生类自己的虚函数 f3
    // void f3(){ cout << "Derived 的 f3" << endl;}
};

typedef void(*func)(void);


template<typename T>
void walk(T* obj, size_t offset) {
  printf("\n==-------------------------------==\n");
  printf("[static]class:%s   size:%ld\n", typeid(T).name(), sizeof(T));
  auto *vtable = *(void**)((long*)obj + offset);
  
  printf("vtable addr: %p\n", vtable);
  func f1 = (func)(*(func*)vtable);
  if (f1) f1();

  func f2 = (func)(*((long*)vtable + 1));
  if (offset == 0) f2();

  // func f3 = (func)(*((long*)vtable + 2));
  // if (offset == 0) f3();

  auto rtti =  (*((std::type_info**)vtable - 1));

  // printf("%s\n", typeid(*a).name());
  printf("rtti->name(): %s\n", rtti->name());

  auto offset_to_top = (*((long*)vtable - 2));

  printf("offset_to_top: %ld\n", offset_to_top);

  printf("\n==-------------------------------==\n");
}

int main() {

  Derived* a = new Derived;

  walk(a, 0);
  walk(a, 1);
}