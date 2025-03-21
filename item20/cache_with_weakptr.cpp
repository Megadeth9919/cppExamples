#include <memory>
#include <iostream>
#include <unordered_map>

using WidgetID = int;
class Widget { 
public:
  Widget(WidgetID id) {}

  static std::unique_ptr<const Widget> loadWidget(WidgetID id) { 
    printf("loadWidget %d\n", id);
    return std::make_unique<Widget>(id);
  }

  // loadWidget时间开销很大情况下的缓存
  static std::shared_ptr<const Widget> fastLoadWidget(WidgetID id)
  {
    // NOTE: weak_ptr在此处的作用是使cache不持有Widget，而是观察对象是否存在
    static std::unordered_map<WidgetID,
                  std::weak_ptr<const Widget>> cache; 

    auto objPtr = cache[id].lock(); 	// objPtr is std::shared_ptr to cached object (or null if object's not in cache)
    if (!objPtr) { 						// if not in cache
      objPtr = loadWidget(id); 		// load it 
      cache[id] = objPtr; 			// cache it 
    }
    return objPtr;
  }
};



int main() {
  
  {
    auto sp0 = Widget::fastLoadWidget(0);
    auto sp1 = Widget::fastLoadWidget(0);
    auto sp2 = Widget::fastLoadWidget(0);
    printf("%ld\n", sp0.use_count());
  } // sp0 生命周期结束
  Widget::fastLoadWidget(0);
  return 0;
}

