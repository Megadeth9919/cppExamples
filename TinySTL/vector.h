#pragma once
#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>

#include "allocator.h"
#include "memory.h"

namespace tiny_stl {

template <typename T, typename Alloc = allocator<T>>
class vector {
  using value_type = T;
  using pointer = T *;
  using iterator = pointer;
  using reference = T &;
  using const_reference = const T &;
  using rvalue_reference = T &&;
  using alloc_traits = allocator_traits<Alloc>;

 private:
  // NOTE: 这里维护size好还是维护指针好？
  size_t size_ = 0;
  size_t capacity_ = 0;
  pointer data_ = nullptr;

  Alloc allocator_;

 public:
  vector() : size_(0), capacity_(0), data_(nullptr) {}
  ~vector() {
    if (data_) {
      alloc_traits::destroy(begin(), end());
      allocator_.deallocate(data_, capacity_);
    }
  }

  vector(size_t size)
      : size_(size), capacity_(size), data_(allocator_.allocate(size)) {}

  // NOTE: 当对象赋值时有两种情况:
  // 1. Foo foo1 = foo; 这种情况（初始化）会直接调用拷贝构造函数
  // 2. Foo foo1; foo1 = foo; 这种情况会先调用构造函数，然后再调用拷贝赋值函数
  vector(const vector &other)
    : size_(other.size_), 
      capacity_(other.capacity_),
      allocator_(other.get_allocator()),
      data_(allocator_.allocate(capacity_)) {
    std::uninitialized_copy(other.data_, other.data_ + size_, data_);
  }

  vector(vector &&other) 
    : size_(other.size_), 
      capacity_(other.capacity_),
      allocator_(other.get_allocator()),
      data_(other.data_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  vector(std::initializer_list<value_type> init)
    : size_(init.size()),
      capacity_(init.size()),
      data_(allocator_.allocate(capacity_)) {
    std::copy(init.begin(), init.end(), data_);
  }

  iterator begin() { return data_; }
  iterator end() { return data_ + size_; }

  Alloc get_allocator() const { return allocator_; }
  
  size_t size() const { return size_; }
  
  size_t capacity() const { return capacity_; }
  
  bool empty() const { return size_ == 0; }
  
  reference operator[](size_t index) { return *(begin() + index); }

  reference operator=(const vector &other) {
    if (this != &other) {
      if (data_) {
        alloc_traits::destroy(begin(), end());
        allocator_.deallocate(data_, capacity_);
      }
      size_ = other.size_;
      capacity_ = other.capacity_;
      allocator_ = other.get_allocator();
      data_ = allocator_.allocate(capacity_);
      std::uninitialized_copy(other.data_, other.data_ + size_, data_);
    }
    return *this;
  }
  
  iterator insert(iterator pos, const_reference value) {
    if (size_ == capacity_) { 
      // FIXME: 这里将产生两次拷贝，可以优化; 
      // 更新：因为一般情况下扩容的次数比较有限，所以这里的性能影响暂时忽略
      size_t dis = pos - begin();
      expand();
      pos = begin() + dis; // NOTE: 这里的pos需要变一下，因为扩容后，数据存放的位置变了
      std::cout << "expand" << std::endl;
    }
    if (pos != end()) {
      alloc_traits::construct(allocator_,end()); // NOTE: 这里是end()，而不是end() + 1 😂
      std::move_backward(pos, end(), end() + 1); // NOTE: 优化: copy_backward -> move_backward
    }
    // WHY: 对于string对象，这里为什么需要使用construct拷贝构造对象而不是直接赋值？
    // 答：因为pos不一定已经构造
    alloc_traits::construct(allocator_, pos, value); 
    // *pos = value; 
    ++size_;
    return pos;
  }

  iterator erase(iterator pos) {
    assert(pos >= begin() && pos < end());
    alloc_traits::destroy(pos);
    if (pos != end() - 1) {
      std::uninitialized_move(pos + 1, end(), pos);
      alloc_traits::destroy(end() - 1);
    }
    --size_;
    return pos;
  }

  reference back() {
    assert(size_ > 0);
    return *(end() - 1);
  }

  void resize(size_t new_size, value_type& value) {
    if (new_size > capacity_) {
      reallocate(new_size);
      std::fill(end(), data_ + new_size, value);
    }
    size_ = new_size;
  }

  void resize(size_t new_size) {
    if (new_size > capacity_) {
      reallocate(new_size);
      std::fill(end(), data_ + new_size, 0);
    }
    size_ = new_size;
  }

  void resize(size_t new_size, value_type value) {
    if (new_size > capacity_) {
      reallocate(new_size);
      std::fill(end(), data_ + new_size, value);
    }
    size_ = new_size;
  }

  void reserve(size_t new_capacity) {
    if (new_capacity > capacity_)
      reallocate(new_capacity);
  }

  void emplace_back(rvalue_reference value) {
    if (size_ == capacity_) 
      expand();
    alloc_traits::construct(allocator_, end(), std::move(value));
    ++size_;
  }

  void push_back(rvalue_reference value) {
    emplace_back(std::move(value));
  }
  
  void push_back(const_reference value) {
    if (size_ == capacity_) 
      expand();
    insert(end(), value);
  }

  value_type pop_back() {
    assert(size_ > 0);
    value_type value = back();
    alloc_traits::destroy(end() - 1);
    size_--;
    return value;
  }

 private:

  void reallocate(size_t new_capacity) {
    iterator new_data = allocator_.allocate(new_capacity);
    std::uninitialized_move(begin(), end(), new_data);
    // alloc_traits::destroy(begin(), end()); // NOTE: 优化: uninitialized_copy + destroy -> uninitialized_move
    allocator_.deallocate(data_, capacity_);
    data_ = new_data;
    capacity_ = new_capacity;
  }

  void expand() {
    if (capacity_ == 0) {
      capacity_ = 1;
      data_ = allocator_.allocate(capacity_);
      return;
    }
    size_t new_capacity = capacity_ * 2;
    reallocate(new_capacity);
  }
};

}  // namespace tiny_stl