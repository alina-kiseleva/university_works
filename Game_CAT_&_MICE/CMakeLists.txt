cmake_minimum_required(VERSION 3.16)
project(cat_and_mice)

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
    ${PROJECT_SOURCE_DIR}/src/game.cpp
    ${PROJECT_SOURCE_DIR}/src/processInput.cpp
)

# Файлы с SFML и main
file(GLOB SFML_SOURCES
    ${PROJECT_SOURCE_DIR}/src/gui.cpp
    ${PROJECT_SOURCE_DIR}/src/main.cpp
)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Подключаем nlohmann/json
add_library(nlohmann_json INTERFACE)
target_include_directories(nlohmann_json INTERFACE ${PROJECT_SOURCE_DIR}/json/single_include)

# Основная программа
add_executable(${PROJECT_NAME} ${LOGIC_SOURCES} ${SFML_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/json/single_include)
target_link_libraries(${PROJECT_NAME}
    sfml-graphics
    sfml-window
    sfml-system
    nlohmann_json
)

# Упаковка и инсталляция для CPack
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assets")
    install(DIRECTORY assets DESTINATION bin)
endif()

set(CPACK_PACKAGE_NAME "cat_and_mice")
set(CPACK_PACKAGE_VENDOR "alina-kiseleva")
set(CPACK_PACKAGE_VERSION "1.0.0")

if(WIN32)
    set(CPACK_GENERATOR "ZIP")
else()
    set(CPACK_GENERATOR "TGZ")
endif()

include(CPack)