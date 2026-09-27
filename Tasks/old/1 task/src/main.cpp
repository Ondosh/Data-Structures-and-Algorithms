#include <chrono>
#include <random>
#include <iostream>
#include <functional>
#include "../templates/templates.cpp"
#include "../templates/templates.hpp"

using namespace std::chrono;

/**
 * Демонстрирует и сравнивает производительность четырёх алгоритмов поиска
 * на массиве из 1 000 000 000 случайных целых чисел.
 *
 * Порядок запусков:
 *  1. Генерация массива с накопленными случайными шагами (gen_random_array).
 *  2. Последовательный поиск — O(n), не требует сортировки.
 *  3. Бинарный поиск — O(log n), массив уже отсортирован после генерации.
 *  4. Интерполяционный поиск — O(log log n) при равномерном распределении.
 *  5. Поиск по предикату — O(n), ищет первый элемент > 42.
 *
 * Бинарный и интерполяционный поиск измеряются усреднённо (100 000 запусков),
 * чтобы нивелировать влияние кэша процессора на единичный замер.
 *
 * @note Массив выделяется через new[] и освобождается в конце через delete[].
 *       При n = 1 000 000 000 потребуется ~4 ГБ оперативной памяти (int = 4 байта).
 */
int main() {
        // ==================== ЧАСТЬ 1: СОРТИРОВКИ ====================
    std::cout << "\n========== СОРТИРОВКИ ==========\n" << std::endl;
    
    // Для сортировки возьмём массив поменьше (пузырёк очень медленный)
    size_t n_sort = 20'000;  // 10 тысяч элементов
    
    // Генерируем неотсортированный массив
    std::cout << "Генерация неотсортированного массива из " << n_sort << " элементов..." << std::endl;
    int* unsorted_arr = gen_random_unsorted_array(n_sort, 0, 100'000);
    
    // Создаём копии для разных сортировок
    int* arr_for_bubble = new int[n_sort];
    int* arr_for_quick = new int[n_sort];
    
    copy_array(unsorted_arr, arr_for_bubble, n_sort);
    copy_array(unsorted_arr, arr_for_quick, n_sort);
    
    std::cout << "Исходный массив (первые 20 элементов): ";
    print_array(unsorted_arr, n_sort, "");
    
    // Пузырьковая сортировка (только для демонстрации, очень медленная)
    std::cout << "\n--- Пузырьковая сортировка O(n^2) ---" << std::endl;
    auto bubble_time = measure_time_sort_avg(1, bubble_sort<int>, arr_for_bubble, n_sort);
    std::cout << "Время: " << bubble_time.count() << " мкс" << std::endl;
    std::cout << "Отсортирован: " << (isSorted(arr_for_bubble, n_sort) ? "да" : "нет") << std::endl;
    std::cout << "Первые 20 элементов после сортировки: ";
    print_array(arr_for_bubble, n_sort, "");
    
    // Быстрая сортировка
    std::cout << "\n--- Быстрая сортировка O(n log n) ---" << std::endl;
    auto quick_time = measure_time_sort_avg(1,[](int* arr, size_t n) {
        quick_sort(arr, 0, static_cast<int>(n - 1));
    }, arr_for_quick, n_sort);
    std::cout << "Время: " << quick_time.count() << " мкс" << std::endl;
    std::cout << "Отсортирован: " << (isSorted(arr_for_quick, n_sort) ? "да" : "нет") << std::endl;
    std::cout << "Первые 20 элементов после сортировки: ";
    print_array(arr_for_quick, n_sort, "");
    
    // Сравнение
    std::cout << "\n--- Сравнение сортировок ---" << std::endl;
    std::cout << "Быстрая сортировка быстрее пузырьковой в " 
              << static_cast<double>(bubble_time.count()) / quick_time.count() 
              << " раз" << std::endl;
    
    // Очищаем память от временных массивов сортировок
    delete[] unsorted_arr;
    delete[] arr_for_bubble;
    delete[] arr_for_quick;

    // Количество чисел в массиве
    size_t n = 1'000'000'000;

    // gen_random_array строит возрастающий массив: каждый следующий элемент
    // = предыдущий + случайный шаг от 0 до max_step_width.
    // Стартовое значение: -10 000 000, максимальный шаг: 5.
    // Такой массив подходит для бинарного и интерполяционного поиска «из коробки».
    auto arr = measure_and_print("Генерация", gen_random_sorted_array, n, -1'000'000'0, 5);

    // Последовательный поиск ищет значение 10000 перебором слева направо.
    // Измеряется один раз — O(n) делает усреднение бессмысленным (время стабильно).
    auto v1 = measure_and_print("Последовательный поиск", sequential_search<int>, arr, n, 10000);

    // Бинарный и интерполяционный поиск работают за микросекунды,
    // поэтому единичный замер даёт ненадёжный результат из-за кэша.
    // 100 000 запусков с прогревом дают стабильное среднее.
    auto v2 = measure_and_print_avg("Бинарный поиск (среднее)",         100'000, binary_search<int>,        arr, n, 42);
    auto v3 = measure_and_print_avg("Интерполяционный поиск (среднее)", 100'000, interpolation_search<int>, arr, n, 42);

    // Поиск по предикату: ищет первый элемент строго больше 42.
    // std::function<bool(const int&)> нужен для явного указания типа предиката,
    // так как шаблон predicate_search не может вывести тип лямбды автоматически
    // без явного приведения.
    auto v4 = measure_and_print("Поиск по предикату (> 42)",
        predicate_search<int, std::function<bool(const int&)>>,
        arr, n,
        std::function<bool(const int&)>([](const int& x) { return x > 42; })
    );

    delete[] arr;  // Освобождаем ~4 ГБ памяти, выделенной в gen_random_array
}