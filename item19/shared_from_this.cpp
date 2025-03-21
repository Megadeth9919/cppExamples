#include <cassert>
#include <list>
#include <memory>
#include <iostream>

class Widget;
std::list<std::shared_ptr<Widget>> widgets;

class Widget :public std::enable_shared_from_this<Widget> // CRTP
{
public:
  template<typename... Ts>
	static std::shared_ptr<Widget> create(Ts&&... params) {
    return std::shared_ptr<Widget>(new Widget(std::forward<Ts>(params)...));
  } // 工厂函数保证已经有控制块

  void process() {
    // widgets.emplace_back(this); // 裸指针可能会有未定义行为，参照上一节
    widgets.emplace_back(shared_from_this()); // 需要已经有控制块
  }

private:
  Widget(){ std::cout << "constructor\n"; }; // 避免shared_from_this之前还没有创建控制块
};

int main() {
  
  auto a = Widget::create();
  assert(a);
  a->process();

  return 0;
}