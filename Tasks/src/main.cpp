#include <chrono>
#include <random>   // библиотека для генерации случайных чисел
#include <iostream>
#include "../templates/templates.cpp"
#include "../templates/templates.hpp"


using namespace std::chrono;


int main(){
    auto t0 = steady_clock::now();
    size_t n = 10;
    int* arr = gen_random_array(n);
    for (int i = 0; i < n; i++) {
        std::cout << arr[i] << std::endl;
    }
    // конечная отметка времени
    auto t1 = steady_clock::now();
    // преобразование времени (обычно наносекунды) в миллисекунды
    auto delta = duration_cast<milliseconds>(t1 - t0);
    std::cout << "time delta (milliseconds) " << delta.count();

    delete[] arr;
    
    return 0;
}