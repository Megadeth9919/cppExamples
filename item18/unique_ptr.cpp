#include <cassert>
#include <iostream>
#include <memory>

class Investment {  };

class Stock: public Investment {
public:
    Stock(int a) { std::cout << "Stock created\n"; }
};

class Bond: public Investment {
public:
    Bond(int a) { std::cout << "Bond created\n"; }
};

class RealEstate: public Investment {
public:
    RealEstate(int) { std::cout << "RealEstate created\n"; }
};

void makeLogEntry(Investment* investment) {
    std::cout << "Investment logged: " << investment << std::endl;
}


template<typename... Ts> // 模板参数包
auto makeInvestment(Ts&&... params) // 模板参数包展开
{
  auto delInvmt = [](Investment* pInvestment)
	{
    std::cout << "enter destructor\n";
  	makeLogEntry(pInvestment);
  	delete pInvestment; 
	};
  
  std::unique_ptr<Investment, decltype(delInvmt)> pInv(nullptr, delInvmt);
  if (1/*a Stock object should be created*/)
  {
    pInv.reset(new Stock(std::forward<Ts>(params)...)); // 模板参数包转发
  }
  else if ( 1/* a Bond object should be created */ )   
  {     
    pInv.reset(new Bond(std::forward<Ts>(params)...));   
  }   
  else if ( 1/* a RealEstate object should be created */ )   
  {     
    pInv.reset(new RealEstate(std::forward<Ts>(params)...));   
  }   
  return pInv;  // 移动构造函数转交所有权
}

int main() {
  
  auto bond = makeInvestment(1);
  assert(bond);
  std::shared_ptr<Investment> sbond = makeInvestment(1); // 可以转换，因为隐式使用了移动构造
  return 0;
}