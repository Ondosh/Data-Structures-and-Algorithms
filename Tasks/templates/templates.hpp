#ifndef TEMPLATES_HPP
#define TEMPLATES_HPP

#include <iostream>
#include <fstream>
#include <cstddef>
#include <string>
#include <chrono>
#include <utility>

template<typename Func, typename... Args>
double measure_time_avg_micro(int runs, Func func, Args... args) {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < runs; ++i) {
        func(args...);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    return duration.count() / 1000.0 / runs; // возвращаем микросекунды
}

// Последовательный (линейный) поиск
// Перебирает все элементы подряд слева направо.
// Работает на любом массиве — не требует сортировки.
// Сложность: O(n) — в худшем случае просматриваем весь массив.
template<typename T>
long long sequential_search(T* arr, size_t n, T s) {
    for (long long i = 0; i < n; i++) {
        if (s == arr[i])
        {
            return i;  // Нашли — возвращаем индекс
        }
    }
    return -1;  // Элемент не найден
}

// Бинарный поиск
// Каждый шаг делит оставшийся диапазон пополам и отбрасывает
// ту половину, в которой элемент заведомо не может находиться.
// Требует предварительно отсортированного массива.
// Сложность: O(log n) — при n=1 000 000 не более ~20 шагов.
template<typename T>
long long binary_search(T* arr, size_t n, T s) {
    int left = 0;
    int right = n - 1;  // Границы текущего диапазона поиска

    while (left <= right) {
        // Вычисляем середину через left + (right-left)/2, а не (left+right)/2,
        // чтобы избежать переполнения при больших значениях left и right.
        int mid = left + (right - left) / 2;

        if (arr[mid] == s) {
            return mid;          // Попали точно — элемент найден
        }
        else if (arr[mid] < s) {
            left = mid + 1;      // Искомое правее — отсекаем левую половину
        }
        else {
            right = mid - 1;     // Искомое левее — отсекаем правую половину
        }
    }

    return -1;  // Диапазон сжался до нуля — элемент не найден
}

// Интерполяционный поиск
// Возвращает индекс найденного элемента, или -1 если не найден
// Массив должен быть отсортирован по возрастанию
// Лучше всего работает при отсортированном массиве, 
// так как поиск работает по принципу поиска слова в словаре.
// (поиск начинается с той части, в которой наиболее вероятно появление числа)
// BigO (log log n)
template<typename T>
long long interpolation_search(T* arr, size_t n, T target) {
    int left = 0;
    int right = static_cast<int>(n) - 1;

    while (left <= right
           && target >= arr[left]
           && target <= arr[right])
    {
        // Защита от деления на ноль (все элементы одинаковы)
        if (arr[left] == arr[right]) {
            return (arr[left] == target) ? left : -1;
        }

        // Формула интерполяции: предсказываем позицию
        int pos = left + (long long)(target - arr[left])
                        * (right - left)
                        / (arr[right] - arr[left]);

        if (arr[pos] == target)
            return pos;

        if (arr[pos] < target)
            left = pos + 1;   // Ищем в правой части
        else
            right = pos - 1;  // Ищем в левой части
    }

    return -1;  // Элемент не найден
}

// Поиск по предикату
// Возвращает индекс первого элемента, для которого предикат вернул true
// Предикат — любая функция/лямбда вида: bool pred(const T& element)
// Сложность: O(n) — перебираем все элементы, как при линейном поиске
template<typename T, typename Predicate>
long long predicate_search(T* arr, size_t n, Predicate pred) {
    for (long long i = 0; i < n; i++) {
        if (pred(arr[i]))
            return i;
    }
    return -1;
}

int* gen_random_sorted_array(std::size_t n, int least, int max_step_width);

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
    
    // volatile не позволяет компилятору кэшировать переменные 
    volatile auto sink = result; 
    (void)sink;

    auto delta = std::chrono::duration_cast<std::chrono::microseconds>(ts2 - ts1);
    return std::make_tuple(result, delta);
}

// Та же универсальная функция, но с выводом
template<typename Func, typename... Args>
auto measure_and_print(const char* name, Func func, Args&&... args) {
    auto [result, delta] = measure_time(func, std::forward<Args>(args)...);
    std::cout << name << ": результат = " << result 
         << ", время = " << delta.count() << " нс" << std::endl;
    return result;
}

template<typename Func, typename... Args>
auto measure_time_search_avg(int runs, Func func, Args&&... args) {
    // Прогрев — первый запуск не считаем
    func(args...);

    auto ts1 = std::chrono::steady_clock::now();
    for (int i = 0; i < runs; ++i)
        func(args...);
    auto ts2 = std::chrono::steady_clock::now();

    volatile auto sink = 0; (void)sink;

    auto total = std::chrono::duration_cast<std::chrono::microseconds>(ts2 - ts1);
    return total / runs;
}

