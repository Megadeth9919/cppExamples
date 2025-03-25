set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB LIB_SRCS
  "lib/*.cpp"
)

add_library(TinyLLVMADT STATIC ${LIB_SRCS})
target_include_directories(TinyLLVMADT PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


file(GLOB SOURCES "test/*.cpp")
foreach(SOURCE_FILE ${SOURCES})
    # 获取文件名，去掉.cpp后缀
    get_filename_component(EXECUTABLE_NAME ${SOURCE_FILE} NAME_WE)
    
    add_executable(${EXECUTABLE_NAME} ${SOURCE_FILE})
    target_include_directories(${EXECUTABLE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE TinyLLVMADT)
endforeach()

