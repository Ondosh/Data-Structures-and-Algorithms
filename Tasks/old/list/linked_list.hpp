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
 *   nullptr -> [head] <->  [node] <-> ... <-> [tail] -> nullptr
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
public:

    /**
     * @brief Возвращает true если список пуст. O(1).
     */
    bool empty() const { return head_ == nullptr; }


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
    struct Node {
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


    // Правило трёх - если класс явно объявляет деструктор,
    // конструктор копирования или оператор присваивания копированием,
    // скорее всего, ему требуются все три этих метода 

    // Правило пяти - новое правило появившееся в С++11. 
    // если вашему классу нужно явно объявить хотя бы один метод
    // из Правила трёх, то для полной эффективности и безопасности
    // ему необходимо определить все пять методов.
    // Новые 2 метода:
    // Конструктор перемещения и оператор присваивания перемещением

    // Конструктор / деструктор
    // Список инициализации. После : объявленные поля сразу получают nullptr
    DoublyLinkedList() : head_(nullptr), tail_(nullptr) {}

    /**
     * @brief Деструктор — освобождает все узлы.
     * Проходит по списку от head к tail и удаляет каждый узел.
     * Сложность: O(n).
     */
    ~DoublyLinkedList() {
        Node* current = head_;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }

    
    // Вставка


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
