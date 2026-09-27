#include <cassert>
#include <iostream>
#include <stdexcept>
#include "linked_list.hpp"

#define RUN_TEST(func) \
    do { func(); std::cout << "  [OK] " #func "\n"; } while(0)


// push_front / push_back


void test_push_back_size() {
    DoublyLinkedList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    assert(list.size() == 3);
}

void test_push_front_size() {
    DoublyLinkedList<int> list;
    list.push_front(3);
    list.push_front(2);
    list.push_front(1);
    assert(list.size() == 3);
}

void test_push_front_order() {
    // push_front добавляет в начало — список должен быть 1 <-> 2 <-> 3
    DoublyLinkedList<int> list;
    list.push_front(3);
    list.push_front(2);
    list.push_front(1);
    // Проверяем через поиск и размер (прямого доступа по индексу нет)
    assert(list.search(1));
    assert(list.search(2));
    assert(list.search(3));
    assert(list.size() == 3);
}

void test_empty_list() {
    DoublyLinkedList<int> list;
    assert(list.empty());
    assert(list.size() == 0);
}

void test_not_empty_after_push() {
    DoublyLinkedList<int> list;
    list.push_back(42);
    assert(!list.empty());
}


// search


void test_search_found() {
    DoublyLinkedList<int> list;
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);
    assert(list.search(10));   // head
    assert(list.search(20));   // середина
    assert(list.search(30));   // tail
}

void test_search_not_found() {
    DoublyLinkedList<int> list;
    list.push_back(1);
    list.push_back(2);
    assert(!list.search(99));
}

void test_search_empty_list() {
    DoublyLinkedList<int> list;
    assert(!list.search(5));
}


// insert_after


void test_insert_after_middle() {
    DoublyLinkedList<int> list;
    list.push_back(1);
    list.push_back(3);
    list.insert_after(1, 2);   // 1 <-> 2 <-> 3
    assert(list.size() == 3);
    assert(list.search(2));
}

void test_insert_after_tail() {
    DoublyLinkedList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.insert_after(2, 3);   // вставка после tail → 1 <-> 2 <-> 3
    assert(list.size() == 3);
    assert(list.search(3));
}

void test_insert_after_not_found() {
    DoublyLinkedList<int> list;
    list.push_back(1);
    bool threw = false;
    try {
        list.insert_after(99, 5);
    } catch (const std::runtime_error&) {
        threw = true;
    }
    assert(threw);
}


// pop_front / pop_back / remove


void test_pop_front() {
    DoublyLinkedList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.pop_front();           // удаляем 1
    assert(list.size() == 2);
    assert(!list.search(1));
    assert(list.search(2));
}

void test_pop_back() {
    DoublyLinkedList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.pop_back();            // удаляем 3
    assert(list.size() == 2);
    assert(!list.search(3));
    assert(list.search(2));
}

void test_pop_front_until_empty() {
    DoublyLinkedList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.pop_front();
    list.pop_front();
    assert(list.empty());
}

void test_pop_front_empty_throws() {
    DoublyLinkedList<int> list;
    bool threw = false;
    try { list.pop_front(); } catch (const std::runtime_error&) { threw = true; }
    assert(threw);
}

void test_pop_back_empty_throws() {
    DoublyLinkedList<int> list;
    bool threw = false;
    try { list.pop_back(); } catch (const std::runtime_error&) { threw = true; }
    assert(threw);
}

void test_remove_head() {
    DoublyLinkedList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.remove(1);
    assert(list.size() == 2);
    assert(!list.search(1));
}

void test_remove_tail() {
    DoublyLinkedList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.remove(3);
    assert(list.size() == 2);
    assert(!list.search(3));
}

void test_remove_middle() {
    DoublyLinkedList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.remove(2);
    assert(list.size() == 2);
    assert(!list.search(2));
    assert(list.search(1));
    assert(list.search(3));
}

void test_remove_not_found_throws() {
    DoublyLinkedList<int> list;
    list.push_back(1);
    bool threw = false;
    try { list.remove(99); } catch (const std::runtime_error&) { threw = true; }
    assert(threw);
}

void test_remove_single_element() {
    DoublyLinkedList<int> list;
    list.push_back(42);
    list.remove(42);
    assert(list.empty());
}


// concat


void test_concat_basic() {
    DoublyLinkedList<int> a, b;
    a.push_back(1);
    a.push_back(2);
    b.push_back(3);
    b.push_back(4);
    a.concat(b);                  // a: 1 <-> 2 <-> 3 <-> 4
    assert(a.size() == 4);
    assert(a.search(3));
    assert(a.search(4));
}

void test_concat_does_not_modify_other() {
    DoublyLinkedList<int> a, b;
    a.push_back(1);
    b.push_back(2);
    b.push_back(3);
    a.concat(b);
    assert(b.size() == 2);        // b не изменился
}

void test_concat_with_empty() {
    DoublyLinkedList<int> a, b;
    a.push_back(1);
    a.push_back(2);
    a.concat(b);                  // присоединяем пустой
    assert(a.size() == 2);
}

