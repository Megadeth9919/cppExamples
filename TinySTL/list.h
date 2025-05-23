#pragma once
#include <iostream>
#include "allocator.h"
#include <cassert>

namespace tiny_stl {

  
template <typename T>
struct list_node {
  T data;
  list_node* prev;
  list_node* next;
};

template <typename T>
struct list_iterator {

  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = T;
  using reference = T&;
  using pointer = T*;
  using difference_type = std::ptrdiff_t;
  
  using self = list_iterator<T>;
  using node_ptr = list_node<T>*;
  node_ptr node_;

  list_iterator() = delete;
  list_iterator(node_ptr node): node_(node) {}

  self& operator++() {
    node_ = node_->next;
    return *this;
  }

  self operator++(int) {
    self tmp = *this;
    ++(*this);
    return tmp;
  }

  self& operator--() {
    node_ = node_->prev;
    return *this;
  }
  
  self operator--(int) {
    self tmp = *this;
    --(*this);
    return tmp;
  }

  reference operator*() const {
    return node_->data;
  }

  pointer operator->() const {
    return &(node_->data);
  }

  bool operator==(const self& other) const {
    return node_ == other.node_;
  }

  bool operator!=(const self& other) const {
    return node_ != other.node_;
  }
  
};

template<typename T>
struct list_const_iterator {
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = T;
  using reference = const T&;
  using pointer = const T*;
  using difference_type = std::ptrdiff_t;

  using self = list_const_iterator<T>;
  using node_ptr = list_node<T>*;
  node_ptr node_;

  list_const_iterator() = delete;
  list_const_iterator(node_ptr node): node_(node) {}

  self& operator++() {
    node_ = node_->next;
    return *this;
  }

  self operator++(int) {
    self tmp = *this;
    ++(*this);
    return tmp;
  }

  self& operator--() {
    node_ = node_->prev;
    return *this;
  }
  
  self operator--(int) {
    self tmp = *this;
    --(*this);
    return tmp;
  }

  reference operator*() const {
    return node_->data;
  }

  pointer operator->() const {
    return &(node_->data);
  }

  bool operator==(const self& other) const {
    return node_ == other.node_;
  }

  bool operator!=(const self& other) const {
    return node_ != other.node_;
  }

};

template <typename T, typename Alloc = std::allocator<T>>
class list {
  
private:
  using node = list_node<T>;
  using node_ptr = node*;
  using node_alloc_type = typename std::allocator_traits<Alloc>::template rebind_alloc<node>;

public:
  using value_type = T;
  using iterator = list_iterator<T>;
  using const_iterator = list_const_iterator<T>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using reference = T&;
  using const_reference = const T&;
  using pointer = T*;
  using const_pointer = const T*;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using allocator_type = Alloc;  
  
  node_ptr node_;
  size_type size_;
  node_alloc_type allocator_;

public:
  list() {
    init_sentinel(); 
    size_ = 0;
  }

  list(std::initializer_list<value_type> value_list) {
    init_sentinel();
    size_ = 0; // NOTE: 这里得初始化一下，不然是随机值
    for (auto value : value_list)
      push_back(std::move(value));
  }

  list(const list& other) {
    init_sentinel();
    size_ = 0;
    for (auto value : other) {
      push_back(value);
    }
  }

  list(list&& other) {
    init_sentinel();
    size_ = 0;
    swap(other);
  }

  list(size_type size, const_reference value) {
    init_sentinel();
    size_ = 0;
    for (size_type i = 0; i < size; ++i) {
      push_back(value);
    }
  }

  list(size_type size, value_type&& value) {
    init_sentinel();
    size_ = 0;
    auto value_copy = std::move(value);
    for (size_type i = 0; i < size; ++i) {
      push_back(value_copy);
    }
  }

  ~list() {
    clear();
    deinit_sentinel(); // NOTE: 必须用deinit_sentinel，因为node_->data没有被构造
  }

  list& operator=(const list& other) { // NOTE: 当调用此拷贝构造函数时，需要实现list的const版本
    if (this == &other)
      return *this;
    clear();
    for (auto value : other) {
      push_back(value);
    }
    return *this;
  }

  list& operator=(list&& other) {
    if (this == &other)
      return *this;
    swap(other);
    other.clear(); // NOTE: 确保other依旧可以使用
    return *this;
  }

