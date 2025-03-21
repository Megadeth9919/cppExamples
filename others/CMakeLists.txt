set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
file(GLOB SOURCES "*.cpp")

foreach(SOURCE_FILE ${SOURCES})
    get_filename_component(EXECUTABLE_NAME ${SOURCE_FILE} NAME_WE)
    
    add_executable(${EXECUTABLE_NAME} ${SOURCE_FILE})
endforeach()