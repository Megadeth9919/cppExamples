
#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <numeric>
#include <list>
#include <iterator>
#include <iomanip>

#include "list.h"
#include <gtest/gtest.h>

namespace tiny_stl {
namespace test {

class ListPerformanceTest : public ::testing::Test {
protected:
    struct TestData {
        std::vector<int> data;
        std::string description;
        double value;
        std::unique_ptr<int[]> buffer;
        
        TestData() : data(100), value(0.0), buffer(new int[100]) {}
        TestData(int size, std::string desc, double val) 
            : data(size), description(std::move(desc)), value(val), buffer(new int[size]) {}
        
        TestData(TestData&& other) noexcept
            : data(std::move(other.data)),
              description(std::move(other.description)),
              value(other.value),
              buffer(std::move(other.buffer)) {}
        
        TestData& operator=(TestData&& other) noexcept {
            data = std::move(other.data);
            description = std::move(other.description);
            value = other.value;
            buffer = std::move(other.buffer);
            return *this;
        }
        
        TestData(const TestData&) = delete;
        TestData& operator=(const TestData&) = delete;
        
        bool operator<(const TestData& other) const {
            return value < other.value;
        }
    };
    
    static constexpr int SMALL_SIZE = 1000;
    static constexpr int MEDIUM_SIZE = 10000;
    static constexpr int LARGE_SIZE = 100000;
    static constexpr double PERFORMANCE_THRESHOLD = 2.0; // tinystl允许比std慢的最大倍数
    
    // 自动选择合适的时间单位显示
    std::string formatDuration(double seconds) {
        if (seconds >= 1.0) {
            return std::to_string(seconds) + " s";
        } else if (seconds >= 1e-3) {
            return std::to_string(seconds * 1e3) + " ms";
        } else if (seconds >= 1e-6) {
            return std::to_string(seconds * 1e6) + " μs";
        } else {
            return std::to_string(seconds * 1e9) + " ns";
        }
    }
    
    // 比较性能并输出结果
    void comparePerformance(const std::string& operation, 
                          double tiny_time, 
                          double std_time) {
        std::cout << operation 
                  << " | tiny_stl: " << std::setw(12) << formatDuration(tiny_time)
                  << " | std: " << std::setw(12) << formatDuration(std_time)
                  << " | ratio: " << std::setw(6) << std::fixed << std::setprecision(2) 
                  << (tiny_time / std_time) << "x";
        
        if (tiny_time > std_time * PERFORMANCE_THRESHOLD) {
            std::cout << " [FAIL]";
            ADD_FAILURE() << operation << " performance too slow: " 
                         << (tiny_time / std_time) << "x slower than std";
        }
        std::cout << "\n";
    }
    
