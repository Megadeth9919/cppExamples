#include "gtest/gtest.h"
#include "vector.h"
#include <vector>
#include <chrono>
#include <random>
#include <string>
#include <memory>
#include <utility>

namespace tiny_stl {
namespace test {
class VectorPerfTest : public ::testing::Test {
protected:
    static constexpr size_t LARGE_SIZE = 100000;
    static constexpr size_t MEDIUM_SIZE = 10000;
    static constexpr size_t SMALL_SIZE = 100;
    
    void SetUp() override {
        std::random_device rd;
        gen = std::mt19937(rd());
        dis = std::uniform_int_distribution<>(1, 1000);
    }
    
    std::string random_string() {
        std::string str;
        size_t len = dis(gen) % 10000 + 10;
        for(size_t i = 0; i < len; ++i) {
            str.push_back('a' + dis(gen) % 26);
        }
        return str;
    }
    
    std::mt19937 gen;
    std::uniform_int_distribution<> dis;
};

// 测试1: 字符串连续push_back
TEST_F(VectorPerfTest, StringPushBack) {
    auto start = std::chrono::high_resolution_clock::now();
    tiny_stl::vector<std::string> tv;
    for(size_t i = 0; i < MEDIUM_SIZE; ++i) {
        tv.push_back(random_string());
    }
    auto tiny_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start).count();

    start = std::chrono::high_resolution_clock::now();
    std::vector<std::string> sv;
    for(size_t i = 0; i < MEDIUM_SIZE; ++i) {
        sv.push_back(random_string());
    }
    auto std_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start).count();

    std::cout << "String PushBack Performance (ms):\n"
              << "TinySTL: " << tiny_duration << "\n"
              << "Std: " << std_duration << "\n"
              << "Ratio: " << static_cast<double>(tiny_duration)/std_duration << "\n";

    EXPECT_LE(static_cast<double>(tiny_duration)/std_duration, 1.2);
}

// 测试2: 字符串随机插入/删除
TEST_F(VectorPerfTest, StringRandomInsertErase) {
    tiny_stl::vector<std::string> tv;
    for(size_t i = 0; i < MEDIUM_SIZE; ++i) {
        tv.push_back(random_string());
    }
    std::vector<std::string> sv(tv.begin(), tv.end());
    
    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < 1000; ++i) {
        if(dis(gen) % 2) {
            tv.insert(tv.begin() + dis(gen) % tv.size(), random_string());
        } else {
            tv.erase(tv.begin() + dis(gen) % tv.size());
        }
    }
    auto tiny_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start).count();

    start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < 1000; ++i) {
        if(dis(gen) % 2) {
            sv.insert(sv.begin() + dis(gen) % sv.size(), random_string());
        } else {
            sv.erase(sv.begin() + dis(gen) % sv.size());
        }
    }
    auto std_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start).count();

    std::cout << "String Random Insert/Erase Performance (ms):\n"
              << "TinySTL: " << tiny_duration << "\n"
              << "Std: " << std_duration << "\n"
              << "Ratio: " << static_cast<double>(tiny_duration)/std_duration << "\n";
    EXPECT_LE(static_cast<double>(tiny_duration)/std_duration, 2);
}

// 测试3: 智能指针向量操作
TEST_F(VectorPerfTest, SmartPointerVector) {
    auto start = std::chrono::high_resolution_clock::now();
    tiny_stl::vector<std::shared_ptr<std::string>> tv;
    for(size_t i = 0; i < MEDIUM_SIZE; ++i) {
        tv.push_back(std::make_shared<std::string>(random_string()));
    }
    auto tiny_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start).count();
        
        start = std::chrono::high_resolution_clock::now();
        std::vector<std::shared_ptr<std::string>> sv;
        for(size_t i = 0; i < MEDIUM_SIZE; ++i) {
            sv.push_back(std::make_shared<std::string>(random_string()));
        }
        auto std_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - start).count();
            
            std::cout << "Smart Pointer Vector Performance (ms):\n"
            << "TinySTL: " << tiny_duration << "\n"
            << "Std: " << std_duration << "\n"
            << "Ratio: " << static_cast<double>(tiny_duration)/std_duration << "\n";
    EXPECT_LE(static_cast<double>(tiny_duration)/std_duration, 1.2);

}

// 测试4: 复杂对象移动语义
TEST_F(VectorPerfTest, ComplexObjectMove) {
    struct ComplexObj {
        std::string name;
        std::vector<std::pair<int, std::string>> data;
        std::unique_ptr<double[]> values;
        
        ComplexObj() : values(new double[10]) {}
        ComplexObj(const ComplexObj& other) 
        : name(other.name),
          data(other.data),
          values(new double[10]) {
        std::copy(other.values.get(), other.values.get() + 10, values.get());
        }
        ComplexObj(ComplexObj&& other) noexcept
            : name(std::move(other.name)),
              data(std::move(other.data)),
              values(std::move(other.values)) {}
    };

    auto create_temp_vec = [this](){
        tiny_stl::vector<ComplexObj> tmp;
        for(size_t i = 0; i < SMALL_SIZE; ++i) {
            ComplexObj obj;
            obj.name = random_string();
            for(int j = 0; j < 5; ++j) {
                obj.data.emplace_back(dis(gen), random_string());
            }
            tmp.push_back(std::move(obj));
        }
        return tmp;
    };

    auto create_std_vec = [this](){
        std::vector<ComplexObj> tmp;
        for(size_t i = 0; i < SMALL_SIZE; ++i) {
            ComplexObj obj;
            obj.name = random_string();
            for(int j = 0; j < 5; ++j) {
                obj.data.emplace_back(dis(gen), random_string());
            }
            tmp.push_back(std::move(obj));
        }
        return tmp;
    };

    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < 10; ++i) {
        auto v = create_temp_vec();
    }
    auto tiny_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start).count();

    start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < 10; ++i) {
        auto v = create_std_vec();
    }
    auto std_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start).count();

    std::cout << "Complex Object Move Performance (ms):\n"
                << "TinySTL: " << tiny_duration << "\n"
                << "Std: " << std_duration << "\n"
                << "Ratio: " << static_cast<double>(tiny_duration)/std_duration << "\n";
    EXPECT_LE(static_cast<double>(tiny_duration)/std_duration, 1.2);

}

