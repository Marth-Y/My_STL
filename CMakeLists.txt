cmake_minimum_required(VERSION 3.5)
project(MY_STL)

set(CMAKE_CXX_STANDARD 14)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
    message("debug")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
    message("release")
endif()

include_directories(/home/luyimin/My_STL/include/stl)
include_directories(/home/luyimin/My_STL/include/googletest)
add_executable(${PROJECT_NAME} src/vector_test_alloc.cc)

# 查找库文件
# find_library(GTEST_PATH NAMES gtest PATHS /home/luyimin/My_STL/thirdparty/gtest)
# message(${GTEST_PATH})

# find_library(GTEST_MAIN_PATH NAMES gtest_main PATHS /home/luyimin/My_STL/thirdparty/gtest)
# message(${GTEST_MAIN_PATH})

# 链接库文件
target_link_libraries(${PROJECT_NAME} PUBLIC 
/home/luyimin/My_STL/thirdparty/gtest/libgtest.a
/home/luyimin/My_STL/thirdparty/gtest/libgtest_main.a
)

