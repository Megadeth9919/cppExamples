#include <iostream>

using namespace std;
class Base{
public:
    // virtual void f1(){ cout << "Base 的 f1" << endl; }
    // virtual void f2(){ cout << "Base 的 f2" << endl; }
};
class Derived : public Base{
public:
	// 不重写基类的虚函数 f1 和 f2
    // void f1(){ cout << "Derived 的 f1" << endl; }
    // void f2(){ cout << "Derived 的 f2" << endl; }
    // 派生类自己的虚函数 f3
    // virtual void f3(){ cout << "Derived 的 f3" << endl;}
};

typedef void(*func)(void);

template<typename T>
void walk(T* obj) {
  printf("\n==-------------------------------==\n");
  printf("[static]class:%s   size:%ld\n", typeid(T).name(), sizeof(T));

  // NOTE: have no vtable 
  // void *vtable = *(void**)obj;
  
  // auto rtti =  (*((std::type_info**)vtable));

  // printf("rtti->name(): %s\n", rtti->name());

  // auto offset_to_top = (*((long*)vtable - 2));

  // printf("offset_to_top: %ld\n", offset_to_top);

  // printf("vtable addr: %p\n", vtable);
  printf("==-------------------------------==\n");
}

int main() {

  Derived* a = new Derived;
  Derived* b = new Derived;
  Base* c = new Base;

  walk(a);
  walk(b);
  walk(c);    
}