#ifndef TEMPLATES_HPP
#define TEMPLATES_HPP

#include <iostream>
#include <fstream>
#include <cstddef>
#include <string>

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

#endif