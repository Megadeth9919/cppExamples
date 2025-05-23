#include <gtest/gtest.h>
#include "functional.h"
#include <cstdio>

namespace tiny_stl {
namespace test {

void func_hello(int i) {
    printf("#%d Hello\n", i);
}

struct func_printnum_t {
    void operator()(int i) const {
        printf("#%d Numbers are: %d, %d\n", i, x, y);
    }
    int x;
    int y;
};

class FunctionTest : public ::testing::Test {
protected:
    void SetUp() override {
        x = 4;
        y = 2;
    }

    void repeattwice(function<void(int)> const &func) {
        func(1);
        func(2);
    }

    int x;
    int y;
};

TEST_F(FunctionTest, FunctionObject) {
    function<void(int)> f{[](int i) {
        printf("i = %d\n", i);
    }};

    testing::internal::CaptureStdout();
    f(2);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "i = 2\n");
}

TEST_F(FunctionTest, LambdaCapture) {
    testing::internal::CaptureStdout();
    repeattwice([=](int i) {
        printf("#%d Numbers are: %d, %d\n", i, x, y);
    });
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("#1 Numbers are: 4, 2") != std::string::npos);
    EXPECT_TRUE(output.find("#2 Numbers are: 4, 2") != std::string::npos);
}

TEST_F(FunctionTest, FunctorObject) {
    func_printnum_t func_printnum{x, y};
    testing::internal::CaptureStdout();
    repeattwice(func_printnum);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("#1 Numbers are: 4, 2") != std::string::npos);
    EXPECT_TRUE(output.find("#2 Numbers are: 4, 2") != std::string::npos);
}

TEST_F(FunctionTest, FunctionPointer) {
    testing::internal::CaptureStdout();
    repeattwice(func_hello);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("#1 Hello") != std::string::npos);
    EXPECT_TRUE(output.find("#2 Hello") != std::string::npos);
}

TEST_F(FunctionTest, CopySemantics) {
    function<void(int)> f{[](int i) {
        printf("i = %d\n", i);
    }};

    testing::internal::CaptureStdout();
    auto ff = f;
    ff(3);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "i = 3\n");
}

} // namespace test
} // namespace tiny_stl