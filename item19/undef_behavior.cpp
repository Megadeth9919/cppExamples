#include <memory>
#include <iostream>

class Widget { };

int main() {
  
  auto pw = new Widget; // pw是原始指针
  auto loggingDel = [](Widget* x) { 
    std::cout << "destructor\n";
    delete x;  
  };
  // 裸指针构造shared ptr
  std::shared_ptr<Widget> spw1(pw, loggingDel); // 为*pw创建控制块
  std::shared_ptr<Widget> spw2(pw, loggingDel); // 为*pw创建第二个控制块
  // 第一，避免传给std::shared_ptr构造函数原始指针。通常替代方案是使用std::make_shared，
  // 不过上面例子中，我们使用了自定义销毁器，用std::make_shared就没办法做到
  // 第二，如果你必须传给std::shared_ptr构造函数原始指针，直接传new出来的结果，不要传指针变量。

  return 0;
}

