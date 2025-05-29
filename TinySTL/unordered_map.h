#pragma once
#include <cassert>
#include <functional>
#include <type_traits>
#include <utility>

namespace tiny_stl {

template <typename Key, typename T, typename Hash,
typename Pred, typename Alloc>
class _hashtable;



template <typename T, typename = void>
struct hashtable_value_traits {
  using key_type = T;
  using value_type = T;
  using mapped_type = T;

  static const key_type& get_key(const value_type& val) { return val; }
  static const value_type& get_value(const value_type& val) { return val; }
  static const mapped_type& get_mapped(const value_type& val) { return val; }
};

template <typename T>
// NOTE: SFINAE机制 + 模板偏特化，如果T::first_type和T::second_type存在，则匹配这个模板
struct hashtable_value_traits<T, std::void_t<typename T::first_type, typename T::second_type>> {
  using key_type = typename T::first_type;
  using value_type = T;
  using mapped_type = typename T::second_type;

  static const key_type& get_key(const value_type& val) { return val.first; }
  static const value_type& get_value(const value_type& val) { return val; }
  static const mapped_type& get_mapped(const value_type& val) { return val.second; }
};

template <typename T>
struct hashtable_node {
  using value_type = T;
  
  value_type val;
  hashtable_node* next;
};

template<typename Key, typename T, typename Hash, typename Pred, typename Alloc>
struct hashtable_iterator {
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = T;
  using pointer = T*;
  using reference = T&;

  using node_type = hashtable_node<value_type>;
  using node_ptr = node_type*;

  using hashtable_type = _hashtable<Key, value_type, Hash, Pred, Alloc>;

  node_ptr node_;
  hashtable_type* ht_;

  hashtable_iterator(node_ptr n, hashtable_type* ht) : node_(n), ht_(ht) {}

  reference operator*() const { return node_->val; }
  pointer operator->() const { return &(operator*()); }
  bool operator==(const hashtable_iterator& other) const { return node_ == other.node_; }
  bool operator!=(const hashtable_iterator& other) const { return node_ != other.node_; }
};



template <typename Key, typename T, typename Hash,
          typename Pred, typename Alloc>
class _hashtable {

public:
  using value_traits = hashtable_value_traits<T>;
  using key_type = typename value_traits::key_type;
  using value_type = typename value_traits::value_type;
  using mapped_type = typename value_traits::mapped_type;
  using hasher = Hash;
  using key_equal = Pred;
  using allocator_type = Alloc;

  using pointer = value_type*;
  using const_pointer = const value_type*;
  using reference = value_type&;
  using const_reference = const value_type&;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using iterator = hashtable_iterator<Key, T, Hash, Pred, Alloc>;
  using const_iterator = int; // preserve


  using node_type = hashtable_node<value_type>;
  using node_ptr = node_type*;
  using node_allocator = typename Alloc::template rebind<node_type>::other;
  using data_allocator = typename Alloc::template rebind<value_type>::other;
  using table_type = std::vector<node_ptr>;

private:
  table_type buckets_;
  size_type bucket_size_;
  size_type size_;
  hasher hash_;
  key_equal equal_;
  float mlf_; 
  node_allocator node_alloc_;
  data_allocator data_alloc_;

public:
  _hashtable(size_type bucket_size, const hasher& hash = hasher(), const key_equal& equal = key_equal(),
            const float& mlf = 1.0f, const allocator_type& node_alloc = allocator_type(), 
            const node_allocator& data_alloc = data_allocator())
    : bucket_size_(bucket_size),
      size_(0),
      hash_(hash),
      equal_(equal),
      mlf_(mlf),
      node_alloc_(node_alloc),
      data_alloc_(data_alloc) {
    buckets_.reserve(bucket_size_);
    buckets_.assign(bucket_size_, nullptr);
  }

  iterator begin() { 
    for (node_ptr node : buckets_)
      if (node)
        return iterator(node, this);
    return end();
  }

  iterator end() { return iterator(nullptr, this); }

  bool empty() const { return size_ == 0; }
  size_type size() const { return size_; }

  void clear() {
    for (node_ptr& node : buckets_) {
      node_ptr cur = node;
      while (cur) {
        node_ptr next = cur->next;
        destroy_node(cur);
        cur = next;
      }
      node = nullptr;
    }
    size_ = 0;
  }

  template<typename... Args>
  std::pair<iterator, bool> 
  emplace_unique(Args&&... args) {
    node_ptr node = create_node(std::forward<Args>(args)...);
    if ((float)(size_ + 1) / bucket_size_ > mlf_)
      ;// TODO: rehash(bucket_size_ * 2);
    return insert_node_unique(node);
  }

  iterator find(const key_type& key) {
    size_type index = hash(key);
    node_ptr cur = buckets_[index];
    for (; cur; cur = cur->next)
      if (equal_(value_traits::get_key(cur->val), key))
        return iterator(cur, this);
    return end();
  }

  void erase(iterator pos) { // NOTE: 因为是单向链表，所以需要遍历，找到前一个节点断链
    if (pos == end())
      return;
    node_ptr node = pos.node_;
    size_type index = hash(value_traits::get_key(node->val));
    node_ptr cur = buckets_[index];
    if (cur == node) {
      buckets_[index] = node->next;
      destroy_node(node);
      --size_;
      return;
    }
    for (; cur; cur = cur->next) {
      if (cur->next == node) {
        cur->next = node->next;
        destroy_node(node);
        --size_;
        return;
      }
    }
    assert(0), "unreachable";
  }

