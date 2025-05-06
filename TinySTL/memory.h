
#pragma once
#include <algorithm>
#include <utility>

namespace tiny_stl {

namespace impl {

template <typename T, typename... Args>
void construct(T* ptr, Args&&... args) {
  ::new (static_cast<void*>(ptr)) T(std::forward<Args>(args)...);
}

template <typename T, typename... Args>
void construct(T* fisrt, T* last, Args&&... args) {
  for (auto current = fisrt; current != last; ++current) {
    construct(current, std::forward<Args>(args)...);
  }
}

template <typename T, typename... Args>
void construct(T* ptr, size_t size, Args&&... args) {
  for (size_t i = 0; i < size; ++i) {
    construct(ptr + i, std::forward<Args>(args)...);
  }
}

template <typename InputIterator, typename ForwardIterator>
typename std::enable_if<
    std::is_trivially_copyable<
        typename std::iterator_traits<ForwardIterator>::value_type>::value,
    void>::type
uninitialized_copy(InputIterator first, InputIterator last,
                   ForwardIterator result) {
  std::copy(first, last, result);  // TODO: std::copy有时间可以自己实现
}

template <typename InputIterator, typename ForwardIterator>
typename std::enable_if<
    !std::is_trivially_copyable<
        typename std::iterator_traits<ForwardIterator>::value_type>::value,
    void>::type
uninitialized_copy(InputIterator first, InputIterator last,
                   ForwardIterator result) {
  auto dis = std::distance(first, last);
  construct(result, result + dis);
  std::copy(first, last, result);  // TODO: std::copy有时间可以自己实现
}

}  // namespace impl

template <typename InputIterator, typename ForwardIterator>
void uninitialized_copy(InputIterator first, InputIterator last,
                        ForwardIterator result) {
  impl::uninitialized_copy(first, last, result);
}
}  // namespace tiny_stl
