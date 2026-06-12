cmake_minimum_required(VERSION 3.16)
project(cw3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_executable(${PROJECT_NAME}
    main.cpp
    avltree.cpp
    menu.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)  

find_package(GTest REQUIRED)

add_executable(tests 
    tests.cpp  
    avltree.cpp       
)

target_link_libraries(tests 
    PRIVATE 
        GTest::gtest 
        GTest::gtest_main
)

target_include_directories(tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

gtest_discover_tests(tests)