  size_type erase(const key_type& key) {
    size_type index = hash(key);
    node_ptr cur = buckets_[index];
    size_type erased = 0;
    while (cur) {
      node_ptr next = cur->next;
      destroy_node(cur);
      size_--;
      erased++;
      cur = next;
    }
    buckets_[index] = nullptr; // NOTE: 头节点置空表示该桶为空
    return erased;
  }

  size_type count(const key_type& key) const {
    size_type index = hash(key);
    node_ptr cur = buckets_[index];
    size_type res = 0;
    while (cur) {
      res++;
      cur = cur->next;
    }
    return res;
  }

private:
  size_type hash(const key_type& key) const {
    return hash_(key) % bucket_size_;
  }

  template<typename... Args>
  node_ptr create_node(Args&&... args) { // 由参数构造节点，next指针为nullptr
    node_ptr node = node_alloc_.allocate(1);
    data_alloc_.construct(std::addressof(node->val), std::forward<Args>(args)...);
    node->next = nullptr; // NOTE: 必须步骤，否则会出现未定义行为
    return node;
  }

  void destroy_node(node_ptr node) {
    data_alloc_.destroy(std::addressof(node->val));
    node_alloc_.deallocate(node, 1);
  }

  std::pair<iterator, bool>
  insert_node_unique(node_ptr node) { // 将结点插入到桶中，如果桶中已经有相同的结点，则不插入，返回false
    size_type index = hash(value_traits::get_key(node->val));
    node_ptr cur = buckets_[index];
    if (cur == nullptr) {
      buckets_[index] = node;
      ++size_;
      return std::make_pair(iterator(node, this), true);
    }

    for (; cur; cur = cur->next)
      if (equal_(value_traits::get_key(cur->val), value_traits::get_key(node->val)))
        return std::make_pair(iterator(cur, this), false);

    node->next = buckets_[index]; // NOTE: 头插法
    buckets_[index] = node;
    ++size_;
    return std::make_pair(iterator(node, this), true);
  }
};

template <typename Key, typename Tp, typename Hash = std::hash<Key>,
          typename Pred = std::equal_to<Key>,
          typename Alloc = std::allocator<std::pair<const Key, Tp>>>
class unordered_map {

private:
  using hashtable = _hashtable<Key, std::pair<Key, Tp>, Hash, Pred, Alloc>;
  hashtable ht_;

public:
  using key_type = typename hashtable::key_type;
  using value_type = typename hashtable::value_type;
  using mapped_type = typename hashtable::mapped_type;
  using hasher = typename hashtable::hasher;
  using key_equal = typename hashtable::key_equal;
  using allocator_type = typename hashtable::allocator_type;

  using pointer = typename hashtable::pointer;
  using const_pointer = typename hashtable::const_pointer;
  using reference = typename hashtable::reference;
  using const_reference = typename hashtable::const_reference;
  using iterator = typename hashtable::iterator;
  using const_iterator = typename hashtable::const_iterator;
  using size_type = typename hashtable::size_type;
  using difference_type = typename hashtable::difference_type;

public:
  unordered_map(): ht_(100) {}

  mapped_type& operator[](const key_type& key) {
    iterator it = ht_.find(key);
    if (it == ht_.end()) 
      return ht_.emplace_unique(key, mapped_type()).first;
    return it->second;
  }

  mapped_type& operator[](key_type&& key) {
    iterator it = ht_.find(key);
    if (it == ht_.end())
      it = ht_.emplace_unique(std::move(key), mapped_type()).first;
    return it->second;
  }

  mapped_type& at(const key_type& key) {
    iterator it = ht_.find(key);
    if (it == ht_.end())
      throw std::out_of_range("unordered_map::at");
    return it->second;
  }

  mapped_type& at(key_type&& key) {
    iterator it = ht_.find(key);
    if (it == ht_.end())
      throw std::out_of_range("unordered_map::at");
    return it->second;
  }

  iterator find(const key_type& key) {
    return ht_.find(key);
  }

  bool empty() const { return ht_.empty(); }
  size_type size() const { return ht_.size(); }

  iterator begin() { return ht_.begin(); }
  iterator end() { return ht_.end(); }
  // const_iterator begin() const { return ht_.begin(); }
  // const_iterator end() const { return ht_.end(); }

  void clear() { ht_.clear(); }
  
  template<typename... Args>
  std::pair<iterator, bool>
  emplace(Args&&... args) {
    return ht_.emplace_unique(std::forward<Args>(args)...);
  }

  std::pair<iterator, bool>
  insert(const std::pair<Key, Tp>& value) {
    return ht_.emplace_unique(value);
  }

  std::pair<iterator, bool>
  insert(std::pair<Key, Tp>&& value) {
    return ht_.emplace_unique(std::move(value));
  }

  void erase(iterator pos) {
    ht_.erase(pos);
  }

  size_type erase(const key_type& key) {
    return ht_.erase(key);
  }

  size_type count(const key_type& key) const {
    return ht_.count(key);
  }
};
} // namespace tiny_stl