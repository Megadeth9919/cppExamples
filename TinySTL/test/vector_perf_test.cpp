#include <gtest/gtest.h>
#include "vector.h"
#include <vector>
#include <chrono>
#include <random>
#include <string>

namespace tiny_stl {
namespace test {

class VectorPerfTest : public ::testing::Test {
protected:
    static constexpr size_t LARGE_SIZE = 1000000;
    static constexpr size_t MEDIUM_SIZE = 10000;
    
    void SetUp() override {
        // 初始化随机数生成器
        std::random_device rd;
        gen = std::mt19937(rd());
        dis = std::uniform_int_distribution<>(1, 1000);
    }
    
    template<typename Vec>
    void fill_random(Vec& vec, size_t count) {
        for (size_t i = 0; i < count; ++i) {
            vec.push_back(dis(gen));
        }
    }
    
    std::mt19937 gen;
    std::uniform_int_distribution<> dis;
};

// 测试连续push_back性能
TEST_F(VectorPerfTest, PushBackPerf) {
    auto start = std::chrono::high_resolution_clock::now();
    tiny_stl::vector<int> tv;
    fill_random(tv, LARGE_SIZE);
    auto tiny_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start).count();
    
    start = std::chrono::high_resolution_clock::now();
    std::vector<int> sv;
    fill_random(sv, LARGE_SIZE);
    auto std_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start).count();
    
    std::cout << "PushBack Performance (ms):\n"
              << "TinySTL: " << tiny_duration << "\n"
              << "Std: " << std_duration << "\n"
              << "Ratio: " << static_cast<double>(tiny_duration)/std_duration << "\n";
    
    EXPECT_LE(tiny_duration, std_duration * 1.2); // 允许比标准库慢20%
}

// 测试插入性能
// TEST_F(VectorPerfTest, InsertPerf) {
//     tiny_stl::vector<int> tv(LARGE_SIZE);
//     std::vector<int> sv(LARGE_SIZE);
    
//     auto start = std::chrono::high_resolution_clock::now();
//     for (size_t i = 0; i < 1000; ++i) {
//         tv.insert(tv.begin() + tv.size()/2, dis(gen));
//     }
//     auto tiny_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
//         std::chrono::high_resolution_clock::now() - start).count();
    
//     start = std::chrono::high_resolution_clock::now();
//     for (size_t i = 0; i < 1000; ++i) {
//         sv.insert(sv.begin() + sv.size()/2, dis(gen));
//     }
//     auto std_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
//         std::chrono::high_resolution_clock::now() - start).count();
    
//     std::cout << "Insert Performance (ms):\n"
//               << "TinySTL: " << tiny_duration << "\n"
//               << "Std: " << std_duration << "\n"
//               << "Ratio: " << static_cast<double>(tiny_duration)/std_duration << "\n";
    
//     EXPECT_LE(tiny_duration, std_duration * 1.3); // 允许比标准库慢30%
// }

// 测试拷贝性能
TEST_F(VectorPerfTest, CopyPerf) {
    tiny_stl::vector<int> tv_orig;
    fill_random(tv_orig, MEDIUM_SIZE);
    
    auto start = std::chrono::high_resolution_clock::now();
    auto tv_copy = tv_orig;
    auto tiny_duration = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now() - start).count();
    
    std::vector<int> sv_orig;
    fill_random(sv_orig, MEDIUM_SIZE);
    
    start = std::chrono::high_resolution_clock::now();
    auto sv_copy = sv_orig;
    auto std_duration = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now() - start).count();
    
    std::cout << "Copy Performance (us):\n"
              << "TinySTL: " << tiny_duration << "\n"
              << "Std: " << std_duration << "\n"
              << "Ratio: " << static_cast<double>(tiny_duration)/std_duration << "\n";
    
    EXPECT_LE(tiny_duration, std_duration * 1.1); // 允许比标准库慢10%
}

// 测试移动性能
TEST_F(VectorPerfTest, MovePerf) {
    tiny_stl::vector<int> tv_orig;
    fill_random(tv_orig, LARGE_SIZE);
    
    auto start = std::chrono::high_resolution_clock::now();
    auto tv_moved = std::move(tv_orig);
    auto tiny_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::high_resolution_clock::now() - start).count();
    
    std::vector<int> sv_orig;
    fill_random(sv_orig, LARGE_SIZE);
    
    start = std::chrono::high_resolution_clock::now();
    auto sv_moved = std::move(sv_orig);
    auto std_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::high_resolution_clock::now() - start).count();
    
    std::cout << "Move Performance (ns):\n"
              << "TinySTL: " << tiny_duration << "\n"
              << "Std: " << std_duration << "\n"
              << "Ratio: " << static_cast<double>(tiny_duration)/std_duration << "\n";
    
    EXPECT_LE(tiny_duration, std_duration * 1.05); // 允许比标准库慢5%
}

// 测试迭代性能
TEST_F(VectorPerfTest, IterationPerf) {
    tiny_stl::vector<int> tv;
    fill_random(tv, LARGE_SIZE);
    
    volatile int sum = 0; // 防止被优化掉
    auto start = std::chrono::high_resolution_clock::now();
    for (auto it = tv.begin(); it != tv.end(); ++it) {
        sum += *it;
    }
    auto tiny_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start).count();
    
    std::vector<int> sv;
    fill_random(sv, LARGE_SIZE);
    
    sum = 0;
    start = std::chrono::high_resolution_clock::now();
    for (auto it = sv.begin(); it != sv.end(); ++it) {
        sum += *it;
    }
    auto std_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start).count();
    
    std::cout << "Iteration Performance (ms):\n"
              << "TinySTL: " << tiny_duration << "\n"
              << "Std: " << std_duration << "\n"
              << "Ratio: " << static_cast<double>(tiny_duration)/std_duration << "\n";
    
    EXPECT_LE(tiny_duration, std_duration * 1.05); // 允许比标准库慢5%
}

} // namespace test
} // namespace tiny_stl
