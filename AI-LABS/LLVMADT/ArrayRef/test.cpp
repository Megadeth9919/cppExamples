#include "DataFlowProcessor.h"
#include <iostream>
#include <array>

#define ASSERT(cond) \
    if (!(cond)) { \
        std::cerr << "Test failed at line " << __LINE__ << std::endl; \
        return 1; \
    }

int main() {
    int passCount = 0;
    
    // Test 1: 类型安全检查
    {
        DataFlowProcessor dfp(DataFlowProcessor::INT);
        std::vector<float> wrongData{1.1f, 2.2f};
        try {
            dfp.addData(llvm::ArrayRef(wrongData));
            ASSERT(false); // 应抛出异常
        } catch (const std::invalid_argument&) {
            passCount++;
        }
    }
    
    // Test 2: 跨容器访问
    {
        DataFlowProcessor dfp(DataFlowProcessor::FLOAT);
        float rawData[] = {1.0f, 2.0f, 3.0f};
        dfp.addData(llvm::ArrayRef(rawData));
        
        auto view = dfp.getDataView<float>();
        std::vector<float> vec(view.begin(), view.end());
        ASSERT(vec.size() == 3);
        passCount++;
    }
    
    // Test 3: 子范围切片
    {
        DataFlowProcessor dfp(DataFlowProcessor::STRING);
        std::array<std::string, 4> arr{"a", "b", "c", "d"};
        dfp.addData(llvm::ArrayRef(arr));
        
        auto sub = dfp.getSubrange<std::string>(1, 2);
        ASSERT(sub.size() == 2);
        ASSERT(sub[0] == "b");
        ASSERT(sub[1] == "c");
        passCount++;
    }
    
    // Test 4: 空数组处理
    {
        DataFlowProcessor dfp(DataFlowProcessor::INT);
        auto view = dfp.getDataView<int>();
        ASSERT(view.empty());
        ASSERT(view.data() == nullptr);
        passCount++;
    }
    
    // Test 5: 合并操作
    {
        DataFlowProcessor dfp1(DataFlowProcessor::FLOAT);
        std::vector<float> data1{1.0f, 2.0f};
        dfp1.addData(llvm::ArrayRef(data1));
        
        DataFlowProcessor dfp2(DataFlowProcessor::FLOAT);
        float data2[] = {3.0f, 4.0f};
        dfp2.addData(llvm::ArrayRef(data2, 2));
        
        auto merged = DataFlowProcessor::merge(dfp1, dfp2);
        auto view = merged.getDataView<float>();
        ASSERT(view.size() == 4);
        ASSERT(view[3] == 4.0f);
        passCount++;
    }
    
    std::cout << "Passed " << passCount << "/5 tests\n";
    return passCount == 5 ? 0 : 1;
}