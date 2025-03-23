#include <iostream>

using namespace std;
class Base{
public:
    virtual void f1(){ cout << "Base 的 f1" << endl; }
    virtual void f2(){ cout << "Base 的 f2" << endl; }
};
class Derived : public Base{
public:
	// 不重写基类的虚函数 f1 和 f2
    void f1(){ cout << "Derived 的 f1" << endl; }
    void f2(){ cout << "Derived 的 f2" << endl; }
    // 派生类自己的虚函数 f3
    virtual void f3(){ cout << "Derived 的 f3" << endl;}
};

// 定义func为 void _(void)函数
typedef void(*func)(void);


template<typename T>
void walk(T* obj) {
  printf("\n==-------------------------------==\n");
  printf("[static]class:%s   size:%ld\n", typeid(T).name(), sizeof(T));

  // 取对象的前8个字节，即虚表指针
  void *vtable = *(void**)obj;
  
  // 虚表的第一个元素-类中的第一个虚函数的指针
  func f1 = (func)(*(func*)vtable);
  if (f1) f1();

  func f2 = (func)(*((long*)vtable + 1));
  if (f2) f2();

  func f3 = (func)(*((long*)vtable + 2));
  if (f3 && dynamic_cast<Derived*>(obj)) f3();

  // 虚表的前面是RTTI information
  auto rtti =  (*((std::type_info**)vtable - 1));

  // printf("%s\n", typeid(*a).name());
  printf("rtti->name(): %s\n", rtti->name());

  // RTTI information的前面是offset_to_top
  auto offset_to_top = (*((long*)vtable - 2));

  printf("offset_to_top: %ld\n", offset_to_top);

  printf("vtable addr: %p\n", vtable);
  printf("\n==-------------------------------==\n");
}

int main() {

  Derived* a = new Derived;
  Derived* b = new Derived;
  Base* c = new Base;

  walk(a);
  walk(b);
  walk(c);    
}