template<typename Func, typename... Args>
auto measure_time_sort_avg(int runs, Func func, Args&&... args) {

    auto ts1 = std::chrono::steady_clock::now();
    for (int i = 0; i < runs; ++i)
        func(args...);
    auto ts2 = std::chrono::steady_clock::now();

    volatile auto sink = 0; (void)sink;

    auto total = std::chrono::duration_cast<std::chrono::microseconds>(ts2 - ts1);
    return total / runs;
}
// Усреднённое измерение с выводом результата и времени
template<typename Func, typename... Args>
auto measure_and_print_avg(const char* name, int runs, Func func, Args&&... args) {
    // Один запуск чтобы получить результат для вывода
    auto result = func(args...);
    volatile auto sink = result; (void)sink;

    // Прогрев
    func(args...);

    auto ts1 = std::chrono::steady_clock::now();
    for (int i = 0; i < runs; ++i)
        func(args...);
    auto ts2 = std::chrono::steady_clock::now();

    auto total = std::chrono::duration_cast<std::chrono::nanoseconds>(ts2 - ts1);
    auto avg = total / runs;

    std::cout << name 
              << ": результат = " << result
              << ", среднее время (" << runs << " запусков) = " << avg.count() << " нс"
              << std::endl;
    return result;
}

// Пузырьковая сортировка O(n^2)
template<typename T>
void bubble_sort(T* arr, size_t n){
    bool swapped = false;
    for (size_t i = 0; i < n - 1; i++)
    {
        for (size_t j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j+1])
            {
                std::swap(arr[j], arr[j+1]);
                swapped = true;
            }
            
            swapped = false;
        }
        
    }
    
}

/**
 * @brief Разделяет подмассив на две части относительно опорного элемента (pivot).
 *
 * Зачем выносить в отдельную функцию:
 *  1. Единственная ответственность (Single Responsibility Principle) —
 *     quick_sort отвечает за рекурсию, partition — за перестановку.
 *  2. Переиспользование — partition можно вызвать отдельно, например,
 *     для алгоритма "k-й по величине элемент" (nth_element) без полной сортировки.
 *  3. Тестируемость — partition легко проверить юнит-тестом независимо от сортировки.
 *  4. Читаемость — тело quick_sort сокращается до трёх смысловых строк.
 *
 * @param arr   Указатель на массив.
 * @param left  Левая граница подмассива (включительно).
 * @param right Правая граница подмассива (включительно).
 * @return std::pair<int,int> Пара {i, j} — новые границы для рекурсивных вызовов.
 */
template<typename T>
std::pair<int, int> partition(T* arr, int left, int right) {
    // Опорный элемент берём из середины — защита от O(n²) на уже отсортированных данных
    T pivot = arr[left + (right - left) / 2];

    int i = left;
    int j = right;

    while (i <= j) {
        while (arr[i] < pivot) i++;  // ищем элемент, который не на своей стороне
        while (arr[j] > pivot) j--;  // ищем элемент, который не на своей стороне

        if (i <= j) {
            std::swap(arr[i], arr[j]);
            i++;
            j--;
        }
    }

    // Возвращаем обе границы — quick_sort использует их для рекурсии
    return {i, j};
}

/**
 * @brief Сортирует подмассив arr[left..right] методом быстрой сортировки.
 *
 * Делегирует разбиение функции partition, затем рекурсивно сортирует
 * две получившиеся части. Тело функции намеренно минимально —
 * вся логика разбиения инкапсулирована в partition.
 *
 * Сложность: O(n log n) в среднем, O(n²) в худшем случае.
 * Глубина рекурсии: O(log n) в среднем.
 *
 * @param arr   Указатель на массив.
 * @param left  Левая граница сортируемого диапазона.
 * @param right Правая граница сортируемого диапазона.
 */
template<typename T>
void quick_sort(T* arr, int left, int right) {
    if (left >= right) return;  // база рекурсии: 0 или 1 элемент — уже отсортировано

    // Разбиваем и получаем новые границы — вся логика скрыта в partition
    auto [i, j] = partition(arr, left, right);

    // Рекурсивно сортируем левую часть [left..j]
    if (left < j)
        quick_sort(arr, left, j);

    // Рекурсивно сортируем правую часть [i..right]
    if (i < right)
        quick_sort(arr, i, right);
}

// Копирование массива
template<typename T>
void copy_array(const T* src, T* dst, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        dst[i] = src[i];
    }
}

// Вывод массива (для отладки)
template<typename T>
void print_array(const T* arr, size_t n, const std::string& name = "Array") {
    std::cout << name << " [";
    for (size_t i = 0; i < n && i < 20; ++i) {
        std::cout << arr[i];
        if (i < n - 1 && i < 19) std::cout << ", ";
    }
    if (n > 20) std::cout << ", ...";
    std::cout << "]" << std::endl;
}

template<typename T>
long long std_search_wrapper(T* arr, size_t n, T target) {
    auto it = std::search(arr, arr + n, &target, &target + 1);
    
    if (it != arr + n) {
        return it - arr; 
    } else {
        return -1;       
    }
}

#endif