// 测试5: 嵌套容器性能
TEST_F(VectorPerfTest, NestedContainer) {
    auto start = std::chrono::high_resolution_clock::now();
    tiny_stl::vector<tiny_stl::vector<std::string>> tv;
    for(size_t i = 0; i < SMALL_SIZE; ++i) {
        tiny_stl::vector<std::string> inner;
        for(size_t j = 0; j < 10; ++j) {
            inner.push_back(random_string());
        }
        tv.push_back(std::move(inner));
    }
    auto tiny_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start).count();

    start = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<std::string>> sv;
    for(size_t i = 0; i < SMALL_SIZE; ++i) {
        std::vector<std::string> inner;
        for(size_t j = 0; j < 10; ++j) {
            inner.push_back(random_string());
        }
        sv.push_back(std::move(inner));
    }
    auto std_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start).count();

    std::cout << "Nested Container Performance (ms):\n"
                << "TinySTL: " << tiny_duration << "\n"
                << "Std: " << std_duration << "\n"
                << "Ratio: " << static_cast<double>(tiny_duration)/std_duration << "\n";
    EXPECT_LE(static_cast<double>(tiny_duration)/std_duration, 1.2);
}

// 测试6: 自定义对象排序性能
TEST_F(VectorPerfTest, CustomObjectSort) {
    struct Person {
        std::string name;
        int age;
        double salary;
        
        bool operator<(const Person& other) const {
            return salary < other.salary;
        }
    };

    tiny_stl::vector<Person> tv;
    for(size_t i = 0; i < MEDIUM_SIZE; ++i) {
        tv.push_back({random_string(), dis(gen) % 80 + 20, dis(gen) % 100000 / 10.0});
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    std::sort(tv.begin(), tv.end());
    auto tiny_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start).count();

    std::vector<Person> sv(tv.begin(), tv.end());

    start = std::chrono::high_resolution_clock::now();
    std::sort(sv.begin(), sv.end());
    auto std_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start).count();

    std::cout << "Custom Object Sort Performance (ms):\n"
                << "TinySTL: " << tiny_duration << "\n"
                << "Std: " << std_duration << "\n"
                << "Ratio: " << static_cast<double>(tiny_duration)/std_duration << "\n";
    EXPECT_LE(static_cast<double>(tiny_duration)/std_duration, 2);
}

// 测试7: 字符串查找性能
TEST_F(VectorPerfTest, StringFind) {
    tiny_stl::vector<std::string> tv;
    for(size_t i = 0; i < LARGE_SIZE; ++i) {
        tv.push_back(random_string());
    }
    std::string target = tv[tv.size()/2];
    
    auto start = std::chrono::high_resolution_clock::now();
    auto it = std::find(tv.begin(), tv.end(), target);
    auto tiny_duration = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now() - start).count();

    std::vector<std::string> sv(tv.begin(), tv.end());

    start = std::chrono::high_resolution_clock::now();
    auto it_std = std::find(sv.begin(), sv.end(), target);
    auto std_duration = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now() - start).count();

    std::cout << "String Find Performance (us):\n"
                << "TinySTL: " << tiny_duration << "\n"
                << "Std: " << std_duration << "\n"
                << "Ratio: " << static_cast<double>(tiny_duration)/std_duration << "\n";
    EXPECT_LE(static_cast<double>(tiny_duration)/std_duration, 1.2);
}

// 测试8: 对象拷贝性能
TEST_F(VectorPerfTest, ObjectCopy) {
    struct Config {
        std::string name;
        std::vector<std::string> options;
        std::map<int, std::string> settings;
    };

    tiny_stl::vector<Config> tv;
    for(size_t i = 0; i < MEDIUM_SIZE; ++i) {
        Config cfg;
        cfg.name = random_string();
        for(int j = 0; j < 5; ++j) {
            cfg.options.push_back(random_string());
            cfg.settings[j] = random_string();
        }
        tv.push_back(cfg);
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    auto tv_copy = tv;
    auto tiny_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start).count();

    std::vector<Config> sv(tv.begin(), tv.end());
    start = std::chrono::high_resolution_clock::now();
    auto sv_copy = sv;
    auto std_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start).count();

    std::cout << "Object Copy Performance (ms):\n"
                << "TinySTL: " << tiny_duration << "\n"
                << "Std: " << std_duration << "\n"
                << "Ratio: " << static_cast<double>(tiny_duration)/std_duration << "\n";
    EXPECT_LE(static_cast<double>(tiny_duration)/std_duration, 1.2);
}

} // namespace test
} // namespace tiny_stl