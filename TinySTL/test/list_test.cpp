#include <gtest/gtest.h>
#include "list.h"
#include <list>
#include <numeric>
#include <string>
#include <memory>
#include <algorithm>

namespace tiny_stl {
namespace test {

class ListTest : public ::testing::Test {
protected:
    struct ComplexType {
        std::list<int> a;
        std::string name;
        int id;
        std::unique_ptr<int[]> data;
        
        ComplexType() : id(0), data(new int[10]) {}
        ComplexType(std::string n, int i) : name(std::move(n)), id(i), data(new int[10]) {}
        
        // 添加移动构造函数
        ComplexType(ComplexType&& other) noexcept 
            : a(std::move(other.a)),
            name(std::move(other.name)),
            id(other.id),
            data(std::move(other.data)) {}
        
        // 添加移动赋值运算符
        // NOTE: 如果不添加移动赋值运算符，那么list中对data的赋值会出错，因为data是unique_ptr
        ComplexType& operator=(ComplexType&& other) noexcept {
            a = std::move(other.a);
            name = std::move(other.name);
            id = other.id;
            data = std::move(other.data);
            return *this;
        }
        
        // 显式禁止拷贝
        ComplexType(const ComplexType&) = delete;
        ComplexType& operator=(const ComplexType&) = delete;
        
