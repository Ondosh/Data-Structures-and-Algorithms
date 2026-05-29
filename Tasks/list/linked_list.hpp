#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <iostream>
#include <stdexcept>

/**
 * @brief Шаблонный двусвязный список.
 *
 * Каждый узел хранит указатели на предыдущий и следующий узлы,
 * что позволяет двигаться по списку в обоих направлениях.
 *
 * Структура памяти:
 *   nullptr ← [head] ⇄ [node] ⇄ ... ⇄ [tail] → nullptr
 *
 * @tparam T Тип хранимых элементов.
 */
template<typename T>
class DoublyLinkedList {

    
    // Внутренний узел списка
    // Объявлен приватным — пользователь работает только с итератором
    // Можно сделать внешним, но нежелательно с точки зрения инкапсуляции
    // Node является частью этого класса, поэтому не стоит его выделять внешне
    // Не обязательно прятать, т.к. можно использовать например в кольцевых списках
public:    struct Node {
        T     data;   // Полезная нагрузка
        Node* prev;   // Указатель на предыдущий узел (nullptr у head)
        Node* next;   // Указатель на следующий узел  (nullptr у tail)
        
        // Explicit - запрещает неявные преобразования типов вроде
        // foo(Node node){} foo(42) - надо foo(Node(42)
        explicit Node(const T& value)
            : data(value), prev(nullptr), next(nullptr) {}
    };

    Node*  head_;   // Указатель на первый элемент
    Node*  tail_;   // Указатель на последний элемент

<<<<<<< HEAD
    // ПРАВИЛО ТРЁХ
    // Если класс управляет ресурсом вручную, нужно определить три метода:
    //
    //  1. Деструктор
    //  2. Конструктор копирования
    //  3. Оператор присваивания копирования

    // ПРАВИЛО ПЯТИ
    // Если класс управляет ресурсом вручную (здесь — узлы через new/delete),
    // компилятор не может сгенерировать корректные версии этих методов сам.
    // Поэтому мы обязаны определить все пять:
    //
    //  1. Деструктор
    //  2. Конструктор копирования
    //  3. Оператор присваивания копирования
    //  4. Конструктор перемещения
    //  5. Оператор присваивания перемещения
    //
    // Без них компилятор делает поверхностную копию (shallow copy) —
    // копирует только указатели head_/tail_, а не сами узлы.
    // Итог: два объекта владеют одной памятью → double free при деструкторе.
=======

    // Правило трёх - если конструктор 
    // Правило пяти
   // Конструктор / деструктор
    // Список инициализации. После : объявленные поля сразу получают nullptr
    DoublyLinkedList() : head_(nullptr), tail_(nullptr) {}
>>>>>>> e6f483c (upd)

    // 1. Деструктор - освобождает все узлы
    ~DoublyLinkedList() {
        Node* current = head_;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }

    // 2. Конструктор копирования
    // Вызывается при: DoublyLinkedList<int> b = a;
    //                 DoublyLinkedList<int> b(a);
    //                 передача по значению в функцию foo(DoublyLinkedList<int> x)
    // Делаем глубокую копию (deep copy) — создаём новые узлы с теми же данными.
    // После копирования два списка полностью независимы.
    DoublyLinkedList(const DoublyLinkedList& other)
        : head_(nullptr), tail_(nullptr)
    {
        Node* current = other.head_;
        while (current != nullptr) {
            push_back(current->data);   // создаёт новый узел для каждого элемента
            current = current->next;
        }
    }

    // 3. Оператор присваивания копирования
    // Вызывается при: b = a;  (оба уже существуют)
    // Идиома copy-and-swap:
    //   - создаём временную копию other (через конструктор копирования)
    //   - меняем содержимое this и temp местами (swap)
    //   - temp уничтожается и забирает старую память this
    // Преимущество: автоматически безопасен при самоприсваивании (a = a)
    //               и не течёт при исключениях
    DoublyLinkedList& operator=(const DoublyLinkedList& other) {
        if (this == &other) return *this;   // защита от a = a

        DoublyLinkedList temp(other);       // глубокая копия через конструктор копирования
        swap(temp);                         // меняемся ресурсами
        return *this;
        // temp уничтожается здесь — вместе со старыми узлами this
    }

