#include <gtest/gtest.h>
#include "unordered_map.h"
#include <string>
#include <memory>

namespace tiny_stl {
namespace test {

class UnorderedMapTest : public ::testing::Test {
protected:
    struct ComplexKey {
        std::string id;
        int version;
        
        ComplexKey(std::string i, int v) : id(std::move(i)), version(v) {}
        
        bool operator==(const ComplexKey& other) const {
            return id == other.id && version == other.version;
        }
    };
    
    struct ComplexKeyHash {
        size_t operator()(const ComplexKey& key) const {
            return std::hash<std::string>()(key.id) ^ std::hash<int>()(key.version);
        }
    };
    
    struct ComplexValue {
        std::unique_ptr<int[]> data;
        std::string name;
        
        ComplexValue() : data(new int[10]), name("default") {}
        ComplexValue(std::string n) : data(new int[10]), name(std::move(n)) {}
    };
};

//======================================================//
// basic test
//======================================================//
TEST_F(UnorderedMapTest, DefaultConstructor) {
    unordered_map<int, int> m;
    EXPECT_TRUE(m.empty());
    EXPECT_EQ(m.size(), 0);
}

TEST_F(UnorderedMapTest, InsertAndAccess) {
    unordered_map<ComplexKey, ComplexValue, ComplexKeyHash> m;
    
    // Insert with emplace
    m.emplace(ComplexKey("key1", 1), ComplexValue("value1"));
    
    // Insert with insert
    m.insert({ComplexKey("key2", 2), ComplexValue("value2")});
    
    EXPECT_EQ(m.size(), 2);
    EXPECT_EQ(m[ComplexKey("key1", 1)].name, "value1");
    EXPECT_EQ(m.at(ComplexKey("key2", 2)).name, "value2");
}

TEST_F(UnorderedMapTest, OperatorBracket) {
    unordered_map<std::string, ComplexValue> m;
    
    // Access non-existent key creates default value
    m["new_key"];
    EXPECT_EQ(m.size(), 1);
    EXPECT_EQ(m["new_key"].name, "default");
    
    // Modify existing value
    m["new_key"] = ComplexValue("modified");
    EXPECT_EQ(m["new_key"].name, "modified");
}

TEST_F(UnorderedMapTest, EraseAndClear) {
    unordered_map<ComplexKey, int, ComplexKeyHash> m;
    m.emplace(ComplexKey("a", 1), 10);
    m.emplace(ComplexKey("b", 2), 20);
    std::unordered_map<int, int > mm;
    // Erase by key
    EXPECT_EQ(m.erase(ComplexKey("a", 1)), 1);
    EXPECT_EQ(m.size(), 1);
    
    // Erase by iterator
    auto it = m.find(ComplexKey("b", 2));
    m.erase(it);
    EXPECT_TRUE(m.empty());
    
    // Clear
    m.emplace(ComplexKey("c", 3), 30);
    m.clear();
    EXPECT_TRUE(m.empty());
}

TEST_F(UnorderedMapTest, FindAndCount) {
    unordered_map<ComplexKey, std::string, ComplexKeyHash> m;
    m.emplace(ComplexKey("find", 1), "found");
    
    // Find existing key
    auto it = m.find(ComplexKey("find", 1));
    EXPECT_NE(it, m.end());
    EXPECT_EQ(it->second, "found");
    
    // Find non-existent key
    it = m.find(ComplexKey("not_found", 0));
    EXPECT_EQ(it, m.end());
    
    // Count
    EXPECT_EQ(m.count(ComplexKey("find", 1)), 1);
    EXPECT_EQ(m.count(ComplexKey("not_found", 0)), 0);
}

TEST_F(UnorderedMapTest, RehashAndBucketInterface) {
    // unordered_map<int, ComplexValue> m;
    
    // // Insert enough elements to trigger rehash
    // for(int i = 0; i < 100; ++i) {
    //     m.emplace(i, ComplexValue(std::to_string(i)));
    // }
    
    // // Check bucket interface
    // EXPECT_GT(m.bucket_count(), 0);
    // for(size_t i = 0; i < m.bucket_count(); ++i) {
    //     EXPECT_LE(m.bucket_size(i), m.max_bucket_size());
    // }
    
    // // Force rehash
    // m.rehash(200);
    // EXPECT_GE(m.bucket_count(), 200);
}

} // namespace test
} // namespace tiny_stl