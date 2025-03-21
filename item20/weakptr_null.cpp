#include <cassert>
#include <cstdio>
#include <memory>

class Widget {};

// 多线程场景下检查std::weak_ptr是否悬空
void way1(std::weak_ptr<Widget>& wpw) {
  std::shared_ptr<Widget> spw1 = wpw.lock();  // if wpw's expired, spw1 is null
  auto spw2 = wpw.lock(); // same as above, but uses auto
  printf("spw2 is null: %d\n", (spw2 == nullptr));
}

void way2(std::weak_ptr<Widget>& wpw) {
  std::shared_ptr<Widget> spw3(wpw);			// if wpw's expired, throw std::bad_weak_ptr
}

int main() {
  
  
  auto spw = 					// after spw is constructed
  std::make_shared<Widget>(); // the pointed-to Widget's
                // ref count(RC) is 1
                // See Item 21 for in on std::make_shared

  std::weak_ptr<Widget> wpw(spw); // wpw points to same Widget as spw. RC remains 1

  spw = nullptr; 					// RC goes to 0, and the
                  // Widget is destroyed.
                  // wpw now dangles

  way1(wpw); 
  way2(wpw);

  return 0;
}

