# CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project(ArrayRef)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(LLVM_DIR "/home/qiuyuang/sora-mlir/externals/llvm-project/install/lib/cmake/llvm/")

# 查找LLVM配置（要求LLVM 10+）
find_package(LLVM REQUIRED CONFIG)

# 打印LLVM信息用于验证配置
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "LLVM Libraries: ${LLVM_LIBRARIES}")

# 设置LLVM相关变量
include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# 将LLVM的组件映射到具体库名
llvm_map_components_to_libnames(LLVM_LIBS
    # ADT   # 包含ArrayRef
    Support
)

# 添加可执行目标
add_executable(test
    test.cpp
    # 如果实现拆分为多个文件可在此添加
)

# 链接LLVM库
target_link_libraries(test
    PRIVATE
    ${LLVM_LIBS}
)

# 启用编译器警告
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(test PRIVATE
        -Wall
        -Wextra
        -Werror
        -Wno-unused-parameter
    )
endif()