    template<typename ListType>
    double measureInsertPerformance(ListType& list, const std::string& position) {
        auto start = std::chrono::high_resolution_clock::now();
        
        if (position == "front") {
            for (int i = 0; i < MEDIUM_SIZE; ++i) {
                list.push_front(TestData(100, position+"_insert", i));
            }
        } 
        else if (position == "back") {
            for (int i = 0; i < MEDIUM_SIZE; ++i) {
                list.push_back(TestData(100, position+"_insert", i));
            }
        } 
        else { // middle
            auto it = list.begin();
            if (list.size() > 0) {
                std::advance(it, list.size() / 2);
            }
            for (int i = 0; i < SMALL_SIZE; ++i) {
                it = list.insert(it, TestData(100, position+"_insert", i));
            }
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double>(end - start).count();
    }
    
    template<typename ListType>
    double measureErasePerformance(ListType& list, const std::string& position) {
        auto start = std::chrono::high_resolution_clock::now();
        
        if (position == "front") {
            for (int i = 0; i < SMALL_SIZE; ++i) {
                list.pop_front();
            }
        } 
        else if (position == "back") {
            for (int i = 0; i < SMALL_SIZE; ++i) {
                list.pop_back();
            }
        } 
        else { // middle
            auto it = list.begin();
            std::advance(it, list.size() / 2 - SMALL_SIZE / 2);
            for (int i = 0; i < SMALL_SIZE; ++i) {
                it = list.erase(it);
            }
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double>(end - start).count();
    }
    
    template<typename ListType>
    double measureIterationPerformance(ListType& list, bool reverse = false) {
        auto start = std::chrono::high_resolution_clock::now();
        int sum = 0;
        
        if (!reverse) {
            for (const auto& item : list) {
                sum += item.value;
            }
        } else {
            for (auto it = list.rbegin(); it != list.rend(); ++it) {
                sum += it->value;
            }
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        // 验证结果防止被优化掉
        EXPECT_GT(sum, 0); 
        return std::chrono::duration<double>(end - start).count();
    }
    
    template<typename ListType>
    double measureMergePerformance(ListType& list1, ListType& list2) {
        auto start = std::chrono::high_resolution_clock::now();
        list1.merge(list2);
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double>(end - start).count();
    }
};

TEST_F(ListPerformanceTest, InsertPerformanceComparison) {
    std::cout << "=== Insert Performance Comparison ===\n";
    
    // Front insert
    tiny_stl::list<TestData> tiny_list1;
    double tiny_time = measureInsertPerformance(tiny_list1, "front");
    std::list<TestData> std_list1;
    double std_time = measureInsertPerformance(std_list1, "front");
    comparePerformance("Front Insert", tiny_time, std_time);
    
    // Back insert
    tiny_stl::list<TestData> tiny_list2;
    tiny_time = measureInsertPerformance(tiny_list2, "back");
    std::list<TestData> std_list2;
    std_time = measureInsertPerformance(std_list2, "back");
    comparePerformance("Back Insert", tiny_time, std_time);
    
    // Middle insert
    tiny_stl::list<TestData> tiny_list3;
    for (size_t i = 0; i < MEDIUM_SIZE/2; ++i) {
      tiny_list3.emplace_back(100, "base", 0);
    }
    tiny_time = measureInsertPerformance(tiny_list3, "middle");
    std::list<TestData> std_list3;
    for (size_t i = 0; i < MEDIUM_SIZE/2; ++i) {
        std_list3.emplace_back(100, "base", 0);
    }
    std_time = measureInsertPerformance(std_list3, "middle");
    comparePerformance("Middle Insert", tiny_time, std_time);
}

TEST_F(ListPerformanceTest, ErasePerformanceComparison) {
    std::cout << "\n=== Erase Performance Comparison ===\n";
    
    // Prepare data
    auto fillList = [](auto& list, int size) {
        for (int i = 0; i < size; ++i) {
            list.push_back(TestData(100, "to_erase", i));
        }
    };
    
    // Front erase
    tiny_stl::list<TestData> tiny_list1;
    fillList(tiny_list1, LARGE_SIZE);
    double tiny_time = measureErasePerformance(tiny_list1, "front");
    std::list<TestData> std_list1;
    fillList(std_list1, LARGE_SIZE);
    double std_time = measureErasePerformance(std_list1, "front");
    comparePerformance("Front Erase", tiny_time, std_time);
    
    // Back erase
    tiny_stl::list<TestData> tiny_list2;
    fillList(tiny_list2, LARGE_SIZE);
    tiny_time = measureErasePerformance(tiny_list2, "back");
    std::list<TestData> std_list2;
    fillList(std_list2, LARGE_SIZE);
    std_time = measureErasePerformance(std_list2, "back");
    comparePerformance("Back Erase", tiny_time, std_time);
    
    // Middle erase
    tiny_stl::list<TestData> tiny_list3;
    fillList(tiny_list3, LARGE_SIZE);
    tiny_time = measureErasePerformance(tiny_list3, "middle");
    std::list<TestData> std_list3;
    fillList(std_list3, LARGE_SIZE);
    std_time = measureErasePerformance(std_list3, "middle");
    comparePerformance("Middle Erase", tiny_time, std_time);
}

TEST_F(ListPerformanceTest, IterationPerformanceComparison) {
    std::cout << "\n=== Iteration Performance Comparison ===\n";
    
    // Prepare data
    auto fillList = [](auto& list, int size) {
        for (int i = 0; i < size; ++i) {
            list.push_back(TestData(100, "to_iterate", i));
        }
    };
    
    tiny_stl::list<TestData> tiny_list;
    fillList(tiny_list, LARGE_SIZE);
    std::list<TestData> std_list;
    fillList(std_list, LARGE_SIZE);
    
    // Forward iteration
    double tiny_time = measureIterationPerformance(tiny_list);
    double std_time = measureIterationPerformance(std_list);
    comparePerformance("Forward Iteration", tiny_time, std_time);
    
    // Reverse iteration
    tiny_time = measureIterationPerformance(tiny_list, true);
    std_time = measureIterationPerformance(std_list, true);
    comparePerformance("Reverse Iteration", tiny_time, std_time);
}

TEST_F(ListPerformanceTest, MergePerformanceComparison) {
    std::cout << "\n=== Merge Performance Comparison ===\n";
    
    // Prepare sorted data
    auto fillSortedLists = [](auto& list1, auto& list2, int size) {
        for (int i = 0; i < size; i += 2) {
            list1.push_back(TestData(100, "merge1", i));
            list2.push_back(TestData(100, "merge2", i+1));
        }
    };
    
    tiny_stl::list<TestData> tiny_list1, tiny_list2;
    fillSortedLists(tiny_list1, tiny_list2, MEDIUM_SIZE);
    double tiny_time = measureMergePerformance(tiny_list1, tiny_list2);
    
    std::list<TestData> std_list1, std_list2;
    fillSortedLists(std_list1, std_list2, MEDIUM_SIZE);
    double std_time = measureMergePerformance(std_list1, std_list2);
    
    comparePerformance("Merge", tiny_time, std_time);
    
    // Verify merged list is sorted
    double prev = -1.0;
    for (const auto& item : tiny_list1) {
        EXPECT_GT(item.value, prev);
        prev = item.value;
    }
    EXPECT_TRUE(tiny_list2.empty());
}

} // namespace test
} // namespace tiny_stl