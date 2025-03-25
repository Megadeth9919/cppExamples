#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <memory>
#include <new>
#include <type_traits>
#include <utility>

template <typename T, unsigned N>
struct SmallVectorStorage {
  alignas(T) char InlineElts[N * sizeof(T)];
};

template <typename Size_T>
class SmallVectorBase {
protected:
  void *BeginX;       // 指向当前存储空间的起始位置
  Size_T Size;        // 当前元素数量
  Size_T Capacity;    // 当前分配的总容量

  // 禁止默认构造，必须通过子类初始化存储指针
  SmallVectorBase() = delete;

  // 构造时初始化存储指针和容量计数
  explicit SmallVectorBase(void* FirstEl)
  : BeginX(FirstEl), Size(0), Capacity(0) {}
};

template <typename T>
using SmallVectorSizeType =
    std::conditional_t<sizeof(T) < 4 && sizeof(void *) >= 8, uint64_t,
                       uint32_t>;

// template <typename T, typename = void>
// class SmallVectorTemplateCommon
//     : public SmallVectorBase<SmallVectorSizeType<T>> {
//   // using Base = SmallVectorBase<SmallVectorSizeType<T>>;
//   using SmallVectorBase<T>::SmallVectorBase;
// };

// template <typename T>
// class SmallVectorTemplateBase : public SmallVectorTemplateCommon<T> {
//   using SmallVectorTemplateCommon<T>::SmallVectorTemplateCommon;
// };

template <typename T>
class SmallVectorImpl : public SmallVectorBase<SmallVectorSizeType<T>> {
protected:
  SmallVectorImpl(unsigned InlineCapacity, void* InlineStorage)
  : SmallVectorBase<SmallVectorSizeType<T>>(InlineStorage) {
    this->Capacity = InlineCapacity; // 初始容量设为内联容量
  }
public:
  using reference = T &;
  using const_reference = const T &;
  using pointer = T *;
  using const_pointer = const T *;

  reference operator[](size_t Index) {
    assert(Index < this->Size && "Index out of bounds!");
    return static_cast<T*>(this->BeginX)[Index];
  }

  // const_reference operator[](size_t Index) const {
  //   assert(Index < this->Size && "Index out of bounds!");
  //   return static_cast<const T*>(this->BeginX)[Index];
  // }

  size_t size() const { return this->Size; }
  bool empty() const { return this->Size == 0; }
  size_t capacity() const { return this->Capacity; }

  void push_back(const T &Value) {
    if (this->Size >= this->Capacity) {
      this->grow();
    }
  }
};

template <typename T, unsigned N>
class SmallVector: public SmallVectorImpl<T> , public SmallVectorStorage<T, N> {
public:
  // 初始化基类时传递内联存储指针
  SmallVector() : SmallVectorImpl<T>(N, this->InlineElts) {}
};