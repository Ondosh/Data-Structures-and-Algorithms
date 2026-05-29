#pragma once
#include <stdexcept>
#include <cstddef>

template <typename T>
class Stack {
public:
    Stack() = default;
    ~Stack() { clear(); }

    // Конструктор копирования
    Stack(const Stack& other) {
        copyFrom(other);
    }

    // Оператор присваивания копированием
    Stack& operator=(const Stack& other) {
        if (this != &other) {
            clear();           // очищаем текущие данные
            copyFrom(other);   // копируем новые
        }
        return *this;
    }

    // Конструктор перемещения
    Stack(Stack&& other) noexcept
        : top_(other.top_), size_(other.size_)
    {
        other.top_ = nullptr;   // обнуляем источник
        other.size_ = 0;
    }

    // Оператор присваивания перемещением
    Stack& operator=(Stack&& other) noexcept {
        if (this != &other) {
            clear();               // освобождаем старые ресурсы
            top_ = other.top_;     // переносим данные
            size_ = other.size_;
            other.top_ = nullptr;  // источник больше не владеет данными
            other.size_ = 0;
        }
        return *this;
    }

    // Добавляет элемент на вершину стека
    void push(const T& value) {
        top_ = new Node(value, top_);
        ++size_;
    }

    // Удаляет и возвращает элемент с вершины стека
    T pop() {
        if (empty())
            throw std::underflow_error("Stack::pop() — стек пуст");
        Node* tmp = top_;
        T val = tmp->value;
        top_ = top_->next;
        delete tmp;
        --size_;
        return val;
    }

    // Возвращает элемент на вершине стека без удаления (константная версия)
    const T& top() const {
        if (empty())
            throw std::underflow_error("Stack::top() — стек пуст");
        return top_->value;
    }

    // Возвращает элемент на вершине стека без удаления (неконстантная версия)
    T& top() {
        if (empty())
            throw std::underflow_error("Stack::top() — стек пуст");
        return top_->value;
    }

    // Проверяет, пуст ли стек
    bool empty() const { return size_ == 0; }
    
    // Возвращает количество элементов в стеке
    std::size_t size() const { return size_; }

private:
    // Узел односвязного списка для хранения элемента и указателя на следующий узел
    struct Node {
        T value;       // хранимое значение
        Node* next;    // указатель на следующий узел
        Node(const T& v, Node* n) : value(v), next(n) {}
    };

    Node* top_ = nullptr;      // указатель на вершину стека
    std::size_t size_ = 0;     // количество элементов в стеке

    // Удаляет все узлы стека и освобождает память
    void clear() {
        while (top_) {
            Node* tmp = top_;
            top_ = top_->next;
            delete tmp;
        }
        size_ = 0;
    }

    // Копирует данные из другого стека
    void copyFrom(const Stack& other) {
        if (other.empty()) return;

        // Собираем узлы в обратном порядке, затем помещаем их в стек
        const Node* cur = other.top_;
        T* buf = new T[other.size_];
        std::size_t idx = other.size_;
        while (cur) {
            buf[--idx] = cur->value;  // заполняем буфер в обратном порядке
            cur = cur->next;
        }
        // Помещаем элементы из буфера в стек, сохраняя исходный порядок
        for (std::size_t i = 0; i < other.size_; ++i)
            push(buf[i]);
        delete[] buf;
    }
};