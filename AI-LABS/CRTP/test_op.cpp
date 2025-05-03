#include <cassert>
#include "op.h"

void test_add_op() {
    // 测试名称
    assert(AddOp::getName() == "add");
    
    // 测试类型验证
    assert(AddOp::verify(Type::Int, Type::Int) == true);
    assert(AddOp::verify(Type::Float, Type::Float) == true);
    assert(AddOp::verify(Type::Int, Type::Float) == false);
    
    // 测试特性系统
    // static_assert(AddOp::hasOneResult, "AddOp should have one result");
}

void test_mul_op() {
    // 测试名称
    assert(MulOp::getName() == "mul");
    
    // 测试类型验证
    assert(MulOp::verify(Type::Float, Type::Float) == true);
    assert(MulOp::verify(Type::Int, Type::Int) == false);
    
    // 测试特性系统
    // static_assert(!requires { MulOp::hasOneResult; }, 
                //  "MulOp should not have one result");
}

void test_construction() {
    auto add = AddOp::create();
    auto mul = MulOp::create();
    
    // 测试工厂方法
    assert(typeid(*add) == typeid(AddOp));
    assert(typeid(*mul) == typeid(MulOp));
}

int main() {
    test_add_op();
    test_mul_op();
    test_construction();
    std::cout << "All tests passed!\n";
    OpConstructor<AddOp>::build();
    OpConstructor<MulOp>::build();
    return 0;
}