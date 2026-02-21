#include <gtest/gtest.h>
#include "../templates/templates.hpp"

TEST(IsSortedTest, SortedArray) {
    int arr[] = {1, 2, 3, 4, 5};
    EXPECT_TRUE(isSorted(arr, 5));  
}

TEST(IsSortedTest, UnsortedArray1) {
    int arr[] = {5, 3, 1, 4, 2};
    EXPECT_FALSE(isSorted(arr, 5));  
}

TEST(IsSortedTest, UnsortedArray2) {
    int arr[] = {3, 2, 3, 4, 5};
    EXPECT_FALSE(isSorted(arr, 5));  
}

TEST(IsSortedTest, UnsortedArray3) {
    int arr[] = {1, 2, 3, 5, 4};
    EXPECT_FALSE(isSorted(arr, 5));  
}

TEST(ArrayTest, EmptyArray) {
    int arr[] = {};
    EXPECT_TRUE(isSorted(arr, 0));  
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}