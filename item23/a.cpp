#include <cassert>
#include <iostream>
#include <memory>
#include <vector>


// 使用 make系列 + 大括号初始化的方法 
int main() {
  
  auto upv = std::make_unique<std::vector<int>>(10, 20);
  // auto spv = std::make_shared<std::vector<int>>({10, 20}); // 报错，不能完美转发

  // create std::initializer_list
  auto initList = { 10, 20 };
  // create std::vector using std::initializer_list ctor
  auto spv1 = std::make_shared<std::vector<int>>(initList);


  for (auto i : *upv) {
    std::cout << i << " ";
  }
  std::cout << std::endl;


  std::cout << std::endl;

  for (auto i : *spv1) {
    std::cout << i << " ";
  }
  std::cout << std::endl;
  return 0;
}