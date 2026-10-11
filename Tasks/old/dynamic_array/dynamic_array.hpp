#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP

#include <cstddef>
#include <stdexcept>
#include <algorithm>


// ГЛУБОКОЕ И ПОВЕРХНОСТНОЕ КОПИРОВАНИЕ

// Поверхностное копирование (shallow copy) - копируется только указатель.
// Оба объекта начинают указывать на одну и ту же память.
// При уничтожении одного объекта память освобождается,
// второй объект получает висячий указатель - undefined behavior.
//
// Глубокое копирование (deep copy) - выделяется новая память,
// данные полностью копируются. Объекты независимы друг от друга.

// Правило трёх - если класс явно объявляет деструктор,
// конструктор копирования или оператор присваивания копированием,
// скорее всего, ему требуются все три этих метода 

// Правило пяти - новое правило появившееся в С++11. 
// если вашему классу нужно явно объявить хотя бы один метод
// из Правила трёх, то для полной эффективности и безопасности
// ему необходимо определить все пять методов.
// Новые 2 метода:
// Конструктор перемещения и оператор присваивания перемещением

template<typename T>
class DynamicArray {
public:
    // Конструкторы
    
    // Конструктор по умолчанию - пустой массив
    DynamicArray() : data_(nullptr), size_(0), capacity_(0) {}

    // Конструктор с резервированием памяти под n элементов.
    // val - значение которым заполняются элементы (по умолчанию T() - значение по умолчанию для типа T)
    explicit DynamicArray(std::size_t n, const T& val = T())
        : data_(nullptr), size_(0), capacity_(0)
    {
        reserve(n);
        for (std::size_t i = 0; i < n; ++i)
            data_[i] = val;
        size_ = n;
    }

    
    // ПРАВИЛО ПЯТИ - метод 1: Деструктор
    // Освобождает всю выделенную память.
    
    ~DynamicArray() {
        delete[] data_;
    }

    
    // ПРАВИЛО ПЯТИ - метод 2: Конструктор копирования
    // Глубокая копия - выделяем новую память и копируем элементы.
    // Вызывается при: DynamicArray b = a;
    //                 DynamicArray b(a);
    //                 передача по значению в функцию
    
    DynamicArray(const DynamicArray& other)
        : data_(nullptr), size_(0), capacity_(0)
    {
        reserve(other.size_);
        for (std::size_t i = 0; i < other.size_; ++i)
            data_[i] = other.data_[i];
        size_ = other.size_;
    }

    
    // ПРАВИЛО ПЯТИ - метод 3: Оператор присваивания копирования
    // copy-and-swap: создаём копию, меняемся с ней,
    // старая память уничтожается вместе с временным объектом.
    // Вызывается при: b = a;  (оба уже существуют)
    
    DynamicArray& operator=(const DynamicArray& other) {
        if (this == &other) return *this;  // защита от a = a
        DynamicArray tmp(other);           // глубокая копия
        swap(tmp);                         // меняемся ресурсами
        return *this;
        // tmp уничтожается здесь - вместе со старой памятью this
    }

    
    // ПРАВИЛО ПЯТИ - метод 4: Конструктор перемещения
    // "Крадём" указатель у другого объекта вместо копирования.
    // Вызывается при: DynamicArray b = std::move(a);
    // После перемещения other обязан быть в валидном пустом состоянии.
    // noexcept - гарантия что не бросит исключение 
    
