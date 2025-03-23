set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# 收集当前目录下的所有.cpp文件
file(GLOB SOURCES "*.cpp")

# 遍历所有源文件并为每个文件生成一个可执行文件
foreach(SOURCE_FILE ${SOURCES})
    # 获取文件名，去掉.cpp后缀
    get_filename_component(EXECUTABLE_NAME ${SOURCE_FILE} NAME_WE)
    
    # 添加一个可执行文件，名称为文件名去掉.cpp后缀
    add_executable(${EXECUTABLE_NAME} ${SOURCE_FILE})

    if(${SOURCE_FILE} STREQUAL "LLVMRTTI.cpp")
        target_compile_options(${EXECUTABLE_NAME} PRIVATE -fno-rtti)
    endif()
endforeach()