#include <chrono>
#include <random>   // библиотека для генерации случайных чисел
#include <iostream>
#include "../templates/templates.cpp"
#include "../templates/templates.hpp"
#include <functional>

using namespace std::chrono;


int main(){

    using SearchFunc = std::function<int(int*, size_t, int)>;

    int n = 1'000'000'00;
    auto arr = measure_and_print("Генерация", gen_random_array, n, -10000, 5);

    auto v1 = measure_and_print("Последовательный поиск", sequential_search, arr, n, 42);

    auto v2 = measure_and_print("Бинарный поиск", binary_search, arr, n, 42);

    auto v3 = measure_and_print("Интерполяционный поиск", interpolation_search, arr, n, 42);
    delete[] arr;
}
