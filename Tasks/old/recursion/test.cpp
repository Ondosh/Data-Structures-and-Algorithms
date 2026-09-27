#include <cassert>
#include <iostream>
#include "text_utils.hpp"


// Вспомогательный макрос: запускает тест и печатает его имя.
// Если assert внутри func() провалится — программа завершится
// с сообщением об ошибке и номером строки автоматически.

#define RUN_TEST(func) \
    do { func(); std::cout << "  [OK] " #func "\n"; } while(0)


// Шаблон-обёртка: прогоняет один и тот же тест через обе функции,
// чтобы не дублировать assert-ы для итеративной и рекурсивной версий.

void check(const std::string& text, std::size_t expected) {
    assert(count_digits_iterative(text) == expected);
    assert(count_digits_recursive(text) == expected);
}

// Базовые случаи

void test_empty_string() {
    // Пустая строка — цифр нет
    check("", 0);
}

void test_only_dot() {
    // Одна точка — текст сразу заканчивается, цифр нет
    check(".", 0);
}

void test_only_letters() {
    // Только буквы — цифр нет
    check("abc.", 0);
}

void test_only_digits() {
    // Только цифры — все символы считаются
    check("1234567890.", 10);
}

void test_single_digit() {
    check("5.", 1);
}

// Поведение точки

void test_digits_before_dot() {
    // Цифры до точки должны считаться
    check("abc 123.", 3);
}

void test_digits_after_dot_ignored() {
    // Цифры ПОСЛЕ точки не считаются — точка завершает текст
    check("abc.123", 0);
}

void test_dot_in_the_middle() {
    // Точка посередине — считаем только до неё
    check("12.34", 2);
}

void test_no_dot_at_all() {
    // Точки нет вообще — читаем до конца строки
    check("abc 99 def", 2);
}

// Смешанный текст

void test_mixed_text() {
    check("Hello 42, world 7!", 3);
}

void test_spaces_and_digits() {
    check("1 2 3 4 5.", 5);
}

void test_negative_sign_not_counted() {
    // Минус — не цифра, только '0'–'9' считаются
    check("-5 and -3.", 2);
}

void test_special_chars() {
    // Спецсимволы не являются цифрами
    check("!@#$%^1&*()2.", 2);
}

// Граничные значения

void test_all_same_digit() {
    check("1111.", 4);
}

void test_digit_right_before_dot() {
    check("abc1.", 1);
}

void test_digit_right_after_start() {
    check("1abc.", 1);
}

void test_long_text() {
    // Строим строку из 1000 цифр + точка
    std::string s(1000, '7');
    s += '.';
    check(s, 1000);
}



int main() {
    std::cout << "Запуск тестов count_digits:\n";
    std::cout << "Базовые случаи:\n";
    RUN_TEST(test_empty_string);
    RUN_TEST(test_only_dot);
    RUN_TEST(test_only_letters);
    RUN_TEST(test_only_digits);
    RUN_TEST(test_single_digit);

    std::cout << "\nПоведение точки:\n";
    RUN_TEST(test_digits_before_dot);
    RUN_TEST(test_digits_after_dot_ignored);
    RUN_TEST(test_dot_in_the_middle);
    RUN_TEST(test_no_dot_at_all);

    std::cout << "\nСмешанный текст:\n";
    RUN_TEST(test_mixed_text);
    RUN_TEST(test_spaces_and_digits);
    RUN_TEST(test_negative_sign_not_counted);
    RUN_TEST(test_special_chars);

    std::cout << "\nГраничные значения:\n";
    RUN_TEST(test_all_same_digit);
    RUN_TEST(test_digit_right_before_dot);
    RUN_TEST(test_digit_right_after_start);
    RUN_TEST(test_long_text);

    std::cout << std::string(40, '-') << "\n";
    std::cout << "Все тесты пройдены.\n";
    return 0;
}