  list& operator=(std::initializer_list<value_type> value_list) {
    clear();
    for (auto value : value_list)
      push_back(value);
    return *this;
  }

  iterator begin() { return iterator(node_->next); }
  iterator end() { return iterator(node_); }

  const_iterator begin() const { return const_iterator(node_->next); }
  const_iterator end() const { return const_iterator(node_); }

  iterator rbegin() { return iterator(node_->prev); }
  iterator rend() { return iterator(node_); }

  const_iterator rbegin() const { return const_iterator(node_->prev); }
  const_iterator rend() const { return const_iterator(node_); }

  void clear() {
    while (!empty()) {
      pop_front();
    }
  }

  bool empty() const { return node_->next == node_; }

  size_type size() const { return size_; }

  reference front() { return *begin(); }

  // NOTE: list的const版本会强制调用这个函数
  const_reference front() const { return *begin(); }

  reference back() { return *(--end()); }

  const_reference back() const { return *(--end()); }

  void pop_back() {
    if (empty())
      return;
    node_ptr last = node_->prev;
    delete_node(last);
    --size_;
  }

  void pop_front() {
    if (empty()) 
      return;
    node_ptr first = node_->next;
    delete_node(first);
    --size_;
  }

  template <typename... Args>
  iterator emplace(iterator pos, Args&&... value) {
    node_ptr new_node = create_node(std::forward<Args>(value)...);
    insert_node_before(pos.node_, new_node);
    ++size_;
    return iterator(new_node);
  }

  template <typename... Args>
  void emplace_back(Args&&... value) {
    node_ptr new_node = create_node(std::forward<Args>(value)...);
    insert_node_before(node_, new_node);
    ++size_;
  }

  template <typename... Args>
  void emplace_front(Args&&... value) {
    node_ptr new_node = create_node(std::forward<Args>(value)...);
    insert_node_after(node_, new_node);
    ++size_;
  }

  void push_back(const_reference value) {
    node_ptr new_node = create_node(value);
    insert_node_before(node_, new_node);
    ++size_;
  }

  void push_back(value_type&& value) {
    emplace_back(std::move(value));
  }

  void push_front(const_reference value) {
    node_ptr new_node = create_node(value);
    insert_node_after(node_, new_node);
    ++size_;
  }

  void push_front(value_type&& value) {
    emplace_front(std::move(value));
  }

  iterator insert(iterator it, const_reference value) {
    node_ptr new_node = create_node(value);
    insert_node_before(it.node_, new_node);
    ++size_;
    return iterator(new_node);
  }

  iterator insert(iterator it, value_type&& value) {
    node_ptr new_node = create_node(std::move(value));
    insert_node_before(it.node_, new_node);
    ++size_;
    return iterator(new_node);
  }

  iterator erase(iterator it) {
    if (it == end())
      return it;
    auto next_it = iterator(it.node_->next);
    delete_node(it.node_);
    --size_;
    return next_it;
  }

  void swap(list& other) {
    if (this == &other) return;
    std::swap(node_, other.node_);
    std::swap(size_, other.size_);
    std::swap(allocator_, other.allocator_);
  }

  void merge(list& other) {
    if (this == &other) return;
    if (other.empty()) return;
    if (empty()) {
      swap(other);
      return;
    }

    auto insert_other_node = [&](node_ptr pos, node_ptr& other_node) {
      node_ptr other_node_next = other_node->next;
      insert_node_before(pos, other_node);
      other_node = other_node_next;
    };

    size_type new_size = size_ + other.size_;
    node_ptr this_node = node_->next;
    node_ptr other_node = other.node_->next;
    while (this_node != node_ && other_node != other.node_) 
      if (this_node->data < other_node->data)
        this_node = this_node->next;
      else
       insert_other_node(this_node, other_node);
        
    while (other_node != other.node_) 
      insert_other_node(node_, other_node);

    // NOTE: 确保当前list的内存安全，同时确保other仍然可以使用
    other.node_->next = other.node_; 
    other.node_->prev = other.node_;
    other.size_ = 0;
    size_ = new_size; 
  }