        bool operator==(const ComplexType& other) const {
            return name == other.name && id == other.id;
        }
    };
};

//======================================================//
// basic test
//======================================================//
TEST_F(ListTest, DefaultConstructor) {
    // std::allocator<tiny_stl::list_node<int> > allocator;
    // allocator.construct(1);
    list<int> l;
    EXPECT_TRUE(l.empty());
    EXPECT_EQ(l.size(), 0);
}

TEST_F(ListTest, PushPopOperations) {
    list<int> l;
    // Push back sequence
    l.push_back(1);
    l.push_back(2);
    EXPECT_EQ(l.size(), 2);
    EXPECT_EQ(l.back(), 2);
    
    // Push front sequence
    l.push_front(0);
    EXPECT_EQ(l.size(), 3);
    EXPECT_EQ(l.front(), 0);
    
    // Pop operations
    l.pop_back();
    EXPECT_EQ(l.back(), 1);
    l.pop_front();
    EXPECT_EQ(l.front(), 1);
}

TEST_F(ListTest, IteratorValidity) {
    list<int> l{1, 2, 3, 4, 5};
    int sum = 0;
    
    // Forward iteration
    for(auto it = l.begin(); it != l.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 15);
    
    // Range-based for loop
    sum = 0;
    for(int val : l) {
        sum += val;
    }
    EXPECT_EQ(sum, 15);
}

TEST_F(ListTest, InsertEraseOperations) {
    list<int> l{1, 3};
    
    // Insert in middle
    auto it = l.insert(++l.begin(), 2);
    EXPECT_EQ(*it, 2);
    EXPECT_EQ(l.size(), 3);
    
    // Erase element
    it = l.erase(it);
    EXPECT_EQ(*it, 3);
    EXPECT_EQ(l.size(), 2);
    
    // Edge case: erase last element
    it = l.erase(--l.end());
    EXPECT_EQ(it, l.end());
}

TEST_F(ListTest, ComplexTypeHandling) {
    list<ComplexType> l;
    // std::list<ComplexType> l;
    // Test emplace and move semantics
    l.emplace_back("test", 1);
    l.push_back(ComplexType("temp", 2));
    
    EXPECT_EQ(l.size(), 2);
    EXPECT_EQ(l.front().name, "test");
    EXPECT_EQ(l.back().id, 2);
}

TEST_F(ListTest, CopyMoveSemantics) {
    list<int> orig{1, 2, 3};
    
    // Copy constructor
    list<int> copy(orig);
    EXPECT_EQ(copy.size(), 3);
    EXPECT_EQ(copy.back(), 3);
    
    // Move constructor
    list<int> moved(std::move(orig));
    EXPECT_TRUE(orig.empty());
    EXPECT_EQ(moved.size(), 3);
    
    // Copy assignment
    list<int> assigned;
    assigned = copy;
    EXPECT_EQ(assigned.size(), 3);
    
    // Move assignment
    assigned = std::move(moved);
    EXPECT_TRUE(moved.empty());
}

TEST_F(ListTest, SpliceAndMerge) {
    list<int> l1{1, 3, 5};
    list<int> l2{2, 4, 6};
    
    // Merge sorted lists
    l1.merge(l2);
    EXPECT_TRUE(l2.empty());
    
    int prev = 0;
    for(int val : l1) {
        EXPECT_GT(val, prev);
        prev = val;
    }

    list<int> l3{1, 3, 5};
    list<int> l4{2, 4, 6};
    
    // Merge sorted lists
    l3.merge(std::move(l4));
    
    prev = 0;
    for(int val : l3) {
        EXPECT_GT(val, prev);
        prev = val;
    }
}

TEST_F(ListTest, LargeScaleOperations) {
    list<int> l;
    const int N = 10000;
    
    // Mass insertion
    for(int i = 0; i < N; ++i) {
        l.push_back(i);
    }
    EXPECT_EQ(l.size(), N);
    
    // Verify contents
    int expected = 0;
    for(int val : l) {
        EXPECT_EQ(val, expected++);
    }
}

TEST_F(ListTest, EmplaceOperations) {
    list<ComplexType> l;
    
    // Emplace at front and back
    l.emplace_front("first", 1);
    l.emplace_back("last", 2);
    
    EXPECT_EQ(l.size(), 2);
    EXPECT_EQ(l.front().name, "first");
    EXPECT_EQ(l.back().name, "last");
    
    // Emplace in middle
    auto it = l.emplace(++l.begin(), "middle", 3);
    EXPECT_EQ(it->name, "middle");
    EXPECT_EQ(l.size(), 3);
}

TEST_F(ListTest, SwapOperation) {
    list<int> l1{1, 2, 3};
    list<int> l2{4, 5, 6, 7};
    
    l1.swap(l2);
    
    EXPECT_EQ(l1.size(), 4);
    EXPECT_EQ(l2.size(), 3);
    EXPECT_EQ(l1.back(), 7);
    EXPECT_EQ(l2.back(), 3);
}

TEST_F(ListTest, ClearOperation) {
    list<int> l{1, 2, 3, 4, 5};
    
    EXPECT_FALSE(l.empty());
    l.clear();
    EXPECT_TRUE(l.empty());
    EXPECT_EQ(l.size(), 0);
}

TEST_F(ListTest, ReverseOperation) {
    list<int> l{1, 2, 3, 4, 5};
    
    l.reverse();
    
    int expected = 5;
    for(int val : l) {
        EXPECT_EQ(val, expected--);
    }
}

TEST_F(ListTest, UniqueOperation) {
    list<int> l{1, 1, 2, 3, 3, 3, 4, 5, 5};
    
    l.unique();
    
    EXPECT_EQ(l.size(), 5);
    int expected = 1;
    for(int val : l) {
        EXPECT_EQ(val, expected++);
    }
    // Test with predicate
    l = {1, 2, 3, 4, 5, 6};
    l.unique([](int a, int b) { return b == a + 1; });
    EXPECT_EQ(l.size(), 3); // Should remove consecutive increasing by 1
}

// TEST_F(ListTest, SortOperation) {
//     list<int> l{5, 3, 1, 4, 2};
    
//     l.sort();
    
//     int prev = 0;
//     for(int val : l) {
//         EXPECT_GT(val, prev);
//         prev = val;
//     }
    
//     // Test with custom comparator
//     l = {1, 3, 5, 2, 4};
//     l.sort(std::greater<int>());
    
//     prev = 100;
//     for(int val : l) {
//         EXPECT_LT(val, prev);
//         prev = val;
//     }
// }

TEST_F(ListTest, SpliceOperations) {
    list<int> l1{1, 2, 3};
    list<int> l2{4, 5, 6};
    
    // Splice entire list
    l1.splice(l1.end(), l2);
    EXPECT_TRUE(l2.empty());
    EXPECT_EQ(l1.size(), 6);
    
    // Splice single element
    l2 = {7, 8, 9};
    l1.splice(l1.begin(), l2, l2.begin());
    EXPECT_EQ(l1.front(), 7);
    EXPECT_EQ(l2.size(), 2);
    
    // Splice element range
    list<int> l3{10, 11, 12};
    auto first = l3.begin();
    auto last = ++l3.begin();
    l1.splice(--l1.end(), l3, first, last);
    EXPECT_EQ(*(-- --l1.end()), 10);
    EXPECT_EQ(l3.size(), 2);
}

//======================================================//
// stl algorithm test
//======================================================//
TEST_F(ListTest, STLAlgorithmFind) {
    list<int> l{1, 2, 3, 4, 5};
    
    auto it = std::find(l.begin(), l.end(), 3);
    EXPECT_NE(it, l.end());
    EXPECT_EQ(*it, 3);
    
    it = std::find(l.begin(), l.end(), 6);
    EXPECT_EQ(it, l.end());
}

TEST_F(ListTest, STLAlgorithmCount) {
    list<int> l{1, 2, 2, 3, 2, 4, 2};
    
    int count = std::count(l.begin(), l.end(), 2);
    EXPECT_EQ(count, 4);
}

TEST_F(ListTest, STLAlgorithmForEach) {
    list<int> l{1, 2, 3, 4, 5};
    int sum = 0;
    
    std::for_each(l.begin(), l.end(), [&sum](int x) { sum += x; });
    EXPECT_EQ(sum, 15);
}

TEST_F(ListTest, STLAlgorithmTransform) {
    list<int> l{1, 2, 3, 4, 5};
    list<int> result;
    
    std::transform(l.begin(), l.end(), std::back_inserter(result), 
                  [](int x) { return x * 2; });
    
    EXPECT_EQ(result.size(), 5);
    int expected = 2;
    for(int val : result) {
        EXPECT_EQ(val, expected);
        expected += 2;
    }
}

// TEST_F(ListTest, STLAlgorithmRemoveIf) {
//     list<int> l{1, 2, 3, 4, 5, 6, 7, 8, 9};
    
//     l.remove_if([](int x) { return x % 2 == 0; });
    
//     EXPECT_EQ(l.size(), 5);
//     for(int val : l) {
//         EXPECT_TRUE(val % 2 != 0);
//     }
// }

TEST_F(ListTest, STLAlgorithmCopyIf) {
    list<int> l{1, 2, 3, 4, 5, 6, 7, 8, 9};
    list<int> result;
    
    std::copy_if(l.begin(), l.end(), std::back_inserter(result),
                [](int x) { return x > 5; });
    
    EXPECT_EQ(result.size(), 4);
    int expected = 6;
    for(int val : result) {
        EXPECT_EQ(val, expected++);
    }
}

TEST_F(ListTest, STLAlgorithmAccumulate) {
    list<int> l{1, 2, 3, 4, 5};
    
    int sum = std::accumulate(l.begin(), l.end(), 0);
    EXPECT_EQ(sum, 15);
    
    int product = std::accumulate(l.begin(), l.end(), 1, std::multiplies<int>());
    EXPECT_EQ(product, 120);
}

TEST_F(ListTest, STLAlgorithmReverseCopy) {
    list<int> l{1, 2, 3, 4, 5};
    list<int> result;
    
    std::reverse_copy(l.begin(), l.end(), std::back_inserter(result));
    
    EXPECT_EQ(result.size(), 5);
    int expected = 5;
    for(int val : result) {
        EXPECT_EQ(val, expected--);
    }
}

// TEST_F(ListTest, STLAlgorithmSortWithList) {
//     list<int> l{5, 3, 1, 4, 2};
    
//     // STL sort requires random access iterators, which list doesn't have
//     // So we should test that our list works with algorithms that don't require random access
//     l.sort(); // Use list's own sort
    
//     EXPECT_TRUE(std::is_sorted(l.begin(), l.end()));
// }

TEST_F(ListTest, STLAlgorithmMaxElement) {
    list<int> l{3, 1, 4, 1, 5, 9, 2, 6};
    
    auto max_it = std::max_element(l.begin(), l.end());
    EXPECT_NE(max_it, l.end());
    EXPECT_EQ(*max_it, 9);
}

TEST_F(ListTest, STLAlgorithmAdjacentFind) {
    list<int> l{1, 2, 3, 3, 4, 5, 5, 5};
    
    auto it = std::adjacent_find(l.begin(), l.end());
    EXPECT_NE(it, l.end());
    EXPECT_EQ(*it, 3);
    
    // Test with predicate
    it = std::adjacent_find(l.begin(), l.end(), [](int a, int b) { return b == a + 1; });
    EXPECT_NE(it, l.end());
    EXPECT_EQ(*it, 1);
}

//======================================================//
// corner case test
//======================================================//
TEST_F(ListTest, BoundaryConditions) {
    // Empty list operations
    list<int> empty;
    EXPECT_NO_THROW(empty.pop_back());
    EXPECT_NO_THROW(empty.pop_front());
    EXPECT_EQ(empty.begin(), empty.end());
    
    // Single element list
    list<int> single{42};
    EXPECT_EQ(single.front(), single.back());
    single.pop_front();
    EXPECT_TRUE(single.empty());
    
    // Self-splice
    list<int> l{1, 2, 3, 4};
    l.splice(l.begin(), l, --l.end());
    EXPECT_EQ(l.front(), 4);
    EXPECT_EQ(l.back(), 3);
    
    // Self-merge
    l.merge(l);
    EXPECT_EQ(l.size(), 4);
}

TEST_F(ListTest, IteratorValidityAfterOperations) {
    list<int> l{1, 2, 3, 4, 5};
    auto it = ++l.begin();
    
    l.erase(l.begin());
    EXPECT_EQ(*it, 2); // Iterator should still be valid
    
    l.insert(it, 10);
    EXPECT_EQ(*it, 2); // Iterator should still point to same element
    
    list<int> l2{6, 7, 8};
    auto it2 = ++l2.begin();
    l.splice(l.end(), l2);
    EXPECT_EQ(*it2, 7); // Iterator should still be valid after splice
}

TEST_F(ListTest, MoveOnlyTypeOperations) {
    list<std::unique_ptr<int>> l;
    l.push_back(std::make_unique<int>(1));
    l.push_back(std::make_unique<int>(2));
    
    EXPECT_EQ(*l.front(), 1);
    EXPECT_EQ(*l.back(), 2);
    
    list<std::unique_ptr<int>> l2;
    l2 = std::move(l);
    EXPECT_TRUE(l.empty());
    EXPECT_EQ(*l2.front(), 1);
    
    auto it = l2.emplace(l2.begin(), std::make_unique<int>(0));
    EXPECT_EQ(**it, 0);
}

} // namespace test
} // namespace tiny_stl