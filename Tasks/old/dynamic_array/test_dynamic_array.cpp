#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include "dynamic_array.hpp"

#define RUN_TEST(func) \
    do { func(); std::cout << "  [OK] " #func "\n"; } while(0)


// ============================================================
// Конструкторы
// ============================================================

void test_default_constructor_empty() {
    DynamicArray<int> arr;
    assert(arr.size() == 0);
    assert(arr.capacity() == 0);
    assert(arr.empty());
}

void test_constructor_with_size() {
    DynamicArray<int> arr(5, 42);
    assert(arr.size() == 5);
    for (std::size_t i = 0; i < 5; ++i)
        assert(arr[i] == 42);
}

void test_constructor_default_value() {
    DynamicArray<int> arr(3);
    assert(arr.size() == 3);
    for (std::size_t i = 0; i < 3; ++i)
        assert(arr[i] == 0);
}

void test_constructor_zero_size() {
    DynamicArray<int> arr(0);
    assert(arr.empty());
    assert(arr.size() == 0);
}


// ============================================================
// push_back / pop_back
// ============================================================

void test_push_back_basic() {
    DynamicArray<int> arr;
    arr.push_back(10);
    arr.push_back(20);
    arr.push_back(30);
    assert(arr.size() == 3);      // из test_push_back_size
    assert(arr[0] == 10);         // из test_push_back_values
    assert(arr[1] == 20);
    assert(arr[2] == 30);
}

void test_push_back_values() {
    DynamicArray<int> arr;
    arr.push_back(10);
    arr.push_back(20);
    arr.push_back(30);
    assert(arr[0] == 10);
    assert(arr[1] == 20);
    assert(arr[2] == 30);
}

void test_push_back_triggers_resize() {
    DynamicArray<int> arr;
    // Заполняем больше начальной ёмкости чтобы гарантированно вызвать resize
    for (int i = 0; i < 100; ++i)
        arr.push_back(i);
    assert(arr.size() == 100);
    for (int i = 0; i < 100; ++i)
        assert(arr[i] == i);
}

void test_pop_back_size() {
    DynamicArray<int> arr;
    arr.push_back(1);
    arr.push_back(2);
    arr.push_back(3);
    arr.pop_back();
    assert(arr.size() == 2);
}

void test_pop_back_removes_last() {
    DynamicArray<int> arr;
    arr.push_back(1);
    arr.push_back(2);
    arr.push_back(3);
    arr.pop_back();
    assert(arr[0] == 1);
    assert(arr[1] == 2);
}

void test_pop_back_until_empty() {
    DynamicArray<int> arr;
    arr.push_back(1);
    arr.push_back(2);
    arr.pop_back();
    arr.pop_back();
    assert(arr.empty());
}

void test_pop_back_empty_throws() {
    DynamicArray<int> arr;
    bool threw = false;
    try { arr.pop_back(); } catch (const std::out_of_range&) { threw = true; }
    assert(threw);
}

void test_pop_back_shrinks_capacity() {
    // После удаления большого количества элементов ёмкость должна уменьшиться
    DynamicArray<int> arr;
    for (int i = 0; i < 100; ++i)
        arr.push_back(i);
    std::size_t cap_full = arr.capacity();
    for (int i = 0; i < 96; ++i)
        arr.pop_back();
    // capacity должна быть меньше чем при заполнении
    assert(arr.capacity() < cap_full);
}


// ============================================================
// Доступ по индексу
// ============================================================

void test_operator_brackets_read() {
    DynamicArray<int> arr;
    arr.push_back(7);
    arr.push_back(8);
    assert(arr[0] == 7);
    assert(arr[1] == 8);
}

void test_operator_brackets_write() {
    DynamicArray<int> arr;
    arr.push_back(1);
    arr[0] = 99;
    assert(arr[0] == 99);
}

void test_at_valid_index() {
    DynamicArray<int> arr;
    arr.push_back(5);
    arr.push_back(10);
    assert(arr.at(0) == 5);
    assert(arr.at(1) == 10);
}

void test_at_out_of_range_throws() {
    DynamicArray<int> arr;
    arr.push_back(1);
    bool threw = false;
    try { arr.at(5); } catch (const std::out_of_range&) { threw = true; }
    assert(threw);
}

void test_at_empty_array_throws() {
    DynamicArray<int> arr;
    bool threw = false;
    try { arr.at(0); } catch (const std::out_of_range&) { threw = true; }
    assert(threw);
}

void test_at_write() {
    DynamicArray<int> arr;
    arr.push_back(1);
    arr.at(0) = 42;
    assert(arr.at(0) == 42);
}


// ============================================================
// insert
// ============================================================

void test_insert_at_beginning() {
    DynamicArray<int> arr;
    arr.push_back(2);
    arr.push_back(3);
    arr.insert(0, 1);           // [1, 2, 3]
    assert(arr.size() == 3);
    assert(arr[0] == 1);
    assert(arr[1] == 2);
    assert(arr[2] == 3);
}

void test_insert_at_middle() {
    DynamicArray<int> arr;
    arr.push_back(1);
    arr.push_back(3);
    arr.insert(1, 2);           // [1, 2, 3]
    assert(arr.size() == 3);
    assert(arr[0] == 1);
    assert(arr[1] == 2);
    assert(arr[2] == 3);
}

void test_insert_at_end() {
    DynamicArray<int> arr;
    arr.push_back(1);
    arr.push_back(2);
    arr.insert(2, 3);           // [1, 2, 3]
    assert(arr.size() == 3);
    assert(arr[2] == 3);
}

void test_insert_out_of_range_throws() {
    DynamicArray<int> arr;
    arr.push_back(1);
    bool threw = false;
    try { arr.insert(99, 5); } catch (const std::out_of_range&) { threw = true; }
    assert(threw);
}

void test_insert_into_empty() {
    DynamicArray<int> arr;
    arr.insert(0, 42);
    assert(arr.size() == 1);
    assert(arr[0] == 42);
}

void test_insert_preserves_other_elements() {
    DynamicArray<int> arr;
    for (int i = 0; i < 5; ++i) arr.push_back(i);  // [0,1,2,3,4]
    arr.insert(2, 99);                               // [0,1,99,2,3,4]
    assert(arr[0] == 0);
    assert(arr[1] == 1);
    assert(arr[2] == 99);
    assert(arr[3] == 2);
    assert(arr[4] == 3);
    assert(arr[5] == 4);
}


// ============================================================
// erase
// ============================================================

void test_erase_first() {
    DynamicArray<int> arr;
    arr.push_back(1);
    arr.push_back(2);
    arr.push_back(3);
    arr.erase(0);               // [2, 3]
    assert(arr.size() == 2);
    assert(arr[0] == 2);
    assert(arr[1] == 3);
}

void test_erase_middle() {
    DynamicArray<int> arr;
    arr.push_back(1);
    arr.push_back(2);
    arr.push_back(3);
    arr.erase(1);               // [1, 3]
    assert(arr.size() == 2);
    assert(arr[0] == 1);
    assert(arr[1] == 3);
}

void test_erase_last() {
    DynamicArray<int> arr;
    arr.push_back(1);
    arr.push_back(2);
    arr.push_back(3);
    arr.erase(2);               // [1, 2]
    assert(arr.size() == 2);
    assert(arr[0] == 1);
    assert(arr[1] == 2);
}

void test_erase_out_of_range_throws() {
    DynamicArray<int> arr;
    arr.push_back(1);
    bool threw = false;
    try { arr.erase(5); } catch (const std::out_of_range&) { threw = true; }
    assert(threw);
}

void test_erase_empty_throws() {
    DynamicArray<int> arr;
    bool threw = false;
    try { arr.erase(0); } catch (const std::out_of_range&) { threw = true; }
    assert(threw);
}


// ============================================================
// find
// ============================================================

void test_find_existing() {
    DynamicArray<int> arr;
    arr.push_back(10);
    arr.push_back(20);
    arr.push_back(30);
    assert(arr.find(10) == 0);
    assert(arr.find(20) == 1);
    assert(arr.find(30) == 2);
}

void test_find_not_existing() {
    DynamicArray<int> arr;
    arr.push_back(1);
    arr.push_back(2);
    assert(arr.find(99) == -1);
}

void test_find_empty_array() {
    DynamicArray<int> arr;
    assert(arr.find(5) == -1);
}

void test_find_returns_first_occurrence() {
    // При дублирующихся значениях возвращает первое вхождение
    DynamicArray<int> arr;
    arr.push_back(5);
    arr.push_back(5);
    arr.push_back(5);
    assert(arr.find(5) == 0);
}


// ============================================================
// sort
// ============================================================

void test_sort_unsorted() {
    // Случайный порядок
    DynamicArray<int> arr1;
    arr1.push_back(3);
    arr1.push_back(1);
    arr1.push_back(2);
    arr1.sort();
    assert(arr1[0] == 1);
    assert(arr1[1] == 2);
    assert(arr1[2] == 3);
    
    // Обратный порядок (более сложный случай)
    DynamicArray<int> arr2;
    for (int i = 10; i >= 1; --i) arr2.push_back(i);
    arr2.sort();
    for (std::size_t i = 0; i < arr2.size(); ++i)
        assert(arr2[i] == static_cast<int>(i) + 1);
}

void test_sort_already_sorted() {
    DynamicArray<int> arr;
    arr.push_back(1);
    arr.push_back(2);
    arr.push_back(3);
    arr.sort();
    assert(arr[0] == 1);
    assert(arr[1] == 2);
    assert(arr[2] == 3);
}

void test_sort_reverse_order() {
    DynamicArray<int> arr;
    for (int i = 10; i >= 1; --i) arr.push_back(i);
    arr.sort();
    for (std::size_t i = 0; i < arr.size(); ++i)
        assert(arr[i] == static_cast<int>(i) + 1);
}

void test_sort_single_element() {
    DynamicArray<int> arr;
    arr.push_back(42);
    arr.sort();
    assert(arr[0] == 42);
}

void test_sort_empty() {
    DynamicArray<int> arr;
    arr.sort();  // не должен крашиться
    assert(arr.empty());
}

void test_sort_with_duplicates() {
    DynamicArray<int> arr;
    arr.push_back(3);
    arr.push_back(1);
    arr.push_back(3);
    arr.push_back(1);
    arr.sort();
    assert(arr[0] == 1);
    assert(arr[1] == 1);
    assert(arr[2] == 3);
    assert(arr[3] == 3);
}


// ============================================================
// reserve
// ============================================================

void test_reserve_increases_capacity() {
    DynamicArray<int> arr;
    arr.reserve(100);
    assert(arr.capacity() >= 100);
    assert(arr.size() == 0);  // size не меняется
}

void test_reserve_preserves_elements() {
    DynamicArray<int> arr;
    arr.push_back(1);
    arr.push_back(2);
    arr.reserve(100);
    assert(arr[0] == 1);
    assert(arr[1] == 2);
    assert(arr.size() == 2);
}

void test_reserve_smaller_does_nothing() {
    DynamicArray<int> arr;
    arr.reserve(100);
    std::size_t cap = arr.capacity();
    arr.reserve(10);    // меньше текущей — ничего не делаем
    assert(arr.capacity() == cap);
}


// ============================================================
// Правило пяти — копирование и перемещение
// ============================================================

void test_copy_constructor_deep_copy() {
    DynamicArray<int> a;
    a.push_back(1);
    a.push_back(2);
    DynamicArray<int> b = a;        // конструктор копирования
    b[0] = 99;
    assert(a[0] == 1);              // a не изменился — глубокая копия
    assert(b[0] == 99);
}

void test_copy_constructor_size() {
    DynamicArray<int> a;
    a.push_back(1);
    a.push_back(2);
    DynamicArray<int> b = a;
    assert(b.size() == 2);
}

void test_copy_assignment_deep_copy() {
    DynamicArray<int> a;
    a.push_back(10);
    DynamicArray<int> b;
    b = a;                          // оператор присваивания
    b[0] = 55;
    assert(a[0] == 10);             // a не изменился
}

void test_copy_assignment_self() {
    DynamicArray<int> a;
    a.push_back(1);
    a = a;                          // самоприсваивание — не должен крашиться
    assert(a[0] == 1);
    assert(a.size() == 1);
}

void test_move_constructor() {
    DynamicArray<int> a;
    a.push_back(1);
    a.push_back(2);
    DynamicArray<int> b = std::move(a);
    assert(b.size() == 2);
    assert(b[0] == 1);
    assert(a.empty());              // a остался в валидном пустом состоянии
}

void test_move_assignment() {
    DynamicArray<int> a;
    a.push_back(42);
    DynamicArray<int> b;
    b = std::move(a);
    assert(b[0] == 42);
    assert(a.empty());
}

void test_move_assignment_self() {
    DynamicArray<int> a;
    a.push_back(1);
    a = std::move(a);              // самоперемещение — не должен крашиться
    assert(a.size() == 1);
}


// ============================================================
// Шаблонные типы
// ============================================================

void test_string_array() {
    DynamicArray<std::string> arr;
    arr.push_back("hello");
    arr.push_back("world");
    assert(arr.size() == 2);
    assert(arr[0] == "hello");
    assert(arr[1] == "world");
    assert(arr.find("world") == 1);
    assert(arr.find("foo") == -1);
}

void test_double_array() {
    DynamicArray<double> arr;
    arr.push_back(3.14);
    arr.push_back(2.71);
    arr.sort();
    assert(arr[0] == 2.71);
    assert(arr[1] == 3.14);
}


// ============================================================
// main
// ============================================================

int main() {
    std::cout << "Тесты DynamicArray:\n";
    std::cout << std::string(50, '-') << "\n";

    std::cout << "Конструкторы:\n";
    RUN_TEST(test_default_constructor_empty);
    RUN_TEST(test_constructor_with_size);
    RUN_TEST(test_constructor_default_value);
    RUN_TEST(test_constructor_zero_size);

    std::cout << "\npush_back / pop_back:\n";
    RUN_TEST(test_push_back_values);
    RUN_TEST(test_push_back_triggers_resize);
    RUN_TEST(test_pop_back_size);
    RUN_TEST(test_pop_back_removes_last);
    RUN_TEST(test_pop_back_until_empty);
    RUN_TEST(test_pop_back_empty_throws);
    RUN_TEST(test_pop_back_shrinks_capacity);

    std::cout << "\nДоступ по индексу:\n";
    RUN_TEST(test_operator_brackets_read);
    RUN_TEST(test_operator_brackets_write);
    RUN_TEST(test_at_valid_index);
    RUN_TEST(test_at_out_of_range_throws);
    RUN_TEST(test_at_empty_array_throws);
    RUN_TEST(test_at_write);

    std::cout << "\ninsert:\n";
    RUN_TEST(test_insert_at_beginning);
    RUN_TEST(test_insert_at_middle);
    RUN_TEST(test_insert_at_end);
    RUN_TEST(test_insert_out_of_range_throws);
    RUN_TEST(test_insert_into_empty);
    RUN_TEST(test_insert_preserves_other_elements);

    std::cout << "\nerase:\n";
    RUN_TEST(test_erase_first);
    RUN_TEST(test_erase_middle);
    RUN_TEST(test_erase_last);
    RUN_TEST(test_erase_out_of_range_throws);
    RUN_TEST(test_erase_empty_throws);

    std::cout << "\nfind:\n";
    RUN_TEST(test_find_existing);
    RUN_TEST(test_find_not_existing);
    RUN_TEST(test_find_empty_array);
    RUN_TEST(test_find_returns_first_occurrence);

    std::cout << "\nsort:\n";
    RUN_TEST(test_sort_already_sorted);
    RUN_TEST(test_sort_reverse_order);
    RUN_TEST(test_sort_single_element);
    RUN_TEST(test_sort_empty);
    RUN_TEST(test_sort_with_duplicates);

    std::cout << "\nreserve:\n";
    RUN_TEST(test_reserve_increases_capacity);
    RUN_TEST(test_reserve_preserves_elements);
    RUN_TEST(test_reserve_smaller_does_nothing);

    std::cout << "\nПравило пяти:\n";
    RUN_TEST(test_copy_constructor_deep_copy);
    RUN_TEST(test_copy_constructor_size);
    RUN_TEST(test_copy_assignment_deep_copy);
    RUN_TEST(test_copy_assignment_self);
    RUN_TEST(test_move_constructor);
    RUN_TEST(test_move_assignment);
    RUN_TEST(test_move_assignment_self);

    std::cout << "\nШаблонные типы:\n";
    RUN_TEST(test_string_array);
    RUN_TEST(test_double_array);

    std::cout << std::string(50, '-') << "\n";
    std::cout << "Все тесты пройдены.\n";
    return 0;
}