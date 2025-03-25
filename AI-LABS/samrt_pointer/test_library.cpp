#include "library.cpp"
#include <iostream>
#include <cassert>

class TestRunner {
public:
    template<typename Func>
    void run_test(Func test, const std::string& name) {
        try {
            test();
            std::cout << "[PASS] " << name << "\n";
            ++passed;
        } catch (const std::exception& e) {
            std::cout << "[FAIL] " << name << " - " << e.what() << "\n";
            ++failed;
        }
        ++total;
    }

    void report() const {
        std::cout << "\nTest Results:\n"
                  << "Total:  " << total << "\n"
                  << "Passed: " << passed << "\n"
                  << "Failed: " << failed << "\n";
    }

    int total = 0;
    int passed = 0;
    int failed = 0;
};

// 自定义断言宏
#define ASSERT_TRUE(expr) \
    if (!(expr)) throw std::runtime_error("Assertion failed: " #expr " is false")
#define ASSERT_EQ(a, b) \
    if ((a) != (b)) throw std::runtime_error("Assertion failed: " #a " != " #b)

// 测试用例实现
void test_unique_ptr_transfer() {
    auto record1 = BorrowRecord::create_record("Alice");
    ASSERT_TRUE(record1);
    
    auto record2 = BorrowRecord::create_record("Bob");
    record1 = std::move(record2);
    ASSERT_TRUE(!record2);  // 转移后应为空
}

void test_shared_book_ownership() {
    auto book = create_book("Modern C++");
    auto reader1 = std::make_shared<Reader>("Alice");
    auto reader2 = std::make_shared<Reader>("Bob");
    
    reader1->borrow_book(book);
    reader2->borrow_book(book);
    
    ASSERT_EQ(book.use_count(), 3);
    ASSERT_EQ(book->get_borrow_count(), 2);
}

void test_weak_ptr_cycle_break() {
    auto alice = std::make_shared<Reader>("Alice");
    auto bob = std::make_shared<Reader>("Bob");
    
    alice->add_friend(bob);
    bob->add_friend(alice);
    
    ASSERT_EQ(alice->count_valid_friends(), 1);
    
    // 验证弱引用不影响对象生命周期
    auto alice_weak = std::weak_ptr(alice);
    auto bob_weak = std::weak_ptr(bob);
    
    alice.reset();
    bob.reset();
    
    ASSERT_TRUE(alice_weak.expired());
    ASSERT_TRUE(bob_weak.expired());
}

void test_record_auto_cleanup() {
    auto book = create_book("Clean Code");
    {
        auto reader = std::make_shared<Reader>("Charlie");
        reader->borrow_book(book);
        ASSERT_EQ(book->get_borrow_count(), 1);
    }  // 作用域结束自动释放
    
    // 再次借阅应创建新记录
    book->borrow("David");
    ASSERT_EQ(book->get_borrow_count(), 2);
}

int main() {
    TestRunner runner;
    
    runner.run_test(test_unique_ptr_transfer, "UniquePtr所有权转移");
    runner.run_test(test_shared_book_ownership, "共享书籍所有权");
    runner.run_test(test_weak_ptr_cycle_break, "弱引用打破循环");
    runner.run_test(test_record_auto_cleanup, "资源自动释放");
    
    runner.report();
    return runner.failed > 0 ? 1 : 0;
}