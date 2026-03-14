#include <gtest/gtest.h>
#include "../templates/templates.hpp"
#include "../templates/templates.cpp"
#include <algorithm>

// ============================================================================
// Тесты для gen_random_array
// ============================================================================

TEST(GenRandomArrayTest, ReturnsNonNull) {
    // least <= 0, max_step_width >= 1
    int* arr = gen_random_array(10, -100, 100);
    EXPECT_NE(arr, nullptr);
    delete[] arr;
}

TEST(GenRandomArrayTest, FirstElementInRange) {
    const int n = 100;
    const int least = -10000;
    const int max_step = 1000;
    
    int* arr = gen_random_array(n, least, max_step);
    ASSERT_NE(arr, nullptr);
    
    // Первый элемент должен быть в [least, 0]
    EXPECT_GE(arr[0], least);
    EXPECT_LE(arr[0], 0);
    
    delete[] arr;
}

TEST(GenRandomArrayTest, MonotonicIncreasing) {
    const int n = 1000;
    const int least = -500;
    const int max_step = 100;
    
    int* arr = gen_random_array(n, least, max_step);
    ASSERT_NE(arr, nullptr);
    
    // Все последующие элементы должны быть строго больше предыдущих
    for (int i = 1; i < n; ++i) {
        EXPECT_GT(arr[i], arr[i - 1]) << "Index: " << i;
    }
    
    delete[] arr;
}

TEST(GenRandomArrayTest, EmptyArray) {
    int* arr = gen_random_array(0, -100, 100);
    EXPECT_NE(arr, nullptr);  // или EXPECT_EQ, если функция возвращает nullptr для n=0
    delete[] arr;
}

TEST(GenRandomArrayTest, SingleElement) {
    // Для n=1 генерируется только первый элемент в диапазоне [least, 0]
    const int least = -50;
    int* arr = gen_random_array(1, least, 100);
    ASSERT_NE(arr, nullptr);
    EXPECT_GE(arr[0], least);
    EXPECT_LE(arr[0], 0);
    delete[] arr;
}

// ============================================================================
// Тесты для sequential_search (работает с любым массивом)
// ============================================================================

TEST(SequentialSearchTest, FoundAtBeginning) {
    int arr[] = {42, 1, 2, 3, 4};
    EXPECT_EQ(sequential_search(arr, 5, 42), 0);
}

TEST(SequentialSearchTest, FoundAtMiddle) {
    int arr[] = {1, 2, 42, 3, 4};
    EXPECT_EQ(sequential_search(arr, 5, 42), 2);
}

TEST(SequentialSearchTest, FoundAtEnd) {
    int arr[] = {1, 2, 3, 4, 42};
    EXPECT_EQ(sequential_search(arr, 5, 42), 4);
}

TEST(SequentialSearchTest, NotFound) {
    int arr[] = {1, 2, 3, 4, 5};
    EXPECT_EQ(sequential_search(arr, 5, 42), -1);
}

TEST(SequentialSearchTest, EmptyArray) {
    EXPECT_EQ(sequential_search(nullptr, 0, 42), -1);
}

TEST(SequentialSearchTest, SingleElementFound) {
    int arr[] = {42};
    EXPECT_EQ(sequential_search(arr, 1, 42), 0);
}

TEST(SequentialSearchTest, SingleElementNotFound) {
    int arr[] = {1};
    EXPECT_EQ(sequential_search(arr, 1, 42), -1);
}

TEST(SequentialSearchTest, WithDuplicates) {
    int arr[] = {1, 42, 42, 42, 5};
    EXPECT_EQ(sequential_search(arr, 5, 42), 1);  // первое вхождение
}

// ============================================================================
// Тесты для binary_search (массив ДОЛЖЕН быть отсортирован!)
// ============================================================================

TEST(BinarySearchTest, FoundAtBeginning) {
    int arr[] = {42, 100, 200, 300, 400};
    std::sort(arr, arr + 5);
    EXPECT_EQ(binary_search(arr, 5, 42), 0);
}

