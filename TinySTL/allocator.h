#pragma once

#include <cstddef>
#include <iterator>
#include <new>
#include <utility>

namespace tiny_stl {

template <typename T>
class allocator {
 public:
  using value_type = T;
  using pointer = T*;

  template <typename U>
  struct rebind {
    using other = allocator<U>;
  };

  allocator() = default;
  template <typename U>

  allocator(const allocator<U>&) noexcept {}

  constexpr size_t max_size() const noexcept { return size_t(-1) / sizeof(T); }

  pointer allocate(size_t n) {
    if (n > this->max_size()) throw std::bad_alloc();

    return static_cast<pointer>(::operator new(n * sizeof(T), std::align_val_t(8)));
  }

  void deallocate(pointer p, size_t n) { ::operator delete(p); }
};

template <typename Alloc>
struct allocator_traits {
  using allocator_type = Alloc;

  template <typename T, typename... Args>
  static void construct(Alloc& alloc, T* ptr, Args&&... args) {
    ::new (static_cast<void*>(ptr)) T(std::forward<Args>(args)...);
  }

  template <typename T>
  static void destroy(T* p, std::false_type) {
    p->~T();
  }

  template <typename T>
  static void destroy(T p, std::true_type) {}
  

  template <typename Iterator>
  static void destroy(Iterator p) {
    destroy(p, std::is_trivially_destructible<
      typename std::iterator_traits<Iterator>::value_type>{});
  }

  template <typename Iterator>
  static void destroy(Iterator first, Iterator last) {
    for (; first != last; ++first) {
      destroy(first, std::is_trivially_destructible<
        typename std::iterator_traits<Iterator>::value_type>{});
    }
  }
};

}  // namespace tiny_stl
