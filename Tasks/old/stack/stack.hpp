#pragma once
#include <stdexcept>
#include <cstddef>
#include "../list/linked_list.hpp"

template <typename T>
class Stack {
public:
    Stack() = default;

    ~Stack() = default;

    // Конструктор копирования - глубокое копирование
    Stack(const Stack& other) {
        copyFrom(other);
    }

    // Оператор присваивания копированием - глубокое копирование
    Stack& operator=(const Stack& other) {
        if (this != &other) {
            clear();
            copyFrom(other);
        }
        return *this;
    }

    // Конструктор перемещения
    Stack(Stack&& other) noexcept : list_(std::move(other.list_)) {}

    // Оператор присваивания перемещением
    Stack& operator=(Stack&& other) noexcept {
        if (this != &other) {
            list_ = std::move(other.list_);
        }
        return *this;
    }

    // Добавляет элемент на вершину стека
    void push(const T& value) {
        list_.push_front(value);
    }

    // Удаляет и возвращает элемент с вершины стека
    T pop() {
        if (empty()) {
            throw std::runtime_error("Stack::pop(): стек пуст");
        }
        T value = list_.front();
        list_.pop_front();
        return value;
    }

    // Возвращает элемент на вершине стека без удаления 
    const T& top() const {
        if (empty()) {
            throw std::runtime_error("Stack::top(): стек пуст");
        }
        return list_.front();
    }

    bool empty() const { return list_.empty(); }
    std::size_t size() const { return list_.size(); }
    
    void clear() {
        while (!empty()) {
            list_.pop_front();
        }
    }

private:
    DoublyLinkedList<T> list_;
    
    // Глубокое копирование из другого стека
    void copyFrom(const Stack& other) {
        if (other.empty()) return;
        
        // Создаем временный массив для хранения элементов в обратном порядке
        T* buffer = new T[other.size()];
        Stack<T> temp;  // Временный стек для переворота
        
        // Копируем элементы из other во временный массив в обратном порядке
        Stack<T> other_copy = other;  // Создаем копию, чтобы не изменять original
        for (size_t i = 0; i < other.size(); ++i) {
            buffer[other.size() - 1 - i] = other_copy.top();
            other_copy.pop();
        }
        
        // Вставляем элементы в правильном порядке в list_
        for (size_t i = 0; i < other.size(); ++i) {
            list_.push_front(buffer[i]);
        }
        
        delete[] buffer;
    }
};