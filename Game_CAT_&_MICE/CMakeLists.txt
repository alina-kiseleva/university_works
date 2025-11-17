cmake_minimum_required(VERSION 3.16)
project(my_game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

# Только логические файлы для тестов
file(GLOB LOGIC_SOURCES
    ${PROJECT_SOURCE_DIR}/src/cell.cpp
    ${PROJECT_SOURCE_DIR}/src/enemy.cpp
    ${PROJECT_SOURCE_DIR}/src/field.cpp
    ${PROJECT_SOURCE_DIR}/src/gameManager.cpp
    ${PROJECT_SOURCE_DIR}/src/player.cpp
    ${PROJECT_SOURCE_DIR}/src/hand.cpp
    ${PROJECT_SOURCE_DIR}/src/directDamageSpell.cpp
    ${PROJECT_SOURCE_DIR}/src/areaDamageSpell.cpp
    ${PROJECT_SOURCE_DIR}/src/trapSpell.cpp
    ${PROJECT_SOURCE_DIR}/src/enemyTower.cpp
    ${PROJECT_SOURCE_DIR}/src/saveManager.cpp
)

# Файлы с SFML и main
file(GLOB SFML_SOURCES
    ${PROJECT_SOURCE_DIR}/src/game.cpp
    ${PROJECT_SOURCE_DIR}/src/gui.cpp
    ${PROJECT_SOURCE_DIR}/src/main.cpp
)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
find_package(GTest REQUIRED)

# Подключаем nlohmann/json
add_library(nlohmann_json INTERFACE)
target_include_directories(nlohmann_json INTERFACE /home/alinak1s/json/single_include)

# Основная программа
add_executable(my_game ${LOGIC_SOURCES} ${SFML_SOURCES})
target_include_directories(my_game PRIVATE /home/alinak1s/json/single_include)
target_link_libraries(my_game
    sfml-graphics
    sfml-window
    sfml-system
    nlohmann_json
)

# Тесты - только логика
# file(GLOB TEST_SOURCES ${PROJECT_SOURCE_DIR}/test/*.cpp)
# add_executable(testing ${TEST_SOURCES} ${LOGIC_SOURCES})
# target_link_libraries(testing GTest::gtest_main GTest::gtest)