void test_concat_to_empty() {
    DoublyLinkedList<int> a, b;
    b.push_back(1);
    b.push_back(2);
    a.concat(b);                  // a был пустым
    assert(a.size() == 2);
    assert(a.search(1));
    assert(a.search(2));
}


// Разные типы (шаблон)


void test_string_list() {
    DoublyLinkedList<std::string> list;
    list.push_back("hello");
    list.push_back("world");
    assert(list.search("hello"));
    assert(list.search("world"));
    assert(!list.search("foo"));
    assert(list.size() == 2);
}

void test_double_list() {
    DoublyLinkedList<double> list;
    list.push_back(3.14);
    list.push_back(2.71);
    assert(list.search(3.14));
    list.remove(3.14);
    assert(!list.search(3.14));
}


// После удаления из середины — соседи корректно перешиты?
void test_remove_middle_links() {
    DoublyLinkedList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.remove(2);
    // Можно ли добавить после 1 и до 3?
    list.insert_after(1, 99);
    assert(list.search(99));
    assert(list.size() == 3);
}

// После pop_front — новый head не имеет prev?
void test_pop_front_head_prev_is_null() {
    DoublyLinkedList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.pop_front();
    // head теперь 2, его prev должен быть nullptr
    // Косвенно: push_front должен корректно работать после этого
    list.push_front(0);
    assert(list.size() == 2);
    assert(list.search(0));
}

// Можно ли модифицировать список после concat?
void test_concat_then_remove() {
    DoublyLinkedList<int> a, b;
    a.push_back(1);
    b.push_back(2);
    a.concat(b);
    a.remove(2); // не должен трогать b
    assert(a.size() == 1);
    assert(b.size() == 1); // b не пострадал
}

// concat с самим собой — UB или корректное поведение?
void test_concat_self() {
    DoublyLinkedList<int> a;
    a.push_back(1);
    a.push_back(2);
    a.concat(a); // бесконечный цикл?
    // Минимум — не крашится
}

void test_remove_duplicate_removes_first_only() {
    DoublyLinkedList<int> list;
    list.push_back(5);
    list.push_back(5);
    list.remove(5);
    assert(list.size() == 1); // удалён только первый
    assert(list.search(5));   // второй остался
}

void test_search_with_duplicates() {
    DoublyLinkedList<int> list;
    list.push_back(1);
    list.push_back(1);
    assert(list.search(1));
    assert(list.size() == 2);
}

void test_pop_front_after_single_push_throws() {
    DoublyLinkedList<int> list;
    list.push_front(1);
    list.pop_front();       // удаляем единственный элемент
    bool threw = false;
    try { list.pop_front(); } catch (const std::runtime_error&) { threw = true; }
    assert(threw);
}

int main() {
    std::cout << "Тесты DoublyLinkedList:\n";
    std::cout << std::string(45, '-') << "\n";

    std::cout << "push / empty / size:\n";
    RUN_TEST(test_push_back_size);
    RUN_TEST(test_push_front_size);
    RUN_TEST(test_push_front_order);
    RUN_TEST(test_empty_list);
    RUN_TEST(test_not_empty_after_push);

    std::cout << "\nsearch:\n";
    RUN_TEST(test_search_found);
    RUN_TEST(test_search_not_found);
    RUN_TEST(test_search_empty_list);

    std::cout << "\ninsert_after:\n";
    RUN_TEST(test_insert_after_middle);
    RUN_TEST(test_insert_after_tail);
    RUN_TEST(test_insert_after_not_found);

    std::cout << "\npop_front / pop_back / remove:\n";
    RUN_TEST(test_pop_front);
    RUN_TEST(test_pop_back);
    RUN_TEST(test_pop_front_until_empty);
    RUN_TEST(test_pop_front_empty_throws);
    RUN_TEST(test_pop_back_empty_throws);
    RUN_TEST(test_remove_head);
    RUN_TEST(test_remove_tail);
    RUN_TEST(test_remove_middle);
    RUN_TEST(test_remove_not_found_throws);
    RUN_TEST(test_remove_single_element);

    std::cout << "\nconcat:\n";
    RUN_TEST(test_concat_basic);
    RUN_TEST(test_concat_does_not_modify_other);
    RUN_TEST(test_concat_with_empty);
    RUN_TEST(test_concat_to_empty);

    std::cout << "\nРазные типы:\n";
    RUN_TEST(test_string_list);
    RUN_TEST(test_double_list);

    std::cout << "\nновые\n";
    RUN_TEST(test_remove_middle_links);
    RUN_TEST(test_pop_front_head_prev_is_null);
    RUN_TEST(test_concat_then_remove);
    RUN_TEST(test_concat_self);
    RUN_TEST(test_remove_duplicate_removes_first_only);
    RUN_TEST(test_search_with_duplicates);
    RUN_TEST(test_pop_front_after_single_push_throws);
    std::cout << std::string(45, '-') << "\n";
    std::cout << "Все тесты пройдены.\n";
    return 0;
}
