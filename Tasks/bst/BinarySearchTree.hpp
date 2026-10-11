#pragma once

#include <iostream>
#include <vector>
#include <algorithm> // std::max

// =========================================================================
// БИНАРНОЕ ДЕРЕВО ПОИСКА (Binary Search Tree, BST)
// =========================================================================
//
// Главное правило (свойство BST) — выполняется для КАЖДОГО узла:
//   * все значения в ЛЕВОМ поддереве  МЕНЬШЕ значения узла;
//   * все значения в ПРАВОМ поддереве БОЛЬШЕ значения узла.
//
//            8
//          /   \       <- слева меньше, справа больше
//         3     10
//        / \      \    (и так для каждого узла)
//       1   6      14
//          / \     /
//         4   7   13
//
// Благодаря правилу в каждом узле мы знаем, в какую сторону идти,
// и отбрасываем целое поддерево. Поэтому поиск, вставка и удаление
// работают за O(h), где h — высота дерева:
//   * сбалансированное дерево: h ~ log2(n)  -> O(log n);
//   * вырожденное (вставляли 1, 2, 3, 4...) — по сути список -> O(n).
//
// Дубликаты не храним: insert повторного значения ничего не делает.
// От типа T требуется только operator< .
//
// Файл .hpp, потому что класс шаблонный: реализация шаблона должна быть
// видна компилятору целиком.

template <typename T>
class BinarySearchTree {
private:
    struct Node {
        T data;
        Node* left;
        Node* right;

        explicit Node(const T& value) : data(value), left(nullptr), right(nullptr) {}
    };

    Node* root_;   // корень (nullptr, если дерево пустое)
    size_t size_;  // количество узлов

    // Рекурсивное удаление всех узлов (для деструктора)
    void clearRecursive(Node* node) {
        if (!node) return;
        clearRecursive(node->left);
        clearRecursive(node->right);
        delete node;
    }

    void inOrderRec(Node* node, std::vector<T>& result) const {
        if (!node) return;
        inOrderRec(node->left, result);
        result.push_back(node->data);
        inOrderRec(node->right, result);
    }

    int heightRec(Node* node) const {
        if (!node) return 0;
        return 1 + std::max(heightRec(node->left), heightRec(node->right));
    }

    // Печать дерева "лёжа на боку": корень слева, правое поддерево сверху
    void printRec(Node* node, int depth) const {
        if (!node) return;
        printRec(node->right, depth + 1);
        for (int i = 0; i < depth; ++i) std::cout << "    ";
        std::cout << node->data << "\n";
        printRec(node->left, depth + 1);
    }

public:
    BinarySearchTree() : root_(nullptr), size_(0) {}

    ~BinarySearchTree() {
        clearRecursive(root_);
    }

    // Копирование запрещено: иначе две копии указывали бы на одни и те же
    // узлы, и деструктор удалил бы их дважды.
    BinarySearchTree(const BinarySearchTree&) = delete;
    BinarySearchTree& operator=(const BinarySearchTree&) = delete;

    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    // ---------------------------------------------------------------
    // ПОИСК
    // ---------------------------------------------------------------
    // Начинаем с корня. В каждом узле сравниваем:
    //   value <  data -> идём влево;
    //   value >  data -> идём вправо;
    //   иначе         -> нашли.
    // Упёрлись в nullptr — значения в дереве нет.
    bool contains(const T& value) const {
        Node* current = root_;
        while (current) {
            if (value < current->data) {
                current = current->left;
            } else if (current->data < value) {
                current = current->right;
            } else {
                return true;
            }
        }
        return false;
    }

    // ---------------------------------------------------------------
    // ДОБАВЛЕНИЕ
    // ---------------------------------------------------------------
    // Спускаемся так же, как при поиске, запоминая родителя.
    // Когда дошли до nullptr — это и есть место нового узла:
    // подвешиваем его к родителю слева или справа.
    // Новый узел всегда становится ЛИСТОМ.
    //
    // Возвращает false, если такое значение уже есть (дубликат).
    bool insert(const T& value) {
        if (!root_) {
            root_ = new Node(value);
            ++size_;
            return true;
        }

        Node* parent = nullptr;
        Node* current = root_;
        while (current) {
            parent = current;
            if (value < current->data) {
                current = current->left;
            } else if (current->data < value) {
                current = current->right;
            } else {
                return false; // дубликат
            }
        }

        Node* newNode = new Node(value);
        if (value < parent->data) {
            parent->left = newNode;
        } else {
            parent->right = newNode;
        }
        ++size_;
        return true;
    }

    // ---------------------------------------------------------------
    // УДАЛЕНИЕ
    // ---------------------------------------------------------------
    // 1) Ищем узел и его родителя (как в поиске).
    // 2) Три случая:
    //    а) у узла нет детей  -> просто удаляем, у родителя ставим nullptr;
    //    б) один ребёнок      -> родитель "перепрыгивает" через узел
    //                            и указывает сразу на ребёнка;
    //    в) два ребёнка       -> находим ПРЕЕМНИКА: минимум правого
    //                            поддерева (шаг вправо, затем до упора влево).
    //                            Копируем его значение в удаляемый узел,
    //                            а удаляем физически уже преемника.
    //                            У преемника нет левого ребёнка, поэтому
    //                            для него срабатывает случай а) или б).
    //
    // Возвращает true, если значение найдено и удалено.
    bool remove(const T& value) {
        // ---- Шаг 1: поиск узла и его родителя ----
        Node* parent = nullptr;
        Node* current = root_;
        while (current) {
            if (value < current->data) {
                parent = current;
                current = current->left;
            } else if (current->data < value) {
                parent = current;
                current = current->right;
            } else {
                break;
            }
        }
        if (!current) return false; // не нашли

        // ---- Шаг 2в: два ребёнка -> сводим к удалению преемника ----
        if (current->left && current->right) {
            Node* successorParent = current;
            Node* successor = current->right;
            while (successor->left) {
                successorParent = successor;
                successor = successor->left;
            }
            current->data = successor->data;

            // Дальше удаляем преемника (у него максимум один ребёнок — правый)
            parent = successorParent;
            current = successor;
        }

        // ---- Шаг 2а/2б: ноль или один ребёнок ----
        Node* child = current->left ? current->left : current->right; // может быть nullptr

        if (!parent) {
            root_ = child;              // удаляли корень
        } else if (parent->left == current) {
            parent->left = child;
        } else {
            parent->right = child;
        }

        delete current;
        --size_;
        return true;
    }

    // ---------------------------------------------------------------
    // ВСПОМОГАТЕЛЬНОЕ (для проверки)
    // ---------------------------------------------------------------

    // Симметричный обход (лево -> узел -> право).
    // Для BST он всегда даёт значения В ПОРЯДКЕ ВОЗРАСТАНИЯ.
    std::vector<T> inOrder() const {
        std::vector<T> result;
        inOrderRec(root_, result);
        return result;
    }

    // Высота: число узлов на самом длинном пути от корня до листа
    int height() const { return heightRec(root_); }

    void print() const {
        if (!root_) {
            std::cout << "(дерево пустое)\n";
            return;
        }
        printRec(root_, 0);
    }
};