TEST(BinarySearchTest, FoundAtMiddle) {
    int arr[] = {10, 20, 42, 50, 60};  // уже отсортирован
    EXPECT_EQ(binary_search(arr, 5, 42), 2);
}

TEST(BinarySearchTest, FoundAtEnd) {
    int arr[] = {1, 2, 3, 4, 42};
    std::sort(arr, arr + 5);
    EXPECT_EQ(binary_search(arr, 5, 42), 4);
}

TEST(BinarySearchTest, NotFound) {
    int arr[] = {1, 2, 3, 4, 5};
    EXPECT_EQ(binary_search(arr, 5, 42), -1);
}

TEST(BinarySearchTest, EmptyArray) {
    EXPECT_EQ(binary_search(nullptr, 0, 42), -1);
}

TEST(BinarySearchTest, SingleElementFound) {
    int arr[] = {42};
    EXPECT_EQ(binary_search(arr, 1, 42), 0);
}

TEST(BinarySearchTest, SingleElementNotFound) {
    int arr[] = {1};
    EXPECT_EQ(binary_search(arr, 1, 42), -1);
}

TEST(BinarySearchTest, WithDuplicates) {
    int arr[] = {1, 42, 42, 42, 5};
    std::sort(arr, arr + 5);
    int result = binary_search(arr, 5, 42);
    EXPECT_NE(result, -1);
    EXPECT_EQ(arr[result], 42);
}

// ============================================================================
// Тесты для interpolation_search
// ============================================================================

TEST(InterpolationSearchTest, FoundAtBeginning) {
    int arr[] = {42, 100, 200, 300, 400};
    std::sort(arr, arr + 5);
    EXPECT_EQ(interpolation_search(arr, 5, 42), 0);
}

TEST(InterpolationSearchTest, FoundAtMiddle) {
    int arr[] = {10, 20, 42, 50, 60};
    EXPECT_EQ(interpolation_search(arr, 5, 42), 2);
}

TEST(InterpolationSearchTest, FoundAtEnd) {
    int arr[] = {1, 2, 3, 4, 42};
    EXPECT_EQ(interpolation_search(arr, 5, 42), 4);
}

TEST(InterpolationSearchTest, NotFound) {
    int arr[] = {1, 2, 3, 4, 5};
    EXPECT_EQ(interpolation_search(arr, 5, 42), -1);
}

TEST(InterpolationSearchTest, EmptyArray) {
    EXPECT_EQ(interpolation_search(nullptr, 0, 42), -1);
}

TEST(InterpolationSearchTest, SingleElementFound) {
    int arr[] = {42};
    EXPECT_EQ(interpolation_search(arr, 1, 42), 0);
}

TEST(InterpolationSearchTest, SingleElementNotFound) {
    int arr[] = {1};
    EXPECT_EQ(interpolation_search(arr, 1, 42), -1);
}

TEST(InterpolationSearchTest, AllElementsEqual) {
    // Проверка защиты от деления на ноль
    int arr[] = {7, 7, 7, 7, 7};
    EXPECT_EQ(interpolation_search(arr, 5, 7), 0);
    EXPECT_EQ(interpolation_search(arr, 5, 42), -1);
}

TEST(InterpolationSearchTest, WithDuplicates) {
    int arr[] = {1, 42, 42, 42, 5};
    std::sort(arr, arr + 5);
    int result = interpolation_search(arr, 5, 42);
    EXPECT_NE(result, -1);
    EXPECT_EQ(arr[result], 42);
}

TEST(InterpolationSearchTest, UniformDistribution) {
    // Лучший случай для интерполяционного поиска — равномерное распределение
    int arr[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    EXPECT_EQ(interpolation_search(arr, 10, 10),  0);
    EXPECT_EQ(interpolation_search(arr, 10, 50),  4);
    EXPECT_EQ(interpolation_search(arr, 10, 100), 9);
    EXPECT_EQ(interpolation_search(arr, 10, 55),  -1);
}

// ============================================================================
// main()
// ============================================================================

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}