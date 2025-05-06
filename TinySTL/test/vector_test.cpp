#include <gtest/gtest.h>
#include "vector.h"
#include <string>
#include <vector>

std::vector<int> v{1, 2, 3};
namespace tiny_stl {
namespace test {

class VectorTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 测试前的初始化
    }
    
    void TearDown() override {
        // 测试后的清理
    }
};

TEST_F(VectorTest, DefaultConstructor) {
    vector<int> v;
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);
}

TEST_F(VectorTest, InitializerListConstructor) {
    vector<int> v{1, 2, 3};
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

TEST_F(VectorTest, CopyConstructor) {
    vector<int> orig{1, 2, 3};
    std::vector<int> a;
    vector<int> copy(orig);
    
    EXPECT_EQ(copy.size(), 3);
    EXPECT_EQ(copy[0], 1);
    EXPECT_EQ(copy[1], 2);
    EXPECT_EQ(copy[2], 3);
}

TEST_F(VectorTest, MoveConstructor) {
    vector<int> orig{1, 2, 3};
    vector<int> moved(std::move(orig));
    
    EXPECT_EQ(moved.size(), 3);
    EXPECT_TRUE(orig.empty());
}

TEST_F(VectorTest, PushBack) {
    std::vector<int> a;
    
    vector<int> v;
    v.push_back(1);
    v.push_back(2);
    
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
}

TEST_F(VectorTest, PopBack) {
    vector<int> v{1, 2, 3};
    v.pop_back();
    
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v.back(), 2);

    auto& back = v.back();
    back = 100;
    EXPECT_EQ(v.back(), 100); // back() 返回的是引用
}

TEST_F(VectorTest, Reserve) {
    vector<int> v;
    v.reserve(100);
    
    EXPECT_GE(v.capacity(), 100);
    EXPECT_TRUE(v.empty());
}

TEST_F(VectorTest, Resize) {
    vector<int> v{1, 2, 3};
    v.resize(5);
    
    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[3], 0); // 默认构造的值
    
    v.resize(2);
    EXPECT_EQ(v.size(), 2);
}

TEST_F(VectorTest, Insert) {
    vector<int> v{1, 3};
    auto it = v.insert(v.begin() + 1, 2);
    
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(*it, 2);
    EXPECT_EQ(v[1], 2);
}

TEST_F(VectorTest, Erase) {
    vector<int> v{1, 2, 3};
    auto it = v.erase(v.begin() + 1);
    
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(*it, 3);
    EXPECT_EQ(v[1], 3);
}

TEST_F(VectorTest, Iterator) {
    vector<int> v{1, 2, 3};
    int sum = 0;
    for (auto it = v.begin(); it != v.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 6);

    sum = 0;
    for (auto it : v) {
        sum += it;
    }
    EXPECT_EQ(sum, 6);
}

TEST_F(VectorTest, ComplexType) {
    vector<std::string> v;
    v.push_back("hello");
    v.push_back("world");
    
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], "hello");
    EXPECT_EQ(v[1], "world");
}

} // namespace test
} // namespace tiny_stl

