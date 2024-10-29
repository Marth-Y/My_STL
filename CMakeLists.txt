cmake_minimum_required(VERSION 3.5)
project(MY_STL)

set(CMAKE_CXX_STANDARD 11)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
    message("debug")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
    message("release")
endif()

include_directories(/home/luyimin/My_STL/)
add_executable(${PROJECT_NAME} vector_test_alloc.cc)
