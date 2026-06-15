cmake_minimum_required(VERSION 3.16)
project(lab3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

find_package(GTest REQUIRED)
add_library(rope_lib main.cpp)

add_executable(tests tests/test.cpp)

target_link_libraries(tests 
    PRIVATE 
        rope_lib
        GTest::GTest 
        GTest::Main
)

gtest_discover_tests(tests)