#include <vector>
#include <mutex>
#include <utility>

class Message {
public:
    size_t id;
    size_t size;
    float* data;

    Message() = default;

    Message(size_t id, size_t size) {
      this->id = id;
      this->size = size;
      data = new float[size];
    }

    ~Message() {
      delete [] data;
    }
    
    // 实现必要的特殊成员函数
    Message(const Message& other) {
      id = other.id;
      size = other.size;
      data = new float[size];
      std::copy(other.data, other.data + size, data);
    }

    Message& operator=(const Message& other) {
      if (data != nullptr)
       delete [] data;
      id = other.id;
      size = other.size;
      data = new float[size];
      std::copy(other.data, other.data + size, data);
      return *this;
    }

    Message(Message&& other) noexcept {
      id = other.id;
      size = other.size;
      data = other.data;
      other.data = nullptr;
    }

    Message& operator=(Message&& other) noexcept {
      if (data != nullptr)
        delete [] data;
      id = other.id;
      size = other.size;
      data = other.data;
      other.data = nullptr;
      return *this;
    }
    
    void process() const;
};

class MessageQueue {
    std::vector<Message> queue;
    mutable std::mutex mtx;
    size_t next_id = 0;
    
public:
    MessageQueue() = default;
    template<typename... Args>
    void emplace_message(Args&&... args);
    
    Message extract_message();
    size_t size() const;
    
    template<typename Processor, typename... Args>
    void process_messages(Processor&& proc, Args&&... args);
};

// 实现以下模板函数
template<typename... Args>
void MessageQueue::emplace_message(Args&&... args) {
  // 需要学生实现
  std::lock_guard<std::mutex> lock(mtx);
  queue.emplace_back(std::forward<Args>(args)...);
}

inline Message MessageQueue::extract_message() {
  next_id++;
  return std::move(queue[next_id - 1]);
}

template<typename Processor, typename... Args>
void MessageQueue::process_messages(Processor&& proc, Args&&... args) { 
  //若proc为左值，则会发生引用折叠，proc此时为左值引用
  std::lock_guard<std::mutex> lock(mtx);
  for (auto i = next_id; i < queue.size(); i++)
    proc(queue[i], std::forward<Args>(args)...);
}

inline size_t MessageQueue::size() const {
  return queue.size() - next_id;
}