    DynamicArray(DynamicArray&& other) noexcept
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_)
    {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    
    // ПРАВИЛО ПЯТИ - метод 5: Оператор присваивания перемещения
    // Вызывается при: b = std::move(a);  (оба уже существуют)
    
    DynamicArray& operator=(DynamicArray&& other) noexcept {
        if (this == &other) return *this;  // защита от a = std::move(a)
        delete[] data_;                    // освобождаем свою старую память
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
        return *this;
    }

    
    // Добавление / удаление
    

    // Добавляет элемент в конец. O(1).
    // При заполнении ёмкость удваивается - это даёт O(1) 
    // вместо O(n) при каждом добавлении.
    void push_back(const T& value) {
        ensure_capacity();
        data_[size_++] = value;
    }

    // Удаляет последний элемент. O(1).
    // При сильном опустошении (size < capacity/4) сжимаем массив вдвое -
    // освобождаем ненужную память, как требует задание.
    void pop_back() {
        if (size_ == 0)
            throw std::out_of_range("pop_back: массив пуст");
        --size_;

        // Освобождение памяти при удалении большого количества элементов:
        // если занято меньше четверти - уменьшаем ёмкость вдвое
        if (size_ > 0 && size_ < capacity_ / 4)
            reserve(capacity_ / 2);
    }

    // Вставка элемента в середину по индексу. O(n).
    // Сдвигаем элементы вправо, затем вставляем.
    void insert(std::size_t pos, const T& value) {
        if (pos > size_)
            throw std::out_of_range("insert: индекс за пределами массива");
        ensure_capacity();
        for (std::size_t i = size_; i > pos; --i)
            data_[i] = std::move(data_[i - 1]);
        data_[pos] = value;
        ++size_;
    }

    // Удаление элемента из середины по индексу. O(n).
    // Сдвигаем элементы влево.
    void erase(std::size_t pos) {
        if (pos >= size_)
            throw std::out_of_range("erase: индекс за пределами массива");
        for (std::size_t i = pos; i + 1 < size_; ++i)
            data_[i] = std::move(data_[i + 1]);
        --size_;
    }

    // Изменяет размер массива до new_size элементов.
    // Если new_size > size_ - добавляет элементы со значением val (по умолчанию T()).
    // Если new_size < size_ - усекает массив, лишние элементы удаляются.
    void resize(std::size_t new_size, const T& val = T()) {
        if (new_size > capacity_)
            reserve(new_size);                        // выделяем память если не хватает

        for (std::size_t i = size_; i < new_size; ++i)
            data_[i] = val;                           // инициализируем новые элементы

        size_ = new_size;

        // Освобождаем память если массив сильно опустел
        if (size_ > 0 && size_ < capacity_ / 4)
            reserve(capacity_ / 2);
    }

    // Доступ по индексу
    

    // operator[] - без проверки границ, O(1)
    T& operator[](std::size_t i)             { return data_[i]; }
    const T& operator[](std::size_t i) const { return data_[i]; }

    // at() - с проверкой границ, бросает исключение, O(1)
    T& at(std::size_t i) {
        if (i >= size_) throw std::out_of_range("at: индекс за пределами массива");
        return data_[i];
    }
    const T& at(std::size_t i) const {
        if (i >= size_) throw std::out_of_range("at: индекс за пределами массива");
        return data_[i];
    }

    
    // Поиск. O(n).
    // Возвращает индекс первого вхождения или -1 если не найдено.
    
    int find(const T& value) const {
        for (std::size_t i = 0; i < size_; ++i)
            if (data_[i] == value) return static_cast<int>(i);
        return -1;
    }

    
    // Сортировка. O(n log n).
    // Использует std::sort - интроспективная сортировка.
    
    void sort() {
        std::sort(data_, data_ + size_);
    }

    
    // Утилиты
    

    std::size_t size() const noexcept { return size_; }
    std::size_t capacity() const noexcept { return capacity_; }
    bool empty() const noexcept { return size_ == 0; }

    // Резервирует память под new_cap элементов.
    // Если new_cap <= capacity_ - ничего не делает.
    // Перевыделяет массив и переносит существующие элементы.
    void reserve(std::size_t new_cap) {
        if (new_cap <= capacity_) return;
        T* new_data = new T[new_cap];
        for (std::size_t i = 0; i < size_; ++i) // здесь выполняется перемещение старого массива в новый
            new_data[i] = std::move(data_[i]);
        delete[] data_;
        data_ = new_data;
        capacity_ = new_cap;
    }

    // Меняет содержимое двух массивов местами за O(1).
    void swap(DynamicArray& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

private:
    T* data_; // указатель на данные
    std::size_t size_; // текущее количество элементов
    std::size_t capacity_;  // выделенная ёмкость

    // Проверяет что есть место для ещё одного элемента.
    // Если нет - удваивает ёмкость (или ставит 1 если массив был пуст).
    // Удвоение даёт O(1) для push_back.
    void ensure_capacity() {
        if (size_ + 1 > capacity_)
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
};

#endif // DYNAMIC_ARRAY_HPP