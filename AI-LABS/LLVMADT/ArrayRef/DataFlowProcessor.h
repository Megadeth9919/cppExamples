#include <iostream>
#include <llvm/ADT/ArrayRef.h>
#include <stdexcept>
#include <string>
#include <vector>

class DataFlowProcessor {
public:
  enum DataType { INT, FLOAT, STRING };

  // 构造时指定数据类型，后续操作必须类型匹配
  explicit DataFlowProcessor(DataType type) : type_(type) {}

  // 添加数据，需要类型检查
  template <typename T> void addData(llvm::ArrayRef<T> data) { 
    typeCheck<T>();
    /* 学生实现 */
    if constexpr (std::is_same<T, int>::value) {
      intData_.insert(intData_.end(), data.begin(), data.end());
    } else if constexpr (std::is_same<T, float>::value) {
      floatData_.insert(floatData_.end(), data.begin(), data.end());
    } else if constexpr (std::is_same<T, std::string>::value) {
      stringData_.insert(stringData_.end(), data.begin(), data.end());
    }
  }

  // 获取当前数据视图（必须返回ArrayRef）
  template <typename T> llvm::ArrayRef<T> getDataView() const {
    typeCheck<T>();
    /* 学生实现 */ 
    if constexpr (std::is_same<T, int>::value) {
      return llvm::ArrayRef<T>(intData_);
    } else if constexpr (std::is_same<T, float>::value) {
      return llvm::ArrayRef<T>(floatData_);
    } else if constexpr (std::is_same<T, std::string>::value) {
      return llvm::ArrayRef<T>(stringData_);
    }
  }

  // 获取指定范围的子数据流（需处理边界）
  template <typename T>
  llvm::ArrayRef<T> getSubrange(size_t start, size_t length) const {
    /* 学生实现 */
    typeCheck<T>();
    auto full = getDataView<T>();
    if (start >= full.size()) {
      throw std::out_of_range("Start index out of range");
    }
    return full.slice(start, std::min(length, full.size() - start));
  }

  // 合并两个数据流（使用ArrayRef避免拷贝）
  // static DataFlowProcessor merge(const DataFlowProcessor &a,
  //                                const DataFlowProcessor &b) {
  //   /* 学生实现 */
    
  // }

private:
  DataType type_;
  std::vector<int> intData_;
  std::vector<float> floatData_;
  std::vector<std::string> stringData_;

  // 类型检查工具函数
  template <typename T> void typeCheck() const { /* 学生实现 */
    static_assert(std::is_same_v<T, int> || std::is_same_v<T, float> ||
                      std::is_same_v<T, std::string>,
                  "Unsupported data type");

    bool type_matched = false;
    if constexpr (std::is_same_v<T, int>) {
      type_matched = (type_ == INT);
    } else if constexpr (std::is_same_v<T, float>) {
      type_matched = (type_ == FLOAT);
    } else {
      type_matched = (type_ == STRING);
    }

    if (!type_matched) {
      throw std::invalid_argument("Type mismatch in data operation");
    }
  }
};