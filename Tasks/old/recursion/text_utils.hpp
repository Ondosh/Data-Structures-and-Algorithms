#ifndef TEXT_UTILS_HPP
#define TEXT_UTILS_HPP

#include <cstddef>
#include <string>

/**
 * @brief Считает количество цифр в строке итеративно.
 *
 * Перебирает все символы строки слева направо, останавливается на точке '.'
 * или конце строки. Каждый символ из диапазона '0'–'9' увеличивает счётчик.
 *
 * Сложность: O(n), где n — длина текста до точки.
 *
 * @param text Строка с текстом, оканчивающимся точкой.
 * @return std::size_t Количество найденных цифр.
 */
std::size_t count_digits_iterative(const std::string& text);

/**
 * @brief Считает количество цифр в строке рекурсивно.
 *
 * На каждом шаге проверяет один символ по индексу pos, затем рекурсивно
 * обрабатывает остаток строки. База рекурсии — конец строки или символ '.'.
 *
 * Сложность: O(n), глубина стека вызовов: O(n).
 *
 * @param text Строка с текстом, оканчивающимся точкой.
 * @param pos  Текущая позиция в строке (по умолчанию 0 — начало).
 * @return std::size_t Количество найденных цифр.
 */
std::size_t count_digits_recursive(const std::string& text, std::size_t pos = 0);

/**
 * @brief Читает содержимое текстового файла в строку.
 *
 * @param filename Путь к файлу.
 * @return std::string Содержимое файла, или пустая строка при ошибке.
 */
std::string read_file(const std::string& filename);

#endif // TEXT_UTILS_HPP
