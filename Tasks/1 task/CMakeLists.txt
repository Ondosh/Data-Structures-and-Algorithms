cmake_minimum_required(VERSION 3.10)
project(SAOD LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# === Подключение Google Test ===
include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# === Основное приложение ===
add_executable(main src/main.cpp)
target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/include)

# === Тесты ===
add_executable(unit_tests
    tests/test.cpp
    # НЕ добавляйте templates.hpp сюда!
)
target_include_directories(unit_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(unit_tests PRIVATE GTest::gtest_main)

# === Регистрация тестов ===
include(GoogleTest)
gtest_discover_tests(unit_tests)