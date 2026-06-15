cmake_minimum_required(VERSION 3.16)
project(lab4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

find_package(GTest REQUIRED)
add_library(segment_tree_lib main.cpp)

add_executable(tests tests/tests.cpp)

target_link_libraries(tests 
    PRIVATE 
        segment_tree_lib
        GTest::GTest 
        GTest::Main
)

gtest_discover_tests(tests)