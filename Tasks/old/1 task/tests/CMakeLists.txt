add_executable(unit_tests test.cpp)

# Линковка с Google Test
target_link_libraries(unit_tests PRIVATE GTest::gtest_main)

# Включение поддержки тестов в CMake
include(GoogleTest)
gtest_discover_tests(unit_tests)