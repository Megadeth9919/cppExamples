#pragma once

#include <memory>
#include <type_traits>
#include <utility>

namespace tiny_stl {

// function主要的作用是？
// 1. 统一封装可调用对象
//  function 提供了一种统一的方式来处理不同类型的可调用对象，比如：
//  - 普通函数指针（如 func_hello）
//  - Lambda 表达式（如 [=](int i) { ... }）
//  - 函数对象（如 func_printnum_t 的实例）
// 2. 作为高阶函数的参数

template <typename FnSig>
struct function {
 static_assert(!std::is_same_v<FnSig, FnSig>, "not a valid function signature");
};

template <typename Ret, typename... Args>
struct function<Ret(Args...)> {

private:
  struct base {
    virtual Ret call(Args... args) = 0;
  };

  template <typename Fn> // base + 模板impl = 类型擦除
  struct impl : base{
    Fn f;
  
    template<typename... CArgs>
    impl(std::in_place_t, CArgs&&... args) : f(std::forward<CArgs>(args)...) {}

    Ret call(Args... args) { return f(std::forward<Args>(args)...); }
  
  };

  std::unique_ptr<base> base_;

public:
  function() = default;
  function(std::nullptr_t) noexcept : function() {}

  template <
      typename Fn,
      typename = std::enable_if_t<std::is_invocable_r_v<Ret, std::decay_t<Fn>, Args...> &&
                       std::is_copy_constructible_v<Fn>> >
  function(Fn &&f) : base_(std::make_unique<std::decay_t<impl<Fn>> >(std::in_place, std::forward<Fn>(f))) {}

  Ret operator()(Args... args) const { return base_->call(std::forward<Args>(args)...); }
};

} // namespace tiny_stl