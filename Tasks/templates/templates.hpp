#ifndef TEMPLATES_HPP
#define TEMPLATES_HPP

#include <iostream>
#include <fstream>
#include <cstddef>
#include <string>

// Последовательный (линейный) поиск
// Перебирает все элементы подряд слева направо.
// Работает на любом массиве — не требует сортировки.
// Сложность: O(n) — в худшем случае просматриваем весь массив.
int sequential_search(int* arr, size_t n, int s);

// Бинарный поиск
// Каждый шаг делит оставшийся диапазон пополам и отбрасывает
// ту половину, в которой элемент заведомо не может находиться.
// Требует предварительно отсортированного массива.
// Сложность: O(log n) — при n=1 000 000 не более ~20 шагов.
int binary_search(int* arr, size_t n, int s);

// Интерполяционный поиск
// Возвращает индекс найденного элемента, или -1 если не найден
// Массив должен быть отсортирован по возрастанию
// Лучше всего работает при отсортированном массиве, 
// так как поиск работает по принципу поиска слова в словаре.
// (поиск начинается с той части, в которой наиболее вероятно появление числа)
int interpolation_search(int* arr, size_t n, int target);

int* gen_random_array(std::size_t n, int least, int max_step_width);

/**
 * @brief Записывает массив в текстовый файл, разделяя элементы пробелами.
 * 
 * @tparam T Тип элементов массива (должен поддерживать оператор вывода <<).
 * @param arr Указатель на первый элемент массива.
 * @param size Количество элементов в массиве.
 * @param filename Имя файла для записи.
 * @return true Если файл успешно открыт и данные записаны.
 * @return false Если не удалось открыть файл или произошла ошибка записи.
 * 
 * @note Файл перезаписывается при каждом вызове. Элементы записываются в одну строку.
 */
template<typename T>
bool writeArrayToFile(const T* arr, std::size_t size, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;

    for (std::size_t i = 0; i < size; ++i) {
        file << arr[i] << (i < size - 1 ? " " : "");
    }

    return file.good();
}

/**
 * @brief Проверяет, отсортирован ли массив по неубыванию.
 * 
 * @tparam T Тип элементов массива (должен поддерживать оператор сравнения >).
 * @param arr Указатель на первый элемент массива.
 * @param size Количество элементов в массиве.
 * @return true Если массив пуст, содержит один элемент или отсортирован.
 * @return false Если найдена пара элементов, где предыдущий больше следующего.
 * 
 * @note Проверка выполняется за O(n). Сравнение строгое: arr[i] > arr[i+1].
 */
template<typename T>
bool isSorted(const T* arr, std::size_t size) {
    if (size < 2) {
        return true;
    }
    for (std::size_t i = 0; i < size - 1; ++i) {
        if (arr[i] > arr[i + 1]) {
            return false;
        }
    }
    return true;
}

// Универсальная функция для измерения времени
template<typename Func, typename... Args>
auto measure_time(Func func, Args&&... args) {
    auto ts1 = std::chrono::steady_clock::now();
    auto result = func(std::forward<Args>(args)...);
    auto ts2 = std::chrono::steady_clock::now();
    auto delta = std::chrono::duration_cast<std::chrono::microseconds>(ts2 - ts1);
    return std::make_tuple(result, delta);
}

// Та же универсальная функция, но с выводом
template<typename Func, typename... Args>
auto measure_and_print(const char* name, Func func, Args&&... args) {
    auto [result, delta] = measure_time(func, std::forward<Args>(args)...);
    std::cout << name << ": результат = " << result 
         << ", время = " << delta.count() << " мкс" << std::endl;
    return result;
}

#endif