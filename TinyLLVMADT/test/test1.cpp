#include "smallVector.h"

#include <cassert>
#include "smallVector.h"

int main() {
    SmallVector<int, 4> vec;
    
    // 测试push_back
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    assert(vec.size() == 3);
    assert(vec[0] == 1);
    assert(vec[1] == 2);
    assert(vec[2] == 3);
    
    // 测试front/back
    assert(vec.front() == 1);
    assert(vec.back() == 3);
    
    // 测试pop_back
    vec.pop_back();
    assert(vec.size() == 2);
    assert(vec.back() == 2);
    
    // 测试clear
    vec.clear();
    assert(vec.empty());
    
    return 0;
}