    // 4. Конструктор перемещения
    // Вызывается при: DoublyLinkedList<int> b = std::move(a);
    //                 возврат локального списка из функции (NRVO может оптимизировать)
    // Перемещение дешевле копирования — просто "крадём" указатели,
    // вместо того чтобы обходить и копировать все узлы.
    // После перемещения other обязан быть в валидном пустом состоянии
    // (деструктор other должен отработать корректно).
    DoublyLinkedList(DoublyLinkedList&& other) noexcept
        : head_(other.head_), tail_(other.tail_)
    {
        other.head_ = nullptr;  // other больше не владеет узлами
        other.tail_ = nullptr;
    }

    // 5. Оператор присваивания перемещения
    // Вызывается при: b = std::move(a);  (оба уже существуют)
    // Та же идея: крадём ресурсы, отдаём свои старые узлы на уничтожение.
    DoublyLinkedList& operator=(DoublyLinkedList&& other) noexcept {
        if (this == &other) return *this;   // защита от a = std::move(a)

        swap(other);            // забираем ресурсы other, отдаём ему наши старые
        other.head_ = nullptr;  // other сбрасывается в пустое состояние
        other.tail_ = nullptr;
        return *this;
    }

    // ============================================================
    // Вспомогательный метод для идиомы copy-and-swap
    // Меняет местами внутренние указатели двух списков за O(1).
    // noexcept — гарантирует что swap никогда не бросит исключение,
    // что важно для строгой гарантии безопасности исключений.
    // ============================================================
    void swap(DoublyLinkedList& other) noexcept {
        std::swap(head_, other.head_);
        std::swap(tail_, other.tail_);
    }    
    // Правило трёх - если конструктор 
    // Правило пяти
   // Конструктор / деструктор
    // Список инициализации. После : объявленные поля сразу получают nullptr
    DoublyLinkedList() : head_(nullptr), tail_(nullptr) {}


    
    // Вставка
    

    /**
     * @brief Вставляет элемент в начало списка. O(1).
     *
     * Новый узел становится head. Если список был пуст —
     * он же становится и tail.
     *
     * @param value Значение для вставки.
     */
    void push_front(const T& value) {
        Node* node = new Node(value);

        if (head_ == nullptr) {
            // Список пуст — единственный узел одновременно head и tail
            head_ = tail_ = node;
        } else {
            // Подвешиваем новый узел перед текущим head
            node->next  = head_;
            head_->prev = node;
            head_       = node;
        }
    }

    /**
     * @brief Вставляет элемент в конец списка. O(1).
     *
     * Благодаря указателю tail_ не нужно обходить весь список.
     *
     * @param value Значение для вставки.
     */
    void push_back(const T& value) {
        Node* node = new Node(value);

        if (tail_ == nullptr) {
            head_ = tail_ = node;
        } else {
            node->prev  = tail_;
            tail_->next = node;
            tail_       = node;
        }
    }

    /**
     * @brief Вставляет элемент после первого вхождения after_value. O(n).
     *
     * Сначала ищем узел с нужным значением (O(n)),
     * затем вставляем новый узел сразу после него (O(1)).
     * Если значение не найдено — бросаем исключение.
     *
     * @param after_value Значение узла, после которого вставляем.
     * @param value       Значение для вставки.
     * @throws std::runtime_error Если after_value не найден.
     */
    void insert_after(const T& after_value, const T& value) {
        Node* target = find_node(after_value);  // O(n)

        if (target == nullptr) {
            throw std::runtime_error("insert_after: значение не найдено");
        }

        // Если вставляем после хвоста — используем готовый push_back
        if (target == tail_) {
            push_back(value);
            return;
        }

        // Вставка в середину: new_node встаёт между target и target->next
        //   [target] <-> [new_node] <-> [target->next]
        Node* new_node   = new Node(value);
        Node* after_node = target->next;

        new_node->prev   = target;
        new_node->next   = after_node;
        target->next     = new_node;
        after_node->prev = new_node;
    }

    
    // Удаление
    

    /**
     * @brief Удаляет первый элемент списка. O(1).
     * @throws std::runtime_error Если список пуст.
     */
    void pop_front() {
        if (head_ == nullptr) {
            throw std::runtime_error("pop_front: список пуст");
        }

        Node* to_delete = head_;
        head_ = head_->next;

        if (head_ != nullptr) {
            head_->prev = nullptr;  // Новый head не имеет предшественника
        } else {
            tail_ = nullptr;        // Список стал пустым
        }

        delete to_delete;
    }

    /**
     * @brief Удаляет последний элемент списка. O(1).
     * @throws std::runtime_error Если список пуст.
     */
    void pop_back() {
        if (tail_ == nullptr) {
            throw std::runtime_error("pop_back: список пуст");
        }

        Node* to_delete = tail_;
        tail_ = tail_->prev;

        if (tail_ != nullptr) {
            tail_->next = nullptr;  // Новый tail не имеет продолжения
        } else {
            head_ = nullptr;        // Список стал пустым
        }

        delete to_delete;
    }

