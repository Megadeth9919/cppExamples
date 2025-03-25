#include "MessageSystem.h"
#include <thread>
#include <iostream>
#include <sstream>

// 自定义测试断言
#define TEST_CHECK(cond) \
    do { \
        if (!(cond)) { \
            std::cerr << "FAIL: " << #cond << " (Line " << __LINE__ << ")\n"; \
            return false; \
        } \
    } while(0)

// 测试用例容器
struct TestCase {
    const char* name;
    bool (*func)();
};

// 消息生命周期测试
bool test_message_lifecycle() {
    Message m(1, 1024);
    TEST_CHECK(m.data != nullptr);
    
    Message m2 = std::move(m);
    TEST_CHECK(m.data == nullptr);
    TEST_CHECK(m2.data != nullptr);
    
    Message m3;
    m3 = std::move(m2);
    TEST_CHECK(m2.data == nullptr);
    TEST_CHECK(m3.data != nullptr);
    return true;
}

// 队列操作测试
bool test_queue_operations() {
    MessageQueue q;
    
    // 测试右值插入
    q.emplace_message(1, 1024);
    TEST_CHECK(q.size() == 1);
    
    Message m = q.extract_message();
    TEST_CHECK(m.size == 1024);
    
    // 测试左值移动
    Message tmp(2, 2048);
    q.emplace_message(std::move(tmp));
    TEST_CHECK(tmp.data == nullptr);
    return true;
}

// 完美转发测试
bool test_perfect_forwarding() {
    MessageQueue q;
    int call_count = 0;
    
    auto processor = [&](const Message& m, int factor) {
        ++call_count;
        TEST_CHECK(m.size * factor > 0);
    };
    
    q.emplace_message(1, 1024);
    q.emplace_message(2, 2048);
    
    q.process_messages(processor, 2);
    TEST_CHECK(call_count == 2);
    
    // 测试移动处理器
    auto&& moved_proc = std::move(processor);
    q.process_messages(std::move(moved_proc), 3);
    TEST_CHECK(call_count == 4);
    return true;
}

// 并发测试
bool test_concurrency() {
    MessageQueue q;
    constexpr int N = 1000;
    
    std::thread producer([&] {
        for (int i = 0; i < N; ++i) {
            q.emplace_message(i+1, 512);
        }
    });
    
    std::thread consumer([&] {
        for (int i = 0; i < N; ) {
            if (q.size() > 0) {
                auto m = q.extract_message();
                ++i;
            }
        }
    });
    
    producer.join();
    consumer.join();
    TEST_CHECK(q.size() == 0);
    return true;
}

// 运行所有测试
int main() {
  TestCase tests[] = {
      {"Message Lifecycle", test_message_lifecycle},
      {"Queue Operations", test_queue_operations},
      {"Perfect Forwarding", test_perfect_forwarding},
      {"Concurrency Test", test_concurrency}
  };

  int passed = 0;
  for (auto& tc : tests) {
      std::cout << "Running: " << tc.name << "... ";
      if (tc.func()) {
          ++passed;
          std::cout << "PASS\n";
      } else {
          std::cout << "FAIL\n";
      }
  }

  std::cout << "\nResults: " << passed << "/" << 4 << " passed\n";
  return passed == 4 ? 0 : 1;
}