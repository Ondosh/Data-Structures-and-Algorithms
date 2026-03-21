#include <chrono>
#include <random>
#include <iostream>
#include <functional>
#include "../templates/templates.cpp"
#include "../templates/templates.hpp"

using namespace std::chrono;

int main() {
    size_t n = 1'000'000'000;
    auto arr = measure_and_print("Генерация", gen_random_array, n, -1'000'000'0, 5);

    auto v1 = measure_and_print("Последовательный поиск",  sequential_search<int>,    arr, n, 10000);
    // Усреднённые измерения — до delete[]!
    auto v2 = measure_and_print_avg("Бинарный поиск (среднее)",          100'000, binary_search<int>,        arr, n, 42);
    auto v3 = measure_and_print_avg("Интерполяционный поиск (среднее)",  100'000, interpolation_search<int>, arr, n, 42);

    auto v4 = measure_and_print("Поиск по предикату (> 10000)",
        predicate_search<int, std::function<bool(const int&)>>,
        arr, n,
        std::function<bool(const int&)>([](const int& x) { return x > 42; })
    );

    delete[] arr;
}