    /**
     * @brief Удаляет первое вхождение элемента с заданным значением. O(n).
     *
     * Находим узел (O(n)), перешиваем соседей (O(1)), освобождаем память.
     *
     * @param value Значение для удаления.
     * @throws std::runtime_error Если значение не найдено.
     */
    void remove(const T& value) {
        Node* target = find_node(value);  // O(n)

        if (target == nullptr) {
            throw std::runtime_error("remove: значение не найдено");
        }

        // Случай 1: удаляем head
        if (target == head_) { pop_front(); return; }

        // Случай 2: удаляем tail
        if (target == tail_) { pop_back();  return; }

        // Случай 3: узел в середине — перешиваем ссылки соседей
        //   [prev] <-> [target] <-> [next]
        //   [prev] <──────────────> [next]
        target->prev->next = target->next;
        target->next->prev = target->prev;
        delete target;
    }

    
    // Поиск
    

    /**
     * @brief Ищет первое вхождение значения. O(n).
     *
     * Обходит список слева направо. Двусвязность здесь не даёт
     * преимущества по скорости — она нужна для O(1) вставки/удаления
     * зная указатель на узел.
     *
     * @param value Искомое значение.
     * @return true  Если элемент найден.
     * @return false Если элемент не найден.
     */
    bool search(const T& value) const {
        return find_node(value) != nullptr;
    }

    
    // Объединение (конкатенация)
    

    /**
     * @brief Присоединяет другой список к концу текущего. O(n).
     *
     * Обходит other и добавляет копию каждого узла через push_back —
     * O(1) за узел, O(n) суммарно. Исходный список other не изменяется.
     *
     * Почему копируем, а не перевешиваем указатели:
     *   Если бы мы просто соединили tail_ с other.head_, то other
     *   и this разделяли бы одни и те же узлы — двойное освобождение
     *   памяти в деструкторах. Копирование безопаснее.
     *
     * @param other Список, элементы которого добавляются в конец.
     */
    void concat(const DoublyLinkedList<T>& other) {
        Node* current = other.head_;
        while (current != nullptr) {
            push_back(current->data);   // push_back — O(1) за счёт tail_
            current = current->next;
        }
    }

    
    // Размер
    

    /**
     * @brief Возвращает количество элементов. O(n).
     *
     * Специально не кэшируем размер в отдельном поле — это
     * выполнение требования задания. На практике в std::list
     * size() работает за O(1) именно потому что счётчик хранится
     * и обновляется при каждой вставке/удалении.
     *
     * @return std::size_t Количество узлов.
     */
    std::size_t size() const {
        std::size_t count = 0;
        Node* current = head_;
        while (current != nullptr) {
            ++count;
            current = current->next;
        }
        return count;
    }

    
    // Утилиты
    

    /**
     * @brief Возвращает true если список пуст. O(1).
     */
    bool empty() const { return head_ == nullptr; }

    /**
     * @brief Выводит список в формате: [a <-> b <-> c]. O(n).
     * Удобно для отладки.
     */
    void print(const std::string& name = "") const {
        if (!name.empty()) std::cout << name << ": ";
        std::cout << "[";
        Node* current = head_;
        while (current != nullptr) {
            std::cout << current->data;
            if (current->next != nullptr) std::cout << " <-> ";
            current = current->next;
        }
        std::cout << "]\n";
    }

private:

    /**
     * @brief Внутренний метод: ищет узел по значению. O(n).
     *
     * Вынесен отдельно чтобы не дублировать цикл поиска в search(),
     * remove() и insert_after().
     *
     * @return Node* Указатель на узел или nullptr если не найден.
     */
    Node* find_node(const T& value) const {
        Node* current = head_;
        while (current != nullptr) {
            if (current->data == value) return current;
            current = current->next;
        }
        return nullptr;
    }

    /**
     * @brief Внутренний метод: ищет узел по значению. O(n).
     *
     * Вынесен отдельно чтобы не дублировать цикл поиска в search(),
     * remove() и insert_after().
     *
     * @return Node* Указатель на узел или nullptr если не найден.
     */
Node* find_node_by_index(size_t index) const {
    Node* current = head_;
    for (size_t i = 0; i < index && current != nullptr; i++) {
        current = current->next;
    }
    return current;
}
};

#endif // LINKED_LIST_HPP
