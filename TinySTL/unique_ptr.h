#pragma once

#include <memory>

namespace tiny_stl {
template <typename T, typename Deleter = std::default_delete<T>> // NOTE: std::default_delete可以处理数组
class unique_ptr {
private:
  T* ptr_;
  Deleter deleter_;

public:
  unique_ptr(std::nullptr_t = nullptr) noexcept : ptr_(nullptr) {}

  explicit unique_ptr(T* p) noexcept : ptr_(p) {} // NOTE: explicit 避免隐式转换, 比如 unique_ptr<int> p = new int(10);

  explicit unique_ptr(T* p, Deleter deleter) noexcept : ptr_(p), deleter_(deleter) {}

  unique_ptr(unique_ptr<T>&& other) noexcept : ptr_(other.release()) {}

  unique_ptr<T, Deleter>& operator=(unique_ptr<T, Deleter>&& other) noexcept {
    if (*this == other)
      return *this;
    if (ptr_)
      deleter_(ptr_);
    ptr_ = other.release();
    return *this;
  }

  template<typename U, typename UDeleter,
    typename = std::enable_if<std::is_convertible_v<U, T> > >
  unique_ptr(unique_ptr<U, UDeleter>&& other) noexcept : ptr_(other.release()) {}

  unique_ptr(const unique_ptr<T, Deleter>&) = delete;
  unique_ptr<T, Deleter>& operator=(const unique_ptr<T>&) = delete;

  ~unique_ptr() {
    deleter_(ptr_);
  }

  T* operator->() const noexcept {
    return ptr_;
  }

  T& operator*() const noexcept {
    return *ptr_;
  }

  bool operator!() const noexcept {
    return ptr_ == nullptr;
  }

  operator bool() const noexcept {
    return ptr_ != nullptr;
  }

  bool operator==(const unique_ptr<T, Deleter>& other) const noexcept {
    return ptr_ == other.ptr_;
  }

  void swap(unique_ptr<T, Deleter>& other) noexcept {
    std::swap(ptr_, other.ptr_);
  }

  void reset(T* p = nullptr) noexcept{
    deleter_(ptr_);
    ptr_ = p;
  }

  T* release() noexcept {
    return std::exchange(ptr_, nullptr);
  }

  T* get() const noexcept {
    return ptr_;
  }


};

// NOTE: 针对数组的特化版本，因为当T为Tp[]时，会退化为Tp*
template<typename T, typename Deleter>
class unique_ptr<T[], Deleter> : public unique_ptr<T, Deleter> {
  using unique_ptr<T, Deleter>::unique_ptr; // 继承构造函数
public:
  T& operator[](std::size_t i) const {
    return this->get()[i];
  }
};

template <typename T, typename... Args>
unique_ptr<T> make_unique(Args&&... args) {
  return unique_ptr<T>(new T(std::forward<Args>(args)...));
}

} // namespace tiny_stl