
#include <list>
#include <unordered_map>
using namespace std;

class LRUCache {
public:
  LRUCache(int capacity) {
      cap = capacity;
  }

  int get(int key) {
      auto it = map.find(key);
      if (it == map.end()) return -1;
      cache.splice(cache.begin(), cache, it->second);
      return it->second->second;
  }
  
  void put(int key, int value) {
      auto it = map.find(key);
      if (it != map.end()) cache.erase(it->second);
      cache.push_front(make_pair(key, value));
      map[key] = cache.begin();
      if (cache.size() > cap) {
          int k = cache.back().first;
          cache.pop_back();
          map.erase(k);
      }
  }

private:
    int cap;
    list<pair<int, int>> cache; // 缓存双向链表
    unordered_map<int, list<pair<int, int>>::iterator> map; // 缓存键值对应的迭代器
};

int main() {
   LRUCache cache(2);
   cache.put(1, 1);
   cache.put(2, 2);
   cache.get(1);       // 返回  1
   cache.put(3, 3);    // 该操作会使得密钥 2 作废
   cache.get(2);       // 返回 -1 (未找到)
   cache.put(4, 4);    // 该操作会使得密钥 1 作废
   cache.get(1);       // 返回 -1 (未找到)
   cache.get(3);       // 返回  3
   cache.get(4);       // 返回  4
   return 0;
}