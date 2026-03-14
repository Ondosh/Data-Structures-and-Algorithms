#include <chrono>
#include <random>   // библиотека для генерации случайных чисел
#include <iostream>
#include "../templates/templates.cpp"
#include "../templates/templates.hpp"
#include <functional>

using namespace std::chrono;


int main(){

    int n = 1'000'000'0;
    auto arr = measure_and_print("Генерация", gen_random_array, n, -1000000, 5);

    auto v1 = measure_and_print("Последовательный поиск", sequential_search<int>, arr, n, 42);

    auto v2 = measure_and_print("Бинарный поиск", binary_search<int>, arr, n, 42);

    auto v3 = measure_and_print("Интерполяционный поиск", interpolation_search<int>, arr, n, 42);
    delete[] arr;
}
