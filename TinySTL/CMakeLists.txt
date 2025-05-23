cmake_minimum_required(VERSION 3.10)
project(tiny_stl_allocator LANGUAGES CXX)

# Enable C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_TYPE Debug)

set(GTEST_ROOT "/home/qiuyuang/cppExamples/googletest/")
set(GTEST_INCLUDE_DIR "/home/qiuyuang/cppExamples/googletest/install/include")
find_package(GTest REQUIRED)

# Library target
add_library(TinySTL INTERFACE)
target_include_directories(TinySTL INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB TESTSRC "test/*.cpp")

# Test executable
enable_testing()
add_executable(stl_test ${TESTSRC})
target_link_libraries(stl_test
    PRIVATE
        TinySTL
        GTest::GTest
)

# Add test
add_test(NAME allocator_test COMMAND allocator_test)