#include <iostream>
#include <string>
#include "text_utils.hpp"
#include "../templates/templates.hpp"   // measure_and_print_avg, measure_time


// Вспомогательные обёртки для measure_and_print_avg:
// шаблонные функции не принимают перегрузки напрямую через указатель,
// поэтому оборачиваем вызовы в лямбды.


int main() {
    // Читаем текст из файла
    const std::string filename = "input.txt";
    std::string text = read_file(filename);

    if (text.empty()) {
        std::cerr << "Ошибка: не удалось открыть файл";
        return 1;
    }

    std::cout << "Файл прочитан. Длина текста: " << text.size() << " символов.\n";
    std::cout << "Первые 80 символов: \""
              << text.substr(0, std::min<std::size_t>(80, text.size()))
              << "\"\n\n";

    // Однократный подсчёт (проверка корректности)
    std::size_t result_iter = count_digits_iterative(text);
    std::size_t result_rec  = count_digits_recursive(text);

    std::cout << "Результат итерации : " << result_iter << " цифр\n";
    std::cout << "Результат рекурсии : " << result_rec  << " цифр\n";

    if (result_iter != result_rec) {
        std::cerr << "ОШИБКА: результаты не совпадают!\n";
        return 2;
    }
    std::cout << "Результаты совпадают.\n\n";

    // Замер времени через шаблоны из templates.hpp
    const int RUNS = 10'000;   // количество повторений для усреднения
    std::cout << "Замер времени (" << RUNS << " запусков каждой функции):\n";
    std::cout << std::string(55, '-') << "\n";

    // Итеративная версия
    measure_and_print_avg(
        "Итеративная",
        RUNS,
        [&]() { return count_digits_iterative(text); }
    );

    // Рекурсивная версия
    measure_and_print_avg(
        "Рекурсивная ",
        RUNS,
        [&]() { return count_digits_recursive(text); }
    );

    std::cout << std::string(55, '-') << "\n";

    //Однократное измерение через measure_time
    std::cout << "\nОднократный замер (measure_time):\n";

    auto [res1, time1] = measure_time(
        [&]() { return count_digits_iterative(text); }
    );
    std::cout << "  Итеративная: " << time1.count() << " мкс\n";

    auto [res2, time2] = measure_time(
        [&]() { return count_digits_recursive(text); }
    );
    std::cout << "  Рекурсивная: " << time2.count() << " мкс\n";

    return 0;
}
