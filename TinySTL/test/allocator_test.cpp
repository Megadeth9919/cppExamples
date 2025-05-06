#include <gtest/gtest.h>
#include "allocator.h"
#include <memory>

namespace std {
// namespace tiny_stl {
namespace test {

class AllocatorTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 测试前的初始化
    }
    
    void TearDown() override {
        // 测试后的清理
    }
};

TEST_F(AllocatorTest, AllocateDeallocateBasic) {
    allocator<int> alloc;
    const size_t count = 10;
    
    int* ptr = alloc.allocate(count);
    ASSERT_NE(ptr, nullptr);
    
    alloc.deallocate(ptr, count);
}

TEST_F(AllocatorTest, RebindWorks) {
    allocator<int> int_alloc;
    allocator<double> double_alloc = int_alloc;  // 测试rebind构造
    
    double* ptr = double_alloc.allocate(5);
    ASSERT_NE(ptr, nullptr);
    double_alloc.deallocate(ptr, 5);
}

TEST_F(AllocatorTest, ValueTypeCorrect) {
    allocator<char> alloc;
    using value_type = typename decltype(alloc)::value_type;
    static_assert(std::is_same_v<value_type, char>, "Value type mismatch");
}

TEST_F(AllocatorTest, ConstructDestroyViaTraits) {
    tiny_stl::allocator<int> alloc;
    int* ptr = alloc.allocate(1);
    
    // 使用allocator_traits构造
    tiny_stl::allocator_traits<decltype(alloc)>::construct(alloc, ptr, 42);
    ASSERT_EQ(*ptr, 42);
    
    // 使用allocator_traits析构
    tiny_stl::allocator_traits<decltype(alloc)>::destroy(ptr);
    alloc.deallocate(ptr, 1);
}

TEST_F(AllocatorTest, ComplexTypeConstruction) {
    tiny_stl::allocator<std::string> alloc;
    std::string* ptr = alloc.allocate(1);
    
    // 多参数构造测试
    tiny_stl::allocator_traits<decltype(alloc)>::construct(
        alloc, ptr, 5, 'x');  // "xxxxx"
    ASSERT_EQ(*ptr, "xxxxx");
    
    tiny_stl::allocator_traits<decltype(alloc)>::destroy(ptr);
    alloc.deallocate(ptr, 1);
}

} // namespace test
} // namespace tiny_stl
