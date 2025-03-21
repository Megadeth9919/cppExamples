#include <iostream>
class Base {
public:
    Base() {
        std::cout << "Base构造函数" << std::endl;
    }

    Base(int x) {
        std::cout << "Base构造函数" << x << std::endl;
    }
};
class Derived : public Base {
public:
using Base::Base;
};

int main() {
    Derived d(2); // 创建Derived对象
    return 0; // main函数结束，对象d被销毁
}