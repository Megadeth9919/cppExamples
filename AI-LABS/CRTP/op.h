#include <iostream>
#include <memory>
#include <string>
#include <type_traits>

// ========== 基础类型系统 ==========
enum class Type { Int, Float };

std::string typeToString(Type ty) {
  switch (ty) {
  case Type::Int: return "int";
  case Type::Float: return "float";
  }
}

// ========== Op特性系统 ==========
template <typename OpT>
struct TraitBase {};  // 特性基类

template <typename OpT>
struct OneResult : TraitBase<OpT> {
  static constexpr bool hasOneResult = true;

  // SFINAE check for ResultType in OpT
  template<typename T = OpT, typename = typename T::ResultType>
  static constexpr bool hasResultType(int) { return true; }
  
  template<typename T = OpT>
  static constexpr bool hasResultType(...) { return false; }
  
  // Compile-time check
  // static_assert(hasResultType<OpT>(0),
  //              "Operations with OneResult must define ResultType");
};

template <typename OpT>
struct NoTraits : TraitBase<OpT> {};  // 空特性

// ========== Op基类 ==========
template <typename ConcreteOp, template <typename> class... Traits>
class OpBase : public Traits<ConcreteOp>... {  
public:
  using ConcreteType = ConcreteOp;

  // 访问派生类的静态方法
  static std::string getName() { return ConcreteOp::getOpName(); }

  // 创建操作实例（CRTP关键点）
  static std::unique_ptr<ConcreteOp> create() {
    return std::make_unique<ConcreteOp>();
  }

  // 类型验证转发到派生类
  static bool verify(Type input, Type output) {
    return ConcreteOp::verifyTypes(input, output);
  }

  // ==== 学生需要实现的部分 ====
  // 需要添加静态断言确保派生类实现了必要方法
};

// ========== 具体Op定义 ==========
// AddOp需要实现：
// 1. 操作名称为"add"
// 2. 验证输入类型必须相同
// 3. 支持OneResult特性
class AddOp : public OpBase<AddOp, OneResult> {
public:
  // ==== 学生需要实现的部分 ====
  using ResultType = int;

  static bool verify(Type input, Type output) {
    return input == output;
  }

  static std::string getOpName() {
    return "add";
  }

};

// MulOp需要实现：
// 1. 操作名称为"mul"
// 2. 验证输入为Float类型
// 3. 不支持任何特性
class MulOp : public OpBase<MulOp> {
public:
  // ==== 学生需要实现的部分 ====
  static bool verify(Type input, Type output) {
    return Type::Float == output;
  }

  static std::string getOpName() {
    return "mul";
  }
};

// ========== Op构造器 ==========
template <typename OpTy> class OpConstructor {
public:
  static void build() {
    std::cout << "Building op: " << OpTy::getName() << "\n";
    auto op = OpTy::create();
    if constexpr (std::is_base_of_v<OneResult<OpTy>, OpTy>) {
      std::cout << "  - Has one result\n";
    }
  }
};