  void merge(list&& other) { 
    // NOTE: 无法直接调用merge(list& other)
    // 因为other是右值引用，不能传递给merge(list& other)
    // 这是因为右值引用除了传递给&&，就只能传递给const&了
    if (this == &other) return;
    if (other.empty()) return;
    if (empty()) {
      swap(other);
      return;
    }

    auto insert_other_node = [&](node_ptr pos, node_ptr& other_node) {
      node_ptr other_node_next = other_node->next;
      insert_node_before(pos, other_node);
      other_node = other_node_next;
    };

    size_type new_size = size_ + other.size_;
    node_ptr this_node = node_->next;
    node_ptr other_node = other.node_->next;
    while (this_node != node_ && other_node != other.node_) 
      if (this_node->data < other_node->data)
        this_node = this_node->next;
      else
       insert_other_node(this_node, other_node);
        
    while (other_node != other.node_) 
      insert_other_node(node_, other_node);

    // NOTE: 确保当前list的内存安全，同时确保other仍然可以使用
    other.node_->next = other.node_; 
    other.node_->prev = other.node_;
    other.size_ = 0;
    size_ = new_size; 
  }

  void reverse() {
    if (empty()) return;
    node_ptr cur = node_->next;
    while (cur != node_) {
      std::swap(cur->next, cur->prev);
      cur = cur->prev;
    }
    std::swap(node_->next, node_->prev);
  }

  template<typename BinaryPredicate>
  void unique(BinaryPredicate pred) {
    if (empty()) return;
    node_ptr cur = node_->next;
    while(cur != node_) {
      node_ptr next = cur->next;
      if (pred(cur->data, next->data)) {
        delete_node(next);
        size_--;
      } else {
        cur = next;
      }
    }
  }

  void unique() { unique(std::equal_to<value_type>()); }

  void splice(iterator pos, list &other, iterator begin, iterator end) {
    if (other.empty() || begin == end) return;
    node_ptr pos_node = pos.node_;
    node_ptr first_node = begin.node_;
    node_ptr end_node = end.node_;
    node_ptr last_node = end_node->prev;
    size_type dis = std::distance(begin, end);
    assert((first_node != other.node_) && (last_node != other.node_));

    unlink_nodes(first_node, end_node);
    insert_nodes_before(pos_node, first_node, last_node);
    size_ += dis;
    other.size_ -= dis;
  }

  void splice(iterator pos, list &other, iterator it) {
    assert(it != other.end());
    // NOTE: 致命错误，++it返回的还是it本身，其并没有新建一个迭代器
    // 而且这样导致begin和end都被++被改变了
    // splice(pos, other, it, ++it); 
    // 正确做法： （std::next的参数按值传递）
    splice(pos, other, it, std::next(it));
  }

  void splice(iterator pos, list &other) {
    splice(pos, other, other.begin(), other.end());
  }

private:

template<typename ...Args>
node_ptr create_node(Args&&... args) {
  node_ptr new_node = allocator_.allocate(1);
  allocator_.construct(&(new_node->data), std::forward<Args>(args)...);
  new_node->next = nullptr;
  new_node->prev = nullptr;
  return new_node;
}

void unlink_nodes(node_ptr begin, node_ptr end) {
  node_ptr begin_prev = begin->prev; // NOTE: end是要操作的结点的最后一个结点的下一个结点
  begin_prev->next = end;
  end->prev = begin_prev;
}

void unlink_node(node_ptr node) {
  unlink_nodes(node, node->next);
}

void delete_node(node_ptr node) {
  unlink_node(node);
  allocator_.destroy(&(node->data));
  allocator_.deallocate(node, 1);
}

void insert_nodes_before(node_ptr pos, node_ptr first, node_ptr last) {
  node_ptr pos_prev = pos->prev;
  pos_prev->next = first;
  first->prev = pos_prev;
  last->next = pos;
  pos->prev = last;
}

void insert_nodes_after(node_ptr pos, node_ptr first, node_ptr last) {
  node_ptr pos_next = pos->next;
  pos->next = first;
  first->prev = pos;
  last->next = pos_next;
  pos_next->prev = last;
}

void insert_node_before(node_ptr pos, node_ptr new_node) {
  insert_nodes_before(pos, new_node, new_node);
}

void insert_node_after(node_ptr pos, node_ptr new_node) {
  insert_nodes_after(pos, new_node, new_node);
}

void init_sentinel() {
  node_ = allocator_.allocate(1);
  node_->next = node_;
  node_->prev = node_;
}

void deinit_sentinel() {
  allocator_.deallocate(node_, 1);
}

};

} // namespace tiny_stl