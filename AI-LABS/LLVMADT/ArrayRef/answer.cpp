#include <llvm/ADT/ArrayRef.h>
#include <vector>
#include <string>
#include <stdexcept>
#include <type_traits>

class DataFlowProcessor {
public:
    enum DataType { INT, FLOAT, STRING };
    
    explicit DataFlowProcessor(DataType type) : type_(type) {}
    
    template <typename T>
    void addData(llvm::ArrayRef<T> data) {
        typeCheck<T>();
        
        if constexpr (std::is_same_v<T, int>) {
            intData_.insert(intData_.end(), data.begin(), data.end());
        } else if constexpr (std::is_same_v<T, float>) {
            floatData_.insert(floatData_.end(), data.begin(), data.end());
        } else if constexpr (std::is_same_v<T, std::string>) {
            stringData_.insert(stringData_.end(), data.begin(), data.end());
        }
    }
    
    template <typename T>
    llvm::ArrayRef<T> getDataView() const {
        typeCheck<T>();
        
        if constexpr (std::is_same_v<T, int>) {
            return intData_;
        } else if constexpr (std::is_same_v<T, float>) {
            return floatData_;
        } else if constexpr (std::is_same_v<T, std::string>) {
            return stringData_;
        }
    }
    
    template <typename T>
    llvm::ArrayRef<T> getSubrange(size_t start, size_t length) const {
        auto full = getDataView<T>();
        if (start > full.size()) {
            throw std::out_of_range("Invalid subrange start");
        }
        return full.slice(start, std::min(length, full.size() - start));
    }
    
    static DataFlowProcessor merge(const DataFlowProcessor& a,
                                  const DataFlowProcessor& b) {
        if (a.type_ != b.type_) {
            throw std::invalid_argument("Cannot merge different types");
        }
        
        DataFlowProcessor result(a.type_);
        auto a_view = a.getDataViewByType();
        auto b_view = b.getDataViewByType();
        
        if (a.type_ == INT) {
            result.intData_.insert(result.intData_.end(), a_view.ints.begin(), a_view.ints.end());
            result.intData_.insert(result.intData_.end(), b_view.ints.begin(), b_view.ints.end());
        } else if (a.type_ == FLOAT) {
            result.floatData_.insert(result.floatData_.end(), a_view.floats.begin(), a_view.floats.end());
            result.floatData_.insert(result.floatData_.end(), b_view.floats.begin(), b_view.floats.end());
        } else {
            result.stringData_.insert(result.stringData_.end(), a_view.strings.begin(), a_view.strings.end());
            result.stringData_.insert(result.stringData_.end(), b_view.strings.begin(), b_view.strings.end());
        }
        return result;
    }
    
private:
    DataType type_;
    std::vector<int> intData_;
    std::vector<float> floatData_;
    std::vector<std::string> stringData_;
    
    template <typename T>
    void typeCheck() const {
        static_assert(
            std::is_same_v<T, int> ||
            std::is_same_v<T, float> ||
            std::is_same_v<T, std::string>,
            "Unsupported data type"
        );
        
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
    
    union DataView {
        llvm::ArrayRef<int> ints;
        llvm::ArrayRef<float> floats;
        llvm::ArrayRef<std::string> strings;
        
        DataView(llvm::ArrayRef<int> d) : ints(d) {}
        DataView(llvm::ArrayRef<float> d) : floats(d) {}
        DataView(llvm::ArrayRef<std::string> d) : strings(d) {}
    };
    
    DataView getDataViewByType() const {
        switch (type_) {
            case INT: return DataView(intData_);
            case FLOAT: return DataView(floatData_);
            case STRING: return DataView(stringData_);
        }
        llvm_unreachable("Invalid data type");
    }
};