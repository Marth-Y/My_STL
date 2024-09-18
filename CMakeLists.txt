cmake_minimum_required(VERSION 3.5)
project(MY_STL)

set(CMAKE_CXX_STANDARD 11)

include_directories(/home/luyimin/My_STL/)
add_executable(${PROJECT_NAME} vector_test_alloc.cc)
