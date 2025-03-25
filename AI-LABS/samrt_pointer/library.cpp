#include <iostream>
#include <memory>
#include <vector>
#include <string>

class BorrowRecord; // 前向声明

class Book {
public:
    Book(const std::string& title) : title_(title) {}
    
    // TODO: 使用合适的智能指针类型声明当前借阅记录
    std::unique_ptr<BorrowRecord> current_record_;
    
    void borrow(const std::string& borrower) {
        // TODO: 创建新的unique_ptr借阅记录
        // 要求：旧记录应被自动释放
        current_record_ = std::make_unique<BorrowRecord>(borrower);
        ++borrow_count_;
    }
    
    size_t get_borrow_count() const { return borrow_count_; }
    std::string get_title() const { return title_; }
    
private:
    std::string title_;
    size_t borrow_count_ = 0;
};

class Reader;
using ReaderPtr = std::shared_ptr<Reader>;

class Reader {
public:
    Reader(const std::string& name) : name_(name) {}
    
    void add_friend(const ReaderPtr& friend_ptr) {
        // TODO: 使用weak_ptr记录好友关系
        friends_.push_back(std::weak_ptr<Reader>(friend_ptr));
    }
    
    void borrow_book(std::shared_ptr<Book> book) {
        book->borrow(name_);
        borrowed_books_.push_back(book);
    }
    
    // TODO: 实现获取好友数量方法（需处理可能失效的weak_ptr）
    size_t count_valid_friends() const {
        size_t count = 0;
        for (const auto& f : friends_) {
            // TODO: 检查weak_ptr有效性
            if (f.lock()) ++count;
        }
        return count;
    }
    
private:
    std::string name_;
    std::vector<std::shared_ptr<Book>> borrowed_books_;
    std::vector<std::weak_ptr<Reader>> friends_; // TODO: 使用合适的智能指针
};

class BorrowRecord {
public:
    BorrowRecord(const std::string& borrower) 
        : borrower_(borrower), timestamp_(time(nullptr)) {}
    
    // TODO: 实现所有权转移（unique_ptr相关）
    static std::unique_ptr<BorrowRecord> create_record(const std::string& borrower) {
        return std::make_unique<BorrowRecord>(borrower);
    }
    
private:
    std::string borrower_;
    time_t timestamp_;
};

// TODO: 实现书籍创建函数
std::shared_ptr<Book> create_book(const std::string& title) {
    return std::make_shared<Book>(title);
}