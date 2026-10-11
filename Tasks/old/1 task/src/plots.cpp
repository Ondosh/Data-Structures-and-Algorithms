#include "../templates/templates.cpp"
#include "../templates/templates.hpp"
#include <vector>
#include <fstream>
#include <algorithm>
#include <chrono>

int main() {
    std::ofstream out("results.csv");
    out << "n;seq;bin;interp\n";

    const int runs = 100; // увеличиваем количество запусков

    for (int k = 1; k <= 20; k++) {
        size_t n = 1ULL << k;

        // Генерируем отсортированный массив
        std::vector<int> arr(n);
        for (size_t i = 0; i < n; ++i)
            arr[i] = static_cast<int>(i * 2);

        int target = -1; // худший случай (не найден)
        int* data = arr.data();

        // Разные runs для разных алгоритмов
        int runs_slow = 5000;     // для линейного
        int runs_fast = 100000;                          // для бинарного (всегда много)
        
        auto seq_time = measure_time_avg_micro(runs_slow, sequential_search<int>, data, n, target);
        auto cseq_time = measure_time_avg_micro(runs_slow, std_search_wrapper<int>, data, n, target);
        auto bin_time = measure_time_avg_micro(runs_fast, binary_search<int>, data, n, target);
        auto int_time = measure_time_avg_micro(runs_fast, interpolation_search<int>, data, n, target);
        out << n << ";" << seq_time << ";" << cseq_time << ";" 
            << bin_time << ";" << int_time << "\n";
        std::cout << "n = " << n << " done\n";
    }

    out.close();
}